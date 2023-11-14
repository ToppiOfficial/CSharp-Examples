#include <WiFi.h>
#include <NTPClient.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

/* Firebase Database */
#define FireBaseAPIKey "your_firebase_api_key"
#define FireBaseDatabaseURL "your_firebase_database_url"

/* Insert wifi name and password  */
#define ssid "your_wifi_ssid"
#define password "your_wifi_password"

/* Library setup */
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
WiFiUDP ntpUDP;

// Use time.google instead, it is faster than the latter.
// If time google doesn't work use the latter: ntpServer = "pool.ntp.org";
const char* ntpServer = "time.google.com";
const char* DataSheetURL = "your_data_sheet_url";

bool signupOK = false;

// Set time client. 28800 == GMT+8 Timezone
NTPClient timeClient(ntpUDP, ntpServer, 28800);

void setup() {
  Serial.begin(115200);
  ConnectToWIFI();
  ConnectToFirebase();
}

void loop() {
  // Your main program loop
  // The if statement is meant to execute code if there is a
  // successful connection to the firebase
  if (Firebase.ready() && signupOK) {
    // Add your code logic here

    /* example of sending data to firebase

    if (Firebase.RTDB.pushString(&fbdo, "DHT/Temperature", Temp)) {
      Serial.print("PASSED! ");
      Serial.print("PATH: " + fbdo.dataPath());
      Serial.print("  TYPE: " + fbdo.dataType());
    }
    else {
      Serial.print("FAILED - ");
      Serial.println("REASON: " + fbdo.errorReason());
    }

  */
    
  }
}

void ConnectToWIFI() {
  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void ConnectToFirebase() {
  // Initialize Firebase
  config.api_key = FireBaseAPIKey;
  config.database_url = FireBaseDatabaseURL;

  // Sign up on the firebase database.
  // The two empy parameters are meant to access as anonymous user
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Initialize and update time
  timeClient.begin();
  timeClient.update();
}

// Get the current time and date. return as String
// NOTE: THIS IS A TERRIBLE WAY OF DOING THIS. TOO BAD!
String GetCurrentDateTime() {
  timeClient.update();

  // Get the current date and time
  time_t now = timeClient.getEpochTime();
  struct tm *timeinfo = localtime(&now);

  // Cheat sheet comment:
  // Stores each values of the timeclient into separate integers
  // which will be formatted accordingly later.
  /* 
    TODO: Find out why the starting year is 1900 when it supposed
    to be 1970...
  */
  int year = timeinfo->tm_year + 1900; // Years since 1900
  int month = timeinfo->tm_mon + 1;    // Month (0-11)
  int day = timeinfo->tm_mday;         // Day of the month (1-31)
  int hour = timeinfo->tm_hour;        // Hour (0-23)
  int minute = timeinfo->tm_min;       // Minute (0-59)
  int second = timeinfo->tm_sec;       // Second (0-59)

  // Format: "YY_MM_DD-HH_MM_SS"
  String formattedDateTime = String(year) + "/" +
                             (month < 10 ? "0" : "") + String(month) + "/" +
                             (day < 10 ? "0" : "") + String(day) + ":" +
                             (hour < 10 ? "0" : "") + String(hour) + "-" +
                             (minute < 10 ? "0" : "") + String(minute) + "-" +
                             (second < 10 ? "0" : "") + String(second);

 return formattedDateTime;
}
