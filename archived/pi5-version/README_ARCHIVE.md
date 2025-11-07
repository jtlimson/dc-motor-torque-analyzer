# Raspberry Pi 5 Version - ARCHIVED

This is the original Raspberry Pi 5 + Web Dashboard version of the Mini 4WD Torque Analyzer.

## Status: ARCHIVED

**This version has been archived in favor of the simpler OLED standalone version.**

- **Date archived:** 2025-01-15
- **Reason:** OLED version is simpler, cheaper, and better for most users
- **Status:** Fully functional, available for reference or upgrade

## What This Version Offers

### System Architecture

```
ESP32 (Bluetooth) → Raspberry Pi 5 (Web Server) → Phone/Browser (Dashboard)
```

### Features

- ✓ Real-time web dashboard
- ✓ Live updating charts (torque, power)
- ✓ Multiple simultaneous viewers
- ✓ Remote access via WiFi
- ✓ Beautiful web interface
- ✓ WebSocket real-time updates

### Components Required

**Electronics:**
- ESP32 Development Board
- HX711 + Load Cell (100-200g)
- INA219 Current Sensor
- **Raspberry Pi 5** (any RAM size)
- XL4015 Current Limiter
- Breadboard + wires

**Cost:** ~$110 (vs $40 for OLED version)

## Files Included

### Firmware
- `torque_analyzer.ino` - ESP32 firmware with Bluetooth LE

### Raspberry Pi Software
- `raspberry-pi/web_server.py` - Flask web server + BLE receiver
- `raspberry-pi/requirements.txt` - Python dependencies
- `raspberry-pi/templates/index.html` - Web dashboard UI

### Documentation
- `README_ORIGINAL.md` - Original project README

## Why Use This Version?

**Choose Pi version if you need:**
- 📊 Real-time graphs and charts
- 🌐 Remote viewing from phone/tablet/computer
- 📝 Web-based data logging
- 👥 Multiple simultaneous viewers
- 🖥️ Professional dashboard interface

**Don't need these features?**
→ Use the OLED standalone version instead (cheaper, simpler)

## Setup Instructions

### Quick Setup

1. **Build electronics** (same as OLED version)
2. **Upload ESP32 firmware:**
   ```bash
   # Open torque_analyzer.ino in Arduino IDE
   # Upload to ESP32
   ```

3. **Setup Raspberry Pi:**
   ```bash
   cd raspberry-pi
   pip3 install -r requirements.txt
   python3 web_server.py
   ```

4. **Access dashboard:**
   - Open browser on phone
   - Go to: `http://[pi-ip]:5000`

### Detailed Instructions

See main project documentation:
- `../../docs/BUILD_GUIDE.md` - Complete build guide
- `../../docs/WIRING.md` - Wiring diagrams (Pi version)
- `../../docs/CALIBRATION.md` - Load cell calibration

## System Requirements

### ESP32
- Arduino IDE 2.x
- ESP32 board support
- Libraries: HX711, Adafruit INA219

### Raspberry Pi 5
- Raspberry Pi OS (64-bit recommended)
- Python 3.9+
- Bluetooth enabled
- WiFi configured
- Libraries: flask, flask-socketio, bleak

### Client (Phone/Computer)
- Modern web browser
- Connected to same WiFi network
- JavaScript enabled

## Wiring

Same as OLED version, except:

**Remove:**
- OLED display
- 4 control buttons

**Wiring is simpler without buttons/display!**

```
ESP32 Connections:
├─ GPIO 16, 4    → HX711 (Load Cell)
├─ GPIO 21, 22   → INA219 (I2C)
└─ USB           → Power

Motor Protection:
Battery → XL4015 (2.5A) → INA219 → Motor
```

## Web Dashboard Features

### Real-Time Display

**Main Dashboard Cards:**
- Current (A)
- Torque (mN·m)
- Power (W)
- Voltage (V)
- Force (grams)

**Live Charts:**
- Torque history (last 50 points)
- Power history (last 50 points)
- Auto-updating graphs

**Status Indicator:**
- Connected (green)
- Disconnected (red)
- Last update timestamp

### Screenshots

```
┌─────────────────────────────────────┐
│  Mini 4WD Torque Analyzer          │
│  Status: Connected ✓                │
├─────────────────────────────────────┤
│                                     │
│  ┌─────┐  ┌─────┐  ┌─────┐        │
│  │TORQUE│  │POWER│  │CURRENT│      │
│  │58.3 │  │2.34 │  │0.85 │        │
│  │mN·m │  │ W   │  │ A   │        │
│  └─────┘  └─────┘  └─────┘        │
│                                     │
│  ┌─────────────────────────────┐   │
│  │  Torque History            │   │
│  │         /\    /\           │   │
│  │    /\  /  \  /  \          │   │
│  └─────────────────────────────┘   │
│                                     │
│  ┌─────────────────────────────┐   │
│  │  Power History             │   │
│  │         /\    /\           │   │
│  │    /\  /  \  /  \          │   │
│  └─────────────────────────────┘   │
│                                     │
└─────────────────────────────────────┘
```

## When to Use This Version

### Use Cases

**✓ Good for:**
- Laboratory/workshop setup
- Long-term data collection
- Automated testing rigs
- Research and analysis
- Teaching demonstrations
- Remote monitoring

