# Wiring Diagram - OLED Display Version

Complete wiring for standalone DC Motor Stall Torque Analyzer with OLED display and button controls.

## System Overview

```
ESP32 Standalone System
=======================

Inputs:
- HX711 (Load Cell) → Force measurement
- INA219 (Current/Voltage) → Electrical measurement

Outputs:
- SSD1306 OLED Display → Show measurements
- PWM Motor Control → Control motor speed

Controls:
- 4 Buttons → Navigate menus and control
```

## Complete Pin Connections

### ESP32 Pin Assignment

| ESP32 Pin | Connected To | Function |
|-----------|-------------|----------|
| **GPIO 16** | HX711 DT | Load cell data |
| **GPIO 4** | HX711 SCK | Load cell clock |
| **GPIO 21** | SDA (I2C) | OLED & INA219 data |
| **GPIO 22** | SCL (I2C) | OLED & INA219 clock |
| **GPIO 27** | Motor Driver PWM | Motor speed control |
| **GPIO 32** | Button UP | Increase/Up |
| **GPIO 33** | Button DOWN | Decrease/Down |
| **GPIO 25** | Button MODE | Change display mode |
| **GPIO 26** | Button RESET | Reset max values |
| **3.3V** | Power rail | Power for sensors |
| **GND** | Ground rail | Common ground |
| **VIN** | 5V (optional) | Power from USB/battery |

## Detailed Wiring

### 1. HX711 Load Cell Amplifier

```
HX711 Module → ESP32
-----------------------
VCC         → 3.3V
GND         → GND
DT (Data)   → GPIO 16
SCK (Clock) → GPIO 4

Load Cell → HX711
-----------------------
Red    → E+
Black  → E-
White  → A-  (or A+ if readings are inverted)
Green  → A+  (or A- if readings are inverted)
```

### 2. INA219 Current/Voltage Sensor

```
INA219 → ESP32 (I2C)
-----------------------
VCC    → 3.3V
GND    → GND
SDA    → GPIO 21 (I2C SDA)
SCL    → GPIO 22 (I2C SCL)

INA219 Power Measurement
-----------------------
VIN+   → Current Limiter Output (or Battery +)
VIN-   → Motor (+) wire
GND    → Battery (-) / Common GND
```

### 3. SSD1306 OLED Display (128x64)

```
OLED Display → ESP32 (I2C)
-----------------------
VCC (or VDD)  → 3.3V
GND           → GND
SDA (or D/A)  → GPIO 21 (I2C SDA, shared with INA219)
SCL (or CLK)  → GPIO 22 (I2C SCL, shared with INA219)

Note: OLED and INA219 share the same I2C bus
```

**OLED Module Types:**
- 4-pin I2C version (most common): VCC, GND, SDA, SCL
- Check module voltage: 3.3V or 5V compatible
- Most SSD1306 modules work with 3.3V

### 4. Control Buttons

```
Button Connections (Pull-up configuration)
==========================================

Each button:
    One side → ESP32 GPIO pin
    Other side → GND

Button UP:
    GPIO 32 ──┬──[Button]── GND
              │
           (internal pull-up enabled)

Button DOWN:
    GPIO 33 ──┬──[Button]── GND
              │
           (internal pull-up enabled)

Button MODE:
    GPIO 25 ──┬──[Button]── GND
              │
           (internal pull-up enabled)

Button RESET:
    GPIO 26 ──┬──[Button]── GND
              │
           (internal pull-up enabled)

Note: Code uses INPUT_PULLUP, so no external resistors needed.
Button pressed = LOW (0V), Released = HIGH (3.3V)
```

**Optional - Debounce Capacitors:**
Add 100nF (0.1µF) capacitor across each button for cleaner operation:
```
GPIO ──┬──[Button]── GND
       │
      [100nF]
       │
      GND
```

### 5. Motor Driver (PWM Control)

```
Option A: Direct MOSFET Control (Simple)
=========================================

ESP32 GPIO 27 ──► [1kΩ]──► MOSFET Gate (IRLZ44N)
                              │
                        MOSFET Drain ── Battery (+)
                              │
                        MOSFET Source ── Motor (+)

Motor (-) ── Battery (-)


Option B: Motor Driver Module (Recommended)
============================================

BTS7960 or L298N Motor Driver:

ESP32 → Motor Driver
--------------------
GPIO 27 (PWM) → R_PWM / EN pin
GND           → GND
3.3V or 5V    → VCC (logic power)

Motor Driver → Motor
--------------------
M+  → Motor (+)
M-  → Motor (-)
B+  → Battery (+) through current limiter
B-  → Battery (-)
```

### 6. Motor Protection (Current Limiter)

**IMPORTANT:** Always include current limiting!

```
Protected Motor Power Path
==========================

Battery (+) ──► [XL4015 Current Limiter] ──► INA219 VIN+
                   (set to 2.5A)                 │
                                            INA219 VIN- ──► Motor (+)
                                                 │
Battery (-) ◄────────────────────────────────Motor (-)
                                                 │
                                            INA219 GND ──► ESP32 GND
```

