#include <M5Core2.h>

///////////////////////////////////////////////////////////////
// Variables
///////////////////////////////////////////////////////////////
bool isGraphMode = false;
float accX;
float accY;
float accZ;

// Screen heights/widths shouldn't be read until M5.begin() is called
int screenWidth;
int screenHeight;
int barWidth;
int barHalf;

///////////////////////////////////////////////////////////////
// Put your setup code here, to run once
///////////////////////////////////////////////////////////////
void setup() {
    // TODO 2: Init device and accelerometer
    M5.begin();
    M5.IMU.Init();
    screenWidth = M5.Lcd.width();
    screenHeight = M5.Lcd.height();
    barWidth = screenWidth / 4;
    barHalf = barWidth / 2;
    //Serial.printf("%d x %d\n", screenWidth, screenHeight);

    // Initialize text options and black screen
    M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.setTextColor(TFT_WHITE);
    M5.Lcd.setTextWrap(true);
    M5.Lcd.setTextSize(3);
}

///////////////////////////////////////////////////////////////
// Put your main code here, to run repeatedly
///////////////////////////////////////////////////////////////
void loop() {
    // Read in button data and store
    M5.update(); // Reads data

    // If button was just pressed/released, update mode and button state
    if (M5.BtnA.wasPressed())
        isGraphMode = !isGraphMode;

    // TODO 1: Get the accelerometer data and adjust to m/s^2 so that 1=>9.8
    M5.IMU.getAccelData(&accX, &accY, &accZ);
    accX *= 9.8;
    accY *= 9.8;
    accZ *= 9.8;

    // Draw the black screen
    M5.Lcd.fillScreen(BLACK);

    // If in RAW DATA mode, print the raw values
    if (!isGraphMode) {
        // Initialize the cursor/font
        M5.Lcd.setTextSize(3);
        M5.Lcd.setCursor(0, 0);

        // TODO 3: Print out the data (white if positive, red if negative)
        M5.Lcd.setTextColor(TFT_WHITE);
        M5.Lcd.printf("Gravity\n(m/s^2)\n\n");
        
        M5.Lcd.setTextColor( (accX >= 0) ? TFT_WHITE : TFT_RED);
        M5.Lcd.printf("X=%.2f\n\n", accX);
        
        M5.Lcd.setTextColor( (accY >= 0) ? TFT_WHITE : TFT_RED);
        M5.Lcd.printf("Y=%.2f\n\n", accY);
        
        M5.Lcd.setTextColor( (accZ >= 0) ? TFT_WHITE : TFT_RED);
        M5.Lcd.printf("Z=%.2f\n\n", accZ);
    } else { // If in GRAPH mode, print the XYZ bar graphs

        // TODO 6: Multiply such that each m/s^2 takes up mult pixels
        int mult = 10;
        accX *= mult;
        accY *= mult;
        accZ *= mult;

        // TODO 9: Draw x-axis marks
        // M5.Lcd.setTextSize(2);
        // M5.Lcd.setTextColor(TFT_WHITE);

        // TODO 5: Draw the bar graphs - draws down and to right; (x,y) is top-left corner
        M5.Lcd.fillRect(barWidth - barHalf, screenHeight - abs(accX), barWidth, abs(accX), accX >=0 ? TFT_BLUE : TFT_CYAN); // Params: (topLeftX, topLeftY, width, height, color)
        M5.Lcd.fillRect(barWidth*2 - barHalf, screenHeight - abs(accY), barWidth, abs(accY), accY >= 0 ? TFT_RED : TFT_PINK);
        M5.Lcd.fillRect(barWidth*3 - barHalf, screenHeight - abs(accZ), barWidth, abs(accZ), accZ >= 0 ? TFT_GREEN : TFT_YELLOW);

        // TODO 7: Draw the X/Y/Z labels on the graph at the bottom
        // M5.Lcd.setTextColor(TFT_BLACK);
        // M5.Lcd.print("X");
        // M5.Lcd.print("Y");
        // M5.Lcd.print("Z");

        // TODO 8: Draw the actual values above the graph
        // M5.Lcd.setTextColor(TFT_WHITE);
        // M5.Lcd.setTextSize(2);
    }

    // TODO 4: Arbitrary delay to slow down screen updates
    delay(50);
}

//////////////////////////////////////////////////////////////////////////////////
// For more documentation see the following link:
// https://github.com/m5stack/m5-docs/blob/master/docs/en/api/
//////////////////////////////////////////////////////////////////////////////////