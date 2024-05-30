#include <FirebaseESP32.h>
#include <TinyGPSPlus.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

#define WIFI_SSID "___"
#define WIFI_PASSWORD "___"
#define API_KEY "______"
#define DATABASE_URL "____"


#define RXD2 16
#define TXD2 17 

FirebaseData fbdo;
FirebaseJson json;
FirebaseAuth auth;
FirebaseConfig config;
TinyGPSPlus gps;
 
const int myTimeZoneOffsetHours = 5;
const int myTimeZoneOffsetMinutes = 0;
float longitude, latitude, speed, altitude, degree;
int numSat, hdop;
String day, month, year;
String utcHour, utcMinute, utcSecond, combinedUTC;
String updatedHour, updatedMinute, updatedSecond, direction, combinedDate, combinedTime;

void displayInfo(); 
void setup() {
  Serial.begin(115200);
  Serial2.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  Serial.println("Please Wait");
  Serial.print("Connecting to WI-FI.");
  int count = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    count += 3;
    delay(300);
  }
  pinMode(LED_BUILTIN, OUTPUT);
  delay(5000);
  
  Serial.print("\nConnected with: ");
  Serial.println(WIFI_SSID);
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  digitalWrite(LED_BUILTIN, HIGH);
  delay(5000);
  Serial.println("\nPlease Wait ...");
  Serial.println(F("Waiting for the GPS to connect."));
  
  delay(5000);
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  Firebase.signUp(&config, &auth, "", "");
  Firebase.begin(&config, &auth);
}
 
void loop() {
  while (Serial2.available() > 0)
    if (gps.encode(Serial2.read()))
      displayInfo();
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println(F("No GPS detected: check wiring."));
    while (true)
      ;
  }
}
 
void displayInfo() {
  if (gps.location.isValid()) {
    Serial.println();
    Serial.print(F("Coordinates - "));
    Serial.print("Lng: ");
    longitude = gps.location.lng();
    Serial.print(longitude, 15);
    Serial.print(F(", "));
    Serial.print("Lat: ");
    latitude = gps.location.lat();
    Serial.println(latitude, 15);
    Serial.print("Number of Satellites Detected - ");
    numSat = gps.satellites.value();
    Serial.println(numSat);
    Serial.print("Speed - ");
    speed = gps.speed.kmph();
    Serial.print(speed, 2);
    Serial.println(" km/h");
    Serial.print("Traveller is heading towards - ");
    direction = TinyGPSPlus::cardinal(gps.course.deg());
    Serial.println(direction);
    Serial.print("Altitude - ");
    altitude = gps.altitude.meters();
    Serial.print(altitude);
    Serial.println(" meters");
    Serial.print("Course in Degree - ");
    degree = gps.course.deg();
    Serial.println(degree);
    Serial.print("Horizontal Dim. of Precision - ");
    hdop = gps.hdop.value();
    Serial.println(hdop);
    Serial.print("Date - ");
    day = gps.date.day();
    month = gps.date.month();
    year = gps.date.year();
    if (day.toInt() < 10) {
      Serial.print("0");
      day = "0" + day;
    }
    Serial.print(day);
    Serial.print("/");
    if (month.toInt() < 10) {
      Serial.print("0");
      month = "0" + month;
    }
    Serial.print(month);
    Serial.print("/");
    Serial.println(year);
    combinedDate = day + "/" + month + "/" + year;
 
    utcHour = gps.time.hour();
    utcMinute = gps.time.minute();
    utcSecond = gps.time.second();
    Serial.print("UTC Time - ");
    if (utcHour.toInt() < 10) {
      utcHour = "0" + utcHour;
    }
    Serial.print(utcHour);
    Serial.print(":");
    if (utcMinute.toInt() < 10) {
      utcMinute = "0" + utcMinute;
    }
    Serial.print(utcMinute);
    Serial.print(":");


    if (utcSecond.toInt() < 10) {
      utcSecond = "0" + utcSecond;
    }
    Serial.println(utcSecond);
    combinedUTC = utcHour + ":" + utcMinute + ":" + utcSecond;
 
    int totalMinutes = utcHour.toInt() * 60 + utcMinute.toInt();
    totalMinutes += myTimeZoneOffsetHours * 60 + myTimeZoneOffsetMinutes;
 
    if (totalMinutes < 0) {
      totalMinutes += 24 * 60;
    }
 
    updatedHour = (totalMinutes / 60) % 24;
    updatedMinute = totalMinutes % 60;
    updatedSecond = utcSecond;
 
    Serial.print("Local Time - ");
    if (updatedHour.toInt() < 10) {
      updatedHour = "0" + updatedHour;
    }
    Serial.print(updatedHour);
    Serial.print(":");
    if (updatedMinute.toInt() < 10) {
      updatedMinute = "0" + updatedMinute;
    }
    Serial.print(updatedMinute);
    Serial.print(":");
    Serial.println(updatedSecond);
    combinedTime = updatedHour + ":" + updatedMinute + ":" + updatedSecond;
    if (Firebase.ready()) {
      json.setFloatDigits(6);
      json.set("Latitude", latitude);
      json.set("Longitude", longitude);
      json.setFloatDigits(2);
      json.set("Speed", speed);
      json.set("Altitude", altitude);
      json.set("Satellites", numSat);
      json.set("Direction", direction);
      json.set("Degree", degree);
      json.set("HDoP", hdop);
      json.set("Date", combinedDate);
      json.set("UTC_Time", combinedUTC);
      json.set("Local_Time", combinedTime);
      if (Firebase.updateNode(fbdo, "/GPS-Data", json)) {
        Serial.println("All the above Data has been sent successfully to Firebase.");
      } else {
        Serial.println("Failed to send data.");
        Serial.println(fbdo.errorReason());
      }
    }
  } else {
    Serial.println("\nPlease Wait ...");
    Serial.println(F("Waiting for the GPS to connect ..."));
  }
  delay(1000);
}
