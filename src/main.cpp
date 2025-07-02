#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// configuración Wifi
const char* ssid="Christian";
const char* password="christian123";

// Configurar MQTT
const char* mqtt_server="192.168.185.102";
const int mqtt_port=1883;
const char* mqtt_user="dev_Sala001";
const char* mqtt_pass="8TmCdfqofEe4AcK6s5EHug";
const char* mqtt_topic= "iot/christian_001/Sala001/686480b6e2c84af9c57a89d7/sdata";

WiFiClient espClient;
PubSubClient client(espClient);

// conixion a wifi

void conectarWiFi()
{
  Serial.print("conectado a wifi");
  WiFi.begin(ssid,password);
  while(WiFi.status()!= WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Conectado a Wifi con IP: ");
  Serial.println(WiFi.localIP());
}

// conectar al broker 
void conectarMQTT()
{
   while(!client.connected())
   {
    Serial.print("Conectando a MQTT...");
    if (client.connect("ESP32Client",mqtt_user, mqtt_pass))
    {
      Serial.println("Conectado al broker MQTT");
    }
    else
    {
        Serial.print("Fallo la conexión, rc= ");
        Serial.print(client.state());
        Serial.println("intentar en 3 segundos");
        delay(3000);
    }
   }
}

// Envío de datos {"save":1, "value":20}
void enviarJSON(int save, float value)
{
  StaticJsonDocument<100> doc;
  doc["save"]=save;
  doc["value"]=value;
  
  char buffer[100];
  serializeJson(doc,buffer);

  client.publish(mqtt_topic, buffer);
  client.print("Publicando JSON: ");
  Serial.println(buffer);
}


void setup() 
{
  Serial.begin(115200);
  conectarWiFi();
  client.setServer(mqtt_server, mqtt_port);
  conectarMQTT();
}

void loop() 
{
  if(!client.connected())
  {
    conectarMQTT();
  } 
  client.loop();
  enviarJSON(1, random(75,80)); 
  delay(5000);
}


