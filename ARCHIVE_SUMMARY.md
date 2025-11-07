# Archive Summary - Raspberry Pi Version

**Date:** 2025-01-15
**Action:** Archived Raspberry Pi 5 + Web Dashboard version
**Reason:** OLED standalone version is simpler, cheaper, and better for most users

---

## What Was Archived

### Files Moved to `archived/pi5-version/`

**Raspberry Pi Software:**
- ✓ `raspberry-pi/web_server.py` → `archived/pi5-version/raspberry-pi/web_server.py`
- ✓ `raspberry-pi/requirements.txt` → `archived/pi5-version/raspberry-pi/requirements.txt`
- ✓ `raspberry-pi/templates/index.html` → `archived/pi5-version/raspberry-pi/templates/index.html`

**Original Documentation:**
- ✓ `README.md` → `archived/pi5-version/README_ORIGINAL.md`

**Bluetooth Firmware:**
- ✓ `esp32/torque_analyzer.ino` → `archived/pi5-version/torque_analyzer.ino` (copy kept in esp32/)

**New Archive Documentation:**
- ✓ Created `archived/pi5-version/README_ARCHIVE.md` - Complete Pi version guide

### Files Kept in Main Project

**Still Available:**
- `esp32/torque_analyzer.ino` - Original Bluetooth firmware (for reference/upgrade)
- `docs/WIRING.md` - Original wiring guide (Pi version)
- `docs/BUILD_GUIDE.md` - Still applies to both versions
- `docs/CALIBRATION.md` - Same for both versions
- `docs/MECHANICAL_SETUP_LEVER.md` - Same for both versions
- `docs/MOTOR_PROTECTION.md` - Same for both versions

---

## New Project Structure

### Main Project (OLED Version)

```
mini4wd-torque-analyzer/
├── README.md                       # ★ OLED version (main)
├── PROJECT_STRUCTURE.md            # Project organization guide
├── OLED_VERSION_SUMMARY.md         # What's new in OLED
├── ARCHIVE_SUMMARY.md              # This file
│
├── esp32/
│   ├── torque_analyzer_oled.ino   # ★ Main firmware (OLED)
│   └── torque_analyzer.ino        # Bluetooth (still available)
│
└── docs/
    ├── OLED_SETUP_GUIDE.md        # ★ Complete OLED setup
    ├── WIRING_OLED.md             # ★ OLED wiring
    ├── BUILD_GUIDE.md
    ├── CALIBRATION.md
    ├── MECHANICAL_SETUP_LEVER.md
    └── MOTOR_PROTECTION.md
```

### Archived (Pi Version)

```
archived/
└── pi5-version/
    ├── README_ARCHIVE.md           # Pi version documentation
    ├── README_ORIGINAL.md          # Original README
    ├── torque_analyzer.ino         # Bluetooth firmware
    └── raspberry-pi/
        ├── web_server.py           # Flask server
        ├── requirements.txt        # Python deps
        └── templates/
            └── index.html          # Web dashboard
```

---

## What Changed in Main Project

### Before Archive
```
Main README → Pi version (Bluetooth + Web Dashboard)
Main firmware → torque_analyzer.ino (Bluetooth)
Focus → Advanced web-based system
Cost → $110
```

### After Archive
```
Main README → OLED version (Standalone display)
Main firmware → torque_analyzer_oled.ino (Direct display)
Focus → Simple standalone system
Cost → $40
```

---

## Why Archive the Pi Version?

### Reasons

1. **Cost:** $110 vs $40
2. **Complexity:** 2 hours setup vs 1 hour
3. **Dependencies:** Requires Pi, WiFi, Python, Linux knowledge
4. **Portability:** Not battery-friendly, needs WiFi
5. **Overkill:** Most users don't need web dashboard

### User Feedback

**Most users wanted:**
- ✓ Simple direct display
- ✓ Portable operation
- ✓ Lower cost
- ✓ Faster setup

**Few users needed:**
- Graphs and logging
- Remote viewing
- Multiple viewers
- Web interface

**Solution:** Make OLED main version, keep Pi version available for those who need it

