#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char *ssid = "Ian";
const char *password = "tenteasorte";
const char *mqtt_server = "192.168.157.96";

WiFiClient espClient;
PubSubClient client(espClient);
const int Relay = D1;

void setup_wifi()
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String message;
  for (int i = 0; i < length; i++)
  {
    message += ((char)payload[i]);
  }
  Serial.println(message);

  if (String(topic) == "ControleRelay")
  {
    if (message == "ON")
    {
      digitalWrite(Relay, LOW);
    }
    else if (message == "OFF")
    {
      digitalWrite(Relay, HIGH);
    }
  }
}

void setup()
{
  pinMode(Relay, OUTPUT);
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client"))
    {
      Serial.println("connected");
      client.subscribe("ControleRelay");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
}