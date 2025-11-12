# DC Motor Torque Analyzer

**Standalone stall torque measurement system with OLED display**

## Quick Overview

Standalone system with direct OLED display:

```
ESP32 + OLED Display + Buttons
        ↓
   Standalone System
```

### Features

✅ **Real-time display** on 128x64 OLED screen
✅ **Button controls** for easy navigation
✅ **Three display modes** (Realtime, Maximum Values, Motor Control)
✅ **Fixed voltage control** with current regulation
✅ **Current limiting** for motor protection
✅ **Portable** - Battery powered operation

## What It Displays

### Current Measurements
- **I** (A) - Current draw
- **T** (mN·m) - Torque in milliNewton-meters
- **T** (kg·cm) - Torque in kilogram-centimeters
- **T** (mN·m) - Torque per Amp
- **P** (W) - Power consumption
- **V** (V) - Voltage under load

### Maximum Values
- Peak current
- Peak torque (both units)
- Peak power

### Motor Control
- Fixed voltage setting
- Current limit setting
- Real-time monitoring

## Components Needed

### Electronics

| Component | 
|-----------|
| ESP32 DevKit/Arduino Etc. |
| OLED Display (SSD1306 128x64) | 
| HX711 + Load Cell (100-200g) | 
| INA219 Current Sensor | 
| XL4015 Current Limiter |
| 4x Tactile Buttons |
| Breadboard + Wires |

### Mechanical
- Aluminum lever arm (80mm strip)
- Wooden base (150mm square)
- M3 hardware
- Motor mount

> **💡 How Torque Measurement Works:**
>
> The motor tries to **rotate** the lever arm, but the load cell **resists** that rotation. This resistance creates a measurable force. It's like trying to open a door that someone is blocking - the harder you push, the more force is measured. Force × lever length = torque!
>
> **Detailed explanation:** `docs/MECHANICAL_SETUP_LEVER.md`

## Quick Start

### 1. Hardware Setup (30 min)

**Wire components:**
```
ESP32 Pins:
├─ GPIO 16, 4    → HX711 (Load Cell)
├─ GPIO 21, 22   → I2C (OLED + INA219)
├─ GPIO 27       → Motor PWM
└─ GPIO 32,33,25,26 → Buttons (UP, DOWN, MODE, RESET)
```

**See:** `docs/WIRING_OLED.md` for complete wiring diagram

### 2. Software Setup (15 min)

**Install libraries:**
- HX711 (Bogdan Necula)
- Adafruit INA219
- Adafruit GFX
- Adafruit SSD1306

**Upload firmware:**
- Open `esp32/torque_analyzer_oled.ino`
- Update lever arm length: `TORQUE_ARM_RADIUS_MM`
- Upload to ESP32

### 3. Calibration (10 min)

1. Remove weight from load cell
2. Reset ESP32 (auto-tares)
3. Apply 100g weight
4. Calculate new calibration factor
5. Update code and re-upload

**See:** `docs/CALIBRATION.md` for details

### 4. Current Limiter Setup (10 min)

**Set XL4015 to 2.5A:**
1. Adjust output voltage to match battery
2. Short circuit output with ammeter
3. Adjust CC pot until 2.5A
4. Done!

**See:** `docs/MOTOR_PROTECTION.md` for details

## Using the Display

### Button Controls

```
┌─────────┐  ┌─────────┐
│   UP    │  │  MODE   │  ← Top row
│ GPIO 32 │  │ GPIO 25 │
└─────────┘  └─────────┘

┌─────────┐  ┌─────────┐
│  DOWN   │  │  RESET  │  ← Bottom row
│ GPIO 33 │  │ GPIO 26 │
└─────────┘  └─────────┘
```

**Functions:**
- **UP** - Increase current limit
- **DOWN** - Decrease current limit  
- **MODE** - Cycle through display modes
- **RESET** - Clear maximum values

### Display Modes

Press **MODE** button to cycle through:

#### Mode 1: Realtime Display
```
=== REALTIME ===
I: 2.45 A
T: 8.83 mN.m
T: 0.90 kg.cm
P: 3.68 W (V×I)
V: 1.50 V
```
Shows live measurements during stall test

#### Mode 2: Maximum Values
```
=== MAX STALL ===
Max I: 2.5 A
Max T: 8.9 mN.m
Max T: 0.91 kg.cm
Max P: 3.75 W
```
Shows peak stall values recorded

