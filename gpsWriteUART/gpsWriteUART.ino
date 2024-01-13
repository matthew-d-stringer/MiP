#include <TinyGPSPlus.h>

static const uint32_t GPSBaud = 9600;

// The TinyGPSPlus object
TinyGPSPlus gps;

void setup()
{
  Serial.begin(115200);
  Serial1.begin(GPSBaud);
}

void loop(){  
  while (Serial1.available() > 0)
    if (gps.encode(Serial1.read()))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}

void displayInfo(){
  char buf[10];
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    sprintf(buf, "%f", gps.location.lat());
    Serial1.write(buf);
    Serial1.write(",");
    sprintf(buf, "%f", gps.location.lng());
    Serial1.write(buf);
    Serial1.flush();
  }
  else
  {
    Serial.println(F("INVALID"));
  }
}