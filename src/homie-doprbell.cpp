#include <Homie.h>

//const int PIN_BELL = A0;
unsigned long previousMillis = 0;
unsigned long lastTime = 0;
int delayValue = 200;
int bit = 0;

//type has to be an ESH type, like ESH:Switch or ESH:PowerOutlet
HomieNode bellNode("doorbell", "ESH:Alarm");

void setupPublish(){
  bellNode.setProperty("").send("gong,itemtype");
  bellNode.setProperty("gong").send("FALSE");
  bellNode.setProperty("itemtype").send("Switch");
}

void loopHandler() {
  if (bit == 0){
    setupPublish();
    bit = 1;
  }
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= delayValue){
    int bellState = analogRead(A0);
    Serial.println(bellState);
    if (bellState > 80 && currentMillis - lastTime >= 2000){
      lastTime = millis();
      Serial.println("Ding Dong");
      bellNode.setProperty("gong").send("ON");
      delay(10);
      bellNode.setProperty("gong").send("OFF");
    }
    previousMillis = currentMillis;
  }

}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  pinMode(A0, INPUT);

  Homie_setFirmware("Magic Doorbell", "0.0.2");
  Homie.setLoopFunction(loopHandler);
  Homie.setup();
}

void loop() {
  Homie.loop();
}
