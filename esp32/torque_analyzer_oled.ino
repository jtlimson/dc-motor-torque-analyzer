/*
 * DC Motor Stall Torque Analyzer - OLED Display Version
 *
 * Reads load cell (via HX711) and current/voltage (via INA219)
 * Computes stall torque (mN.m and kg.cm) and electrical power (V×I)
 * Displays on SSD1306 OLED screen
 * Controls motor with fixed voltage ON/OFF + current limiting
 *
 * Display shows:
 * - Real-time: Current (A), Stall Torque (mN.m, kg.cm), Power (W)
 * - Maximum stall values recorded
 * - Test control: Fixed voltage ON/OFF, Current limit
 *
 * Note: PWM removed - uses simple ON/OFF with voltage regulation
 * Power calculated as P = V × I (electrical power, sufficient for stall analysis)
 *
 * Author: Generated for DC Motor Testing
 * License: MIT
 */

#include <HX711.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ==================== PIN DEFINITIONS ====================
#define HX711_DOUT_PIN  16    // Load cell data pin
#define HX711_SCK_PIN   4     // Load cell clock pin
// I2C pins for INA219 and OLED: SDA=21, SCL=22 (default)

// Button pins
#define BTN_UP_PIN      32    // Up/Increase button
#define BTN_DOWN_PIN    33    // Down/Decrease button
#define BTN_MODE_PIN    25    // Mode/Select button
#define BTN_RESET_PIN   26    // Reset max values button

// Motor ON/OFF control (no PWM needed)
#define MOTOR_ENABLE_PIN  27    // Digital output to enable motor
// Note: Voltage controlled by external regulator
// Current limited by XL4015 or similar current limiter

// ==================== OLED CONFIGURATION ====================
#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64
#define OLED_RESET      -1    // Reset pin (or -1 if sharing Arduino reset)
#define SCREEN_ADDRESS  0x3C  // OLED I2C address (0x3C or 0x3D)

// ==================== MEASUREMENT CONFIGURATION ====================
#define TORQUE_ARM_RADIUS_MM  60.0   // Lever arm length (measure precisely!)
#define CALIBRATION_FACTOR  7050.0   // HX711 calibration (positive for compression)
#define SAMPLE_RATE_MS  100          // Sampling interval (100ms = 10Hz)

// Motor protection
#define MAX_CURRENT_LIMIT_MA  2500.0 // Maximum safe current (2.5A)

// ==================== GLOBAL OBJECTS ====================
HX711 scale;
Adafruit_INA219 ina219;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ==================== DATA STRUCTURES ====================
struct SensorData {
  float force_grams;       // Force in grams
  float torque_mNm;        // Torque in milli-Newton-meters
  float torque_kgcm;       // Torque in kilogram-centimeters
  float voltage_V;         // Voltage in Volts
  float current_mA;        // Current in milliamps
  float power_W;           // Power in Watts
  uint32_t timestamp_ms;   // Timestamp
};

struct MaxValues {
  float max_current_mA;
  float max_torque_mNm;
  float max_torque_kgcm;
  float max_power_W;
};

SensorData data;
MaxValues maxValues;

// ==================== UI STATE ====================
enum DisplayMode {
  MODE_REALTIME,     // Show real-time values
  MODE_MAX_VALUES,   // Show maximum values
  MODE_MOTOR_CONTROL // Motor voltage/current control
};

DisplayMode currentMode = MODE_REALTIME;

// Motor control
int motorPWM = 255;           // 0-255 (0-100%)
float currentLimitMA = 2500;  // Current limit in mA
bool motorEnabled = false;

// Button state
bool btnUpPressed = false;
bool btnDownPressed = false;
bool btnModePressed = false;
bool btnResetPressed = false;

