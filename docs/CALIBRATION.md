# Load Cell Calibration Guide

Proper calibration is critical for accurate torque measurements. This guide covers load cell calibration in detail.

## Why Calibration is Needed

Load cells produce electrical signals proportional to applied force, but:
- Each load cell has slightly different characteristics
- Signal amplification varies between HX711 modules
- Temperature affects readings
- Mechanical mounting introduces offset

**Calibration corrects these variations for accurate gram/Newton readings.**

---

## Calibration Methods

### Method 1: Simple Calibration (Recommended)

**What you need:**
- Known weight (50g, 100g, or 200g)
- Kitchen scale (to verify weight)

**Steps:**

1. **Upload base firmware:**
   - Upload `torque_analyzer.ino` to ESP32
   - Open Serial Monitor (115200 baud)

2. **Zero the scale:**
   - Remove all weight from load cell
   - Wait for stable reading
   - Press ESP32 reset button
   - Load cell is now "tared" (zeroed)

3. **Record zero reading:**
   ```
   Force: 0.23g | Torque: 0.02 mN.m | ...
   ```
   Small fluctuations (±1g) are normal.

4. **Apply known weight:**
   - Gently place/hang a 100g weight
   - Wait 2-3 seconds for stable reading
   - Record displayed value

   Example:
   ```
   Force: 87.34g | Torque: 85.67 mN.m | ...
   ```

5. **Calculate new calibration factor:**

   Formula:
   ```
   New Factor = Old Factor × (Known Weight / Displayed Weight)
   ```

   Example:
   ```
   Known Weight: 100.0g
   Displayed Weight: 87.34g
   Old Factor: -7050 (from code)

   New Factor = -7050 × (100.0 / 87.34)
              = -7050 × 1.1449
              = -8071
   ```

6. **Update code:**
   ```cpp
   #define CALIBRATION_FACTOR  -8071.0  // Your calculated value
   ```

7. **Re-upload and verify:**
   - Upload updated code
   - Reset ESP32
   - Apply 100g weight again
   - Should now read: `Force: 100.0g` (±2%)

8. **Multi-point verification:**
   - Test with 50g: Should read ~50g
   - Test with 200g: Should read ~200g
   - If readings are off by >5%, repeat calibration

---

### Method 2: Advanced Multi-Point Calibration

For maximum accuracy, use multiple known weights.

**What you need:**
- Multiple calibrated weights (50g, 100g, 150g, 200g)
- Spreadsheet software (Excel/Google Sheets)

**Steps:**

1. **Collect raw ADC readings:**

   Modify Arduino code to print raw values:
   ```cpp
   void loop() {
     long raw = scale.read_average(10);
     Serial.print("Raw: ");
     Serial.println(raw);
     delay(1000);
   }
   ```

2. **Record data table:**

   | Weight (g) | Raw ADC Value |
   |------------|---------------|
   | 0 | 8523146 |
   | 50 | 8172634 |
   | 100 | 7822122 |
   | 150 | 7471610 |
   | 200 | 7121098 |

3. **Calculate calibration factor:**

   Using linear regression:
   ```
   Slope = (Raw_200g - Raw_0g) / (200 - 0)
   Slope = (7121098 - 8523146) / 200
   Slope = -7010.24

   Calibration Factor ≈ -7010
   ```

4. **Update and test as in Method 1**

---

## Calibration Weights

### Finding Known Weights

**Option 1: Calibrated weights**
- Purchase from Amazon (~$10 for 50g-200g set)
- Most accurate method

**Option 2: Coins** (Verify with kitchen scale!)

| Country | Coin | Weight |
|---------|------|--------|
| USA | Nickel (5¢) | 5.00g |
| USA | Penny (post-1982) | 2.50g |
| USA | Quarter (25¢) | 5.67g |
| Canada | Loonie ($1) | 6.27g |
| EU | 1 Euro | 7.50g |
| Japan | 10 Yen | 4.50g |

**Combine coins to reach target weight:**
- 100g = 20 US Nickels
- 50g = 10 US Nickels

**IMPORTANT:** Verify coin weight with kitchen scale first!

**Option 3: Kitchen items**
- 1 US tablespoon water ≈ 15g
- 1 stick butter ≈ 113g
- Weigh with kitchen scale first!

---

## Load Cell Orientation

### Tension vs Compression

Load cells can measure:
- **Tension** (pulling force) - Typical for motor torque
- **Compression** (pushing force)

**For Mini 4WD torque testing:**
- Use **tension mode**
- String pulls on load cell
- Positive force = motor pulling

### Mounting Tips

```
        Motor
          |
        Pulley
          |
       String (tension)
          |
    Load Cell ━━━ Fixed Mount
    (free end)    (fixed end)
```

**Correct mounting:**
- One end firmly attached to fixed mount
- Other end free to move (attach string here)
- Load cell horizontal for best accuracy
- No bending or twisting forces

