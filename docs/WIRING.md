# Wiring Diagram

## ESP32 Pinout Connections

### Load Cell + HX711 Amplifier

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
White  → A-
Green  → A+
```

### INA219 Current Sensor

```
INA219 → ESP32
-----------------------
VCC    → 3.3V
GND    → GND
SDA    → GPIO 21 (I2C SDA)
SCL    → GPIO 22 (I2C SCL)

INA219 Power Connections
-----------------------
VIN+   → Motor Battery Positive
VIN-   → Motor Positive Wire
GND    → Motor Battery Negative (common ground)
```

### Complete System Wiring

```
                                    ESP32
                          ┌──────────────────────┐
                          │                      │
                          │    GPIO 16 ◄──────── DT   (HX711)
                          │    GPIO 4  ◄──────── SCK  (HX711)
                          │                      │
                          │    GPIO 21 ◄──────── SDA  (INA219)
                          │    GPIO 22 ◄──────── SCL  (INA219)
                          │                      │
                          │    3.3V   ───────────┬──► HX711 VCC
                          │                      └──► INA219 VCC
                          │                      │
                          │    GND    ───────────┬──► HX711 GND
                          │                      └──► INA219 GND
                          │                      │
                          └──────────────────────┘


     Mini 4WD Motor Test Setup
     ========================

     Battery (+) ─────► INA219 VIN+
                           │
                      INA219 VIN- ─────► Motor (+)
                           │
                      Motor (-) ─────► Battery (-)
                           │
                      INA219 GND ────► ESP32 GND (common ground)


     Load Cell Mechanical Setup (Lever Arm Design)
     ==============================================

              ┌─────────────┐
              │   Motor     │  ← Mounted vertically
              └──────┬──────┘
                     │ shaft (pointing up)
                     │
              =======●======= Lever arm (horizontal, 60-80mm)
                     ↓
                     ↓ Pushes down (compression)
              ┌─────────────┐
              │  Load Cell  │  ← Measures compression force
              └──────┬──────┘
                     │
                Fixed Mount

     Note: Motor shaft rotates lever arm, which pushes down on load cell.
     Torque = Force × Lever arm length (measured from shaft center to contact point)

     IMPORTANT: The motor doesn't push directly down! The motor TRIES TO ROTATE
     the lever arm, but the load cell RESISTS that rotation. This resistance
     creates the measurable force. Think of it like trying to open a door that
     someone is blocking - the harder you push, the more force is measured.

     See MECHANICAL_SETUP_LEVER.md for detailed physics explanation and build instructions.
```

## Motor Protection Circuit (IMPORTANT!)

### Why Protection is Needed

⚠️ **WARNING:** During stall torque testing, Mini 4WD motors can draw **5-7A**, which can:
- Burn out the motor (normal operation: 0.5-1.5A)
- Damage the INA219 sensor (max 3.2A)
- Create fire hazard
- Destroy test equipment

**Solution:** Add current limiting between battery and motor.

### Protection Circuit Wiring

```
     Motor Power Path with Current Limiter
     ======================================

     Battery (+) ──► [Current Limiter] ──► INA219 VIN+
                                               │
                                          INA219 VIN- ──► Motor (+)
                                               │
     Battery (-) ◄──────────────────────── Motor (-)
                            │
                       INA219 GND ──► ESP32 GND
```

### Option 1: Adjustable Current Limiter Module (Easiest)

**Use XL4015 or LM2596 Buck Converter with Current Limit**

Components:
- XL4015 DC-DC Buck Converter Module (~$3-5)
- Has adjustable current limiting (set to 2-3A)

Wiring:
```
XL4015 Module Connections
-------------------------
IN+     → Battery (+)
IN-     → Battery (-)
OUT+    → INA219 VIN+
OUT-    → Battery (-) (common ground)

Adjustment:
- Set output voltage to battery voltage (2.4V or 3V)
- Set current limit to 2.5A using CC adjustment pot
```

**How to Set Current Limit:**
1. Connect input power
2. Short OUT+ to OUT- with ammeter in series
3. Adjust CC potentiometer until ammeter reads 2.5A
4. Remove short circuit
5. Current is now limited to 2.5A

### Option 2: Polyfuse (Simple, Passive)

**Use Resettable Polyfuse (PPTC)**

Components:
- 2.5A or 3A Polyfuse (~$1-2)
- Part example: Bourns MF-R250 (2.5A hold, 5A trip)

Wiring:
```
Battery (+) ──► [Polyfuse] ──► INA219 VIN+
```

**How it Works:**
- Normal operation: Low resistance, motor runs freely
- High current (>5A): Heats up, becomes high resistance
- Automatically limits current
- Resets when cool (~30 seconds)

**Pros:**
- Very simple (just add one component)
- No adjustment needed
- Automatic reset

**Cons:**
- Slower response than active limiters
- May allow brief current spikes
- Voltage drop at high current

### Option 3: DIY MOSFET Current Limiter (Advanced)

**Build Your Own with MOSFET + Op-Amp**

Components:
- IRLZ44N MOSFET (logic-level, 47A max)
- LM358 Op-Amp
- 0.1Ω, 5W current sense resistor
- 10kΩ potentiometer
- Resistors: 10kΩ (x2), 1kΩ
- Small heatsink for MOSFET

Circuit:
```
Battery (+) ──► MOSFET Drain
                   │
              MOSFET Source ──┬──► [0.1Ω Sense] ──► INA219 VIN+
                              │
                         Sense voltage to Op-Amp
                         (compares to limit setpoint)
                              │
                         Op-Amp output ──► MOSFET Gate
