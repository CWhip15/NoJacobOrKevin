#include <Arduino.h>
#include <M5Core2.h>

void setup() {
   M5.begin();
}

int redVal = 0x0000;
int greenVal = 0x0000;
int blueVal = 0x0000;

void loop() {
  m5.update();
  int screenWidth = M5.Lcd.width();
  int screenHeight = M5.Lcd.height();

  if (m5.BtnA.isPressed()) {
    if (redVal == 0xF800) {
      redVal = 2048;
    } else {
      redVal += 2048;
    }
  }

  if (m5.BtnB.isPressed()) {
    if (greenVal == 0x07E0) {
      greenVal = 0x0000;
    } else {
      greenVal += 32;
    }
  }

  if (m5.BtnC.isPressed()) {
    if (blueVal == 0x001F) {
      blueVal = 0x0000;
    } else {
      blueVal++;
    }
  }

  M5.lcd.fillRect(0, 0, screenWidth / 3, screenHeight, redVal);
  M5.lcd.fillRect(screenWidth / 3, 0, screenWidth / 3, screenHeight, greenVal);
  M5.lcd.fillRect((screenWidth / 3) * 2, 0, screenWidth / 3, screenHeight, blueVal);

  delay(100);
}