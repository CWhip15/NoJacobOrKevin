#include <Arduino.h>
#include <M5Core2.h>

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  // Serial.println("Hellow from M5Core2 Device!");
  // m5.Lcd.setTextSize(3);
  // m5.Lcd.setTextColor(WHITE);
  // m5.Lcd.text
  // m5.Lcd.fillScreen(DARKGREEN);
  // m5.Lcd.println("Hello world?");
  // m5.Lcd.drawCircle(100, 100, 50, RED);
  // m5.Lcd.fillCircle(100, 100, 50, RED);
  // static unsigned long lastTS;
  // lastTS = millis();
}
// buttonAPresses = 0;
unsigned int led = 0;

void loop() {
  m5.update();

  

  // if (m5.BtnA.isPressed()) {
  //   Serial.printf("Button A has been pressed %lu times! \n", buttonAPresses++);
  // }

  // if (M5.Touch.ispressed()) {
  //   TouchPoint tp = m5.Touch.getPressPoint();
  //   Serial.printf("You pressed touch screen @ (%d,%d).\n", tp.x, tp.y);
  // }
  // Serial.println("Hellow from M5Core2 Device! (again)");
  // if (led == 1) {
  //   m5.Axp.SetLed(0);
  //   led = 0;
  // } else {
  //   m5.Axp.SetLed(1);
  //   led = 1;
  // }
  // delay(1000);
  // m5.update();
  // if (m5.BtnA.isPressed()) {
  //   m5.Axp.SetLed(1);
  //   led = 1;
  // } else {
  //   m5.Axp.SetLed(0);
  //   led = 0;
  // }
  // put your main code here, to run repeatedly:
}