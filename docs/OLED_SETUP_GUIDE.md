# OLED Display Version - Complete Setup Guide

Standalone Mini 4WD Torque Analyzer with OLED display - no Raspberry Pi or smartphone needed!

## Overview

This version displays all measurements directly on a 128x64 OLED screen with button controls for navigation.

### What You Can Do

**✓ Measure in real-time:**
- Current (A)
- Torque (mN·m and kg·cm)
- Power (W)
- Voltage (V)

**✓ Track maximum values:**
- Peak current
- Peak torque
- Peak power

**✓ Control motor speed:**
- Adjust PWM (0-100%)
- Set current limit
- Real-time monitoring

**✓ Three display modes:**
1. **REALTIME** - Live measurements
2. **MAX VALUES** - Peak values recorded
3. **MOTOR CONTROL** - Adjust motor speed

## Parts List

### Electronics ($36-60 total)

| Component | Specification | Qty | Cost | Where to Buy |
|-----------|--------------|-----|------|--------------|
| **ESP32 DevKit** | ESP32-WROOM-32 | 1 | $8-12 | Amazon, AliExpress |
| **OLED Display** | SSD1306 128x64 I2C | 1 | $5-8 | Amazon, Adafruit |
| **HX711 Module** | 24-bit ADC | 1 | $2-5 | Amazon, Sparkfun |
| **Load Cell** | 100-200g beam | 1 | $5-10 | Amazon, eBay |
| **INA219** | I2C current sensor | 1 | $5-8 | Amazon, Adafruit |
| **Tactile Buttons** | 12mm momentary | 4 | $2 | Amazon, local |
| **XL4015 Module** | Current limiter | 1 | $3-5 | Amazon, eBay |
| **Breadboard** | 830 tie-points | 1 | $3-5 | Amazon, local |
| **Jumper Wires** | M-M, M-F set | 1 | $3-5 | Amazon, local |

### Mechanical Parts ($5-10)

- Aluminum strip 80mm × 15mm × 2mm (lever arm)
- Wooden base 150mm × 150mm
- M3 screws and nuts
- Motor mount (DIY or 3D printed)

### Tools

- Soldering iron and solder
- Multimeter
- Wire strippers
- Small screwdriver
- Hot glue gun (optional)

## Software Setup

### Step 1: Install Arduino IDE

1. Download Arduino IDE: https://www.arduino.cc/en/software
2. Install for your operating system
3. Launch Arduino IDE

### Step 2: Install ESP32 Board Support

1. Open Arduino IDE
2. Go to `File > Preferences`
3. In "Additional Board Manager URLs", add:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Click OK
5. Go to `Tools > Board > Boards Manager`
6. Search "esp32"
7. Install "esp32 by Espressif Systems"
8. Wait for installation to complete

### Step 3: Install Required Libraries

Go to `Sketch > Include Library > Manage Libraries` and install:

1. **HX711** by Bogdan Necula
   - Search "HX711"
   - Install latest version

2. **Adafruit INA219** by Adafruit
   - Search "Adafruit INA219"
   - Install latest version

3. **Adafruit GFX Library** by Adafruit
   - Search "Adafruit GFX"
   - Install latest version

4. **Adafruit SSD1306** by Adafruit
   - Search "Adafruit SSD1306"
   - Install latest version

### Step 4: Configure Arduino IDE

1. Connect ESP32 to computer via USB
2. Select board: `Tools > Board > ESP32 Arduino > ESP32 Dev Module`
3. Select port: `Tools > Port > [Your ESP32 Port]`
   - Windows: COM3, COM4, etc.
   - Mac: /dev/cu.usbserial-XXXX
   - Linux: /dev/ttyUSB0, /dev/ttyUSB1
4. Set upload speed: `Tools > Upload Speed > 115200`

**Troubleshooting - Port not showing:**
- Install CH340 or CP2102 USB driver (depends on your ESP32 board)
- Try different USB cable
- Check Device Manager (Windows) or System Report (Mac)

## Hardware Assembly

### Step 1: Prepare Breadboard

1. **Place ESP32** in center of breadboard
2. **Connect power rails:**
   - ESP32 3.3V → Breadboard positive rail (+)
   - ESP32 GND → Breadboard negative rail (-)

### Step 2: Wire HX711 Load Cell