**Alternative - Polyfuse:**
```
Battery (+) ──► [2.5A Polyfuse] ──► INA219 VIN+ ──► Motor (+)
```

## Complete System Wiring Diagram

```
                                        ESP32
                          ┌────────────────────────────┐
                          │                            │
                          │  GPIO 16 ◄─────────────── HX711 DT
                          │  GPIO 4  ◄─────────────── HX711 SCK
                          │                            │
                          │  GPIO 21 ◄─────┬──────────┐ I2C SDA
                          │                │          │
                          │  GPIO 22 ◄─────┼────┐     │ I2C SCL
                          │                │    │     │
                          │  GPIO 27 ────────► Motor PWM
                          │                │    │     │
                          │  GPIO 32 ◄───[BTN UP]     │
                          │  GPIO 33 ◄───[BTN DOWN]   │
                          │  GPIO 25 ◄───[BTN MODE]   │
                          │  GPIO 26 ◄───[BTN RESET]  │
                          │                │    │     │
                          │  3.3V   ───────┼────┼─────┼──► Power Rail
                          │                │    │     │
                          │  GND    ───────┼────┼─────┼──► Ground Rail
                          │                │    │     │
                          └────────────────┼────┼─────┼───┘
                                           │    │     │
                                           │    │     │
                                      ┌────▼────▼─────▼────┐
                                      │    I2C Bus         │
                                      ├────────────────────┤
                                      │   OLED (0x3C)      │
                                      │   INA219 (0x40)    │
                                      └────────────────────┘


     Motor Power Circuit
     ===================

     Battery (+) ──► [Current Limiter 2.5A] ──► INA219 VIN+
                                                     │
                        ESP32 GPIO 27 (PWM) ────►[Motor Driver]
                                                     │
                                                INA219 VIN- ──► Motor (+)
                                                     │
     Battery (-) ◄───────────────────────────────Motor (-)
                                                     │
                                                INA219 GND ──► ESP32 GND
```

## Breadboard Layout

### Compact Layout (Recommended)

```
Breadboard View
===============

    Power Rails              Components                  Buttons
    +3.3V   GND
      │      │           [ESP32 DevKit]
      │      │                │
      ├──────┼────────────────┤
      │      │                │
      │      │           [HX711]
      │      │                │
      ├──────┼────────────────┤
      │      │                │
      │      │    I2C Bus ────┼──── [OLED Display]
      │      │            │   │         (on top)
      ├──────┼────────────┼───┤
      │      │            │   │
      │      │            └───┼──── [INA219]
      │      │                │
      │      │                │
      │      └────[BTN UP]────┤
      │      └────[BTN DOWN]──┤
      │      └────[BTN MODE]──┤
      │      └────[BTN RESET]─┤
      │                       │
      └───────────────────────┘
```

### Step-by-Step Breadboard Assembly

**Step 1: Place ESP32**
- Insert ESP32 in center of breadboard
- Straddle the center divider

**Step 2: Connect Power Rails**
- 3.3V pin → Positive rail
- GND pin → Negative rail

**Step 3: Add HX711**
- Place near ESP32
- Wire VCC, GND, DT (GPIO 16), SCK (GPIO 4)

**Step 4: Add I2C Devices**
- Connect OLED: SDA (GPIO 21), SCL (GPIO 22), VCC, GND
- Connect INA219: SDA (GPIO 21), SCL (GPIO 22), VCC, GND
- Both share same I2C pins

**Step 5: Add Buttons**
- 4 tactile push buttons
- One side to GPIO, other side to GND
- UP (32), DOWN (33), MODE (25), RESET (26)

**Step 6: Motor Connections**
- PWM output (GPIO 27) to motor driver
- INA219 in series with motor power

## PCB Considerations (Optional)

For a permanent installation, consider designing a PCB:

**PCB Layout Tips:**
- Place ESP32 module in center
- OLED display on one edge (easy viewing)
- Buttons in a row at bottom
- HX711 and INA219 near their connectors
- Wide power traces (motor current)
- Separate analog and digital grounds

**Connectors:**
- 4-pin for load cell
- 2-pin screw terminals for motor
- 2-pin screw terminals for battery
- USB connector for ESP32 programming

## Wiring Tips

### I2C Bus
1. **Keep I2C wires short** (< 30cm) for reliability
2. **Twist SDA and SCL together** to reduce interference
3. **Add pull-up resistors** if I2C is unreliable:
   - 4.7kΩ from SDA to 3.3V
   - 4.7kΩ from SCL to 3.3V
4. **Check I2C addresses:**
   - OLED: 0x3C or 0x3D (common)
   - INA219: 0x40 (default)

