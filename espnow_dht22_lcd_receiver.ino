#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 20 chars and 2 line display

typedef struct struct_message {
    float a;
    float b;
    float c;
} struct_message;

struct_message myData;

void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) 
{
  lcd.clear(); 
  memcpy(&myData, incomingData, sizeof(myData));
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");lcd.print(myData.a);
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");lcd.print(myData.b); 
  lcd.setCursor(0, 2);
  lcd.print("LDR: ");lcd.print(myData.c);
}
 
void setup() {
  Serial.begin(115200);
    lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
    lcd.backlight(); 
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
 
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  }