**HX711 to ESP32:**
```
HX711 VCC → 3.3V
HX711 GND → GND
HX711 DT  → GPIO 16
HX711 SCK → GPIO 4
```

**Load Cell to HX711:**
```
Red wire   → E+
Black wire → E-
White wire → A-
Green wire → A+
```

*Note: If readings are inverted, swap A+ and A-*

### Step 3: Wire INA219 Current Sensor

**INA219 to ESP32 (I2C):**
```
INA219 VCC → 3.3V
INA219 GND → GND
INA219 SDA → GPIO 21
INA219 SCL → GPIO 22
```

### Step 4: Wire OLED Display

**OLED to ESP32 (I2C - shares bus with INA219):**
```
OLED VCC → 3.3V
OLED GND → GND
OLED SDA → GPIO 21 (same as INA219)
OLED SCL → GPIO 22 (same as INA219)
```

*Note: Both OLED and INA219 connect to same SDA/SCL pins*

### Step 5: Wire Control Buttons

**Four tactile buttons:**

```
Button UP:
  One side → GPIO 32
  Other side → GND

Button DOWN:
  One side → GPIO 33
  Other side → GND

Button MODE:
  One side → GPIO 25
  Other side → GND

Button RESET:
  One side → GPIO 26
  Other side → GND
```

**Button layout suggestion:**
```
┌─────┐  ┌─────┐
│ UP  │  │MODE │
└─────┘  └─────┘

┌─────┐  ┌─────┐
│DOWN │  │RESET│
└─────┘  └─────┘
```

### Step 6: Wire Motor and Current Limiter

**Battery → Current Limiter → INA219 → Motor**

```
1. Battery (+) → XL4015 IN+
2. Battery (-) → XL4015 IN-
3. XL4015 OUT+ → INA219 VIN+
4. XL4015 OUT- → Battery (-) [common ground]
5. INA219 VIN- → Motor (+)
6. Motor (-) → Battery (-)
7. INA219 GND → ESP32 GND [IMPORTANT!]
```

### Step 7: Wire Motor PWM Control

**For motor driver (BTS7960 or L298N):**
```
ESP32 GPIO 27 → Motor Driver PWM/EN pin
ESP32 GND → Motor Driver GND
Motor Driver → Motor terminals
```

**For direct MOSFET control:**
```
ESP32 GPIO 27 → 1kΩ resistor → MOSFET Gate
MOSFET Drain → Battery (+)
MOSFET Source → Motor (+)
Motor (-) → Battery (-)
```

### Step 8: Verify Wiring

**Before powering on, check with multimeter:**

1. ✓ No shorts between 3.3V and GND
2. ✓ All I2C devices on same SDA/SCL (GPIO 21, 22)
3. ✓ Buttons connected to GND (not 3.3V!)
4. ✓ Load cell wiring correct
5. ✓ All grounds connected together
6. ✓ INA219 in series with motor power

## Software Configuration

### Step 1: Open Firmware File

1. Navigate to project folder: `mini4wd-torque-analyzer/esp32/`
2. Open `torque_analyzer_oled.ino` in Arduino IDE

### Step 2: Configure Settings

**Update these values in the code:**

```cpp
// ==================== MEASUREMENT CONFIGURATION ====================
#define TORQUE_ARM_RADIUS_MM  60.0   // YOUR lever arm length in mm
#define CALIBRATION_FACTOR  7050.0   // Load cell calibration factor
#define SAMPLE_RATE_MS  100          // Update rate (100ms = 10Hz)

// Motor protection
#define MAX_CURRENT_LIMIT_MA  2500.0 // Current limit (2.5A)
```

**Important measurements:**

1. **TORQUE_ARM_RADIUS_MM**
   - Measure from motor shaft center to load cell contact point
   - Use calipers or precise ruler
   - Example: 60mm
   - This MUST be accurate for correct torque readings!

2. **CALIBRATION_FACTOR**
   - Start with 7050.0 (typical value)
   - Calibrate later with known weight
   - Positive for compression, negative for tension

3. **MAX_CURRENT_LIMIT_MA**
   - Set to 2500 (2.5A) for standard motors
   - Can increase to 3000 for high-torque motors

### Step 3: Check I2C Addresses

**Default addresses:**
- OLED: 0x3C
- INA219: 0x40

