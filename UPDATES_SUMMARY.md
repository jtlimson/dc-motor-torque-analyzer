# Project Updates Summary

This document summarizes the improvements made to the Mini 4WD Torque Analyzer design.

## Major Design Changes

### 1. Lever Arm Design (Instead of Pulley/String)

**Old Design:**
- Motor → Pulley → String (tension) → Load Cell
- Problems: String can slip, stretch, break

**New Design (IMPROVED):**
- Motor → Lever Arm → Push Load Cell (compression)
- Benefits:
  - ✓ No slipping or breaking
  - ✓ More accurate (no string stretch)
  - ✓ Easier to build
  - ✓ Professional/industrial standard
  - ✓ Better for stall torque testing

**Documentation:** `docs/MECHANICAL_SETUP_LEVER.md`

### 2. Motor Protection Circuit (CRITICAL SAFETY)

**Problem Identified:**
- During stall testing, motors can draw 5-7A (vs 0.5-1.5A normal)
- Can burn out motor in seconds
- Can damage INA219 sensor (max 3.2A)
- Fire hazard

**Solution Added:**
Four protection options provided:

1. **XL4015 Current Limiter Module** (Recommended)
   - Easy to use ($5)
   - Adjustable limit (set to 2.5A)
   - Fast response
   - Most user-friendly

2. **Polyfuse (Resettable Fuse)**
   - Simplest ($1-2)
   - Passive protection
   - Automatic reset
   - Good for budget builds

3. **DIY MOSFET Current Limiter**
   - Professional quality
   - Most precise control
   - Educational/learning project
   - Complete schematic provided

4. **PWM Motor Driver**
   - Software-controlled
   - Variable speed
   - Good for automation
   - BTS7960 or L298N

**Documentation:** `docs/MOTOR_PROTECTION.md`

## New Documentation Files

### Core Documentation
1. **`README.md`** - Updated with safety warnings and new design
2. **`QUICKSTART.md`** - 30-minute setup guide
3. **`UPDATES_SUMMARY.md`** - This file

### Technical Guides
4. **`docs/WIRING.md`** - Updated with:
   - Lever arm mechanical diagram
   - Complete motor protection wiring
   - Four protection circuit options
   - Current limiter setup instructions

5. **`docs/MECHANICAL_SETUP_LEVER.md`** - NEW
   - Complete lever arm design guide
   - Build instructions
   - Parts list
   - Troubleshooting
   - Professional tips

6. **`docs/MOTOR_PROTECTION.md`** - NEW
   - Detailed protection circuits
   - Component specifications
   - Schematics and diagrams
   - Calibration procedures
   - Safety guidelines

7. **`docs/CALIBRATION.md`** - Load cell calibration
8. **`docs/BUILD_GUIDE.md`** - Complete step-by-step build

### Code Files
9. **`esp32/torque_analyzer.ino`** - ESP32 firmware
10. **`raspberry-pi/web_server.py`** - Python web server
11. **`raspberry-pi/requirements.txt`** - Dependencies
12. **`raspberry-pi/templates/index.html`** - Web dashboard

## Key Features of Updated Design

### Mechanical Improvements
- **Lever arm torque measurement**
  - 60-80mm aluminum lever
  - Direct compression on load cell
  - No consumables (string replacement)
  - More durable

### Safety Improvements
- **Current limiting mandatory for stall tests**
  - Protects motor from burnout
  - Protects INA219 sensor
  - Multiple protection options
  - Clear safety warnings

### Documentation Improvements
- **Comprehensive guides**
  - Quick start (30 min)
  - Detailed build guide
  - Multiple diagrams
  - Troubleshooting sections
  - Safety guidelines

## Updated Parts List

### Required Electronics
| Item | Purpose | Cost |
|------|---------|------|
| ESP32 | Microcontroller | $8-12 |
| HX711 | Load cell amplifier | $2-5 |
| Load Cell (100-200g) | Force measurement | $5-10 |
| INA219 | Current/voltage sensor | $5-8 |
| **XL4015 Module** | **Current limiter** | **$3-5** |
| Raspberry Pi 5 | Web server | $60-80 |
| Breadboard & wires | Connections | $5-10 |

**Total: ~$88-130** (with protection)

### Required Mechanical
| Item | Purpose | Cost |
|------|---------|------|
| Aluminum strip 80mm | Lever arm | $2 |
| Wood base 150mm sq | Mounting | $3 |
| M3 screws/nuts | Hardware | $2 |
| Motor mount | Hold motor | DIY/3D print |

**Total: ~$5-10**

## What You Can Measure

### Torque Measurements
- Real-time torque (mN·m)
- Stall torque (safely, with protection)
- Torque curves vs load

### Power Measurements
- Voltage under load
- Current draw
- Power consumption (Watts)
- Motor efficiency

### Applications
- Compare different motors
- Test motor break-in
- Optimize gear ratios
- Battery discharge analysis
- Quality control testing

## Safety Guidelines

### Always Required
1. ✓ Use current limiter (2.5A) for stall tests
2. ✓ Limit stall tests to 3 seconds maximum
3. ✓ Allow 30 second cooling between tests
4. ✓ Monitor motor temperature
5. ✓ Have emergency disconnect ready

