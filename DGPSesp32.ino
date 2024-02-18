#include <WiFi.h>
#include <TinyGPS++.h>

const char *ssid = "ESP32-Access-Point";
const char *password = "";

WiFiServer server(22);

// Create a TinyGPS++ object
TinyGPSPlus gps;

void setup() {
  Serial.begin(115200);

  /// Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.begin();
}

void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients
  char c;
  if (client) {                             // If a new client connects,
    //Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.available()) {            // loop while the client's connected            // if there's bytes to read from the client,
      c = client.read();             // read a byte, then
      if (c != '\r') {  // if you got anything else but a carriage return character,
        currentLine += c;      // add it to the end of the currentLine
      }
    }
    Serial.println("Incoming data: " + currentLine);
     
    float latitude = getLatitude();
    float longitude = getLongitude();
    client.print("Latitude: ");
    client.println(latitude);
    //client.flush();
    client.print("Longitude: ");
    client.println(longitude);
    client.flush();

    Serial.println("Location data sent to BeagleBone");

    // Close the connection
    // client.stop();
  }
  else {
    Serial.println("error");
    client.println("error");
  }
  delay(1000);
}

  // Get GPS data (you will need to replace this with your actual code)
float getLatitude() {
  // Replace with code to get latitude from your GPS module
  if (gps.location.isValid()) {
    Serial.println(gps.location.lat(), 6);
    return gps.location.lat();
  }
    else
  {
    Serial.println(F("INVALID"));
    return 0.0;
  }
  //return gps.location.lat();
}

float getLongitude() {
  // Replace with code to get longitude from your GPS module
  if (gps.location.isValid()) {
    Serial.println(gps.location.lng(), 6);
    return gps.location.lng();
  }
    else
  {
    Serial.println(F("INVALID"));
    return 0.0;
  }
  //return gps.location.lng();
}