If OLED doesn't work, try changing:
```cpp
#define SCREEN_ADDRESS  0x3D  // Instead of 0x3C
```

### Step 4: Upload Firmware

1. Connect ESP32 via USB
2. Select correct board and port in Arduino IDE
3. Click **Upload** button (→)
4. Wait for "Done uploading" message
5. Open Serial Monitor: `Tools > Serial Monitor`
6. Set baud rate to **115200**

**Expected output:**
```
Mini 4WD Torque Analyzer - OLED Version
========================================
Initializing HX711...
HX711 initialized and tared.
Initializing INA219...
INA219 initialized.
Buttons initialized.
PWM initialized.
Setup complete!
```

**If you see errors:**
- "SSD1306 OLED not found!" → Check OLED wiring or I2C address
- "Failed to find INA219" → Check INA219 wiring
- "HX711 not ready" → Check load cell wiring

## Load Cell Calibration

### Quick Calibration Procedure

**Step 1: Zero the scale**
1. Remove all weight from load cell
2. Press ESP32 reset button
3. Load cell automatically tares (zeros)

**Step 2: Apply known weight**
1. Apply 100g weight to load cell
2. Watch Serial Monitor
3. Note the displayed weight

**Step 3: Calculate calibration factor**

Formula:
```
New Factor = Old Factor × (Known Weight / Displayed Weight)
```

Example:
```
Known weight: 100.0g
Displayed weight: 87.3g
Old factor: 7050

New factor = 7050 × (100.0 / 87.3) = 8076
```

**Step 4: Update code**
```cpp
#define CALIBRATION_FACTOR  8076.0  // Your calculated value
```

**Step 5: Re-upload and verify**
1. Upload updated code
2. Apply 100g weight again
3. Should now read 100.0g ±2g

**For detailed calibration, see:** `docs/CALIBRATION.md`

## Using the Analyzer

### Display Modes

The analyzer has **3 display modes**. Press **MODE** button to cycle through:

#### Mode 1: REALTIME (Default)

Shows live measurements:
```
=== REALTIME ===
I: 0.45 A
T: 58.3 mN.m
T: 0.59 kg.cm
P: 1.24 W
V: 2.76 V
```

**What it shows:**
- **I** = Current draw (Amperes)
- **T** = Torque in milliNewton-meters
- **T** = Torque in kilogram-centimeters
- **P** = Power consumption (Watts)
- **V** = Voltage under load

**Units explained:**
- **mN·m** (milliNewton-meter) = SI unit, scientific
- **kg·cm** (kilogram-centimeter) = Common in hobby motors
- **Conversion:** 1 kg·cm = 98.0665 mN·m

#### Mode 2: MAX VALUES

Shows peak values recorded since last reset:
```
=== MAX VALUES ===
I: 2.45 A
T: 124.5 mN.m
T: 1.27 kg.cm
P: 3.82 W

RESET to clear
```

**Use this to:**
- Record stall torque
- Find peak current draw
- Measure maximum power
- Compare different motors

**To reset max values:** Press **RESET** button

#### Mode 3: MOTOR CONTROL

Control motor speed and view limits:
```
=== MOTOR CTRL ===
PWM: 75 %
Limit: 2.5 A
Current: 1.85 A
Torque: 0.92 kg.cm

UP/DN: Adjust PWM
```

**Controls:**
- **UP button** → Increase PWM (motor speed)
- **DOWN button** → Decrease PWM (motor speed)
- PWM range: 0-100%
- Current limiting active if exceeds limit

**Safety features:**
- Automatic current limiting
- Motor only runs in MOTOR CONTROL mode
- Auto-shutdown if current too high

### Button Functions

**UP Button (GPIO 32)**
- Mode 3: Increase motor PWM (+10%)

**DOWN Button (GPIO 33)**
- Mode 3: Decrease motor PWM (-10%)

**MODE Button (GPIO 25)**
- Cycle through display modes:
  - REALTIME → MAX VALUES → MOTOR CONTROL → REALTIME

**RESET Button (GPIO 26)**
- Mode 2: Reset all maximum values to zero

### Testing Procedure

#### Test 1: No Load Test

1. Mode: **REALTIME**
2. Motor disconnected or not running
3. Should show:
   - Current: ~0 A
   - Torque: ~0 mN·m
   - Power: ~0 W
   - Voltage: Battery voltage

