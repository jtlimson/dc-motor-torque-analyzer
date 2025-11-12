# Quick Start Guide - OLED Version

Get your DC Motor Stall Torque Analyzer running in 30 minutes!

## Prerequisites

- ESP32 board
- HX711 + Load Cell
- INA219 current sensor
- OLED display (SSD1306 128x64)
- 4 tactile buttons
- XL4015 current limiter
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

### INA219 + OLED to ESP32 (I2C Bus)
```
INA219 VCC → ESP32 3.3V
INA219 GND → ESP32 GND
INA219 SDA → ESP32 GPIO 21 (shared with OLED)
INA219 SCL → ESP32 GPIO 22 (shared with OLED)

OLED VCC   → ESP32 3.3V
OLED GND   → ESP32 GND
OLED SDA   → ESP32 GPIO 21 (shared with INA219)
OLED SCL   → ESP32 GPIO 22 (shared with INA219)
```

### Buttons to ESP32
```
Button UP    → ESP32 GPIO 32
Button DOWN  → ESP32 GPIO 33
Button MODE  → ESP32 GPIO 25
Button RESET → ESP32 GPIO 26
(All buttons connect other pin to GND)
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

**See `docs/WIRING_OLED.md` for detailed diagrams**

---

## Step 2: Upload ESP32 Firmware (5 min)

1. Install Arduino IDE
2. Install ESP32 board support
3. Install libraries: `HX711`, `Adafruit INA219`, `Adafruit GFX`, `Adafruit SSD1306`
4. Open `esp32/torque_analyzer_oled.ino`
5. Update torque arm radius:
   ```cpp
   #define TORQUE_ARM_RADIUS_MM  10.0  // YOUR VALUE
   ```
6. Upload to ESP32
7. Check OLED display - should show "REALTIME" mode

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

## Step 4: Setup Current Limiter (5 min)

1. Connect XL4015 between battery and motor
2. Adjust output voltage to match battery voltage
3. Short circuit output with ammeter
4. Adjust CC potentiometer until 2.5A current limit
5. Remove short circuit

**See `docs/MOTOR_PROTECTION.md` for detailed setup**

---

## Step 5: Test Display Modes (2 min)

1. Press MODE button to cycle through:
   - **REALTIME** - Live measurements
   - **MAX VALUES** - Peak values recorded
   - **MOTOR CONTROL** - Adjust motor PWM
2. Use UP/DOWN buttons in Motor Control mode
3. Use RESET button to clear maximum values

---

## Testing

1. **No load**: Should show ~0 torque, ~0 current
2. **Apply force to lever**: Force/torque should increase
3. **Test control mode**: Set current limit, turn motor ON
4. **Stall test**: Block motor shaft for 3 seconds max

---

## Expected Stall Values (Small DC Motors @ Fixed Voltage)

| Motor Type | Stall Torque | Stall Current | Stall Power |
|------------|--------|---------|-------|
| Small DC (6V) | 5-8 mN·m | 2.0-2.5 A | 3-4 W |
| Gear Motor | 8-15 mN·m | 1.5-2.0 A | 2-3 W |
| High Torque | 10-20 mN·m | 2.5-3.0 A | 4-5 W |

---

## Troubleshooting

**ESP32 not uploading?**
- Install CH340 USB driver
- Hold BOOT button while uploading

**OLED display blank?**
- Check I2C wiring (GPIO 21, 22)
- Verify OLED address (usually 0x3C)
- Check 3.3V power to OLED

**Buttons not working?**
- Check buttons wired to GND (not VCC)
- Verify GPIO pin assignments

**Load cell reading 0?**
- Check wiring: Red→E+, Black→E-, White→A-, Green→A+
- Verify 3.3V on HX711

**INA219 not detected?**
- Verify I2C wiring (GPIO 21, 22)
- Check common ground connection

**Torque values incorrect?**
- Re-calibrate load cell
- Verify torque arm radius measurement

---

## Full Documentation

- **`README.md`** - Project overview
- **`docs/OLED_SETUP_GUIDE.md`** - Complete OLED version setup
- **`docs/WIRING_OLED.md`** - OLED version wiring diagrams
- **`docs/CALIBRATION.md`** - Load cell calibration
- **`docs/MOTOR_PROTECTION.md`** - Current limiting setup

---

## Need Help?

1. Check troubleshooting sections in docs
2. Verify all wiring with multimeter
3. Test each component individually

Happy testing!
