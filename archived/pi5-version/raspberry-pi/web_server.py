#!/usr/bin/env python3
"""
Mini 4WD Torque Analyzer - Raspberry Pi Web Server
Receives BLE data from ESP32 and serves web dashboard
"""

import asyncio
import json
import logging
from datetime import datetime
from flask import Flask, render_template, jsonify
from flask_socketio import SocketIO
from bleak import BleakClient, BleakScanner
import threading

# Configuration
ESP32_DEVICE_NAME = "Mini4WD_Torque"
SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8"

# Setup logging
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

# Flask app
app = Flask(__name__)
app.config['SECRET_KEY'] = 'mini4wd_torque_secret'
socketio = SocketIO(app, cors_allowed_origins="*")

# Global variables
latest_data = {
    'force_grams': 0,
    'torque_mNm': 0,
    'voltage_V': 0,
    'current_mA': 0,
    'power_W': 0,
    'timestamp': None,
    'connected': False
}

ble_client = None
ble_task = None

# ==================== BLE FUNCTIONS ====================

async def find_device():
    """Scan for ESP32 device"""
    logger.info(f"Scanning for device: {ESP32_DEVICE_NAME}")
    devices = await BleakScanner.discover()

    for device in devices:
        if device.name and ESP32_DEVICE_NAME in device.name:
            logger.info(f"Found device: {device.name} ({device.address})")
            return device

    logger.warning(f"Device {ESP32_DEVICE_NAME} not found")
    return None

def notification_handler(sender, data):
    """Handle BLE notifications from ESP32"""
    try:
        # Decode the JSON data
        json_str = data.decode('utf-8')
        sensor_data = json.loads(json_str)

        # Update global data
        latest_data['force_grams'] = sensor_data.get('f', 0)
        latest_data['torque_mNm'] = sensor_data.get('t', 0)
        latest_data['voltage_V'] = sensor_data.get('v', 0)
        latest_data['current_mA'] = sensor_data.get('i', 0)
        latest_data['power_W'] = sensor_data.get('p', 0)
        latest_data['timestamp'] = datetime.now().strftime('%Y-%m-%d %H:%M:%S.%f')[:-3]
        latest_data['connected'] = True

        # Emit to web clients via WebSocket
        socketio.emit('sensor_data', latest_data)

        logger.info(f"Data: Force={latest_data['force_grams']:.2f}g, "
                   f"Torque={latest_data['torque_mNm']:.2f}mN.m, "
                   f"Power={latest_data['power_W']:.3f}W")

    except Exception as e:
        logger.error(f"Error parsing notification: {e}")

async def connect_and_receive():
    """Connect to ESP32 and receive data"""
    global ble_client

    while True:
        try:
            # Find device
            device = await find_device()
            if not device:
                logger.info("Retrying in 5 seconds...")
                await asyncio.sleep(5)
                continue

            # Connect to device
            async with BleakClient(device.address) as client:
                ble_client = client
                logger.info(f"Connected to {device.name}")

                # Subscribe to notifications
                await client.start_notify(CHARACTERISTIC_UUID, notification_handler)
                logger.info("Subscribed to notifications")

                # Keep connection alive
                while client.is_connected:
                    await asyncio.sleep(1)

                logger.warning("Disconnected from device")
                latest_data['connected'] = False

        except Exception as e:
            logger.error(f"BLE Error: {e}")
            latest_data['connected'] = False
            await asyncio.sleep(5)

def run_ble_loop():
    """Run BLE event loop in separate thread"""
    loop = asyncio.new_event_loop()
    asyncio.set_event_loop(loop)
    loop.run_until_complete(connect_and_receive())

# ==================== FLASK ROUTES ====================

@app.route('/')
def index():
    """Serve main dashboard page"""
    return render_template('index.html')

@app.route('/api/data')
def get_data():
    """API endpoint for current data"""
    return jsonify(latest_data)

@app.route('/api/status')
def get_status():
    """API endpoint for connection status"""
    return jsonify({
        'connected': latest_data['connected'],
        'device_name': ESP32_DEVICE_NAME
    })

# ==================== SOCKETIO EVENTS ====================

@socketio.on('connect')
def handle_connect():
    """Handle WebSocket client connection"""
    logger.info('Web client connected')
    socketio.emit('sensor_data', latest_data)

@socketio.on('disconnect')
def handle_disconnect():
    """Handle WebSocket client disconnection"""
    logger.info('Web client disconnected')

# ==================== MAIN ====================

if __name__ == '__main__':
    logger.info("Starting Mini 4WD Torque Analyzer Server")

    # Start BLE thread
    ble_thread = threading.Thread(target=run_ble_loop, daemon=True)
    ble_thread.start()
    logger.info("BLE thread started")

    # Get local IP for display
    import socket
    hostname = socket.gethostname()
    local_ip = socket.gethostbyname(hostname)

    logger.info("="*50)
    logger.info(f"Server starting on http://{local_ip}:5000")
    logger.info(f"Open this URL on your phone: http://{local_ip}:5000")
    logger.info("="*50)

    # Start Flask server
    socketio.run(app, host='0.0.0.0', port=5000, debug=False)