#### Test 2: Free Run Test

1. Mode: **MOTOR CONTROL**
2. Press UP to increase PWM to 50%
3. Motor runs freely (no load on shaft)
4. Should show:
   - Current: 0.2-0.5 A
   - Torque: low (motor friction only)
   - PWM: 50%

#### Test 3: Load Test

1. Mode: **MOTOR CONTROL**
2. PWM at 100%
3. Lever arm pushes against load cell
4. Should show:
   - Current: 0.5-1.5 A
   - Torque: Actual motor torque
   - Force reading on load cell

#### Test 4: Stall Torque Test ⚠️

**IMPORTANT: Only with current limiting!**

1. Mode: **MAX VALUES** (to record peak)
2. Press MODE to switch to **MOTOR CONTROL**
3. Increase PWM to 100%
4. Motor shaft blocked (maximum load)
5. Current should be limited to 2.5A
6. Test for **maximum 3 seconds**
7. Press DOWN to reduce PWM to 0%
8. Switch to MAX VALUES mode to see peak torque

**Safety:**
- ⚠️ Always use current limiter
- ⚠️ Maximum 3 seconds stall time
- ⚠️ Let motor cool 30 seconds between tests
- ⚠️ Watch for smoke/overheating

## Current Limiter Setup (XL4015)

**CRITICAL:** Set current limit before testing!

### Adjusting XL4015 Module

**You need:**
- Multimeter (ammeter mode)
- Small screwdriver
- Wire for short circuit

**Step 1: Set output voltage**
1. Connect battery to XL4015 IN+ and IN-
2. Measure voltage at OUT+ and OUT-
3. Adjust **CV potentiometer** until output = battery voltage
   - 2x AA: Set to 2.4-3.0V
4. Power LED should be on (red)

**Step 2: Set current limit**
1. **DISCONNECT motor!**
2. Connect ammeter: OUT+ → Ammeter → Wire
3. **Short circuit:** Touch wire to OUT-
4. Adjust **CC potentiometer** while watching ammeter
5. When ammeter reads **2.5A**, stop adjusting
6. Charge LED turns on (blue/green) - this is correct!
7. Remove short circuit

**Step 3: Verify**
1. Connect motor normally
2. Run at full stall (blocked shaft)
3. Current should not exceed 2.5A
4. If motor stops during stall = current limiting working ✓

**For detailed instructions:** `docs/MOTOR_PROTECTION.md`

## Troubleshooting

### OLED Display Blank

**Possible causes:**
1. Wrong I2C address (try 0x3D instead of 0x3C)
2. Not enough power (check 3.3V with multimeter)
3. Contrast too low (normal for new displays)
4. Wiring incorrect (check SDA/SCL)

**Solutions:**
- Run I2C scanner (see WIRING_OLED.md)
- Check wiring with multimeter (continuity mode)
- Try different OLED module (some are defective)
- Add 4.7kΩ pull-up resistors on SDA and SCL

### Buttons Don't Respond

**Check:**
- Buttons wired to GND (not 3.3V!)
- GPIO pins correct (32, 33, 25, 26)
- Button is normally-open type
- Use Serial Monitor to see button presses

### Load Cell Reads Wrong Values

**Check:**
- Calibration factor (recalibrate with known weight)
- Load cell wiring (correct color code)
- Lever arm measurement (must be precise)
- Sign of calibration factor (+ for compression, - for tension)

### Current Reading Always Zero

**Check:**
- INA219 wiring (SDA, SCL, VCC, GND)
- INA219 power connections (VIN+, VIN-, GND)
- Motor actually drawing current (motor on?)
- I2C address (should be 0x40)

### Motor Won't Run

**Check:**
- PWM wiring (GPIO 27)
- Motor driver powered and enabled
- In MOTOR CONTROL mode (motor only runs in this mode)
- PWM value > 0 (increase with UP button)
- Current limiter not tripped

### Current Exceeds 3A (Dangerous!)

**STOP IMMEDIATELY!**
- Current limiter not working or bypassed
- Do NOT continue testing
- Check XL4015 setup
- Verify current limit with ammeter

## Expected Values

### Typical Mini 4WD Motors (2.4-3V)

