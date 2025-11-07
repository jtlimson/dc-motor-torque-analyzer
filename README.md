# Mini 4WD Torque Analyzer - OLED Standalone Version

**Standalone torque measurement system with OLED display - No Raspberry Pi or phone needed!**

> **Note:** This is the main/recommended version. The original Raspberry Pi 5 + Web Dashboard version has been archived to `archived/pi5-version/` and is available for advanced users who need web-based data logging and graphs.

## Quick Overview

This is the **simplified standalone version** with direct OLED display:

```
ESP32 + OLED Display + Buttons
        ↓
   Standalone System
   (No Pi or Phone!)
```

### Features

✅ **Real-time display** on 128x64 OLED screen
✅ **Button controls** for easy navigation
✅ **Three display modes** (Realtime, Max Values, Motor Control)
✅ **Motor speed control** with PWM
✅ **Current limiting** for motor protection
✅ **Portable** - Battery powered operation
✅ **Cheaper** - Only ~$40 vs $110 with Pi

## What It Displays

### Current Measurements
- **I** (A) - Current draw
- **T** (mN·m) - Torque in milliNewton-meters
- **T** (kg·cm) - Torque in kilogram-centimeters
- **P** (W) - Power consumption
- **V** (V) - Voltage under load

### Maximum Values
- Peak current
- Peak torque (both units)
- Peak power

### Motor Control
- PWM percentage (0-100%)
- Current limit setting
- Real-time monitoring

## Components Needed

### Electronics (~$40)

| Component | Cost |
|-----------|------|
| ESP32 DevKit | $8-12 |
| OLED Display (SSD1306 128x64) | $5-8 |
| HX711 + Load Cell (100-200g) | $7-15 |
| INA219 Current Sensor | $5-8 |
| XL4015 Current Limiter | $3-5 |
| 4x Tactile Buttons | $2 |
| Breadboard + Wires | $6-10 |

**Total: ~$36-60**

### Mechanical (~$10)
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
- **UP** - Increase motor PWM (in Motor Control mode)
- **DOWN** - Decrease motor PWM (in Motor Control mode)
- **MODE** - Cycle through display modes
- **RESET** - Clear maximum values

### Display Modes

Press **MODE** button to cycle through:

#### Mode 1: REALTIME
```
=== REALTIME ===
I: 0.85 A
T: 58.3 mN.m
T: 0.59 kg.cm
P: 2.34 W
V: 2.76 V
```
Shows live measurements as motor runs

#### Mode 2: MAX VALUES
```
=== MAX VALUES ===
I: 2.45 A
T: 124.5 mN.m
T: 1.27 kg.cm
P: 6.73 W

RESET to clear
```
Shows peak values recorded. Press RESET to clear.

#### Mode 3: MOTOR CONTROL
```
=== MOTOR CTRL ===
PWM: 75 %
Limit: 2.5 A
Current: 1.85 A
Torque: 0.92 kg.cm

UP/DN: Adjust PWM
```
Control motor speed with UP/DOWN buttons

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

✅ **Automatic current limiting** (2.5A default)
✅ **Motor only runs in Control mode**
✅ **Visual current display**
✅ **Protection warnings**
✅ **Emergency stop** (DOWN button to 0%)

⚠️ **Important:**
- Always use current limiter for stall tests
- Maximum 3 seconds stall time
- Let motor cool 30 seconds between tests

## Testing Guide

### Test 1: Free Run
1. Mode: MOTOR CONTROL
2. UP button to 50% PWM
3. Motor runs freely
4. Current: 0.2-0.5A

### Test 2: Load Test
1. Mode: MOTOR CONTROL
2. UP button to 100% PWM
3. Lever arm pushes load cell
4. Current: 0.5-1.5A
5. Torque shown on display

### Test 3: Stall Torque
1. Switch to MAX VALUES mode
2. Press MODE to MOTOR CONTROL
3. UP button to 100% PWM
4. Motor blocked (max load)
5. Test for 3 seconds max
6. DOWN to 0% PWM
7. Switch to MAX VALUES to see peak

**Expected stall torque:**
- Standard motor: 3-5 mN·m (0.3-0.5 kg·cm)
- Torque motor: 5-8 mN·m (0.5-0.8 kg·cm)

## Typical Values (Mini 4WD Motors)

### Normal Operation
| Motor Type | Current | Torque (mN·m) | Torque (kg·cm) |
|------------|---------|---------------|----------------|
| Standard | 0.5-1.0A | 0.5-1.0 | 0.05-0.10 |
| High-Speed | 0.3-0.8A | 0.3-0.7 | 0.03-0.07 |
| Torque | 0.8-1.5A | 1.0-2.0 | 0.10-0.20 |

### Stall (2.5A Limit)
| Motor Type | Current | Torque (mN·m) | Torque (kg·cm) |
|------------|---------|---------------|----------------|
| Standard | 2.5A | 3-5 | 0.30-0.51 |
| High-Speed | 2.0A | 2-4 | 0.20-0.41 |
| Torque | 2.5A | 5-8 | 0.51-0.82 |

## Documentation

**Quick Start:**
- `OLED_VERSION_README.md` - This file
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

**See:** `docs/OLED_SETUP_GUIDE.md` Troubleshooting section

## Comparison: OLED vs Pi Version

| Feature | OLED Version | Pi Version |
|---------|-------------|-----------|
| **Display** | 128x64 OLED | Phone browser |
| **Cost** | ~$40 | ~$110 |
| **Setup time** | 1 hour | 2 hours |
| **Portable** | Yes (battery) | Need WiFi |
| **Data logging** | Serial only | Web logging |
| **Graphs** | No | Yes (real-time) |
| **Remote viewing** | No | Yes (phone/tablet) |

**Choose OLED version if:**
- ✓ Want simple standalone system
- ✓ Budget conscious
- ✓ Need portable operation
- ✓ Don't need data logging

**Choose Pi version if:**
- ✓ Want graphs and data logging
- ✓ Need remote viewing
- ✓ Multiple simultaneous viewers
- ✓ Web-based dashboard

## Upgrading to Pi Version

The OLED version can be upgraded later:

1. Keep all existing hardware
2. Add Bluetooth code (already written)
3. Add Raspberry Pi
4. Run web server
5. Access from phone/computer

**Both versions can coexist:**
- OLED shows data locally
- Pi shows data remotely
- Best of both worlds!

## Applications

**Motor Testing:**
- Compare different motors
- Break-in testing
- Quality control

**Optimization:**
- Find optimal gear ratio
- Test battery performance
- Tune for efficiency

**Research:**
- Motor characterization
- Torque/speed curves
- Efficiency mapping

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
- ✅ Motor speed control
- ✅ Max value tracking
- ✅ Safety current limiting
- ✅ Portable operation

**Cost: ~$40-60**

**Build time: ~2 hours**

**Start measuring torque today! 🏎️⚡**

For complete instructions, see: **`docs/OLED_SETUP_GUIDE.md`**
