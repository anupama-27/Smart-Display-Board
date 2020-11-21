#include "FirebaseESP8266.h"
#include<LiquidCrystal.h>
#include<ESP8266WiFi.h>

const int rs = 0, en = 2, d4 = 14, d5 = 12, d6 = 13, d7 = 15;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define firebase_host "firebase_host_key"
#define auth_key "firebase_auth_key"

#define wifi_ssid "wifi_name"      
#define wifi_pass "wifi_password"

FirebaseData fbdata;

void setup() 
{
  Serial.begin(115200);
  WiFi.begin(wifi_ssid, wifi_pass);
  Serial.print("Connecting to ");
  Serial.print(wifi_ssid);
  
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }

  Serial.println();
  Serial.print("Connected to ");
  Serial.println(wifi_ssid);
  Serial.print("IP Address is: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(firebase_host, auth_key);
  Firebase.reconnectWiFi(true);
  
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Smart Display");
}

void loop() 
{
  if (Firebase.getString(fbdata, "DisplayBoard/Message"))
  {
   String message = fbdata.stringData();           //reads the firebase data
   Serial.println(message);
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print(message);
   
   for (int positionCounter = 0; positionCounter < 13; positionCounter++) 
   {
    lcd.scrollDisplayLeft();        // scroll one position left:
    delay(350);                      // wait a bit:
   }
   // scroll 29 positions (string length + display length) to the right
   // to move it offscreen right:
  
   for (int positionCounter = 0; positionCounter < 29; positionCounter++) 
   {
    lcd.scrollDisplayRight();           // scroll one position right:
    delay(100);                      // wait a bit:
    }
    // scroll 16 positions (display length + string length) to the left
    // to move it back to center:
  
    for (int positionCounter = 0; positionCounter < 16; positionCounter++) 
    {
      lcd.scrollDisplayLeft();          // scroll one position left:
      delay(350);                       // wait a bit:
    }
  }
  else
  {
    Serial.print("Error in getInt, ");
    Serial.println(fbdata.errorReason());
  }
}
