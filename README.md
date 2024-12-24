# Shift Notification System

This project is a WiFi-enabled shift notification system using an ESP32. It allows users to send shift availability notifications to specific Telegram chat IDs via buttons for different shifts.
An LCD screen displays shift details, location, and bonus status before sending the notification.

I thought this would speed up staffing to avoid understaffing, overworking, and to keep in compliance with regulations.

## Features

- **WiFi Connectivity**: Connects to your WiFi network to send notifications.
- **Telegram Integration**: Sends shift details to specified Telegram chat IDs.
- **Shift Buttons**: Dedicated buttons for day, evening, and night shifts.
- **LCD Display**: Displays shift details, location, and bonus status.
- **Debounce and Long Press**: Ensures button presses are accurately registered and supports long press for confirmation.
- **I2C Communication**: Interfaces with an LCD display using I2C protocol.

## Components Used

- ESP32 microcontroller
- LiquidCrystal I2C LCD (16x2)
- Buttons for day, evening, and night shifts
- Floor and bonus switches
- WiFi connection

## Prerequisites

### Hardware

- ESP32 development board
- LCD with I2C interface (e.g., 0x27 or 0x3F address)
- Buttons (momentary switches)
- Resistors (for pull-up configuration, if needed)
- Connecting wires and breadboard (optional)

### Software

- Arduino IDE
- Required libraries:
  - WiFi.h
  - HTTPClient.h
  - LiquidCrystal_I2C.h
  - UrlEncode.h

## Installation

### Arduino IDE Setup

1. Install the necessary libraries:
   - In Arduino IDE, go to **Tools > Manage Libraries...**
   - Search for and install the following:
     - "LiquidCrystal I2C"
     - "UrlEncode" library

2. Select the correct board and port:
   - Go to **Tools > Board > ESP32 Arduino** and choose your ESP32 board.
   - Go to **Tools > Port** and select the correct port.

3. Copy the provided code into the Arduino IDE.

### Configure WiFi and Telegram Details

1. Replace the following placeholders in the code:
   - `YOUR_WIFI_HERE`: Your WiFi SSID.
   - `WIFI_PW`: Your WiFi password.
   - `TELEGRAM_BOT_TOKEN`: Your Telegram bot token.
   - `CHAT_ID_1`: Your Telegram chat ID (add more if needed).

## Wiring Diagram

| ESP32 Pin | Component           |
|-----------|---------------------|
| SDA (13)  | I2C SDA of LCD      |
| SCL (14)  | I2C SCL of LCD      |
| 4         | Day button          |
| 5         | Evening button      |
| 18        | Night button        |
| 26        | Floor switch input  |
| 27        | Bonus switch input  |

Ensure pull-up resistors are used for button inputs, if necessary.

## Usage

1. Upload the code to your ESP32 using the Arduino IDE.
2. Power on the ESP32 and ensure it connects to WiFi (check serial monitor for status).
3. Press a button to:
   - Display shift details on the LCD.
   - The LCD will turn off after 5 seconds of inactivity to save power.
4. Hold a button to:
   - Send a notification to Telegram when the button is held for more than 1 second.

## Functions

### `setup()`

- Initializes WiFi, I2C communication, and LCD.
- Configures buttons and switches as inputs.

### `loop()`

- Monitors button states and displays shift details.
- Handles long presses to send Telegram notifications.

### `showDetails()`

- Displays shift details (time, location, and bonus status) on the LCD.

### `handleLongPress()`

- Sends Telegram notifications if a button is held for longer than 1 second.

### `sendShiftAlert()`

- Sends a formatted message to Telegram with shift details.

## Notes

- Ensure the Telegram bot token and chat IDs are correctly configured.
- Adjust debounce and timeout durations as needed.
- Test the I2C address of your LCD (0x27 or 0x3F) to ensure compatibility.

## Troubleshooting

- **WiFi Not Connecting**: Verify SSID and password.
- **Telegram Notifications Not Sent**: Check the bot token, chat ID, and internet connectivity.
- **LCD Not Working**: Test with a different I2C address.

## Future Improvements

- Add more buttons for additional shifts.
- Integrate more features like date and time.
- Use secure HTTPS connections for API requests.
- Optimize power consumption for battery-powered setups.

## License

This project is open-source. Feel free to modify and distribute.
