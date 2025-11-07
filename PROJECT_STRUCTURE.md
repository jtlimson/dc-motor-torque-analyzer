# Project Structure

Mini 4WD Torque Analyzer - Complete project organization and file guide.

## Current Version: OLED Standalone

**Main implementation:** Standalone OLED display version (cheaper, simpler)
**Archived:** Raspberry Pi 5 web dashboard version (available for upgrade)

---

## Directory Structure

```
mini4wd-torque-analyzer/
│
├── README.md                          # Main project README (OLED version)
├── PROJECT_STRUCTURE.md               # This file - project organization
├── QUICKSTART.md                      # Quick setup guide (original)
├── OLED_VERSION_SUMMARY.md            # What's new in OLED version
├── UPDATES_SUMMARY.md                 # Summary of all improvements
│
├── esp32/                             # ESP32 Firmware
│   ├── torque_analyzer_oled.ino      # ★ MAIN: OLED standalone version
│   └── torque_analyzer.ino           # Original Bluetooth version (still available)
│
├── docs/                              # Documentation
│   ├── OLED_SETUP_GUIDE.md           # ★ OLED version complete setup
│   ├── WIRING_OLED.md                # ★ OLED version wiring diagrams
│   ├── WIRING.md                     # Original wiring (Pi version)
│   ├── BUILD_GUIDE.md                # Complete build guide
│   ├── CALIBRATION.md                # Load cell calibration
│   ├── MECHANICAL_SETUP_LEVER.md     # Lever arm design guide
│   └── MOTOR_PROTECTION.md           # Current limiting circuits
│
├── archived/                          # Archived versions
│   └── pi5-version/                  # Raspberry Pi 5 version (archived)
│       ├── README_ARCHIVE.md         # Pi version documentation
│       ├── README_ORIGINAL.md        # Original project README
│       ├── torque_analyzer.ino       # Bluetooth ESP32 firmware
│       └── raspberry-pi/             # Pi software
│           ├── web_server.py         # Flask web server + BLE
│           ├── requirements.txt      # Python dependencies
│           └── templates/
│               └── index.html        # Web dashboard UI
│
└── raspberry-pi/                      # Empty (files moved to archive)
    └── templates/                     # Empty (files moved to archive)
```

## File Categories

### 📘 Start Here (New Users)

**Essential reading for OLED version:**

1. **`README.md`**
   - Main project overview
   - OLED standalone version
   - Quick feature list
   - Component requirements
   - Cost breakdown

2. **`docs/OLED_SETUP_GUIDE.md`**
   - Complete step-by-step setup
   - Software installation
   - Hardware assembly
   - Calibration procedures
   - Testing guide
   - Troubleshooting

3. **`docs/WIRING_OLED.md`**
   - Complete wiring diagrams
   - Pin assignments
   - Button connections
   - I2C bus setup
   - Breadboard layout

### 🔧 Building & Setup

**Construction guides:**

- **`docs/MECHANICAL_SETUP_LEVER.md`**
  - Lever arm design
  - Build instructions
  - Parts list
  - Measurements
  - Testing procedures

- **`docs/MOTOR_PROTECTION.md`**
  - Current limiting circuits
  - XL4015 setup
  - Polyfuse option
  - DIY MOSFET circuit
  - Safety guidelines

- **`docs/CALIBRATION.md`**
  - Load cell calibration
  - Known weight method
  - Multi-point calibration
  - Troubleshooting
  - Maintenance

### 💻 Firmware

**ESP32 code:**

- **`esp32/torque_analyzer_oled.ino`** ⭐ **USE THIS**
  - OLED standalone version
  - Button controls
  - Three display modes
  - Motor PWM control
  - No Bluetooth needed
  - ~400 lines

- **`esp32/torque_analyzer.ino`**
  - Original Bluetooth version
  - For Pi version
  - Available for upgrade
  - ~300 lines

### 📚 Reference Documentation

**Technical details:**

- **`docs/BUILD_GUIDE.md`**
  - Comprehensive build guide
  - Tools required
  - Safety notes
  - Testing procedures
  - Troubleshooting

- **`docs/WIRING.md`**
  - Original wiring (Pi version)
  - Power supply options
  - Verification steps
  - Safety notes

