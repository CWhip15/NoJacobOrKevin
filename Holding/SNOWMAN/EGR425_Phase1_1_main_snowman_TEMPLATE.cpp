#include <M5Core2.h>

///////////////////////////////////////////////////////////////
// Put your setup code here, to run once
///////////////////////////////////////////////////////////////
void setup() {
	// Initialize the device
    M5.begin();
	
    // Set up some variables for use in drawing
    int screenWidth = M5.Lcd.width();
    int screenHeight = M5.Lcd.height();



	// TODO 0: Draw the base of the snowman
    // https://github.com/m5stack/m5-docs/blob/master/docs/en/api/lcd.md#fillcircle
    M5.lcd.fillCircle(screenWidth / 2, screenWidth / 2, 50, TFT_WHITE);
    M5.lcd.fillCircle(screenWidth / 2, screenWidth / 3, 35, TFT_WHITE);
    M5.lcd.fillCircle(screenWidth / 2, screenWidth / 4.5, 25, TFT_WHITE);
    M5.lcd.fillCircle(screenWidth / 2.1, screenWidth / 4.6, 5, TFT_BLACK);
    M5.lcd.fillCircle(screenWidth / 1.9, screenWidth / 4.6, 5, TFT_BLACK);
    M5.lcd.fillTriangle(screenWidth / 2, screenWidth / 4.4, screenWidth / 2, screenWidth / 4.0, screenWidth / 1.75, screenWidth / 4.2, TFT_ORANGE);
    M5.lcd.fillCircle(screenWidth / 2, screenWidth / 3.1, 5, TFT_BLACK);
    M5.lcd.fillCircle(screenWidth / 2, screenWidth / 2.65, 5, TFT_BLACK);
    M5.lcd.fillCircle(screenWidth / 2, screenWidth / 2.30, 5, TFT_BLACK);

    // TODO 1: Draw the body and head (2 iterations)

    // TODO 2: Draw 3 buttons

    // TODO 3: Draw 2 eyes

    // TODO 4: Draw a carrot nose
}

///////////////////////////////////////////////////////////////
// Put your main code here, to run repeatedly
///////////////////////////////////////////////////////////////
void loop() {}

//////////////////////////////////////////////////////////////////////////////////
// For more documentation see the following link:
// https://github.com/m5stack/m5-docs/blob/master/docs/en/api/
//////////////////////////////////////////////////////////////////////////////////