**Free Running (No Load):**
| Motor | Current | Torque | Power |
|-------|---------|--------|-------|
| Standard | 0.2-0.5A | 0.2-0.4 mN·m | 0.5-1.0W |
| High-Speed | 0.1-0.3A | 0.1-0.3 mN·m | 0.3-0.7W |
| Torque | 0.3-0.6A | 0.3-0.5 mN·m | 0.7-1.2W |

**Under Load:**
| Motor | Current | Torque | Power |
|-------|---------|--------|-------|
| Standard | 0.5-1.0A | 0.5-1.0 mN·m | 1.0-2.5W |
| High-Speed | 0.3-0.8A | 0.3-0.7 mN·m | 0.7-2.0W |
| Torque | 0.8-1.5A | 1.0-2.0 mN·m | 1.5-4.0W |

**Stall (With 2.5A Limit):**
| Motor | Current | Torque | Power |
|-------|---------|--------|-------|
| Standard | 2.5A | 3-5 mN·m | 6-7W |
| High-Speed | 2.0A | 2-4 mN·m | 5-6W |
| Torque | 2.5A | 5-8 mN·m | 7-8W |

**Torque Conversion:**
- 1 mN·m = 0.0102 kg·cm
- 1 kg·cm = 98.07 mN·m

**Example:**
- 50 mN·m = 0.51 kg·cm
- 100 mN·m = 1.02 kg·cm

## Usage Tips

### Comparing Motors

1. **Record baseline:**
   - Test first motor
   - Note max torque in MAX VALUES mode
   - Write down values

2. **Test second motor:**
   - Press RESET to clear max values
   - Test second motor same way
   - Compare results

3. **Make a chart:**
   ```
   Motor A: 1.24 kg·cm stall torque, 2.3A
   Motor B: 0.87 kg·cm stall torque, 1.8A
   → Motor A is 43% stronger
   ```

### Motor Break-In Testing

1. Run motor at 50% PWM for 5 minutes
2. Record torque every minute
3. Torque should increase as motor breaks in
4. Plot torque vs. time

### Battery Discharge Test

1. Fully charged battery
2. Run motor at constant load
3. Record voltage and torque over time
4. See how battery affects performance

### Gear Ratio Optimization

1. Test motor with different gear ratios
2. Record torque and speed
3. Find optimal ratio for your application

## Advanced Features

### Serial Monitor Output

Open Serial Monitor (115200 baud) to see:
```
I:0.45A T:58.3mN.m T:0.59kg.cm P:1.24W V:2.76V PWM:75%
I:0.47A T:61.2mN.m T:0.62kg.cm P:1.29W V:2.75V PWM:75%
```

**Use for:**
- Data logging (copy to spreadsheet)
- Debugging
- Fine-tuning calibration

### Future Enhancements

**Can be added:**
1. RPM sensor (Hall effect or IR)
2. Temperature sensor (on motor)
3. Data logging to SD card
4. Efficiency calculations (mechanical vs electrical power)
5. Bluetooth for phone app
6. Add Raspberry Pi for web dashboard (original design)

## Maintenance

### Regular Checks

**Every session:**
- Verify load cell reads 0 with no load
- Check button responsiveness
- Inspect wiring for loose connections

**Weekly:**
- Re-calibrate load cell
- Clean OLED screen
- Tighten lever arm if loose

**Monthly:**
- Check battery voltage
- Verify current limiter setting
- Update firmware if improvements available

## Summary

You now have a standalone torque analyzer!

**What you built:**
- ✓ Real-time torque measurement
- ✓ Current and power monitoring
- ✓ Maximum value tracking
- ✓ Motor speed control
- ✓ Safety current limiting
- ✓ Portable, no computer needed

**Next steps:**
1. Test your Mini 4WD motors
2. Compare different motors
3. Optimize your setup
4. Share your results!

**Optional upgrades:**
- Add Raspberry Pi for web dashboard
- Design custom PCB
- 3D print enclosure
- Add more sensors

---

For more information:
- `docs/WIRING_OLED.md` - Detailed wiring
- `docs/MOTOR_PROTECTION.md` - Current limiting
- `docs/CALIBRATION.md` - Precise calibration
- `docs/MECHANICAL_SETUP_LEVER.md` - Lever arm design

**Happy testing! 🏎️⚡**
