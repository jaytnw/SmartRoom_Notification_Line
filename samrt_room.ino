void Line_Notify(String message) ;

#include <ESP8266WiFi.h>
#include <time.h>

// Config connect WiFi
#define WIFI_SSID "Mordang_FL4#1"
#define WIFI_PASSWORD ""

// Line config
#define LINE_TOKEN "cOgk0jCCAxcjtofinXmpscoTNz3pahZB1CSSfTlZ3AQ"

#define SW D2

int timezone = 7 * 3600;
int dst = 0;



String message1 = "ประตูห้องเปิด";
String message2 = "ประตูห้องปิด";

void setup() {
//  pinMode(D0,OUTPUT);
//  pinMode(D1,OUTPUT);
  pinMode(SW, INPUT);

//  digitalWrite(D0,LOW);
//  digitalWrite(D1,HIGH);

  Serial.begin(115200);
  
//    WiFi.mode(WIFI_STA);
//  // connect to wifi.
//  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
//  Serial.print("connecting");
//
//  while (WiFi.status() != WL_CONNECTED) {
//    Serial.print(".");
//    delay(500);
//  }
//  Serial.println();
//  Serial.print("connected: ");
//  Serial.println(WiFi.localIP());

  
  configTime(timezone, dst, "pool.ntp.org", "time.nist.gov");
  Serial.println("\nWaiting for Internet time");

  while (!time(nullptr)) {
    Serial.print("*");
    delay(1000);
  }
  Serial.println("\nTime response....OK");
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    con();
  }

  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);
  String day = String(p_tm->tm_mday);
  //  Serial.print("/");
  String mon = String(p_tm->tm_mon + 1);
  //  Serial.print("/");
  String year = String(p_tm->tm_year + 1900);

  //  Serial.print(" ");

  String hour = String(p_tm->tm_hour);
  //  Serial.print(":");
  String mins = String(p_tm->tm_min);
  //  Serial.print(":");
  String sec = String(p_tm->tm_sec);

  String date = day + "/" + mon + "/" + year;
  String times = hour + ":" + mins + ":" + sec;


  String ons = date + " เวลา " + times + " " + message1;
  String off = date + " เวลา " + times + " " + message2;

  if (digitalRead(SW) == HIGH) {
    while (digitalRead(SW) == HIGH) delay(10);

    Serial.println(ons);
    Line_Notify(ons);
    // Serial.println();
  }
  else if (digitalRead(SW) == LOW) {
    while (digitalRead(SW) == LOW) delay(10);

    Serial.println(off);
    Line_Notify(off);
    // Serial.println();
  }
  delay(10);
}

void Line_Notify(String message) {
  WiFiClientSecure client;

  if (!client.connect("notify-api.line.me", 443)) {
    Serial.println("connection failed");
    return;
  }

  String req = "";
  req += "POST /api/notify HTTP/1.1\r\n";
  req += "Host: notify-api.line.me\r\n";
  req += "Authorization: Bearer " + String(LINE_TOKEN) + "\r\n";
  req += "Cache-Control: no-cache\r\n";
  req += "User-Agent: ESP8266\r\n";
  req += "Connection: close\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += "Content-Length: " + String(String("message=" + message).length()) + "\r\n";
  req += "\r\n";
  req += "message=" + message;
  // Serial.println(req);
  client.print(req);

  delay(20);

  // Serial.println("-------------");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
    //Serial.println(line);
  }
  // Serial.println("-------------");
}

void con() {
  WiFi.mode(WIFI_STA);
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
}

