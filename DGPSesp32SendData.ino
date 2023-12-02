#include <WiFi.h>
#include <TinyGPS++.h>

const char *ssid = "ESP32-Access-Point";
const char *password = "your_password";

// Create a TinyGPS++ object
TinyGPSPlus gps;

void setup() {
  Serial.begin(115200);

  // Set up the ESP32 as an access point
  WiFi.softAP(ssid, password);

  Serial.println("Access Point started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  // Get GPS data (you will need to replace this with your actual code)
  float latitude = getLatitude();
  float longitude = getLongitude();

  // Check if GPS data is valid
  if (gps.location.isValid()) {
    // Connect to the BeagleBone over Wi-Fi
    WiFiClient client;
    const char *beagleBoneIP = "192.168.4.2";  // Replace with the BeagleBone's IP address
    const int beagleBonePort = 1234;  // Replace with the BeagleBone's port

    if (client.connect(beagleBoneIP, beagleBonePort)) {
      // Send location data to the BeagleBone
      client.print("Latitude: ");
      client.println(latitude);
      client.print("Longitude: ");
      client.println(longitude);

      Serial.println("Location data sent to BeagleBone");

      // Close the connection
      client.stop();
    } else {
      Serial.println("Unable to connect to BeagleBone");
    }
  }

  delay(5000);  // Delay for 5 seconds before sending the next location data
}

// Replace the following functions with your actual GPS data retrieval code
float getLatitude() {
  // Replace with code to get latitude from your GPS module
  if (gps.location.isValid())
    Serial.print(gps.location.lat(), 6);
    else
  {
    Serial.print(F("INVALID"));
  }
  return 0.0;
}

float getLongitude() {
  // Replace with code to get longitude from your GPS module
  if (gps.location.isValid())
    Serial.print(gps.location.lng(), 6);
    else
  {
    Serial.print(F("INVALID"));
  }
  return 0.0;
}
