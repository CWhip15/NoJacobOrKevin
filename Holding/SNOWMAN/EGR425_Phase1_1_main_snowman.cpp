#include <M5Core2.h>

// LCD Details
// 320 x 240 pixels (width x height)
//      - (0, 0) is the TOP LEFT pixel
//      - Colors seem to be 16-bit as: RRRR RGGG GGGB BBBB (32 Reds, 64 Greens, 32 Blues)
//      TFT_RED         0xF800      /* 255,   0,   0 */     1111 1000 0000 0000
//      TFT_GREEN       0x07E0      /*   0, 255,   0 */     0000 0111 1110 0000
//      TFT_BLUE        0x001F      /*   0,   0, 255 */     0000 0000 0001 1111

///////////////////////////////////////////////////////////////
// Put your setup code here, to run once
///////////////////////////////////////////////////////////////
void setup() {
	// Initialize the device
    M5.begin();
	
    // Set up some variables for use in drawing
    int sWidth = M5.Lcd.width();
    int xScreenMid = sWidth / 2;
    int sHeight = M5.Lcd.height();
    Serial.printf("%d x %d res\n", sWidth, sHeight);

    // Set up some more variables to programmatically create snowman
    int radius = sWidth / 6;
    int diameter = radius * 2;
    int yOffset = 0;
    double ballSizePercentage = .84;

    // Change the background
    M5.Lcd.fillScreen(TFT_CYAN);

	// TODO 0: Draw the base of the snowman
    // https://docs.m5stack.com/en/api/core2/lcd_api
    M5.Lcd.fillCircle(xScreenMid, sHeight - radius, radius, TFT_WHITE);

    // Save the center location of middle and top circles for later
    int yBodyMid = 0;
    int yHeadMid = 0;

    // TODO 1: Draw the body and head (2 iterations)
    for (int i = 0; i < 2; i++) {
        yOffset += diameter * ballSizePercentage;
        radius *= ballSizePercentage;
        diameter = radius * 2;
        int yCircleMid = sHeight - yOffset - radius;
        M5.Lcd.fillCircle(xScreenMid, yCircleMid, radius, TFT_WHITE);

        // Save the y location of the body and head for later
        if (i == 0)
            yBodyMid = yCircleMid;
        else if (i == 1)
            yHeadMid = yCircleMid;
    }

    // TODO 2: Draw 3 buttons
    int rButton = 6;
    for (int i = -1; i < 2; i++)
        M5.Lcd.fillCircle(xScreenMid, yBodyMid + (rButton*3*i), rButton, TFT_BLACK);

    // TODO 3: Draw 2 eyes
    M5.Lcd.fillCircle(xScreenMid - 15, yHeadMid - 5, rButton, TFT_BLACK);
    M5.Lcd.fillCircle(xScreenMid + 15, yHeadMid - 5, rButton, TFT_BLACK);

    // TODO 4: Draw a carrot nose
    M5.Lcd.fillTriangle(xScreenMid, yHeadMid, xScreenMid, yHeadMid+16, xScreenMid+30, yHeadMid+8, TFT_ORANGE);
}

///////////////////////////////////////////////////////////////
// Put your main code here, to run repeatedly
///////////////////////////////////////////////////////////////
void loop() {}

//////////////////////////////////////////////////////////////////////////////////
// For more documentation see the following link:
// https://github.com/m5stack/m5-docs/blob/master/docs/en/api/
//////////////////////////////////////////////////////////////////////////////////