---

## Pi Version Still Available!

### Access Archived Version

**Location:** `archived/pi5-version/`

**Contents:**
- Complete Pi setup guide
- All Python code
- Web dashboard HTML/CSS/JS
- Bluetooth ESP32 firmware
- Setup instructions

**Status:** Fully functional, just archived

### When to Use Pi Version

**Use Pi version if you need:**
- 📊 Real-time graphs
- 🌐 Web-based dashboard
- 📝 Data logging to file
- 👥 Multiple simultaneous viewers
- 🔗 Remote access via WiFi

**Otherwise:** Use OLED version (simpler, cheaper)

### Upgrading from OLED to Pi

**Easy migration path:**

1. Build OLED version now (~$40)
2. Use and test
3. Later, add Raspberry Pi (+$70)
4. Install Pi software from archive
5. Both displays can work together!

**Total cost same as building Pi version first:** $110

**Benefit:** Try OLED first, only upgrade if you really need web features

---

## Files Reference

### For OLED Version (Current)

**Start here:**
1. `README.md` - Main overview
2. `docs/OLED_SETUP_GUIDE.md` - Complete setup
3. `docs/WIRING_OLED.md` - Wiring diagrams

**Firmware:**
- `esp32/torque_analyzer_oled.ino` - Upload this

### For Pi Version (Archived)

**Start here:**
1. `archived/pi5-version/README_ARCHIVE.md` - Pi version guide

**Firmware:**
- `archived/pi5-version/torque_analyzer.ino` - Bluetooth ESP32

**Software:**
- `archived/pi5-version/raspberry-pi/web_server.py` - Flask server
- `archived/pi5-version/raspberry-pi/templates/index.html` - Dashboard

### Common Documentation (Both Versions)

**Applies to both:**
- `docs/BUILD_GUIDE.md` - Building guide
- `docs/CALIBRATION.md` - Load cell calibration
- `docs/MECHANICAL_SETUP_LEVER.md` - Lever arm design
- `docs/MOTOR_PROTECTION.md` - Current limiting

---

## Version Comparison

### Features

| Feature | OLED (Main) | Pi (Archived) |
|---------|-------------|---------------|
| **Display** | OLED 128x64 | Web browser |
| **Controls** | 4 buttons | None (web UI) |
| **Graphs** | No | Yes |
| **Data logging** | Serial only | Web/file |
| **Remote access** | No | Yes |
| **Multi-viewer** | No | Yes |
| **Portable** | Yes | No |
| **WiFi required** | No | Yes |

### Cost

| Component | OLED | Pi | Both |
|-----------|------|-------|------|
| ESP32 | $8 | $8 | $8 |
| Sensors | $20 | $20 | $20 |
| Protection | $5 | $5 | $5 |
| **OLED + Buttons** | **$7** | **-** | **$7** |
| **Raspberry Pi 5** | **-** | **$70** | **$70** |
| Other | $5 | $7 | $5 |
| **Total** | **$45** | **$110** | **$115** |

### Setup Time

| Task | OLED | Pi |
|------|------|-----|
| Hardware assembly | 45 min | 30 min |
| Software setup | 15 min | 60 min |
| Calibration | 15 min | 15 min |
| **Total** | **~1 hour** | **~2 hours** |

---

## Migration Scenarios

### Scenario 1: Start with OLED (Recommended)

```
Now: Build OLED ($40, 1 hour)
     ↓
Use: Test motors, gather experience
     ↓
Later: Add Pi if needed (+$70, +1 hour)
```

**Benefits:**
- Try cheap version first
- Learn the system
- Upgrade only if needed
- No wasted components

### Scenario 2: Start with Pi (Advanced)

```
Now: Build Pi version ($110, 2 hours)
     ↓
Use: Web dashboard, graphs, logging
```

**Good for:**
- Lab/workshop permanent setup
- Need graphs immediately
- Multiple users
- Remote monitoring

### Scenario 3: Build Both (Ultimate)

```
Now: Build OLED ($40, 1 hour)
     ↓
Then: Add Pi system (+$70, +1 hour)
      Keep OLED working
     ↓
Result: Local + Remote display
```

