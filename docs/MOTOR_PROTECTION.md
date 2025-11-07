# Motor Protection Circuit - Detailed Guide

Protect your Mini 4WD motors and test equipment from overcurrent damage during stall torque testing.

## Table of Contents

1. [Why Protection is Critical](#why-protection-is-critical)
2. [Protection Methods Comparison](#protection-methods-comparison)
3. [Option 1: XL4015 Module Setup](#option-1-xl4015-module-setup)
4. [Option 2: Polyfuse Protection](#option-2-polyfuse-protection)
5. [Option 3: DIY MOSFET Current Limiter](#option-3-diy-mosfet-current-limiter)
6. [Option 4: PWM Motor Driver](#option-4-pwm-motor-driver)
7. [Software Current Limiting](#software-current-limiting)
8. [Testing and Calibration](#testing-and-calibration)

---

## Why Protection is Critical

### The Problem

**Normal Operation:**
- Mini 4WD motor: 0.3-0.8A @ 2.4-3V
- Safe for motor and components

**Stall Torque Testing:**
- Motor shaft blocked (maximum load)
- Current spikes to **5-7A** (10x normal!)
- Can burn motor in 5-10 seconds
- INA219 rated max: 3.2A (will be damaged)

### Consequences Without Protection

| Component | Normal | Stall (No Protection) | Result |
|-----------|--------|----------------------|---------|
| Motor | 0.5A, 30°C | 6A, 150°C+ | Burned brushes/commutator |
| INA219 | 0.5A, 25°C | 6A, 80°C+ | Damaged sensor chip |
| Battery | Light load | Heavy load | Rapid discharge/damage |
| Wires | Cool | Hot | Potential fire hazard |

### The Solution

**Add current limiting:** Restrict maximum current to safe level (2-3A)
- Motor still develops ~80% of stall torque
- Components protected
- Safe testing possible

---

## Protection Methods Comparison

| Method | Cost | Difficulty | Response Time | Accuracy | Best For |
|--------|------|-----------|---------------|----------|----------|
| XL4015 Module | $5 | Easy | Fast (µs) | ±5% | Most users |
| Polyfuse | $1 | Very Easy | Slow (seconds) | ±20% | Budget/simple |
| DIY MOSFET | $10 | Hard | Very Fast (ns) | ±1% | Advanced/learning |
| PWM Driver | $8 | Medium | Software (ms) | ±2% | Automation |

**Recommendation:** Start with **XL4015 Module** (Option 1) for best balance.

---

## Option 1: XL4015 Module Setup

### Components Needed

- **XL4015 DC-DC Buck Converter Module** (~$3-5)
  - Input: 4-38V
  - Output: 1.25-36V, 5A max
  - Built-in current limiting
  - Alternative: LM2596 (lower power, 3A max)

### Module Features

```
XL4015 Module Layout
====================

IN+ IN-  [Module]  OUT+ OUT-
  │  │     ┌─────┐    │   │
  │  └─────┤ PWR ├────┘   │
  └────────┤ LED ├────────┘
           │     │
           │  CV │ ← Constant Voltage pot (voltage adjust)
           │  CC │ ← Constant Current pot (current limit adjust)
           └─────┘
```

### Wiring

```
     Battery           XL4015 Module          INA219           Motor
        │                                        │               │
   (+) ─┴──► IN+ ──[Buck Converter]── OUT+ ──► VIN+ ─────┐    │
                                                    │      │    │
   (-) ─┬──► IN- ──────────────────── OUT- ──┬─── GND    └──► VIN- ──► (+)
        │                                     │                          │
        │                                     │                          │
        └─────────────────────────────────────┴──────────────────────► (-)

Common ground connects: Battery (-), XL4015 OUT-, INA219 GND, Motor (-), ESP32 GND
```

### Setup Procedure

**Tools needed:**
- Multimeter
- Small screwdriver (for pot adjustment)
- Test load (motor or power resistor)
- Wire or paperclip (for short circuit test)

**Step 1: Set Output Voltage**

1. Connect battery to IN+ and IN-
2. Measure voltage at OUT+ and OUT- with multimeter
3. Adjust **CV potentiometer** until output matches battery voltage
   - For 2x AA (3V): Set to 3.0V
   - For 2x AA (2.4V): Set to 2.4V
4. Power LED should be on (red)

**Step 2: Set Current Limit**

1. **Disconnect motor** (important!)
2. Connect ammeter in series: OUT+ → Ammeter → Wire
3. **Short circuit:** Touch wire to OUT- (creates maximum current demand)
4. Adjust **CC potentiometer** while monitoring ammeter
5. When current reaches **2.5A**, stop adjusting
6. Module is now in CC mode (current limit active)
7. Charge LED turns on (typically blue/green)
8. Remove short circuit

**Step 3: Verify Setup**

1. Remove short circuit and ammeter
2. Connect motor normally
3. Run motor at full stall
4. Current should not exceed 2.5A
5. Voltage may drop slightly (normal under load)

### Indicators

**Power LED (Red):**
- ON: Module powered
- OFF: No input power

**Charge LED (Blue/Green):**
- ON: Current limit active (CC mode)
- OFF: Normal operation (CV mode)

**During stall test:**
- Charge LED should turn ON (current limiting)
- Motor still runs but at reduced power
- This is correct operation!

### Troubleshooting XL4015

**Problem: Output voltage incorrect**
- Adjust CV pot clockwise (increase) or counter-clockwise (decrease)
- Check input voltage is sufficient (min 4V for 3V output)

**Problem: Current limit not working**
- Verify CC pot adjustment with short circuit test
- Check for loose connections
- Module may be damaged (test with new module)

**Problem: Module gets hot**
- Normal at high current (add heatsink if very hot)
- Ensure good ventilation
- May indicate wrong voltage setting (too much buck conversion)

**Problem: Output voltage drops under load**
- This is normal in CC mode
- Indicates current limiting is working
- Motor gets reduced voltage to limit current

---

## Option 2: Polyfuse Protection

### What is a Polyfuse?

**Resettable Fuse (PPTC - Polymeric Positive Temperature Coefficient)**
- Passive component (no adjustment needed)
- Low resistance when cool (normal operation)
- High resistance when hot (protection mode)
- Automatically resets when cool

### How it Works

```
Normal Current (< 2.5A):
    Polyfuse = 0.1Ω (low resistance)
    Voltage drop = 0.25V (negligible)
    Motor runs normally

High Current (> 5A):
    Polyfuse heats up → Resistance increases to 100Ω+
    Current drops to safe level (~50mA)
    Motor stops running
    After 30-60 seconds, polyfuse cools and resets
```

### Selecting a Polyfuse

**Key Specifications:**

| Parameter | Description | Mini 4WD Recommendation |
|-----------|-------------|-------------------------|
| Hold Current (Ihold) | Max current without tripping | 2.5-3.0A |
| Trip Current (Itrip) | Current that triggers protection | 5.0-6.0A |
| Max Voltage | Maximum voltage rating | 6V or higher |
| Resistance | Resistance when cool | <0.5Ω |

**Recommended Parts:**

1. **Bourns MF-R250** - 2.5A hold, 5A trip, 6V, 0.5Ω
2. **Littelfuse 30R260** - 2.6A hold, 5.2A trip, 60V, 0.3Ω
3. **TE Connectivity MINISMDC260F** - 2.6A hold, 5A trip, 15V

### Wiring

```
Simple Inline Connection
========================

Battery (+) ──┬──► [Polyfuse] ──► INA219 VIN+ ──► Motor (+)
              │
Battery (-) ──┴────────────────► INA219 GND  ──► Motor (-)
                                      │
                                 ESP32 GND
```

**Installation:**
1. Solder polyfuse inline with battery positive wire
2. Insulate with heat shrink tubing
3. That's it!

### Testing Polyfuse

1. **Normal operation test:**
   - Run motor at low-medium load
   - Polyfuse should stay cool
   - Motor runs normally

2. **Protection test:**
   - Run motor at stall
   - Within 3-5 seconds, polyfuse heats up
   - Motor stops or runs very slowly
   - Wait 30 seconds, polyfuse cools and resets

### Pros and Cons

**Advantages:**
- ✓ Very simple (one component)
- ✓ No adjustment or calibration
- ✓ Automatic reset (no manual intervention)
- ✓ Cheap ($1-2)
- ✓ No external power needed

**Disadvantages:**
- ✗ Slower response (2-5 seconds to trip)
- ✗ Less precise (±20% tolerance)
- ✗ Allows brief overcurrent before tripping
- ✗ Can't measure true stall torque (motor stops)
- ✗ Slight voltage drop in normal operation

**Best for:** Basic protection, budget builds, simple setups

---

## Option 3: DIY MOSFET Current Limiter

### Circuit Design

Professional-grade current limiter using MOSFET and op-amp feedback.

### Schematic

```
Current Limiter Circuit
=======================

                                            +Vbat (3V)
                                                │
                                                │
Battery (+) ───┬─────────────────► MOSFET Drain (D)
               │                              │
             [10kΩ]                           │
               │                    MOSFET Gate (G)
               ├──────────────────────┤
               │                      │
           [1kΩ]              LM358 Pin 1 (OUT)
               │                      │
              GND             LM358 Pin 8 ──────► +Vbat (3V)
                              LM358 Pin 4 ──────► GND
                                      │
                        ┌─────────────┴─────────────┐
                        │                           │
         Current    ──► Pin 2 (IN-)            Pin 3 (IN+) ◄── Set Point
         Sense              │                       │
                            │                  [10kΩ POT]
                            │                       │
                            │                      GND
                            │
              ┌─────────────┴─────────────┐
              │                           │
    MOSFET Source (S) ──► [0.1Ω, 5W] ──► INA219 VIN+ ──► Motor (+)
              │                 │
              │             Sense voltage
              │            (0-0.25V @ 0-2.5A)
              │
             GND
```

### Components List

| Component | Specification | Quantity | Cost | Notes |
|-----------|--------------|----------|------|-------|
| MOSFET | IRLZ44N (N-channel, logic-level) | 1 | $1 | Or IRLB8721 |
| Op-Amp | LM358 (dual op-amp) | 1 | $0.50 | Or LM324 |
| Current Sense Resistor | 0.1Ω, 5W wire-wound | 1 | $1 | Must be low-inductance |
| Potentiometer | 10kΩ linear | 1 | $1 | For current adjustment |
| Resistors | 10kΩ (1/4W) | 2 | $0.20 | ±5% tolerance OK |
| Resistors | 1kΩ (1/4W) | 1 | $0.10 | ±5% tolerance OK |
| Capacitors | 100nF ceramic | 2 | $0.20 | Power supply decoupling |
| Capacitors | 10µF electrolytic | 1 | $0.20 | Op-amp stability |
| Heatsink | TO-220 heatsink | 1 | $1 | For MOSFET |
| Protoboard | 5×7cm | 1 | $1 | Or custom PCB |

**Total Cost: ~$7-10**

### How it Works

1. **Current flows** through MOSFET and sense resistor (0.1Ω)
2. **Voltage develops** across resistor: V = I × R
   - 1A → 0.1V
   - 2.5A → 0.25V
3. **Op-amp compares** sense voltage to setpoint (from pot)
4. **If current too high:**
   - Sense voltage > setpoint
   - Op-amp output goes LOW
   - MOSFET gate voltage drops
   - MOSFET resistance increases
   - Current decreases
5. **Negative feedback loop** maintains constant current

### Building Instructions

**Step 1: Prepare Protoboard**

Layout:
```
    Components Layout (Top View)
    =============================

    10kΩ POT                 LM358          MOSFET (IRLZ44N)
    (limit adjust)          (op-amp)         [D] [G] [S]
         │                      │              │   │   │
    ┌────┴────┐            ┌───┴───┐          └───┼───┘
    │    │    │            │ 1   8 │              │
    └────┼────┘            │ 2   7 │         [Heatsink]
         │                 │ 3   6 │              │
      Setpoint             │ 4   5 │          [0.1Ω 5W]
                           └───────┘              │
                                              To INA219
```

**Step 2: Solder Power Supply Decoupling**

1. Solder 100nF capacitor across op-amp power pins (Pin 8 to Pin 4)
2. Solder 100nF capacitor across battery input
3. Keep leads short

**Step 3: Install Current Sense Resistor**

1. Solder 0.1Ω resistor between MOSFET Source and output
2. Use heavy gauge wire (16-18 AWG)
3. Keep leads short and direct

**Step 4: Wire Op-Amp**

Connections:
```
LM358 Pinout (DIP-8)
====================

Pin 1 (OUT1)  ──► 1kΩ ──► MOSFET Gate
Pin 2 (IN1-)  ──► Sense resistor (+) side
Pin 3 (IN1+)  ──► Potentiometer wiper
Pin 4 (GND)   ──► Common GND
Pin 5-7       ──► Not used (or use second op-amp for LED indicator)
Pin 8 (VCC)   ──► Battery (+) or regulated 5V
```

**Step 5: Wire Setpoint Potentiometer**

```
Potentiometer Wiring
====================

Pin 1 (CCW)  ──► GND
Pin 2 (Wiper)──► Op-amp Pin 3 (IN+)
Pin 3 (CW)   ──► 10kΩ resistor ──► Sense resistor (+) side

This creates a voltage divider to set the limit point
```

**Step 6: Install MOSFET**

1. Bend MOSFET pins to fit protoboard
2. Attach heatsink with thermal paste
3. Solder carefully (don't overheat MOSFET)

**Pinout (IRLZ44N, TO-220 package, front view):**
```
    ┌───────┐
    │   │   │
   [G] [D] [S]
    │   │   │
  Gate Drain Source
```

**Step 7: Final Wiring**

Input:
```
Battery (+) ──► MOSFET Drain
Battery (-) ──► Common GND
```

Output:
```
Sense Resistor → ──► INA219 VIN+
Common GND ──────► INA219 GND
```

### Calibration

**Step 1: Set Current Limit**

1. Connect ammeter: Circuit output → Ammeter → Wire
2. Short wire to GND (creates max current demand)
3. Adjust potentiometer until ammeter reads 2.5A
4. Remove short circuit

**Step 2: Test Under Load**

1. Connect motor
2. Stall motor (block shaft)
3. Verify current stays at 2.5A ±0.1A
4. MOSFET should be warm (40-50°C is normal)

### Troubleshooting DIY Circuit

**Problem: No current flow**
- Check MOSFET orientation (Gate, Drain, Source)
- Verify gate voltage (should be 2-3V)
- Test MOSFET separately (may be damaged)

**Problem: Current limit too high/low**
- Adjust potentiometer
- Check sense resistor value (should be 0.1Ω)
- Verify op-amp is powered (Pin 8 = +V, Pin 4 = GND)

**Problem: Circuit oscillates (unstable)**
- Add 10µF capacitor across op-amp output and GND
- Reduce gate resistor to 470Ω
- Check for loose connections

**Problem: MOSFET overheats**
- Add larger heatsink
- Check sense resistor (may be shorted)
- Verify current limit is set correctly

### Advanced: Add Current Display

Use second op-amp in LM358 to drive LED indicator:

```
Pin 5 (OUT2) ──► LED (+ current limiting resistor) ──► GND
Pin 6 (IN2-) ──► Reference (setpoint)
Pin 7 (IN2+) ──► Sense voltage

LED turns on when current limiting is active
```

---

## Option 4: PWM Motor Driver

### Software-Controlled Current Limiting

Use ESP32 to read INA219 and adjust PWM to limit current.

### Components

- BTS7960 Motor Driver Module (~$8)
- Or L298N Motor Driver (~$3)

### Wiring

```
BTS7960 to ESP32
================

BTS7960 Pin    ESP32 Pin
-----------    ---------
VCC (logic)  → 5V (from USB)
GND          → GND
R_EN         → GPIO 25 (PWM)
R_PWM        → GPIO 25 (same as R_EN, or separate)
L_EN         → GND (disable reverse)
L_PWM        → GND

B+           → Battery (+)
B-           → Battery (-)
M+           → Motor (+)
M-           → Motor (-)
```

### ESP32 Code Modification

Add to `torque_analyzer.ino`:

```cpp
// PWM Configuration
#define MOTOR_PWM_PIN 25
#define PWM_FREQ 1000  // 1kHz
#define PWM_CHANNEL 0
#define PWM_RESOLUTION 8  // 0-255

// Current limit
#define MAX_CURRENT_MA 2500.0  // 2.5A limit

void setup() {
  // ... existing code ...

  // Setup PWM for motor control
  ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(MOTOR_PWM_PIN, PWM_CHANNEL);
  ledcWrite(PWM_CHANNEL, 255);  // Start at full speed
}

void loop() {
  readSensors();

  // Current limiting logic
  static int pwmDuty = 255;  // 0-255

  if (data.current_mA > MAX_CURRENT_MA) {
    // Current too high - reduce PWM
    pwmDuty = max(0, pwmDuty - 5);
  } else if (data.current_mA < MAX_CURRENT_MA - 200) {
    // Current OK - can increase PWM
    pwmDuty = min(255, pwmDuty + 1);
  }

  ledcWrite(PWM_CHANNEL, pwmDuty);

  // ... rest of code ...
}
```

### Advantages

- Software-controlled (easy to adjust)
- Can log PWM duty cycle
- Variable speed control
- No additional hardware (driver module only)

### Disadvantages

- Slower response (100ms loop time)
- More complex code
- PWM affects torque reading (not pure DC)

---

## Software Current Limiting

### Monitor-Only Approach

Even without hardware limiting, ESP32 can provide warnings.

### Code Implementation

Add to ESP32 firmware:

```cpp
// Safety limits
#define MAX_SAFE_CURRENT_MA 2500.0
#define MAX_STALL_TIME_MS 3000  // 3 seconds max stall

void loop() {
  readSensors();

  // Check for overcurrent
  static unsigned long overcurrentStartTime = 0;

  if (data.current_mA > MAX_SAFE_CURRENT_MA) {
    if (overcurrentStartTime == 0) {
      overcurrentStartTime = millis();
      Serial.println("WARNING: Overcurrent detected!");
    }

    unsigned long overcurrentDuration = millis() - overcurrentStartTime;

    if (overcurrentDuration > MAX_STALL_TIME_MS) {
      Serial.println("CRITICAL: Stall time exceeded! Disconnect power!");
      // Could trigger relay/MOSFET to cut power here
    }
  } else {
    overcurrentStartTime = 0;  // Reset timer
  }

  // ... rest of code ...
}
```

### Dashboard Warning

Update `raspberry-pi/templates/index.html`:

```javascript
socket.on('sensor_data', function(data) {
    // ... existing code ...

    // Add overcurrent warning
    if (data.current_mA > 2500) {
        document.body.style.backgroundColor = '#ff0000';  // Red alert
        alert('OVERCURRENT! Disconnect motor immediately!');
    } else {
        document.body.style.backgroundColor = '';
    }
});
```

---

## Testing and Calibration

### Verification Procedure

**Test 1: No Load**
1. Motor disconnected
2. Current should be ~0mA
3. Limiter should not activate

**Test 2: Light Load**
1. Motor running freely (no shaft load)
2. Current: 200-500mA
3. Limiter should not activate

**Test 3: Medium Load**
1. Motor with moderate shaft resistance
2. Current: 1000-1500mA
3. Limiter should not activate

**Test 4: Stall Load**
1. Motor shaft completely blocked
2. Current should be limited to 2500mA ±100mA
3. Limiter SHOULD activate
4. Motor runs but can't overcome block
5. Test for max 3 seconds, then disconnect

### Success Criteria

✓ Stall current stays below 2.5A
✓ Motor doesn't burn or smoke
✓ INA219 stays cool
✓ Measurements remain stable

---

## Summary Recommendations

**Best Overall: XL4015 Module (Option 1)**
- Easy to use
- Accurate limiting
- Fast response
- Good value

**Simplest: Polyfuse (Option 2)**
- Add to any project
- No setup needed
- Cheap backup protection

**Most Flexible: PWM Driver (Option 4)**
- Software control
- Variable speed
- Data logging

**Learning Project: DIY MOSFET (Option 3)**
- Understand electronics
- Maximum control
- Professional quality

**For Maximum Safety: Combine Methods**
- XL4015 module (primary protection)
- Polyfuse (backup protection)
- Software monitoring (warning system)

---

For more information:
- `WIRING.md` - Complete wiring diagrams
- `BUILD_GUIDE.md` - Full build instructions
- `MECHANICAL_SETUP_LEVER.md` - Lever arm design
