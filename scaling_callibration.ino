#include "HX711.h"

#define DT_PIN 4
#define SCK_PIN 5

HX711 scale;

float actual_weight = 178;

void setup() {
 Serial.begin(115200);
 scale.begin(DT_PIN, SCK_PIN);
//  scale.set_scale();  // Start scale
 scale.tare();       // Reset scale to zero
}

void loop() {
 float current_weight=scale.get_units(3);  // get average of 20 scale readings
 float scale_factor=(current_weight/actual_weight);  // divide the result by a known weight

 Serial.println("---------------------");  // Print the scale factor to use
 Serial.println(current_weight);  // Print the scale factor to use
 Serial.println(actual_weight);
 Serial.println(scale_factor);
 
 delay(1000);
}

