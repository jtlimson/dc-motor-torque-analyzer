# Mini 4WD Torque Analyzer - Complete Build Guide

This guide will walk you through building a complete torque and power measurement system for Mini 4WD motors.

## Table of Contents

1. [Overview](#overview)
2. [Parts List](#parts-list)
3. [Tools Required](#tools-required)
4. [Step 1: Mechanical Setup](#step-1-mechanical-setup)
5. [Step 2: Electronics Wiring](#step-2-electronics-wiring)
6. [Step 3: ESP32 Firmware Setup](#step-3-esp32-firmware-setup)
7. [Step 4: Load Cell Calibration](#step-4-load-cell-calibration)
8. [Step 5: Raspberry Pi Setup](#step-5-raspberry-pi-setup)
9. [Step 6: Testing](#step-6-testing)
10. [Troubleshooting](#troubleshooting)

---

## Overview

The torque analyzer measures:
- **Torque** (mN·m and kg·cm) - Using a load cell and lever arm (compression method)
- **Power** (Watts) - Using an INA219 current/voltage sensor
- **Current draw** (mA) - Motor current consumption
- **Voltage** (V) - Battery voltage under load

**Two versions available:**
- **OLED Version** (Main) - Standalone with OLED display and buttons
- **Pi Version** (Archived) - Bluetooth to Raspberry Pi web dashboard

> **Note:** This guide covers both versions. For OLED-specific setup, see `docs/OLED_SETUP_GUIDE.md`

---

## Parts List

### Electronics Components

| Item | Quantity | Estimated Cost | Notes |
|------|----------|----------------|-------|
| ESP32 DevKit-C | 1 | $8-12 | Any ESP32 board with Bluetooth |
| HX711 Load Cell Amplifier | 1 | $2-5 | 24-bit ADC for load cell |
| Load Cell (100g-500g) | 1 | $5-10 | Beam type, 100g recommended |
| INA219 Current Sensor | 1 | $5-8 | I2C interface |
| Raspberry Pi 5 | 1 | $60-80 | Any model (2GB+ recommended) |
| Breadboard | 1 | $3-5 | For prototyping |
| Jumper Wires | 1 set | $3-5 | Male-to-male, male-to-female |
| USB Cable (Micro or Type-C) | 1 | $3-5 | For ESP32 programming |
| LiPo Battery (optional) | 1 | $10-15 | 3.7V 500mAh+ for portable use |

**Total Electronics Cost: ~$100-145**

### Mechanical Components

| Item | Quantity | Notes |
|------|----------|-------|
| Lever arm | 1 | Aluminum strip 80mm × 15mm × 2mm (or 3D printed) |
| Wooden base | 1 | 150mm × 150mm × 10mm for stable mounting |
| Motor mount | 1 | 3D printed or DIY wood block (holds motor vertically) |
| M3 screws (10mm) | 4-6 | For mounting |
| M3 nuts | 4-6 | For mounting |
| Shaft coupler (optional) | 1 | 2mm bore, connects lever to shaft |
| Mini 4WD motor | 1 | Motor to be tested |
| Mini 4WD battery holder | 1 | 2x AA batteries (2.4V or 3V) |
| XL4015 Current Limiter | 1 | **CRITICAL** for motor protection |

---

## Tools Required

- Soldering iron and solder
- Wire strippers
- Small screwdriver set
- Multimeter
- Hot glue gun (optional, for securing wires)
- Computer with Arduino IDE
- 3D printer (optional, or use cardboard/wood)

---

## Step 1: Mechanical Setup

### 1.1 Understanding the Torque Measurement Principle

```
                Motor (vertical)
                      |
                      | shaft
                      |
              ========●======== Lever arm (horizontal)
                      ↓
                      ↓ Motor tries to rotate lever
                      ↓ Load cell resists rotation
                 [Load Cell]  ← Compression mode
                      |
                 Fixed Mount
```

**How it works:**
- Motor tries to **rotate** the lever arm
- Load cell **resists** that rotation
- This resistance creates a measurable force
- **Torque = Force × Lever arm length**

**Think of it like trying to open a door that someone is blocking - the harder you push, the more force is measured!**

> **📖 For detailed physics explanation, see:** `docs/MECHANICAL_SETUP_LEVER.md`

### 1.2 Build the Test Rig

**Complete detailed build instructions in:** `docs/MECHANICAL_SETUP_LEVER.md`

**Quick overview:**

**Option A: 3D Printed Rig** (Best)
- Design/print motor holder (vertical mount)
- Print lever arm or use aluminum strip
- Mount load cell holder below lever arm

**Option B: DIY Wood/Cardboard Rig** (Budget-friendly)

1. **Base plate**: 150mm × 150mm wooden board
   - Sand edges smooth
   - Mark motor and load cell positions

2. **Motor mount**:
   - Cut wood block: 40mm × 40mm × 60mm
   - Drill hole for motor body
   - Motor sits **vertically** with shaft pointing UP
   - Secure to base with screws

3. **Lever arm**:
   - Aluminum strip: 80mm × 15mm × 2mm
   - Drill center hole for motor shaft (shaft diameter + 0.1mm)
   - Attach to motor shaft (press-fit or set screw)
   - Must be rigid (not flexible!)

4. **Load cell mount**:
   - Mount load cell **horizontally** below lever arm
   - Fixed end: Screw to base plate
   - Free end: Positioned under lever arm contact point
   - Gap: 1-2mm when motor is off

### 1.3 Assemble and Align

1. **Mount motor** vertically in holder, shaft pointing up

2. **Attach lever arm** to motor shaft
   - Slide onto shaft or use shaft coupler
   - Lever must rotate WITH shaft (no slipping!)
   - Position horizontally when motor is off

3. **Position load cell** horizontally below lever
   - One end of lever arm should contact load cell free end
   - Lever should be 1-2mm above load cell (not touching when motor off)
   - When motor rotates, lever pushes down on load cell

4. **Verify alignment**:
   - Lever arm perpendicular to load cell
   - No side loads (must push straight down)
   - Lever rotates freely without binding

### 1.4 Measure Lever Arm Length

**CRITICAL for accurate torque readings!**

Use calipers or precise ruler to measure from:
- **Start:** Center of motor shaft
- **End:** Center of load cell contact point

```
    Motor Shaft
        ●
        |<------ Measure this distance ------>|
    ====●=======================================
                                              ↓
                                        Contact point
                                              ↓
                                         [Load Cell]
```

**Example:** 60.0 mm

**Record this value** - you'll need it for ESP32 code!

```cpp
#define TORQUE_ARM_RADIUS_MM  60.0  // YOUR measured value
```

**Tips:**
- Measure from shaft CENTER (not edge)
- Measure to contact point on lever (where it touches load cell)
- Use millimeters for precision
- Double-check measurement (±1mm = ±1.7% error)

---

## Step 2: Electronics Wiring

### 2.1 Prepare Components

Gather all electronic components and refer to `docs/WIRING.md` for detailed diagrams.

### 2.2 Wire HX711 to Load Cell

The load cell has 4 wires:

| Load Cell Wire | HX711 Pin |
|----------------|-----------|
| Red | E+ |
| Black | E- |
| White | A- |
| Green | A+ |

**Note:** Some load cells have different colors. Check your datasheet.

### 2.3 Wire HX711 to ESP32

| HX711 Pin | ESP32 Pin |
|-----------|-----------|
| VCC | 3.3V |
| GND | GND |
| DT | GPIO 16 |
| SCK | GPIO 4 |

### 2.4 Wire INA219 to ESP32

| INA219 Pin | ESP32 Pin |
|------------|-----------|
| VCC | 3.3V |
| GND | GND |
| SDA | GPIO 21 |
| SCL | GPIO 22 |

### 2.5 Wire INA219 to Motor Circuit

```
Battery (+) → INA219 VIN+
INA219 VIN- → Motor (+)
Motor (-) → Battery (-)
INA219 GND → ESP32 GND (common ground!)
```

**CRITICAL:** All grounds must be connected together (ESP32 GND, INA219 GND, Battery GND).

### 2.6 Double-Check Connections

Use a multimeter in continuity mode to verify:
- No short circuits between VCC and GND
- All GND are connected together
- Correct pin assignments

---

## Step 3: ESP32 Firmware Setup

### 3.1 Install Arduino IDE

1. Download from: https://www.arduino.cc/en/software
2. Install Arduino IDE

### 3.2 Install ESP32 Board Support

1. Open Arduino IDE
2. Go to `File > Preferences`
3. Add to "Additional Board Manager URLs":
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Go to `Tools > Board > Boards Manager`
5. Search "esp32" and install "esp32 by Espressif Systems"

### 3.3 Install Required Libraries

Go to `Sketch > Include Library > Manage Libraries` and install:
- **HX711** by Bogdan Necula
- **Adafruit INA219** by Adafruit

### 3.4 Update Configuration

1. Open `esp32/torque_analyzer.ino` (or `torque_analyzer_oled.ino` for OLED version)
2. Update this line with your measured **lever arm length**:
   ```cpp
   #define TORQUE_ARM_RADIUS_MM  60.0   // Your measured value in mm
   ```
   - This is the distance from motor shaft center to load cell contact point
   - Measured in Step 1.4 of mechanical setup
   - Example: 60mm, 70mm, 80mm depending on your lever arm
3. Keep calibration factor as default for now (we'll calibrate later)

### 3.5 Upload to ESP32

1. Connect ESP32 to computer via USB
2. Select board: `Tools > Board > ESP32 Arduino > ESP32 Dev Module`
3. Select port: `Tools > Port > [Your ESP32 Port]`
4. Click Upload button
5. Wait for "Done uploading" message

### 3.6 Verify Upload

1. Open Serial Monitor: `Tools > Serial Monitor`
2. Set baud rate to `115200`
3. You should see:
   ```
   Mini 4WD Torque Analyzer - Starting...
   Initializing HX711...
   HX711 initialized and tared.
   Initializing INA219...
   INA219 initialized.
   Initializing BLE...
   BLE Advertising started. Waiting for connection...
   Device Name: Mini4WD_Torque
   Setup complete!
   ```

---

## Step 4: Load Cell Calibration

The load cell needs calibration for accurate readings.

### 4.1 Prepare Calibration Weights

You need known weights:
- 50g (small coins, check with kitchen scale)
- 100g (standard weight or measured coins)
- 200g (optional, for better accuracy)

### 4.2 Calibration Procedure

1. **Tare (zero) the load cell:**
   - Remove all weight from load cell
   - Reset ESP32 (press reset button)
   - Load cell is now "zeroed"

2. **Apply known weight:**
   - Gently hang a 100g weight from the load cell
   - Observe Serial Monitor readings

3. **Calculate calibration factor:**

   The ESP32 will show raw readings like:
   ```
   Force: 1234.56g | Torque: 121.32 mN.m | ...
   ```

   If you applied 100g but it reads 87.3g, calculate:
   ```
   New Calibration Factor = Old Factor × (Actual Weight / Displayed Weight)
   New Calibration Factor = -7050 × (100 / 87.3) = -8076
   ```

4. **Update code:**
   - Change `CALIBRATION_FACTOR` in `torque_analyzer.ino`
   - Re-upload to ESP32

5. **Verify:**
   - Test with multiple known weights
   - Readings should be accurate within ±2%

---

## Step 5: Raspberry Pi Setup

### 5.1 Prepare Raspberry Pi

1. Install Raspberry Pi OS (64-bit recommended)
2. Connect to WiFi
3. Update system:
   ```bash
   sudo apt update && sudo apt upgrade -y
   ```

### 5.2 Install Python Dependencies

1. Navigate to project folder:
   ```bash
   cd ~
   git clone [your-repo] mini4wd-torque-analyzer
   cd mini4wd-torque-analyzer/raspberry-pi
   ```

2. Install system packages:
   ```bash
   sudo apt install -y python3-pip bluetooth bluez
   ```

3. Install Python packages:
   ```bash
   pip3 install -r requirements.txt
   ```

### 5.3 Enable Bluetooth

1. Ensure Bluetooth is enabled:
   ```bash
   sudo systemctl enable bluetooth
   sudo systemctl start bluetooth
   ```

2. Verify:
   ```bash
   bluetoothctl
   > power on
   > exit
   ```

### 5.4 Test Connection

1. Run the web server:
   ```bash
   python3 web_server.py
   ```

2. Look for output:
   ```
   Starting Mini 4WD Torque Analyzer Server
   BLE thread started
   Scanning for device: Mini4WD_Torque
   Found device: Mini4WD_Torque (XX:XX:XX:XX:XX:XX)
   Connected to Mini4WD_Torque
   ==================================================
   Server starting on http://192.168.1.XXX:5000
   Open this URL on your phone: http://192.168.1.XXX:5000
   ==================================================
   ```

3. If device is not found:
   - Ensure ESP32 is powered on
   - Check Bluetooth is enabled on Pi
   - Move ESP32 closer to Pi

### 5.5 Access Dashboard

1. Note the IP address from the output (e.g., `192.168.1.100`)
2. On your phone, open a web browser
3. Navigate to: `http://192.168.1.100:5000`
4. You should see the dashboard with live data!

---

## Step 6: Testing

### 6.1 Initial Test (No Motor Load)

1. Power on system
2. Verify dashboard shows:
   - Force: ~0 grams
   - Torque: ~0 mN·m
   - Voltage: Battery voltage (2.4V or 3V)
   - Current: ~0 mA

### 6.2 Test Load Cell

1. Gently pull on the string
2. Dashboard should show increasing force and torque
3. Release - values should return to ~0

### 6.3 Test Motor

1. Ensure motor is properly mounted
2. Connect motor to battery (through INA219)
3. Motor should spin, pulling the load cell
4. Dashboard should show:
   - Force: Increasing (depends on motor)
   - Torque: Calculated automatically
   - Current: 100-500mA (typical Mini 4WD motor)
   - Power: 0.3-1.5W (typical)

### 6.4 Data Validation

Compare with expected values:
- **Standard Mini 4WD motor**: 0.5-1.0 mN·m torque @ 2.4V
- **High-speed motor**: 0.3-0.7 mN·m torque
- **Torque-tuned motor**: 1.0-2.0 mN·m torque

If values seem incorrect, recheck:
- Torque arm radius measurement
- Load cell calibration
- Wiring connections

---

## Troubleshooting

### Problem: ESP32 Not Found

**Solutions:**
- Install CH340 or CP2102 USB driver
- Try different USB cable
- Press and hold BOOT button while uploading

### Problem: HX711 Reads 0 or Unstable

**Solutions:**
- Check load cell wiring (Red→E+, Black→E-, White→A-, Green→A+)
- Verify 3.3V power supply
- Add 0.1µF capacitor across VCC and GND
- Ensure load cell is not overloaded

### Problem: INA219 Not Detected

**Solutions:**
- Verify I2C wiring (SDA→21, SCL→22)
- Check for I2C address conflict
- Run I2C scanner sketch to detect address
- Try different INA219 board

### Problem: Bluetooth Connection Fails

**Solutions:**
- Ensure Raspberry Pi Bluetooth is enabled
- Power cycle ESP32
- Reduce distance between devices
- Check for other Bluetooth devices interfering

### Problem: Torque Values Too High/Low

**Solutions:**
- Re-calibrate load cell
- Verify torque arm radius measurement
- Check if string is horizontal and taut
- Ensure pulley rotates freely

### Problem: Web Dashboard Not Loading

**Solutions:**
- Check Raspberry Pi IP address: `hostname -I`
- Ensure phone is on same WiFi network
- Disable firewall: `sudo ufw disable`
- Check web server is running: `ps aux | grep python3`

### Problem: High Current But Low Torque

**Causes:**
- Motor inefficiency
- Mechanical binding/friction
- Motor overheating
- Worn brushes/commutator

---

## Next Steps

### Enhancements

1. **Add RPM measurement**: Use IR sensor or hall effect sensor
2. **Data logging**: Save measurements to CSV file
3. **Motor comparison**: Test multiple motors side-by-side
4. **Battery discharge curves**: Monitor performance over time
5. **Temperature monitoring**: Add thermistor to motor
6. **Mobile app**: Create native iOS/Android app
7. **Cloud sync**: Upload data to cloud for analysis

### Advanced Testing

- **Efficiency mapping**: Plot efficiency vs. load
- **Break-in testing**: Monitor motor performance during break-in
- **Gear ratio optimization**: Test different gear combinations
- **Battery comparison**: Compare alkaline vs. rechargeable

---

## Safety Notes

- **Do not exceed rated current** of INA219 (3.2A max)
- **Secure all connections** to prevent shorts
- **Monitor for overheating** during extended tests
- **Use eye protection** when testing at high speeds
- **Keep fingers clear** of moving parts

---

## Support

For questions or issues:
- Check the troubleshooting section
- Review wiring diagrams in `docs/WIRING.md`
- Open an issue on GitHub (if applicable)

---

## Credits

Designed for Mini 4WD enthusiasts to optimize motor performance and analyze tuning results.

Happy testing!