// ==================== SETUP ====================
void setup() {
  Serial.begin(115200);
  Serial.println("DC Motor Stall Torque Analyzer - OLED Version");
  Serial.println("========================================");

  // Initialize I2C
  Wire.begin();

  // Initialize OLED Display
  Serial.println("Initializing OLED...");
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("ERROR: SSD1306 OLED not found!"));
    while(1) { delay(10); }
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("DC Motor Torque"));
  display.println(F("Analyzer v2.0"));
  display.println(F(""));
  display.println(F("Initializing..."));
  display.display();
  delay(2000);

  // Initialize HX711 Load Cell
  Serial.println("Initializing HX711...");
  scale.begin(HX711_DOUT_PIN, HX711_SCK_PIN);
  scale.set_scale(CALIBRATION_FACTOR);
  scale.tare();  // Reset scale to 0
  Serial.println("HX711 initialized and tared.");

  // Initialize INA219
  Serial.println("Initializing INA219...");
  if (!ina219.begin()) {
    Serial.println("ERROR: Failed to find INA219 chip!");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(F("ERROR: INA219"));
    display.println(F("not found!"));
    display.display();
    while (1) { delay(10); }
  }
  ina219.setCalibration_32V_2A();  // For small DC motors
  Serial.println("INA219 initialized.");

  // Initialize buttons
  pinMode(BTN_UP_PIN, INPUT_PULLUP);
  pinMode(BTN_DOWN_PIN, INPUT_PULLUP);
  pinMode(BTN_MODE_PIN, INPUT_PULLUP);
  pinMode(BTN_RESET_PIN, INPUT_PULLUP);
  Serial.println("Buttons initialized.");

  // Setup PWM for motor control
  ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(MOTOR_PWM_PIN, PWM_CHANNEL);
  ledcWrite(PWM_CHANNEL, 0);  // Start with motor off
  Serial.println("PWM initialized.");

  // Initialize max values
  resetMaxValues();

  Serial.println("Setup complete!\n");

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(F("Ready!"));
  display.display();
  delay(1000);
}

// ==================== SENSOR READING ====================
void readSensors() {
  // Read load cell (force in grams)
  if (scale.is_ready()) {
    data.force_grams = scale.get_units(3);  // Average of 3 readings
  } else {
    data.force_grams = 0.0;
  }

  // Read INA219 (voltage and current)
  data.voltage_V = ina219.getBusVoltage_V();
  data.current_mA = ina219.getCurrent_mA();

  // Calculate power (Watts)
  data.power_W = data.voltage_V * (data.current_mA / 1000.0);

  // Calculate torque (mN.m)
  // Torque = Force × Radius
  float force_N = (data.force_grams / 1000.0) * 9.81;
  float radius_m = TORQUE_ARM_RADIUS_MM / 1000.0;
  data.torque_mNm = force_N * radius_m * 1000.0;  // Convert to mN.m

  // Calculate torque (kg.cm)
  // 1 kg.cm = 98.0665 mN.m
  data.torque_kgcm = data.torque_mNm / 98.0665;

  // Timestamp
  data.timestamp_ms = millis();

  // Update max values
  if (data.current_mA > maxValues.max_current_mA) {
    maxValues.max_current_mA = data.current_mA;
  }
  if (data.torque_mNm > maxValues.max_torque_mNm) {
    maxValues.max_torque_mNm = data.torque_mNm;
  }
  if (data.torque_kgcm > maxValues.max_torque_kgcm) {
    maxValues.max_torque_kgcm = data.torque_kgcm;
  }
  if (data.power_W > maxValues.max_power_W) {
    maxValues.max_power_W = data.power_W;
  }
}

// ==================== BUTTON HANDLING ====================
void readButtons() {
  // Read button states (active LOW, using INPUT_PULLUP)
  bool upState = !digitalRead(BTN_UP_PIN);
  bool downState = !digitalRead(BTN_DOWN_PIN);
  bool modeState = !digitalRead(BTN_MODE_PIN);
  bool resetState = !digitalRead(BTN_RESET_PIN);

  // Detect button press (rising edge)
  if (upState && !btnUpPressed) {
    handleUpButton();
  }
  if (downState && !btnDownPressed) {
    handleDownButton();
  }
  if (modeState && !btnModePressed) {
    handleModeButton();
  }
  if (resetState && !btnResetPressed) {
    handleResetButton();
  }

  // Update button states
  btnUpPressed = upState;
  btnDownPressed = downState;
  btnModePressed = modeState;
  btnResetPressed = resetState;
}

void handleUpButton() {
  if (currentMode == MODE_MOTOR_CONTROL) {
    // Increase PWM (motor voltage)
    motorPWM = min(255, motorPWM + 10);
    Serial.printf("Motor PWM: %d (%.1f%%)\n", motorPWM, (motorPWM / 255.0) * 100);
  }
}

void handleDownButton() {
  if (currentMode == MODE_MOTOR_CONTROL) {
    // Decrease PWM (motor voltage)
    motorPWM = max(0, motorPWM - 10);
    Serial.printf("Motor PWM: %d (%.1f%%)\n", motorPWM, (motorPWM / 255.0) * 100);
  }
}

void handleModeButton() {
  // Cycle through display modes
  currentMode = (DisplayMode)((currentMode + 1) % 3);
  Serial.print("Mode: ");
  switch(currentMode) {
    case MODE_REALTIME: Serial.println("Real-time"); break;
    case MODE_MAX_VALUES: Serial.println("Max Values"); break;
    case MODE_MOTOR_CONTROL: Serial.println("Motor Control"); break;
  }
}

