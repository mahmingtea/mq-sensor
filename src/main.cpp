#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

#define SENSOR_PIN 33
#define BUZZER_PIN 18

const char *ssid = "";     // wifi name
const char *password = ""; // wifi password
String BOT_TOKEN = "";     // telegram bot father atangin siam bot siam tur
String chat_id = "";       // telegram ah idbot atangin neih tur
const int triggerValue = 1000;
const int resetValue = 800;
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);
const int noti_interval = 60000 * 3; // Notification hi minute 3 danah a thawn ang
int interval = 0;
unsigned long last_noti = 0;
bool notified = false;
void notificationHandler(float s);

void setup()
{
  Serial.begin(9600);
  pinMode(BUZZER_PIN, OUTPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
  client.setInsecure();
}

void loop()
{
  float s = analogRead(SENSOR_PIN);
  if (s > triggerValue)
  {
    digitalWrite(BUZZER_PIN, HIGH);
    if (!notified)
    {
      notified = true;
      notificationHandler(s);
    }
  }
  if (s < resetValue)
  {
    digitalWrite(BUZZER_PIN, LOW);
    if (notified)
    {
      notified = false;
      last_noti = 0;
      interval = 0;
      String msg = "🟢 *A REH TA E!*\n";
      bot.sendMessage(chat_id, msg, "Markdown");
      delay(1000);
    }
  }
}
void notificationHandler(float s)
{
  if (BOT_TOKEN.length() == 0 || chat_id.length() == 0)
  {
    return;
  }
  if (millis() - last_noti > interval)
  {
    interval = noti_interval;
    last_noti = millis();
    if (s > 3000)
    {
      String msg = "⚠️ *GAS A LEAK NASA LUTK!!* 🔥\n";
      msg += "💥 A PUAH HMA IN CHECK VAT RAWH!!!! 💥";
      bot.sendMessage(chat_id, msg, "Markdown");
    }
    else if (s > 2000)
    {
      String msg = "⚠️ *GAS A LEAK NUAL!*\n";
      msg += "A HLAUHAWM CHECK VAT RAWH";
      bot.sendMessage(chat_id, msg, "Markdown");
    }
    else if (s > 1000)
    {
      String msg = "⚠️ *GAS TLEM A LEAK!*\n";
      msg += "GAS A VANG SI CHECK VAT RAWH";
      bot.sendMessage(chat_id, msg, "Markdown");
    }
  }
}