- **`UPDATES_SUMMARY.md`**
  - Lever arm design (vs string/pulley)
  - Motor protection added
  - All improvements documented

- **`OLED_VERSION_SUMMARY.md`**
  - What's different in OLED version
  - Feature comparison
  - Migration path
  - Design decisions

- **`QUICKSTART.md`**
  - Original quick setup (30 min)
  - Basic wiring
  - Fast calibration
  - Quick test

### 📦 Archived (Pi Version)

**Raspberry Pi version files:**

- **`archived/pi5-version/README_ARCHIVE.md`**
  - Pi version documentation
  - Why it's archived
  - When to use Pi version
  - Setup instructions
  - Migration guide

- **`archived/pi5-version/raspberry-pi/`**
  - `web_server.py` - Flask server + BLE
  - `requirements.txt` - Python deps
  - `templates/index.html` - Web dashboard

- **`archived/pi5-version/torque_analyzer.ino`**
  - Bluetooth ESP32 firmware
  - For Pi version

## Quick File Finder

### "I want to..."

**Build the OLED version:**
→ `README.md` then `docs/OLED_SETUP_GUIDE.md`

**Wire the electronics:**
→ `docs/WIRING_OLED.md`

**Build the lever arm:**
→ `docs/MECHANICAL_SETUP_LEVER.md`

**Set up current limiting:**
→ `docs/MOTOR_PROTECTION.md`

**Calibrate the load cell:**
→ `docs/CALIBRATION.md`

**Upload ESP32 code:**
→ `esp32/torque_analyzer_oled.ino`

**Upgrade to Pi version:**
→ `archived/pi5-version/README_ARCHIVE.md`

**Understand what changed:**
→ `OLED_VERSION_SUMMARY.md`

**See all improvements:**
→ `UPDATES_SUMMARY.md`

**Troubleshoot issues:**
→ `docs/OLED_SETUP_GUIDE.md` (Troubleshooting section)

## Version Comparison

### OLED Standalone (Current)

**Files needed:**
- `esp32/torque_analyzer_oled.ino`
- `docs/OLED_SETUP_GUIDE.md`
- `docs/WIRING_OLED.md`
- `docs/MOTOR_PROTECTION.md`
- `docs/CALIBRATION.md`
- `docs/MECHANICAL_SETUP_LEVER.md`

**Hardware:**
- ESP32
- OLED display (SSD1306)
- 4 buttons
- HX711 + Load cell
- INA219
- XL4015 current limiter
- Lever arm setup

**Cost:** ~$40-60

### Pi + Web Dashboard (Archived)

**Files needed:**
- `archived/pi5-version/torque_analyzer.ino`
- `archived/pi5-version/raspberry-pi/web_server.py`
- `archived/pi5-version/raspberry-pi/requirements.txt`
- `archived/pi5-version/raspberry-pi/templates/index.html`
- Same docs (CALIBRATION, MOTOR_PROTECTION, etc.)

**Hardware:**
- ESP32
- HX711 + Load cell
- INA219
- XL4015 current limiter
- Raspberry Pi 5
- Lever arm setup
- No OLED, no buttons

**Cost:** ~$110

### Combined (Future)

**Possible to have both:**
- OLED display + Buttons (local)
- Bluetooth + Pi + Web (remote)
- Modify code to combine features
- Ultimate flexibility
- Cost: ~$115

## File Sizes

```
Firmware:
├── torque_analyzer_oled.ino      ~15 KB  (OLED version)
└── torque_analyzer.ino           ~12 KB  (Bluetooth version)

Documentation:
├── OLED_SETUP_GUIDE.md           ~50 KB  (Complete guide)
├── WIRING_OLED.md                ~35 KB  (Wiring diagrams)
├── MOTOR_PROTECTION.md           ~45 KB  (Current limiting)
├── MECHANICAL_SETUP_LEVER.md     ~40 KB  (Lever design)
├── CALIBRATION.md                ~25 KB  (Calibration)
└── BUILD_GUIDE.md                ~35 KB  (Build guide)

Total project: ~500 KB (text/code)
```

## Documentation Standards

### File Naming