**Benefits:**
- Best of both worlds
- OLED for quick checks
- Web for detailed analysis
- Ultimate flexibility

---

## Future Plans

### OLED Version Enhancements

**Easy to add:**
1. SD card logging
2. Temperature sensor
3. RPM measurement
4. Battery monitor
5. Motor efficiency calculation

### Pi Version Enhancements

**Could be added to archived version:**
1. Database logging (PostgreSQL)
2. CSV export
3. User accounts
4. Email alerts
5. Mobile app
6. Cloud sync

### Combined Version

**Possible future:**
- Merge OLED + Bluetooth code
- Both displays active
- OLED for local, web for remote
- Best of both worlds

---

## Community

### Sharing Your Build

**OLED version:**
- Take photos of your OLED display
- Share button layout
- Post measurement results
- Contribute improvements

**Pi version:**
- Share dashboard customizations
- Post CSS improvements
- Contribute features
- Share test results

### Contributing

**To improve OLED version:**
- Edit `esp32/torque_analyzer_oled.ino`
- Update documentation
- Test and submit

**To improve Pi version:**
- Edit files in `archived/pi5-version/`
- Enhance web dashboard
- Add features
- Submit improvements

---

## Support

### Getting Help

**For OLED version:**
- See `docs/OLED_SETUP_GUIDE.md`
- Check troubleshooting section
- Review wiring in `docs/WIRING_OLED.md`

**For Pi version:**
- See `archived/pi5-version/README_ARCHIVE.md`
- Check Pi setup instructions
- Review Python dependencies

**Common issues:**
- Calibration: `docs/CALIBRATION.md`
- Motor protection: `docs/MOTOR_PROTECTION.md`
- Mechanical: `docs/MECHANICAL_SETUP_LEVER.md`

### Resources

**Documentation:**
- Main project: `/docs/` folder
- OLED guides: OLED-specific .md files
- Pi guides: `archived/pi5-version/`
- General: BUILD_GUIDE, CALIBRATION, etc.

**Code:**
- OLED firmware: `esp32/torque_analyzer_oled.ino`
- Pi firmware: `archived/pi5-version/torque_analyzer.ino`
- Web server: `archived/pi5-version/raspberry-pi/web_server.py`

---

## Summary

### What Happened

**Archived:**
- Raspberry Pi 5 version moved to `archived/pi5-version/`
- Still fully functional and documented
- Available for advanced users

**Now Main:**
- OLED standalone version
- Simpler, cheaper, more portable
- Perfect for most users

**Result:**
- Better project organization
- Clear main path (OLED)
- Advanced option still available (Pi)
- Easy upgrade path

### Recommendations

**For most users:**
→ Build OLED version (main README)

**For advanced users:**
→ See archived Pi version if you need web features

**For flexible users:**
→ Build OLED first, add Pi later if needed

### Quick Links

**Build OLED version:**
- `README.md` - Start here
- `docs/OLED_SETUP_GUIDE.md` - Complete guide
- `docs/WIRING_OLED.md` - Wiring

**Use Pi version:**
- `archived/pi5-version/README_ARCHIVE.md` - Pi guide

**Understand project:**
- `PROJECT_STRUCTURE.md` - File organization
- `OLED_VERSION_SUMMARY.md` - What's new
- `ARCHIVE_SUMMARY.md` - This file

---

## Conclusion

The Raspberry Pi version has been carefully archived, not deleted. It's a fully functional, well-documented version that remains available for users who need web-based features.

The OLED standalone version is now the recommended starting point because it's:
- ✅ Simpler to build
- ✅ Cheaper ($40 vs $110)
- ✅ Faster to set up
- ✅ More portable
- ✅ Perfect for most Mini 4WD testing

Both versions share the same excellent mechanical design (lever arm), motor protection circuits, and measurement accuracy. The only difference is how you view the data.

**Start building today! 🏎️⚡**

---

*Last updated: 2025-01-15*
*Archive performed: Julius Limson*
*Project: Mini 4WD Torque Analyzer*