#### Mode 3: Test Control
```
=== TEST CTRL ===
V: 1.5V FIXED
I Limit: 2.5A
MOTOR: ON
T: 8.1 mN.m
```
Fixed voltage with current limiting for stall tests

## Torque Units Explained

**mN·m (milliNewton-meter)**
- SI unit, scientific
- Used in technical specs
- Example: 58.3 mN·m

**kg·cm (kilogram-centimeter)**
- Common in hobby/RC motors
- Easier to understand
- Example: 0.59 kg·cm

**Conversion:**
- 1 kg·cm = 98.0665 mN·m
- 1 mN·m = 0.0102 kg·cm

## Safety Features

✅ **Automatic current limiting** (1.0A default)
✅ **Motor only runs in Control mode**
✅ **Visual current display**
✅ **Protection warnings**
✅ **Emergency stop** (DOWN button to 0%)

⚠️ **Important:**
- Always use current limiter for stall tests
- Maximum 10 seconds stall time
- Let motor cool 30 seconds between tests

## Testing Guide

### Stall Torque Test
1. Press MODE to enter TEST CONTROL mode
2. Set current limit with UP/DOWN buttons
3. Turn motor ON (fixed voltage applied)
4. Block motor shaft completely
5. Observe stall torque and current on display
6. Test for maximum 3 seconds
7. Turn motor OFF
8. Press MODE to view MAX STALL for peak readings

**Expected stall torque:**
- Small DC motor: 5-8 mN·m (0.5-0.8 kg·cm)
- Gear motor: 8-15 mN·m (0.8-1.5 kg·cm)

## Typical Stall Values (Small DC Motors)

### Stall Torque @ Current Limit
| Motor Type | Current Limit | Stall Torque (mN·m) | Stall Torque (kg·cm) |
|------------|---------------|---------------------|----------------------|
| Small DC (6V) | 2.5A | 5-8 | 0.51-0.82 |
| Gear Motor | 2.0A | 8-15 | 0.82-1.53 |
| High Torque | 3.0A | 10-20 | 1.02-2.04 |

## Documentation

**Quick Start:**
- `README.md` - This file
- `docs/OLED_SETUP_GUIDE.md` - Complete setup guide

**Technical Details:**
- `docs/WIRING_OLED.md` - Wiring diagrams
- `docs/MECHANICAL_SETUP_LEVER.md` - Lever arm build
- `docs/MOTOR_PROTECTION.md` - Current limiting
- `docs/CALIBRATION.md` - Load cell calibration

**Code:**
- `esp32/torque_analyzer_oled.ino` - Main firmware

## Troubleshooting Quick Reference

| Problem | Solution |
|---------|----------|
| OLED blank | Check I2C address (0x3C or 0x3D) |
| Buttons don't work | Check wired to GND, not VCC |
| Wrong torque reading | Recalibrate load cell |
| Motor won't run | Check PWM pin, in Control mode? |
| Current too high | Verify current limiter setup |

**See:** `docs/OLED_SETUP_GUIDE.md` for troubleshooting details

## Applications

**Motor Testing:**
- Compare different DC motors
- Stall torque characterization
- Quality control

**Optimization:**
- Find optimal gear ratio
- Test battery performance
- Tune for efficiency

**Research:**
- DC motor characterization
- Stall torque analysis
- Current vs torque curves

## Next Steps

1. **Build it:** Follow `docs/OLED_SETUP_GUIDE.md`
2. **Calibrate:** See `docs/CALIBRATION.md`
3. **Test motors:** Start with free run, then load
4. **Record data:** Use Serial Monitor or write down
5. **Compare:** Test multiple motors
6. **Share results:** Help the community!

## Support & Resources

**Documentation:**
- Complete setup guide
- Wiring diagrams
- Troubleshooting
- Calibration procedures

**Community:**
- Share your builds
- Post your results
- Suggest improvements

## License

MIT License - Free to use, modify, and share

---

## Summary

**What you get:**
- ✅ Standalone torque analyzer
- ✅ Real-time OLED display
- ✅ Current/torque/power measurement
- ✅ Fixed voltage control
- ✅ Stall torque measurement
- ✅ Safety current limiting
- ✅ Portable operation

**Build time: ~2 hours**

**Start measuring DC motor stall torque today! ⚡🔧**

For complete build instructions, see: **`docs/OLED_SETUP_GUIDE.md`**
