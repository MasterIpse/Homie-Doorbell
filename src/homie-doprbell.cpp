#include <Homie.h>

//const int PIN_BELL = A0;
unsigned long previousMillis = 0;
unsigned long lastTime = 0;
int delayValue = 200;

//type has to be an ESH type, like ESH:Switch or ESH:PowerOutlet
HomieNode bellNode("doorbell", "ESH:Alarm");


void loopHandler() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= delayValue){
    int bellState = analogRead(A0);
    Serial.println(bellState);
    if (bellState > 500 && currentMillis - lastTime >= 2000){
      lastTime = millis();
      Serial.println("Ding Dong");
      bellNode.setProperty("gong").send("TRUE");
      delay(10);
      bellNode.setProperty("gong").send("FALSE");
    }
    previousMillis = currentMillis;
  }

}

void setupPublish(){
  bellNode.setProperty("").send("gong,itemtype");
  bellNode.setProperty("gong").send("FALSE");
  bellNode.setProperty("itemtype").send("Switch");
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  pinMode(A0, INPUT);

  Homie_setFirmware("Magic Doorbell", "0.0.1");
  Homie.setLoopFunction(loopHandler);
  Homie.setup();
  setupPublish();
}

void loop() {
  Homie.loop();
}