```

**See MOTOR_PROTECTION.md for complete schematic and PCB files.**

### Option 4: Motor Driver Module with Current Limit

**Use L298N or BTS7960 Motor Driver**

Components:
- BTS7960 43A Motor Driver Module (~$5-10)
- Has built-in current sensing
- Can set current limit via PWM

Wiring:
```
BTS7960 Module
--------------
B+      → Battery (+)
B-      → Battery (-)
M+      → Motor (+)
M-      → Motor (-)
VCC     → 5V (for logic)
GND     → ESP32 GND
R_EN    → ESP32 GPIO 25 (PWM control)
L_EN    → GND (or ESP32 GPIO for reverse)
```

**Current Limiting:**
- Use PWM to limit motor power
- Monitor INA219 current in ESP32 code
- Reduce PWM duty cycle if current exceeds 2.5A

### Recommended Solution

**For Beginners: Option 1 (XL4015 Module)**
- Easy to buy and install
- Adjustable current limit
- Good protection

**For Simple Protection: Option 2 (Polyfuse)**
- Cheapest and simplest
- Good enough for basic testing
- Add inline with battery (+)

**For Advanced Users: Option 3 (DIY Circuit)**
- Most precise control
- Fast response time
- Learn electronics

**For PWM Control: Option 4 (Motor Driver)**
- Software-controlled current limiting
- Can vary motor speed
- Good for automated testing

### Wiring with Current Limiter

```
Complete Protected System
=========================

                                    ESP32
                          ┌──────────────────────┐
                          │                      │
                          │    GPIO 16 ◄──────── DT   (HX711)
                          │    GPIO 4  ◄──────── SCK  (HX711)
                          │    GPIO 21 ◄──────── SDA  (INA219)
                          │    GPIO 22 ◄──────── SCL  (INA219)
                          │    GPIO 25 ──────────► PWM (optional, for motor driver)
                          │                      │
                          │    3.3V   ───────────┬──► HX711 VCC
                          │                      └──► INA219 VCC
                          │    GND    ──────────────► Common GND
                          └──────────────────────┘

     Motor Power (Protected)
     =======================

     Battery (+) ──► [Current Limiter 2.5A] ──► INA219 VIN+
                                                     │
                                                INA219 VIN- ──► Motor (+)
                                                     │
     Battery (-) ◄───────────────────────────────Motor (-)
                                                     │
                                                INA219 GND ──► ESP32 GND
```

### Testing Current Limit

1. **Set up current limiter** (adjust to 2.5A)
2. **Connect ammeter** between limiter and motor
3. **Power on system**
4. **Run motor at full load (stall)**
5. **Verify current stays below 2.5A**
6. **Motor should still run, but not burn out**

### Current Limit Recommendations

| Motor Type | Normal Current | Stall Current | Recommended Limit |
|------------|----------------|---------------|-------------------|
| Standard Mini 4WD | 0.3-0.8A | 5-7A | 2.0-2.5A |
| High-speed | 0.2-0.6A | 4-6A | 1.5-2.0A |
| Torque-tuned | 0.5-1.2A | 7-10A | 2.5-3.0A |

**Balance:**
- Too low limit: Motor won't reach full torque
- Too high limit: Motor can still burn out
- **Sweet spot: 2.5A for most Mini 4WD motors**

### Safety Notes

- **Always use current limiting for stall tests**
- **Test for maximum 3 seconds during stall**
- **Let motor cool 30 seconds between tests**
- **Monitor motor temperature** (should be warm, not hot to touch)
- **Have emergency stop** (disconnect battery quickly)
- **Do not bypass protection** for stall testing

## Power Supply Options

### Option 1: USB Powered (Testing/Development)
- ESP32 powered via USB from computer
- Motor powered by Mini 4WD battery (2.4V or 3V)
- **Important:** Connect ESP32 GND to battery GND (common ground)

### Option 2: Battery Powered (Portable)
- ESP32 powered by 3.7V LiPo battery via VIN pin (or USB power bank)
- Motor powered by Mini 4WD battery
- **Important:** Connect all GND together (common ground)

## Wiring Tips

1. **Keep wires short** - Reduces noise in analog readings
2. **Twist I2C wires** - SDA and SCL twisted together reduces interference
3. **Separate power domains** - ESP32 and motor on different power sources (but share GND)
4. **Add capacitors** (optional):
   - 100µF across motor terminals (reduces noise)
   - 0.1µF near INA219 VCC (stabilizes readings)
5. **Strain relief** - Secure wires to prevent disconnection during testing
6. **Common ground is critical** - All GND must be connected together

## Safety Notes

- **Do not exceed 26V on INA219** - Check your motor battery voltage
- **Do not exceed 3.3V on ESP32 GPIO pins** - Use voltage dividers if needed
- **Verify polarity** - Reversed polarity can damage components
- **Start with low motor speeds** - Test the system before full throttle
- **Watch for overheating** - INA219 can heat up with high current (>3A)

## Verification Steps

1. **Before powering on:**
   - Double-check all connections with multimeter
   - Verify no short circuits
   - Check polarity of load cell wires

2. **Power-on sequence:**
   - Connect ESP32 USB first (powers ESP32)
   - Connect motor battery last
   - Check serial monitor for sensor readings

3. **Test each sensor:**
   - HX711: Should see stable reading when load cell is unloaded
   - INA219: Should see voltage reading matching battery voltage
   - Apply small force to load cell: reading should change
   - Run motor briefly: current reading should increase
