#include <WiFi.h>
IPAddress ipadr;
const char* ssid = "***";
const char* password = "***";

#include <HTTPClient.h>
const char* serverName = "http://***:7002/post_data";

#include <DHT.h>
const int PIN_DHT = 13;
DHT dht(PIN_DHT,DHT11);

#include "HX711.h"
#define DT_PIN 4
#define SCK_PIN 5
HX711 scale;

#include <time.h>
#define JST 3600* 9

time_t t;
struct tm *tm;
static const char *wd[7] = {"Sun","Mon","Tue","Wed","Thr","Fri","Sat"};

unsigned long lastTime = 0;
unsigned long timerDelay = 5000; // 5 second

void setup() {


    Serial.begin(115200);  // シリアルモニタを115200bpsで初期化
    WiFi.mode(WIFI_STA);  // Wi-Fiのモードを設定
    WiFi.begin(ssid, password);  // 接続するSSID　キーを指定してWi-Fiを開始
    Serial.println("");         // シリアルプリントの出力改行のみ
    while (WiFi.status() != WL_CONNECTED) {  // 接続が完了するまで待つ
      delay(500);  // 各回500msの時間待つ
      Serial.print(".");// 待ち時間500msごとに１ドット表示　接続が完了すると抜ける
    }

    // wifi設定表示
    Serial.println("");
    Serial.print("Connected to ");  // Wi-Fi接続を表示
    Serial.println(ssid);  // SSID表示
    Serial.print("IP address: ");
    ipadr = WiFi.localIP();
    Serial.println(ipadr);  // シリアルモニタにIPアドレスを表示

    // dht sensor set
    dht.begin();

    // scaling set
    scale.begin(DT_PIN, SCK_PIN);
    scale.set_scale(536); // scaling factor
    scale.tare();

    // time set
    configTime( JST, 0, "ntp.nict.jp", "ntp.jst.mfeed.ad.jp");

}

void post_data(){

  HTTPClient http;
    
  // Your Domain name with URL path or IP address with path
  http.begin(serverName);
  
  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  // humidity and temperature
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float w = scale.get_units(3);
  
  
  String jsonData = "{";
  jsonData += "\"temperature\":";
  jsonData += String(t);
  jsonData += ",\"humidity\":";
  jsonData += String(h);
  jsonData += ",\"weight\":";
  jsonData += String(w);
  jsonData += "}";
  
  Serial.println(jsonData);

  http.addHeader("Content-Type", "application/json" , "Content-Length", jsonData.length());
  
  // Send HTTP POST request
  int httpResponseCode = http.POST(jsonData);
}

void loop(void) {

  // 5秒に１回
  if ((millis() - lastTime) > timerDelay) {
    t = time(NULL);
    tm = localtime(&t);
    // 0分になったら．
    if(tm->tm_min == 0){
      post_data(); // http post
    }
  }
}

// tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday,
// wd[tm->tm_wday], tm->tm_hour, tm->tm_min, tm->tm_sec
