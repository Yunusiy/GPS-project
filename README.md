# IoT GPS Project

This project is designed to track and record GPS data using an ESP32, a GPS module, and Firebase. The GPS coordinates, speed, altitude, and other related data are obtained from the GPS module and sent to Firebase in real-time.

## Components

- ESP32
- GPS module GP-02
- Firebase Realtime Database

## Libraries Required

Ensure you have the following libraries installed in your Arduino IDE:

- `FirebaseESP32.h`
- `TinyGPSPlus.h`
- `TokenHelper.h`
- `RTDBHelper.h`

## Setup Instructions

1. **Hardware Setup:**
   - Connect the GPS module to the ESP32:
     - RXD2 (GPIO 16) -> TX of GPS module
     - TXD2 (GPIO 17) -> RX of GPS module
   - Connect the ESP32 to your computer via USB for programming and power.

2. **Software Setup:**
   - Install the required libraries mentioned above.
   - Replace the placeholders in the code with your WiFi credentials and Firebase API key and URL:
     ```cpp
     #define WIFI_SSID "your_wifi_ssid"
     #define WIFI_PASSWORD "your_wifi_password"
     #define API_KEY "your_firebase_api_key"
     #define DATABASE_URL "your_firebase_database_url"
     ```
   - Upload the provided code to your ESP32 using the Arduino IDE.

## Code Explanation

The code consists of the following main parts:

1. **Initialization:**
   - Initialize serial communication and WiFi connection.
   - Set up the Firebase configuration and authentication.

2. **GPS Data Handling:**
   - Read data from the GPS module and process it using the `TinyGPSPlus` library.
   - Extract and print the GPS data including latitude, longitude, speed, altitude, number of satellites, direction, date, and time.

3. **Firebase Integration:**
   - Format the GPS data into JSON.
   - Send the formatted data to Firebase Realtime Database.

## Key Functions

- `void displayInfo()`: Processes and displays the GPS data. It extracts the required information and sends it to Firebase.
- `void setup()`: Sets up the serial communication, WiFi, and Firebase configurations.
- `void loop()`: Continuously reads data from the GPS module and calls `displayInfo()` to process and send the data to Firebase.

## Running the Project

1. Power the ESP32 and ensure it is connected to the GPS module.
2. Monitor the serial output for information about the WiFi connection, GPS data, and Firebase updates.
3. Verify that the GPS data is being sent to Firebase by checking your Firebase Realtime Database.

## Troubleshooting

- If the GPS data is not being detected, check the wiring between the GPS module and the ESP32.
- Ensure you have a clear view of the sky to get a proper GPS signal.
- Verify that your WiFi credentials and Firebase API key are correctly set in the code.
- Check the serial monitor for any error messages and debug accordingly.

## Additional Notes

- Adjust the `myTimeZoneOffsetHours` and `myTimeZoneOffsetMinutes` variables according to your local timezone.
- Make sure your Firebase database rules allow read and write access for the data paths used.

By following these instructions, you should be able to successfully set up and run your IoT GPS project, tracking and recording GPS data in real-time with Firebase.
