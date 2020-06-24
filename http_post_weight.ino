#include "HX711.h"
#define DT_PIN 18
#define SCK_PIN 19
HX711 scale;

void setup() {

    // wifi接続
    Serial.begin(115200);  // シリアルモニタを115200bpsで初期化
    scale.begin(DT_PIN, SCK_PIN);
    scale.set_scale(386.3);
    scale.tare();
}

void loop(void) {


  Serial.println(scale.get_units(10), 2);

  delay(100);
}
