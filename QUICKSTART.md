# Quick Start Guide

Get your Mini 4WD Torque Analyzer running in 30 minutes!

## Prerequisites

- ESP32 board
- HX711 + Load Cell
- INA219 current sensor
- Raspberry Pi 5
- Basic soldering skills

---

## Step 1: Wire Components (10 min)

### HX711 to ESP32
```
HX711 VCC  → ESP32 3.3V
HX711 GND  → ESP32 GND
HX711 DT   → ESP32 GPIO 16
HX711 SCK  → ESP32 GPIO 4
```

### INA219 to ESP32
```
INA219 VCC → ESP32 3.3V
INA219 GND → ESP32 GND
INA219 SDA → ESP32 GPIO 21
INA219 SCL → ESP32 GPIO 22
```

### Load Cell to HX711
```
Red   → E+
Black → E-
White → A-
Green → A+
```

### Motor Power (through INA219)
```
Battery (+) → INA219 VIN+
INA219 VIN- → Motor (+)
Motor (-)   → Battery (-)
INA219 GND  → ESP32 GND (IMPORTANT!)
```

**See `docs/WIRING.md` for detailed diagrams**

---

## Step 2: Upload ESP32 Firmware (5 min)

1. Install Arduino IDE
2. Install ESP32 board support
3. Install libraries: `HX711`, `Adafruit INA219`
4. Open `esp32/torque_analyzer.ino`
5. Update torque arm radius:
   ```cpp
   #define TORQUE_ARM_RADIUS_MM  10.0  // YOUR VALUE
   ```
6. Upload to ESP32
7. Check Serial Monitor - should see "BLE Advertising started"

---

## Step 3: Calibrate Load Cell (10 min)

1. Remove all weight from load cell
2. Reset ESP32 (load cell auto-tares)
3. Apply 100g known weight
4. Note reading in Serial Monitor (e.g., "87.34g")
5. Calculate new factor:
   ```
   New Factor = -7050 × (100 / 87.34) = -8071
   ```
6. Update code:
   ```cpp
   #define CALIBRATION_FACTOR  -8071.0
   ```
7. Re-upload and verify with 100g weight

**See `docs/CALIBRATION.md` for detailed instructions**

---

## Step 4: Setup Raspberry Pi (5 min)

```bash
# Clone or copy project files
cd ~
mkdir mini4wd-torque-analyzer
cd mini4wd-torque-analyzer

# Install dependencies
sudo apt install -y python3-pip bluetooth bluez
pip3 install flask flask-socketio bleak

# Copy Python files
# (web_server.py, templates/index.html, requirements.txt)

# Run server
python3 raspberry-pi/web_server.py
```

**Look for output:**
```
Server starting on http://192.168.1.XXX:5000
```

---

## Step 5: View Dashboard (2 min)

1. Note the IP address from Pi output
2. Open phone browser
3. Go to: `http://[PI-IP]:5000`
4. See real-time torque and power data!

---

## Testing

1. **No load**: Should show ~0 torque, ~0 current
2. **Pull string**: Force/torque should increase
3. **Run motor**: Should show torque + power readings

---

## Expected Values (Mini 4WD Motors @ 2.4-3V)

| Motor Type | Torque | Current | Power |
|------------|--------|---------|-------|
| Standard | 0.5-1.0 mN·m | 200-400 mA | 0.5-1.2 W |
| High-Speed | 0.3-0.7 mN·m | 150-300 mA | 0.4-0.9 W |
| Torque-Tuned | 1.0-2.0 mN·m | 300-500 mA | 0.7-1.5 W |

---

## Troubleshooting

**ESP32 not uploading?**
- Install CH340 USB driver
- Hold BOOT button while uploading

**Load cell reading 0?**
- Check wiring: Red→E+, Black→E-, White→A-, Green→A+
- Verify 3.3V on HX711

**INA219 not detected?**
- Verify I2C wiring (GPIO 21, 22)
- Check common ground connection

**Bluetooth not connecting?**
- Power cycle ESP32
- Check Pi Bluetooth: `sudo systemctl status bluetooth`

**Torque values incorrect?**
- Re-calibrate load cell
- Verify torque arm radius measurement

---

## Full Documentation

- **`README.md`** - Project overview
- **`docs/BUILD_GUIDE.md`** - Complete step-by-step build
- **`docs/WIRING.md`** - Detailed wiring diagrams
- **`docs/CALIBRATION.md`** - Advanced calibration

---

## Need Help?

1. Check troubleshooting sections in docs
2. Verify all wiring with multimeter
3. Test each component individually

Happy testing!