**✗ Not ideal for:**
- Quick motor comparisons
- Portable testing
- Budget-conscious builds
- Simple hobby use
→ Use OLED version instead

## Migrating from OLED Version

**Already built OLED version? Easy upgrade:**

1. **Keep all hardware** (ESP32, sensors, mechanical setup)
2. **Remove OLED and buttons** (optional - can keep both!)
3. **Change ESP32 code** from `torque_analyzer_oled.ino` to `torque_analyzer.ino`
4. **Add Raspberry Pi** with web server
5. **Access via browser** on any device

**Cost to upgrade:** +$60-80 (just the Pi)

## Combining Both Versions

**Want both OLED and web dashboard?**

**It's possible!** Modify code to have:
- OLED display (local viewing)
- Bluetooth transmission (remote viewing)
- Both active simultaneously

**Benefits:**
- See data locally on OLED
- Also view on phone/computer
- Best of both worlds
- Ultimate flexibility

**Code changes needed:**
- Keep OLED display code
- Keep button controls
- Add Bluetooth transmission
- Requires more memory but ESP32 can handle it

## Technical Specifications

### Bluetooth LE

**Service UUID:**
```
4fafc201-1fb5-459e-8fcc-c5c9c331914b
```

**Characteristic UUID:**
```
beb5483e-36e1-4688-b7f5-ea07361b26a8
```

**Data Format (JSON):**
```json
{
  "f": 125.3,        // force (grams)
  "t": 73.2,         // torque (mN.m)
  "v": 2.87,         // voltage (V)
  "i": 850.5,        // current (mA)
  "p": 2.44,         // power (W)
  "ts": 123456       // timestamp (ms)
}
```

### Web Server

**Framework:** Flask + Flask-SocketIO
**Port:** 5000 (HTTP)
**WebSocket:** Socket.IO
**Update Rate:** Real-time (as data arrives)

**API Endpoints:**
- `GET /` - Main dashboard
- `GET /api/data` - Current data (JSON)
- `GET /api/status` - Connection status
- WebSocket: Real-time data push

### Performance

**Latency:**
- ESP32 → Pi: ~20ms (Bluetooth)
- Pi → Browser: ~10ms (WebSocket)
- Total: ~30ms (30 FPS equivalent)

**Throughput:**
- 10 samples/second
- ~200 bytes/sample
- ~2 KB/s data rate

## Troubleshooting

### ESP32 Not Advertising

**Check:**
- BLE initialized correctly
- Device name: "Mini4WD_Torque"
- Check Serial Monitor for errors

### Pi Can't Find ESP32

**Solutions:**
- Ensure Bluetooth enabled on Pi: `sudo systemctl start bluetooth`
- Move ESP32 closer to Pi
- Restart ESP32
- Check BLE device is advertising: `sudo hcitool lescan`

### Web Dashboard Not Loading

**Check:**
- Flask server running: `python3 web_server.py`
- Correct IP address: `hostname -I`
- Firewall not blocking port 5000
- Phone on same WiFi network

### Graphs Not Updating

**Solutions:**
- Check browser console for errors
- Verify WebSocket connection
- Refresh page
- Check ESP32 is sending data

## Performance Comparison

### OLED vs Pi Version

| Metric | OLED | Pi + Web |
|--------|------|----------|
| **Cost** | $40 | $110 |
| **Setup Time** | 1 hr | 2 hrs |
| **Display Lag** | 0 ms | 30 ms |
| **Portability** | Excellent | Poor |
| **Data Logging** | Serial only | Web/file |
| **Graphs** | No | Yes |
| **Multi-viewer** | No | Yes |
| **WiFi Required** | No | Yes |

## Software Dependencies

### ESP32 (Arduino)
```
HX711 by Bogdan Necula
Adafruit INA219 by Adafruit
ESP32 BLE Arduino by Espressif
```

### Raspberry Pi (Python)
```
flask==3.0.0
flask-socketio==5.3.5
bleak==0.21.1
python-socketio==5.10.0
eventlet==0.33.3
```

## Future Enhancements

**Possible additions:**

1. **Database logging** - Store data in SQLite/PostgreSQL
2. **CSV export** - Download test results
3. **User accounts** - Multiple users/projects
4. **Email alerts** - Notify on overcurrent
5. **API access** - RESTful API for automation
6. **Mobile app** - Native iOS/Android app
7. **Cloud sync** - Upload to cloud storage

## Support & Resources

**Documentation:**
- See main project `/docs/` folder
- All guides apply to Pi version
- Mechanical setup identical
- Only software differs

**Community:**
- Share your Pi dashboard customizations
- Post screenshots
- Contribute improvements

## License

MIT License - Free to use, modify, and share

---

## Summary

This **Raspberry Pi 5 version** is fully functional and offers:

✓ Professional web dashboard
✓ Real-time graphs
✓ Remote monitoring
✓ Multiple viewers
✓ Data logging capabilities

**But it's archived because:**
- More expensive ($110 vs $40)
- More complex setup
- Less portable
- Overkill for most users

**Recommendation:**
- Start with OLED version
- Upgrade to Pi version later if needed
- Or combine both for ultimate system!

---

**For current project, use OLED version:** See main `README.md`

**Questions about Pi version?** All documentation still available in `/docs/`

**Want to upgrade?** All files here, ready to use!
