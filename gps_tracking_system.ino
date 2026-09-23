#include <TinyGPS++.h>

// =====================================================
// GPS
// =====================================================

TinyGPSPlus gps;

HardwareSerial GPS_Serial(1);

#define GPS_RX 16
#define GPS_TX 17


// =====================================================
// SIM800L
// =====================================================

HardwareSerial SIM800L(2);

#define SIM_RX 26
#define SIM_TX 27

// Phone number to receive the SMS
const char* phoneNumber = "+91XXXXXXXXXX";


// =====================================================
// LEDs
// =====================================================

#define GPS_LED 19
#define GSM_LED 18


// =====================================================
// TIMING
// =====================================================

unsigned long lastSMS = 0;

// Send location once every 60 seconds
const unsigned long SMS_INTERVAL = 60000;


// =====================================================
// SETUP
// =====================================================

void setup()
{
  Serial.begin(115200);

  // GPS UART
  GPS_Serial.begin(
    9600,
    SERIAL_8N1,
    GPS_RX,
    GPS_TX
  );

  // SIM800L UART
  SIM800L.begin(
    9600,
    SERIAL_8N1,
    SIM_RX,
    SIM_TX
  );

  pinMode(GPS_LED, OUTPUT);
  pinMode(GSM_LED, OUTPUT);

  Serial.println();
  Serial.println("================================");
  Serial.println("GPS TRACKING SYSTEM");
  Serial.println("ESP32 + NEO-6M + SIM800L");
  Serial.println("================================");

  delay(3000);

  // Initialize SIM800L
  initializeSIM800L();
}


// =====================================================
// MAIN LOOP
// =====================================================

void loop()
{
  // Read GPS data continuously
  while (GPS_Serial.available())
  {
    gps.encode(GPS_Serial.read());
  }


  // Check for updated GPS location
  if (gps.location.isUpdated())
  {
    double latitude = gps.location.lat();
    double longitude = gps.location.lng();

    Serial.println();
    Serial.println("-----------------------------");

    Serial.print("Latitude : ");
    Serial.println(latitude, 6);

    Serial.print("Longitude: ");
    Serial.println(longitude, 6);

    Serial.print("Satellites: ");

    if (gps.satellites.isValid())
      Serial.println(gps.satellites.value());
    else
      Serial.println("Unknown");

    Serial.println("-----------------------------");


    // GPS LED
    digitalWrite(GPS_LED, HIGH);
    delay(100);
    digitalWrite(GPS_LED, LOW);


    // Send SMS only after interval
    if (
      millis() - lastSMS >= SMS_INTERVAL ||
      lastSMS == 0
    )
    {
      sendLocationSMS(
        latitude,
        longitude
      );

      lastSMS = millis();
    }
  }
}


// =====================================================
// INITIALIZE SIM800L
// =====================================================

void initializeSIM800L()
{
  Serial.println("Initializing SIM800L...");

  delay(1000);

  sendATCommand("AT", 1000);
  sendATCommand("ATE0", 1000);
  sendATCommand("AT+CPIN?", 2000);
  sendATCommand("AT+CSQ", 2000);
  sendATCommand("AT+CREG?", 2000);

  // SMS text mode
  sendATCommand(
    "AT+CMGF=1",
    1000
  );

  Serial.println("SIM800L initialization complete.");
}


// =====================================================
// SEND AT COMMAND
// =====================================================

void sendATCommand(
  const char* command,
  unsigned long waitTime
)
{
  Serial.print(">> ");
  Serial.println(command);

  SIM800L.println(command);

  unsigned long startTime = millis();

  while (millis() - startTime < waitTime)
  {
    while (SIM800L.available())
    {
      char c = SIM800L.read();
      Serial.write(c);
    }
  }

  Serial.println();
}


// =====================================================
// SEND GPS LOCATION SMS
// =====================================================

void sendLocationSMS(
  double latitude,
  double longitude
)
{
  Serial.println();
  Serial.println("Sending GPS location SMS...");

  String message = "";

  message += "GPS TRACKING SYSTEM\n";

  message += "Latitude: ";
  message += String(latitude, 6);

  message += "\nLongitude: ";
  message += String(longitude, 6);

  message += "\n\nGoogle Maps:\n";

  message += "https://maps.google.com/?q=";

  message += String(latitude, 6);

  message += ",";

  message += String(longitude, 6);


  // Tell SIM800L SMS number
  SIM800L.print("AT+CMGS=\"");
  SIM800L.print(phoneNumber);
  SIM800L.println("\"");

  delay(1000);


  // Send message
  SIM800L.print(message);

  delay(500);


  // CTRL+Z terminates SMS
  SIM800L.write(26);

  delay(5000);


  Serial.println("Location SMS sent.");

  // GSM LED
  digitalWrite(GSM_LED, HIGH);
  delay(500);
  digitalWrite(GSM_LED, LOW);
}