### Never Do
1. ✗ Stall test without current limiting
2. ✗ Extended stall tests (>5 seconds)
3. ✗ Test without proper ventilation
4. ✗ Leave test rig unattended while powered
5. ✗ Exceed load cell capacity

## Quick Reference

### Typical Mini 4WD Motor Values

**Normal Operation (Free running):**
| Motor Type | Voltage | Current | Torque | Power |
|------------|---------|---------|--------|-------|
| Standard | 2.4-3V | 0.3-0.8A | 0.5-1.0 mN·m | 0.5-1.2W |
| High-Speed | 2.4-3V | 0.2-0.6A | 0.3-0.7 mN·m | 0.4-0.9W |
| Torque-Tuned | 2.4-3V | 0.5-1.2A | 1.0-2.0 mN·m | 0.7-1.5W |

**Stall Testing (With 2.5A limit):**
| Motor Type | Protected Current | Stall Torque | Notes |
|------------|------------------|--------------|-------|
| Standard | 2.5A | 3-4 mN·m | ~80% of true stall |
| High-Speed | 2.0A | 2-3 mN·m | Use 2.0A limit |
| Torque-Tuned | 2.5-3.0A | 5-6 mN·m | Use 3.0A limit |

### Key Measurements to Record

**Lever Arm Length:**
- Measure precisely with calipers
- From motor shaft center to load cell contact
- Typical: 60-80mm
- **Critical for accuracy!**

**Current Limit Setting:**
- Standard motors: 2.5A
- High-speed motors: 2.0A
- Torque motors: 3.0A
- Never exceed 3.2A (INA219 limit)

**Load Cell Capacity:**
- 100g for short levers (30-50mm)
- 200g for medium levers (60-80mm) ← Recommended
- 500g for long levers (100mm+)

## Getting Started

### 30-Minute Quick Setup
See `QUICKSTART.md`

### Complete Build Guide
See `docs/BUILD_GUIDE.md`

### First-Time Builders
Recommended reading order:
1. `README.md` - Project overview
2. `QUICKSTART.md` - Quick overview
3. `docs/MECHANICAL_SETUP_LEVER.md` - Build lever rig
4. `docs/WIRING.md` - Wire electronics with protection
5. `docs/BUILD_GUIDE.md` - Detailed steps
6. `docs/CALIBRATION.md` - Calibrate sensors
7. `docs/MOTOR_PROTECTION.md` - Understand safety

## Common Questions

**Q: Do I really need current limiting?**
A: **YES!** For stall torque testing, absolutely required. Without it, you will burn motors and potentially damage components.

**Q: Can I skip the current limiter for normal testing?**
A: If you only test motors at light loads (not stall), you can skip it. But it's highly recommended as safety backup.

**Q: Which current limiter should I use?**
A: For beginners: XL4015 module ($5, easy setup). For budget: Polyfuse ($1, simple). For learning: DIY MOSFET circuit.

**Q: Why lever arm instead of string/pulley?**
A: More accurate, no slipping, no breaking, easier to build, professional standard. String works but has limitations.

**Q: What load cell capacity do I need?**
A: 100g or 200g for 60-80mm lever arm. Larger capacity = less sensitive. Smaller = more accurate but easier to overload.

**Q: Can I use a different microcontroller?**
A: ESP32 is recommended for built-in Bluetooth. You could use Arduino + Bluetooth module, but needs code changes.

**Q: Do I need the Raspberry Pi?**
A: Technically no - you can view data on ESP32 serial monitor. But Pi + web dashboard gives better visualization.

## Troubleshooting Quick Reference

**Motor burns during stall test:**
- ❌ No current limiter installed
- ✓ Add XL4015 or Polyfuse immediately

**Readings are negative:**
- ❌ Load cell wired backwards
- ✓ Swap A+ and A- wires, or change calibration factor sign

**Current exceeds 3A:**
- ❌ No current limiting
- ✓ Add current limiter before continuing

**Torque values incorrect:**
- ❌ Wrong lever arm length in code
- ✓ Re-measure and update `TORQUE_ARM_RADIUS_MM`

**Load cell reads 0 or unstable:**
- ❌ Poor wiring or bad load cell
- ✓ Check wiring, add capacitors, verify load cell

## Support

For detailed help:
- **Build issues:** See `docs/BUILD_GUIDE.md`
- **Wiring problems:** See `docs/WIRING.md`
- **Calibration help:** See `docs/CALIBRATION.md`
- **Safety concerns:** See `docs/MOTOR_PROTECTION.md`
- **Mechanical design:** See `docs/MECHANICAL_SETUP_LEVER.md`

## Contributing

This is an open design. Feel free to:
- Improve the design
- Create PCB layouts
- Design 3D printed parts
- Add features (RPM sensor, temperature, etc.)
- Create video tutorials
- Share your builds!

## License

MIT License - Free to use, modify, and share.

---

**Happy building and testing! 🏎️⚡**

Remember: **Safety first!** Always use current limiting for stall tests.