### Power Distribution
1. **Star ground topology** - All grounds meet at one point
2. **Separate motor ground** from sensitive analog ground if possible
3. **Add capacitors:**
   - 100µF electrolytic across ESP32 power
   - 100nF ceramic near each IC (HX711, INA219)
   - 0.1µF ceramic across OLED power

### Motor Wiring
1. **Keep motor wires away** from sensor wires
2. **Add 100µF capacitor** across motor terminals (noise suppression)
3. **Use thicker wires** for motor (16-18 AWG)
4. **Secure all connections** - vibration can loosen wires

### Button Wiring
1. **Use quality tactile switches** (rated 50k+ cycles)
2. **Mount buttons accessible** when case is closed
3. **Label buttons clearly:** ↑ ↓ MODE RESET

## I2C Address Checking

If devices don't work, scan I2C bus:

```cpp
// I2C Scanner Code
#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);  // SDA, SCL
  Serial.println("I2C Scanner");

  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.print("Device at 0x");
      Serial.println(addr, HEX);
    }
  }
}

void loop() {}
```

**Expected Output:**
```
Device at 0x3C  (OLED)
Device at 0x40  (INA219)
```

If OLED shows at 0x3D, change in code:
```cpp
#define SCREEN_ADDRESS  0x3D  // Instead of 0x3C
```

## Power Supply Options

### Option 1: USB Powered (Development/Testing)
```
USB Cable → ESP32 VIN/5V
             │
             ├──► ESP32 (3.3V regulator)
             │
             └──► Motor powered separately (through current limiter)
```

**Pros:** Simple, safe, stable power
**Cons:** Tethered to computer/power bank

### Option 2: Battery Powered (Portable)
```
3.7V LiPo → ESP32 VIN pin
             │
             └──► ESP32 (3.3V regulator)

Separate 2x AA (3V) → Motor (through current limiter)
```

**Pros:** Portable, standalone
**Cons:** Battery management needed

### Option 3: Single Power Supply
```
5V Power Supply → ESP32 VIN
                   │
                   └──► All components

Note: Motor voltage will be 5V (may be too high for Mini 4WD motors)
Use PWM at 50-60% to simulate 2.4-3V
```

## Safety Checklist

Before powering on:

- [ ] All I2C devices on same bus (SDA=21, SCL=22)
- [ ] No shorts between VCC and GND (check with multimeter)
- [ ] Load cell wired correctly (Red→E+, Black→E-)
- [ ] INA219 in series with motor positive wire
- [ ] Current limiter installed and set to 2.5A
- [ ] Buttons wired to GND (not VCC!)
- [ ] Motor polarity correct
- [ ] All grounds connected together (common ground)
- [ ] OLED display contrast visible

## Troubleshooting

### OLED Not Displaying

**Check:**
1. I2C address (0x3C or 0x3D) - use I2C scanner
2. VCC connected to 3.3V (not 5V, unless OLED is 5V tolerant)
3. SDA/SCL not swapped
4. Try adding 4.7kΩ pull-up resistors

### Buttons Not Working

**Check:**
1. Wired to GND (not VCC)
2. GPIO pins correct
3. Button is normally-open (not normally-closed)
4. No short circuits

### HX711 Reads 0 or Unstable

**Check:**
1. Load cell wiring colors
2. VCC is 3.3V stable
3. DT and SCK pins correct
4. Calibration factor sign (+ or -)

### INA219 Not Detected

**Check:**
1. I2C wiring (SDA, SCL)
2. I2C address (default 0x40)
3. Power connections
4. Try I2C scanner

### Motor Doesn't Run

**Check:**
1. PWM pin (GPIO 27) connected
2. Motor driver powered
3. Current limiter not tripped (reset if using polyfuse)
4. PWM duty cycle > 0 in motor control mode

## Component Specifications

### Required Components

| Component | Specification | Quantity | Est. Cost |
|-----------|--------------|----------|-----------|
| ESP32 DevKit | Any ESP32 board | 1 | $8-12 |
| OLED Display | SSD1306 128x64, I2C | 1 | $5-8 |
| HX711 | 24-bit ADC module | 1 | $2-5 |
| Load Cell | 100-200g, beam type | 1 | $5-10 |
| INA219 | I2C current sensor | 1 | $5-8 |
| Tactile Buttons | 12mm, momentary | 4 | $2 |
| XL4015 Module | Current limiter | 1 | $3-5 |
| Breadboard | 830 tie-points | 1 | $3-5 |
| Jumper Wires | Male-male, male-female | 1 set | $3-5 |

**Total: ~$36-60** (complete standalone system)

---

## Next Steps

1. Wire all components according to this diagram
2. Upload `esp32/torque_analyzer_oled.ino` to ESP32
3. Calibrate load cell (see CALIBRATION.md)
4. Test display modes with MODE button
5. Adjust motor PWM with UP/DOWN buttons
6. Reset max values with RESET button

For detailed setup, see: `docs/OLED_SETUP_GUIDE.md`
