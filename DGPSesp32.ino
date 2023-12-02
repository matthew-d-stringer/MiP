#include <WiFi.h>
#include <TinyGPS++.h>

const char *ssid = "ESP32-Access-Point";
const char *password = "your_password";

WiFiServer server(22);

String header;

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

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
        float latitude = getLatitude();
        float longitude = getLongitude();
      client.print("Latitude: ");
      client.println(latitude);
      client.print("Longitude: ");
      client.println(longitude);

      Serial.println("Location data sent to BeagleBone");

      // Close the connection
      //client.stop();
    //} else {
      Serial.println("Unable to connect to BeagleBone");
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
  else Serial.println("error");
  delay(5000);
}
  // Get GPS data (you will need to replace this with your actual code)
//   float latitude = getLatitude();
//   float longitude = getLongitude();

//   // Check if GPS data is valid
//   if (gps.location.isValid()) {
//     // Connect to the BeagleBone over Wi-Fi
//     WiFiClient client;
//     const char *beagleBoneIP = "192.168.4.2";  // Replace with the BeagleBone's IP address
//     const int beagleBonePort = 22;  // Replace with the BeagleBone's port

//     if (client.connect(beagleBoneIP, beagleBonePort)) {
//       // Send location data to the BeagleBone
//       client.print("Latitude: ");
//       client.println(latitude);
//       client.print("Longitude: ");
//       client.println(longitude);

//       Serial.println("Location data sent to BeagleBone");

//       // Close the connection
//       //client.stop();
//     //} else {
//       Serial.println("Unable to connect to BeagleBone");
//     //}
//   }

//   delay(5000);  // Delay for 5 seconds before sending the next location data
// }

// Replace the following functions with your actual GPS data retrieval code
float getLatitude() {
  // Replace with code to get latitude from your GPS module
  if (gps.location.isValid())
    Serial.println(gps.location.lat(), 6);
    else
  {
    Serial.println(F("INVALID"));
  }
  return 0.0;
}

float getLongitude() {
  // Replace with code to get longitude from your GPS module
  if (gps.location.isValid())
    Serial.println(gps.location.lng(), 6);
    else
  {
    Serial.println(F("INVALID"));
  }
  return 0.0;
}