**Conventions:**
- `UPPERCASE.md` - Top-level guides
- `lowercase.md` - Specific documentation
- `_OLED` suffix - OLED-specific files
- `_ARCHIVE` suffix - Archived content

### Structure

**All guides include:**
- Table of contents
- Clear sections
- Code examples
- Diagrams (ASCII art)
- Troubleshooting
- Safety notes
- Next steps

## Dependencies

### ESP32 Firmware

**OLED version requires:**
```
Arduino IDE 2.x
ESP32 board support (Espressif)

Libraries:
├── HX711 (Bogdan Necula)
├── Adafruit_INA219
├── Adafruit_GFX
└── Adafruit_SSD1306
```

**Bluetooth version requires:**
```
Arduino IDE 2.x
ESP32 board support (Espressif)

Libraries:
├── HX711 (Bogdan Necula)
├── Adafruit_INA219
└── BLE (built into ESP32 core)
```

### Raspberry Pi (Archived)

**Pi version requires:**
```
Raspberry Pi OS (64-bit)
Python 3.9+

Python packages:
├── flask==3.0.0
├── flask-socketio==5.3.5
├── bleak==0.21.1
├── python-socketio==5.10.0
└── eventlet==0.33.3
```

## Development History

### v1.0 - Original Design
- Bluetooth ESP32
- Raspberry Pi web server
- Web dashboard
- String/pulley torque measurement

### v2.0 - Improvements (UPDATES_SUMMARY.md)
- Lever arm design (better than string)
- Motor protection circuits
- Current limiting mandatory
- Safety improvements

### v2.1 - OLED Version (Current)
- OLED standalone
- Button controls
- No Pi needed
- Simpler, cheaper
- Pi version archived

### Future (Possible)
- Combined version (OLED + Pi)
- RPM sensor
- Temperature monitoring
- SD card logging
- Bluetooth app

## Contributing

### Adding Features

**To add to OLED version:**
1. Edit `esp32/torque_analyzer_oled.ino`
2. Update `docs/OLED_SETUP_GUIDE.md`
3. Test thoroughly
4. Document changes

**To improve documentation:**
1. Edit relevant `.md` file
2. Keep formatting consistent
3. Add examples
4. Update table of contents

### Reporting Issues

**Include:**
- Which version (OLED or Pi)
- Hardware setup
- Error messages
- What you tried
- Photos if helpful

## License

**All files:** MIT License
- Free to use
- Free to modify
- Free to share
- Commercial use OK
- No warranty

## Support

**For help:**
1. Check relevant documentation
2. See troubleshooting sections
3. Review wiring diagrams
4. Check calibration
5. Verify component specifications

**Resources:**
- Project documentation in `/docs/`
- Archived Pi version in `/archived/`
- Code comments in `.ino` files
- ASCII diagrams in all guides

## Summary

### Current Project Focus

**Main version:** OLED Standalone
- Cheaper, simpler, portable
- Perfect for most users
- Complete documentation
- Ready to build

**Archived version:** Raspberry Pi 5
- Available for reference
- Can be used for upgrade
- Fully documented
- Web dashboard features

**Both versions:**
- Share same mechanical design
- Share same sensors
- Share same motor protection
- Can be combined later

### Recommended Path

1. **Start:** Build OLED version
2. **Use:** Test motors, gather data
3. **Decide:** Need web dashboard?
4. **Upgrade:** Add Pi later if needed

**Cost progression:**
- OLED only: $40
- Add Pi later: +$70
- Total: $110 (same as building Pi version first)

**Benefit:** Try OLED first, upgrade only if needed!

---

## Quick Reference

**Current main files:**
- `README.md` - Start here
- `esp32/torque_analyzer_oled.ino` - Upload this
- `docs/OLED_SETUP_GUIDE.md` - Follow this

**Archived Pi files:**
- `archived/pi5-version/` - All Pi version files

**Documentation:**
- `/docs/` folder - All technical guides

**Need help?**
- See `docs/OLED_SETUP_GUIDE.md` troubleshooting
- Check `docs/WIRING_OLED.md` for connections
- Review `docs/MOTOR_PROTECTION.md` for safety

---

*For questions about project organization, see this file: `PROJECT_STRUCTURE.md`*
