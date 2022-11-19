#include <ESP8266WiFi.h>
#include <espnow.h>
#include <DHT.h>
#define DHTPin D5
#define DHTType DHT22
DHT dht(DHTPin, DHTType);
float t,h;
unsigned int ldr;

uint8_t broadcastAddress[] = {0x84, 0xCC, 0xA8, 0xA3, 0xF0, 0x5E};

typedef struct struct_message {
  float a;
  float b;
  float c;
} struct_message;

struct_message myData;

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) 
{
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0)
  {
    Serial.println("Sent");
  }
  else
  {
    Serial.println("Failed"); 
  }
}
 
void setup() {
  Serial.begin(115200);
  delay(100);
  dht.begin();

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);

  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}
 
void loop() {
  t = dht.readTemperature();
  delay(10);
  h = dht.readHumidity();
  delay(10);
  ldr = analogRead(A0);
  
  myData.a = t;
  myData.b = h;
  myData.c = ldr;

  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  delay(2000);
}
