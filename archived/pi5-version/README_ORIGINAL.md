# Mini 4WD Torque Analyzer

A real-time torque and power measurement system for Mini 4WD cars using ESP32, load cell, and INA219 current sensor.

## System Overview

```
Mini 4WD Motor → Load Cell (Force) + INA219 (Current/Voltage)
                        ↓
                     ESP32
                  (Compute torque & power)
                        ↓
                  Bluetooth LE
                        ↓
                 Raspberry Pi 5
                   (Web Server)
                        ↓
              Cellphone Web Browser
              (Real-time Dashboard)
```

## Features

- Real-time torque measurement (mN.m)
- Power consumption monitoring (Watts)
- RPM calculation
- Motor efficiency analysis
- Wireless data transmission
- Web-based dashboard for mobile viewing

## Components Required

### Electronics
- ESP32 Development Board (ESP32-DevKit-C or similar)
- HX711 Load Cell Amplifier Module
- 100g-200g Load Cell (beam type, for compression)
- INA219 Current/Voltage Sensor Module
- **XL4015 Current Limiter Module** (protects motor during stall testing)
- Raspberry Pi 5 (any RAM size)
- Jumper wires
- Breadboard (for prototyping)
- 3.7V LiPo battery (for ESP32, optional if powered via USB)

### Mechanical Parts
- Lever arm (aluminum strip 80mm × 15mm × 2mm, or 3D printed)
- 3D printed test rig or custom mounting bracket
- M3 screws and nuts
- Wooden base (150mm × 150mm)
- Mini 4WD motor to test
- Mini 4WD battery holder

### Tools
- Soldering iron and solder
- Multimeter
- 3D printer (optional, for custom parts)
- Hot glue gun

## Project Structure

```
mini4wd-torque-analyzer/
├── README.md                         # This file
├── QUICKSTART.md                     # 30-minute quick setup guide
├── docs/
│   ├── WIRING.md                    # Wiring diagrams with motor protection
│   ├── CALIBRATION.md               # Sensor calibration guide
│   ├── BUILD_GUIDE.md               # Step-by-step build instructions
│   ├── MECHANICAL_SETUP_LEVER.md    # Lever arm design guide
│   └── MOTOR_PROTECTION.md          # Current limiting circuits
├── esp32/
│   └── torque_analyzer.ino          # ESP32 firmware
├── raspberry-pi/
│   ├── web_server.py                # Flask web server + BLE receiver
│   ├── requirements.txt             # Python dependencies
│   └── templates/
│       └── index.html               # Dashboard UI
```

## Safety Warning ⚠️

**IMPORTANT:** Mini 4WD motors can draw **5-7A during stall testing**, which can:
- Burn out the motor in seconds
- Damage the INA219 sensor (max 3.2A)
- Create fire hazard

**Solution:** Always use **current limiting** (see `docs/MOTOR_PROTECTION.md`)
- Recommended: XL4015 module set to 2.5A limit
- Or: 2.5A Polyfuse inline with battery
- Never perform stall tests without protection!

## Quick Start

1. Wire the components according to `docs/WIRING.md` (includes motor protection)
2. Set up mechanical lever arm (see `docs/MECHANICAL_SETUP_LEVER.md`)
3. Upload `esp32/torque_analyzer.ino` to ESP32
4. Calibrate load cell (see `docs/CALIBRATION.md`)
5. Install dependencies on Raspberry Pi: `pip install -r raspberry-pi/requirements.txt`
6. Run the receiver: `python raspberry-pi/web_server.py`
7. Open browser on phone: `http://[pi-ip]:5000`

**For detailed instructions, see `QUICKSTART.md` or `docs/BUILD_GUIDE.md`**

## Calculations

### Torque (τ)
```
τ = Force × Radius
τ (mN.m) = Load_Cell_Reading (grams) × 9.81 × Radius (meters) × 1000
```

### Power (P)
```
P = Voltage × Current
P (Watts) = V × I
```

### Motor Efficiency
```
Efficiency = (Mechanical Power / Electrical Power) × 100%
Mechanical Power = τ × ω (where ω is angular velocity in rad/s)
```

## License

MIT License - Free to use and modify
