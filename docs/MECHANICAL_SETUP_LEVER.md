# Mechanical Setup - Lever Arm Design

This guide covers the **lever arm compression** method for measuring Mini 4WD motor torque.

## Why Lever Arm Design?

**Advantages over pulley/string:**
- No slipping or string breakage
- Higher accuracy (no string stretch)
- Easier to build and maintain
- Better for stall torque measurement
- More professional/repeatable results

## Overview

```
                Motor
                  |
                  | shaft (vertical)
                  |
         =========●========= Lever arm (horizontal)
                  ↓
                  ↓ Force pushes down
              [Load Cell]  ← Compression mode
                  |
              Fixed Mount
```

**Principle:** Motor shaft rotates lever arm, which pushes down on load cell.
**Torque = Force × Lever arm length**

---

## Parts List

### Mechanical Components

| Part | Specification | Source | Notes |
|------|--------------|--------|-------|
| Lever arm | Aluminum strip 80mm × 15mm × 2mm | Hardware store | Must be rigid, not flexible |
| Motor mount | 3D printed or wood block | DIY | Holds motor vertically |
| Base plate | Wood 150mm × 150mm × 10mm | Hardware store | Stable base |
| Shaft coupler | 2mm bore, 5mm length | Hobby store | Connects lever to motor shaft |
| M3 screws | 10mm length, qty 4-6 | Hardware store | For mounting |
| M3 nuts | qty 4-6 | Hardware store | For mounting |

### Alternative Lever Materials

1. **Aluminum strip** (Best)
   - Rigid, lightweight
   - Easy to cut and drill
   - Available at hardware stores

2. **Plastic ruler** (Good)
   - Already marked with measurements
   - Free/cheap
   - May flex slightly

3. **3D printed arm** (Best for custom)
   - Perfect fit to motor shaft
   - Can add features (bearing mount, adjustable length)
   - Requires 3D printer

4. **Brass strip** (Professional)
   - Very rigid
   - Higher mass (more stable)
   - More expensive

---

## Build Instructions

### Step 1: Prepare Base Plate

1. **Cut base plate:**
   - Wood: 150mm × 150mm × 10mm
   - Sand edges smooth

2. **Mark mounting locations:**
   ```
   Motor mount: Center, towards back
   Load cell mount: Center, 30-40mm forward from motor
   ```

3. **Drill holes:**
   - M3 holes for motor mount
   - M3 holes for load cell mount

### Step 2: Create Motor Mount

**Option A: 3D Printed Mount**
- Design a vertical motor holder
- Include mounting holes for base plate
- Shaft points upward

**Option B: Wood Block**
1. Cut wood block: 40mm × 40mm × 60mm
2. Drill hole for motor body (diameter = motor diameter)
3. Motor sits vertically with shaft pointing up
4. Secure with hot glue or small screws
5. Attach block to base plate with M3 screws

### Step 3: Prepare Lever Arm

1. **Cut lever arm:**
   - Length: 80mm (60mm effective + 20mm for shaft coupling)
   - Width: 15mm
   - Material: 2mm aluminum strip

2. **Drill center hole:**
   - Position: 20mm from one end
   - Diameter: Motor shaft diameter + 0.1mm
   - Use drill press for accuracy

3. **Mark measurement:**
   - Measure from center hole to contact point
   - Example: 60mm effective lever arm
   - Mark this for reference

4. **Optional - Add bearing:**
   - Small ball bearing at contact point
   - Reduces friction with load cell
   - Hot glue or press-fit

5. **Balance check:**
   - Lever should be roughly balanced at shaft hole
   - If heavy on one side, trim or add small weight

### Step 4: Mount Load Cell

1. **Orientation:**
   - Load cell horizontal, parallel to base
   - Arrow (if marked) points down (compression)
   - Fixed end towards back, free end towards front

2. **Height alignment:**
   - Free end of load cell should be directly under lever arm contact point
   - Adjust motor mount height if needed