**Common mistakes:**
- Both ends fixed (won't measure)
- Load cell tilted (introduces error)
- Side loads (damages sensor)

---

## Verifying Calibration

### Test Procedure

1. **Zero test:**
   - No load → Reading should be 0±1g

2. **Known weight test:**
   - 100g → Reading should be 100±2g (2% error)

3. **Linearity test:**
   - 50g → ~50g
   - 100g → ~100g
   - 150g → ~150g
   - Plot values - should be linear

4. **Repeatability test:**
   - Apply and remove 100g five times
   - All readings should be within ±2g

5. **Hysteresis test:**
   - Apply 100g → Record reading
   - Remove weight → Should return to 0
   - Reapply 100g → Should match first reading

### Acceptable Error

- **±2%** - Excellent (for DIY setup)
- **±5%** - Acceptable
- **>5%** - Recalibrate

Example: 100g weight reading 98-102g = 2% error ✓

---

## Troubleshooting Calibration

### Problem: Readings Drift

**Causes:**
- Temperature changes
- Poor electrical connections
- Load cell overload damage

**Solutions:**
- Wait 5 minutes for thermal stabilization
- Check wire connections (resolder if needed)
- Add 0.1µF capacitor across HX711 VCC/GND
- Reduce sampling rate

### Problem: Negative Readings

**Causes:**
- Load cell wired backwards
- Compression instead of tension

**Solutions:**
- Swap A+ and A- wires on HX711
- Or change calibration factor sign (+ to -)

### Problem: Unstable Readings (±10g fluctuation)

**Causes:**
- Vibration
- EMI (electromagnetic interference)
- Poor power supply
- Loose connections

**Solutions:**
- Mount load cell on vibration-dampening surface
- Twist HX711 data wires together
- Use shielded cable
- Add 100µF capacitor across ESP32 power
- Keep wires away from motor

### Problem: Non-Linear Response

**Example:**
- 50g → 48g (Good)
- 100g → 100g (Good)
- 200g → 210g (Bad - non-linear)

**Causes:**
- Load cell overloaded (exceeds max capacity)
- Mechanical binding
- Damaged load cell

**Solutions:**
- Use lower max capacity load cell (e.g., 100g instead of 500g)
- Check for mechanical interference
- Replace load cell if damaged

### Problem: Zero Drift Over Time

**Causes:**
- Temperature changes
- Creep (material relaxation)

**Solutions:**
- Allow warmup time (5 minutes)
- Re-tare before each test session
- Use higher quality load cell
- Temperature compensation (advanced)

---

## Calibration Maintenance

### When to Recalibrate

- After mechanical changes (remounting)
- Every 1-2 months (for accurate work)
- If readings seem incorrect
- After dropping/damaging load cell
- Significant temperature change (>10°C)

### Calibration Log

Keep a record:

| Date | Calibration Factor | Test Weight | Accuracy | Notes |
|------|-------------------|-------------|----------|-------|
| 2025-01-15 | -7050 | 100g | 87.3g | Initial |
| 2025-01-15 | -8071 | 100g | 100.1g | Calibrated |
| 2025-02-20 | -8071 | 100g | 99.8g | Verified |

---

## Advanced: Temperature Compensation

Load cells are affected by temperature. For precision work:

**Method:**
1. Measure at multiple temperatures (15°C, 20°C, 25°C)
2. Record calibration factor for each
3. Add temperature sensor (e.g., DHT22)
4. Apply correction in code:

```cpp
float tempCoeff = 0.02; // 2% per 10°C (typical)
float refTemp = 20.0; // Calibration temperature
float currentTemp = dht.readTemperature();
float tempCorrection = 1.0 + (tempCoeff * (currentTemp - refTemp) / 10.0);
float correctedForce = rawForce * tempCorrection;
```

---

## Summary Checklist

- [ ] Load cell properly mounted (tension mode)
- [ ] Wiring verified (correct color codes)
- [ ] Known calibration weight prepared and verified
- [ ] ESP32 programmed with base firmware
- [ ] Load cell tared (zeroed)
- [ ] Calibration factor calculated and updated
- [ ] Firmware re-uploaded with new factor
- [ ] Multi-point verification completed (50g, 100g, 200g)
- [ ] Accuracy within ±2% acceptable
- [ ] Calibration log updated

---

## Quick Reference

**Calibration Formula:**
```
New Factor = Old Factor × (Known Weight / Displayed Weight)
```

**Torque Formula:**
```
Torque (mN·m) = Force (grams) × 9.81 × Radius (m) × 1000
```

**Expected Mini 4WD Torque Range:**
- Standard motor: 0.5-1.0 mN·m
- High-speed motor: 0.3-0.7 mN·m
- Torque-tuned motor: 1.0-2.0 mN·m

---

For more help, see:
- `BUILD_GUIDE.md` - Complete build instructions
- `WIRING.md` - Electrical connections
- `README.md` - Project overview
