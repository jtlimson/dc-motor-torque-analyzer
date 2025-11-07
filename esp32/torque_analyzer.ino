/*
 * Mini 4WD Torque Analyzer - ESP32 Firmware
 *
 * Reads load cell (via HX711) and current/voltage (via INA219)
 * Computes torque (mN.m) and power (Watts)
 * Transmits data via Bluetooth LE to Raspberry Pi
 *
 * Author: Generated for Mini 4WD Project
 * License: MIT
 */

#include <HX711.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// ==================== PIN DEFINITIONS ====================
#define HX711_DOUT_PIN  16    // Load cell data pin
#define HX711_SCK_PIN   4     // Load cell clock pin
// I2C pins for INA219: SDA=21, SCL=22 (default)

// ==================== CONFIGURATION ====================
#define TORQUE_ARM_RADIUS_MM  10.0   // Distance from shaft center to load cell (mm)
#define CALIBRATION_FACTOR  -7050.0  // HX711 calibration factor (adjust after calibration)
#define SAMPLE_RATE_MS  100          // Sampling interval (100ms = 10Hz)

// BLE Service UUID (generate your own for production)
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

// ==================== GLOBAL OBJECTS ====================
HX711 scale;
Adafruit_INA219 ina219;

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;

// ==================== DATA STRUCTURE ====================
struct SensorData {
  float force_grams;      // Force in grams
  float torque_mNm;       // Torque in milli-Newton-meters
  float voltage_V;        // Voltage in Volts
  float current_mA;       // Current in milliamps
  float power_W;          // Power in Watts
  uint32_t timestamp_ms;  // Timestamp
};

SensorData data;

// ==================== BLE CALLBACKS ====================
class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
    Serial.println("BLE Client Connected!");
  }

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    Serial.println("BLE Client Disconnected!");
  }
};

// ==================== SETUP ====================
void setup() {
  Serial.begin(115200);
  Serial.println("Mini 4WD Torque Analyzer - Starting...");

  // Initialize I2C
  Wire.begin();

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
    while (1) { delay(10); }
  }
  // Configure INA219 for higher current range if needed
  // ina219.setCalibration_16V_400mA();  // For low current motors
  ina219.setCalibration_32V_2A();     // For standard Mini 4WD motors
  Serial.println("INA219 initialized.");

  // Initialize BLE
  Serial.println("Initializing BLE...");
  BLEDevice::init("Mini4WD_Torque");

  // Create BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );

  // Create a BLE Descriptor
  pCharacteristic->addDescriptor(new BLE2902());

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);
  BLEDevice::startAdvertising();

  Serial.println("BLE Advertising started. Waiting for connection...");
  Serial.println("Device Name: Mini4WD_Torque");
  Serial.println("Setup complete!\n");
}

// ==================== SENSOR READING ====================
void readSensors() {
  // Read load cell (force in grams)
  if (scale.is_ready()) {
    data.force_grams = scale.get_units(3);  // Average of 3 readings
  } else {
    data.force_grams = 0.0;
    Serial.println("Warning: HX711 not ready");
  }

  // Read INA219 (voltage and current)
  data.voltage_V = ina219.getBusVoltage_V();
  data.current_mA = ina219.getCurrent_mA();

  // Calculate power (Watts)
  data.power_W = data.voltage_V * (data.current_mA / 1000.0);

  // Calculate torque (mN.m)
  // Torque = Force × Radius
  // Force in Newtons = mass(kg) × 9.81
  // Force in Newtons = (grams / 1000) × 9.81
  float force_N = (data.force_grams / 1000.0) * 9.81;
  float radius_m = TORQUE_ARM_RADIUS_MM / 1000.0;
  data.torque_mNm = force_N * radius_m * 1000.0;  // Convert to mN.m

  // Timestamp
  data.timestamp_ms = millis();
}

// ==================== BLE TRANSMISSION ====================
void transmitData() {
  if (deviceConnected) {
    // Create JSON-like string for transmission
    char buffer[200];
    snprintf(buffer, sizeof(buffer),
             "{\"f\":%.2f,\"t\":%.2f,\"v\":%.3f,\"i\":%.1f,\"p\":%.3f,\"ts\":%lu}",
             data.force_grams,
             data.torque_mNm,
             data.voltage_V,
             data.current_mA,
             data.power_W,
             data.timestamp_ms);

    pCharacteristic->setValue(buffer);
    pCharacteristic->notify();

    // Also print to serial for debugging
    Serial.println(buffer);
  }
}

// ==================== MAIN LOOP ====================
void loop() {
  // Read sensors
  readSensors();

  // Transmit data if connected
  if (deviceConnected) {
    transmitData();
  } else {
    // Print to serial even if not connected (for debugging)
    Serial.printf("Force: %.2fg | Torque: %.2f mN.m | V: %.3fV | I: %.1fmA | P: %.3fW\n",
                  data.force_grams,
                  data.torque_mNm,
                  data.voltage_V,
                  data.current_mA,
                  data.power_W);
  }

  // Handle BLE connection state changes
  if (!deviceConnected && oldDeviceConnected) {
    delay(500); // Give the bluetooth stack time to get ready
    pServer->startAdvertising(); // Restart advertising
    Serial.println("Start advertising again...");
    oldDeviceConnected = deviceConnected;
  }

  if (deviceConnected && !oldDeviceConnected) {
    oldDeviceConnected = deviceConnected;
  }

  // Wait for next sample
  delay(SAMPLE_RATE_MS);
}

// ==================== CALIBRATION FUNCTIONS ====================
// Uncomment and use these functions for calibration

/*
void calibrateLoadCell() {
  Serial.println("\n=== Load Cell Calibration ===");
  Serial.println("Remove all weight from load cell.");
  Serial.println("Send any character to tare...");
  while (!Serial.available()) {}
  Serial.read();

  scale.tare();
  Serial.println("Tare complete.");

  Serial.println("Place known weight (e.g., 100g) on load cell.");
  Serial.println("Send any character to continue...");
  while (!Serial.available()) {}
  Serial.read();

  long reading = scale.get_units(10);
  Serial.print("Raw reading: ");
  Serial.println(reading);

  Serial.println("Enter the known weight in grams:");
  while (!Serial.available()) {}
  float knownWeight = Serial.parseFloat();

  float calibrationFactor = reading / knownWeight;
  Serial.print("Calibration Factor: ");
  Serial.println(calibrationFactor);
  Serial.println("Update CALIBRATION_FACTOR in code with this value.");
}
*/