3. **Secure load cell:**
   - Fixed end: M3 screw to base plate
   - Free end: Must be able to move (don't screw down!)
   - Use small wood blocks to elevate if needed

### Step 5: Assemble System

1. **Mount motor:**
   - Insert motor into vertical mount
   - Shaft pointing up
   - Secure with hot glue or clamp

2. **Attach lever arm:**
   - Slide lever arm onto motor shaft
   - If loose, use set screw or small shaft coupler
   - If tight, ream hole slightly larger
   - Lever must rotate with shaft (no slipping!)

3. **Align lever with load cell:**
   - Lever arm horizontal when motor is off
   - Lever end directly above load cell free end
   - Gap: 1-2mm (lever not resting on load cell when motor is off)

4. **Verify free rotation:**
   - Gently rotate motor shaft by hand
   - Lever should rotate smoothly
   - Should touch load cell on one side only

---

## Critical Measurements

### Lever Arm Length (R)

This is **critical for accuracy!**

**Measurement:**
- From **center of motor shaft** to **center of load cell contact point**
- Use calipers or precise ruler
- Measure in millimeters

**Example:**
```
    Motor Shaft
        ↓
    ====●============================
        |<------ 60.0 mm ----------->| Contact point
```

**Record this value** - you'll enter it in ESP32 code:
```cpp
#define TORQUE_ARM_RADIUS_MM  60.0  // YOUR MEASURED VALUE
```

### Load Cell Alignment

**Horizontal alignment:**
```
Top View:

    Motor
      |
    ==●== Lever
      ↓
    [====] Load Cell

    ✓ Aligned - Force perpendicular to load cell
```

**Incorrect alignment:**
```
    Motor
      |
    ==●== Lever
       ↘
    [====] Load Cell

    ✗ Side load - Will damage load cell!
```

---

## Testing Mechanical Setup

### Free Rotation Test

1. Remove batteries (motor off)
2. Manually rotate motor shaft
3. Lever should rotate 360° smoothly
4. No binding or rubbing

### Contact Test

1. Rotate lever to contact load cell
2. Should make solid contact
3. No wobble or play
4. Contact point should be consistent

### Load Cell Response Test

1. Power on ESP32 (motors off)
2. Watch Serial Monitor
3. Manually push lever against load cell
4. Should see force reading increase
5. Release - should return to ~0

---

## Operating Principle

### No Load (Motor Off)
```
Lever horizontal:     =========●=========
                              ↑ No force
                          [Load Cell]
                          Reading: ~0g
```

### Motor Running (Generating Torque)
```
Motor tries to rotate clockwise:
                     =========●=========
                              ↓ Force
                          [Load Cell]
                          Reading: Force in grams
```

### Calculation

**Torque = Force × Lever Arm Length**

Example:
- Force: 100 grams
- Lever length: 60mm = 0.06m
- Force in Newtons: 100g × 9.81 / 1000 = 0.981 N
- Torque: 0.981 N × 0.06 m = 0.0589 N·m = 58.9 mN·m

**ESP32 does this automatically!**

---

## Advantages of Different Lever Lengths

### Short Lever (30-50mm)
**Pros:**
- Higher force on load cell (easier to measure)
- More rigid (less flex)
- Compact design

**Cons:**
- Lower sensitivity
- Need higher capacity load cell

### Medium Lever (60-80mm) - **Recommended**
**Pros:**
- Good balance of sensitivity and strength
- Standard 100-200g load cell works well
- Easy to build

**Cons:**
- None significant

### Long Lever (100-150mm)
**Pros:**
- Very high sensitivity
- Can use small capacity load cell (50g)

**Cons:**
- May flex (reduces accuracy)
- Larger rig size
- May exceed load cell capacity at stall

**Recommendation:** Use **60-80mm** for Mini 4WD motors

---

## Load Cell Capacity Selection

**Mini 4WD Torque → Force Calculation:**

For a 60mm lever arm:
- Low torque motor (0.5 mN·m) → ~8g force
- Standard motor (1.0 mN·m) → ~17g force
- High torque motor (2.0 mN·m) → ~34g force
- **Stall torque (5 mN·m)** → ~85g force

**Recommended load cell capacity: 100g or 200g**
- Enough range for stall torque
- Good resolution for low loads
- Safe margin (not overloaded)

---

## Stall Torque Testing

**What is stall torque?**
Motor torque when shaft is prevented from rotating (maximum torque).

### Safety Notes

⚠️ **WARNING:** Stall testing generates heat and high current!

**Safety measures:**
1. Test for **maximum 3 seconds**
2. Let motor cool 30 seconds between tests
3. Monitor current - should be 1-3A for Mini 4WD motors
4. Check for smoke/burning smell
5. Don't exceed motor's rated stall current

### Stall Test Procedure

1. **Set lever to contact load cell**
2. **Power on system, zero load cell**
3. **Apply power to motor for 2-3 seconds**
4. **Motor shaft can't rotate (lever blocked by load cell)**
5. **Record maximum torque reading**
6. **Cut power immediately**
7. **Let motor cool 30 seconds**

**Typical stall torque values:**
- Standard Mini 4WD motor: 3-5 mN·m
- High torque motor: 5-8 mN·m
- Brushless motor: 8-15 mN·m

---

## Maintenance

### Regular Checks

**Every session:**
- Verify lever arm is tight on shaft (no slipping)
- Check load cell alignment
- Zero calibration (tare)

**Weekly:**
- Check for lever arm flex/bend
- Tighten mounting screws
- Clean contact surfaces

**Monthly:**
- Re-calibrate load cell
- Check for wear on contact point
- Verify torque arm measurement

### Common Issues

**Lever slipping on shaft:**
- Add set screw
- Use shaft coupler with lock screw
- Apply small drop of threadlock (removable type)

**Inconsistent readings:**
- Check for lever flex (use thicker material)
- Verify load cell alignment
- Check for side loads

**Load cell drifting:**
- Allow 5-minute warmup
- Re-tare before each test
- Check temperature stability

---

## Design Variants

### Professional Version

Add these features:
1. **Adjustable lever length** - Multiple mounting holes
2. **Bearing at contact point** - Reduces friction
3. **Optical RPM sensor** - Measure speed simultaneously
4. **Clutch mechanism** - Prevent motor damage during stall
5. **Emergency stop button** - Quick power cutoff

### Dual Motor Comparison

Mount two motors side-by-side:
- Compare motors simultaneously
- Same load cell reads both
- Switch between motors via relay

### Portable Version

- Battery powered Raspberry Pi (USB power bank)
- Compact design (folds flat)
- Carry case

---

## Summary

**Key measurements:**
- Lever arm length: 60-80mm (measure precisely!)
- Load cell capacity: 100-200g
- Load cell mode: Compression (pushing)

**Critical success factors:**
1. ✓ Rigid lever arm (no flex)
2. ✓ Precise lever length measurement
3. ✓ Perfect alignment (no side loads)
4. ✓ Tight shaft coupling (no slipping)
5. ✓ Proper calibration

**Ready to build?**
1. Gather materials
2. Build base and mounts
3. Install and align components
4. Wire electronics (see WIRING.md)
5. Upload firmware and calibrate
6. Start testing!

---

For more information:
- `BUILD_GUIDE.md` - Complete build instructions
- `WIRING.md` - Electronics wiring
- `CALIBRATION.md` - Load cell calibration