void handleResetButton() {
  // Reset max values
  resetMaxValues();
  Serial.println("Max values reset!");
}

void resetMaxValues() {
  maxValues.max_current_mA = 0;
  maxValues.max_torque_mNm = 0;
  maxValues.max_torque_kgcm = 0;
  maxValues.max_power_W = 0;
}

// ==================== MOTOR CONTROL ====================
void updateMotorControl() {
  // Apply current limiting
  if (data.current_mA > currentLimitMA) {
    // Current too high - reduce PWM
    motorPWM = max(0, motorPWM - 5);
    Serial.println("Current limit active - reducing PWM");
  }

  // Apply PWM to motor
  if (currentMode == MODE_MOTOR_CONTROL && motorEnabled) {
    ledcWrite(PWM_CHANNEL, motorPWM);
  } else {
    ledcWrite(PWM_CHANNEL, 0);  // Motor off when not in control mode
  }
}

// ==================== DISPLAY ====================
void updateDisplay() {
  display.clearDisplay();
  display.setCursor(0, 0);

  switch(currentMode) {
    case MODE_REALTIME:
      displayRealtime();
      break;
    case MODE_MAX_VALUES:
      displayMaxValues();
      break;
    case MODE_MOTOR_CONTROL:
      displayMotorControl();
      break;
  }

  display.display();
}

void displayRealtime() {
  // Header
  display.setTextSize(1);
  display.println(F("=== REALTIME ==="));

  // Current
  display.print(F("I: "));
  display.print(data.current_mA / 1000.0, 2);
  display.println(F(" A"));

  // Torque (mN.m)
  display.print(F("T: "));
  display.print(data.torque_mNm, 1);
  display.println(F(" mN.m"));

  // Torque (kg.cm)
  display.print(F("T: "));
  display.print(data.torque_kgcm, 2);
  display.println(F(" kg.cm"));

  // Power
  display.print(F("P: "));
  display.print(data.power_W, 2);
  display.println(F(" W"));

  // Voltage
  display.print(F("V: "));
  display.print(data.voltage_V, 2);
  display.println(F(" V"));
}

void displayMaxValues() {
  // Header
  display.setTextSize(1);
  display.println(F("=== MAX VALUES ==="));

  // Max Current
  display.print(F("I: "));
  display.print(maxValues.max_current_mA / 1000.0, 2);
  display.println(F(" A"));

  // Max Torque (mN.m)
  display.print(F("T: "));
  display.print(maxValues.max_torque_mNm, 1);
  display.println(F(" mN.m"));

  // Max Torque (kg.cm)
  display.print(F("T: "));
  display.print(maxValues.max_torque_kgcm, 2);
  display.println(F(" kg.cm"));

  // Max Power
  display.print(F("P: "));
  display.print(maxValues.max_power_W, 2);
  display.println(F(" W"));

  // Reset instruction
  display.println(F(""));
  display.println(F("RESET to clear"));
}

void displayMotorControl() {
  // Header
  display.setTextSize(1);
  display.println(F("=== MOTOR CTRL ==="));

  // Motor PWM (voltage %)
  display.print(F("PWM: "));
  display.print((motorPWM / 255.0) * 100, 0);
  display.println(F(" %"));

  // Current limit
  display.print(F("Limit: "));
  display.print(currentLimitMA / 1000.0, 1);
  display.println(F(" A"));

  // Current reading
  display.print(F("Current: "));
  display.print(data.current_mA / 1000.0, 2);
  display.println(F(" A"));

  // Torque
  display.print(F("Torque: "));
  display.print(data.torque_kgcm, 2);
  display.println(F(" kg.cm"));

  // Controls
  display.println(F(""));
  display.println(F("UP/DN: Adjust PWM"));
}

// ==================== MAIN LOOP ====================
void loop() {
  // Read sensors
  readSensors();

  // Read buttons
  readButtons();

  // Update motor control
  updateMotorControl();

  // Update display
  updateDisplay();

  // Print to serial for debugging
  Serial.printf("I:%.2fA T:%.1fmN.m T:%.2fkg.cm P:%.2fW V:%.2fV PWM:%d%%\n",
                data.current_mA / 1000.0,
                data.torque_mNm,
                data.torque_kgcm,
                data.power_W,
                data.voltage_V,
                (int)((motorPWM / 255.0) * 100));

  // Wait for next sample
  delay(SAMPLE_RATE_MS);
}
