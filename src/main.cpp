#include <M5Core2.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "EGR425_Phase1_weather_bitmap_images.h"
#include "WiFi.h"
#include <WiFiUdp.h>
#include <NTPClient.h>

// Worked on by Isaiah Swanson and Cole Whipple

////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////
// TODO 3: Register for openweather account and get API key
String zipCode = "92501";
String urlOpenWeather = "https://api.openweathermap.org/data/2.5/weather?";
String apiKey = "6875abc1304940d4be21b2884f97315a";

// TODO 1: WiFi variables
String wifiNetworkName = "CBU"; // Set wifi name here
String wifiPassword = "";

// Time variables
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// LCD variables
int sWidth;
int sHeight;
boolean tempUnits = 1;
boolean screenState = 1;
const int pad = 10;
int fillScreenColor = 0x0000;
double tempNow = 0.0;
double tempMin = 0.0;
double tempMax = 0.0;
int digits [5] = {0,0,0,0,0};
uint16_t primaryTextColor = TFT_BLACK;

////////////////////////////////////////////////////////////////////
// Method header declarations
////////////////////////////////////////////////////////////////////
String httpGETRequest(const char* serverName);
void drawWeatherImage(String iconId, int resizeMult);
void drawTemps(double tempMin, double tempNow, double tempMax, int primaryTextColor);
void getTemps();
void drawScreen();
void increaseDigit(int index);
void decreaseDigit(int index);

///////////////////////////////////////////////////////////////
// Put your setup code here, to run once
///////////////////////////////////////////////////////////////
void setup() {
    // Initialize the device
    M5.begin();
    
    // Set screen orientation and get height/width 
    sWidth = M5.Lcd.width();
    sHeight = M5.Lcd.height();

    // TODO 2: Connect to WiFi
    WiFi.begin(wifiNetworkName.c_str(), wifiPassword.c_str());
    Serial.printf("Connecting");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.print("\n\nConnected to WiFi network with IP address: ");
    Serial.println(WiFi.localIP());

    drawScreen();
}

// Setting hotzones for the zipcode buttons on the LCD screen
// Hotzones for increasing the digits
static HotZone hz1(18-10, 63-10, 43+10, 86); // 1st digit
static HotZone hz2(82-10, 63-10, 107+10, 86); // 2nd digit
static HotZone hz3(146-10, 63-10, 171+10, 86); // 3rd digit
static HotZone hz4(210-10, 63-10, 235+10, 86); // 4th digit
static HotZone hz5(274-10, 63-10, 299+10, 86); // 5th digit

// Hotzones for decreasing the digits
static HotZone hz6(18-10, 156, 43+10, 179+10); // 1st digit
static HotZone hz7(82-10, 156, 107+10, 179+10); // 2nd digit
static HotZone hz8(146-10, 156, 171+10, 179+10); // 3rd digit
static HotZone hz9(210-10, 156, 235+10, 179+10); // 4th digit
static HotZone hz10(274-10, 156, 299+10, 179+10); // 5th digit

///////////////////////////////////////////////////////////////
// Put your main code here, to run repeatedly
///////////////////////////////////////////////////////////////
void loop() {
    m5.update();

    // Check for button press as often as possible
    if (screenState) {
        if (m5.BtnA.wasPressed()) {
            tempUnits = !tempUnits;
            drawScreen();
        }
    }

    // Checking to see if button was pressed to go to other screen
    if (m5.BtnB.wasPressed()) {
        screenState = !screenState;
        drawScreen();
    }

    // Update zipcode digits when button is pressed
    if (!screenState) {
        if (M5.Touch.ispressed()) {
            TouchPoint touch = M5.Touch.getPressPoint();
            if (hz1.inHotZone(touch)) {
                increaseDigit(0);
                delay(100);
            }
            if (hz2.inHotZone(touch)) {
                increaseDigit(1);
                delay(100);
            }
            if (hz3.inHotZone(touch)) {
                increaseDigit(2);
                delay(100);
            }
            if (hz4.inHotZone(touch)) {
                increaseDigit(3);
                delay(100);
            }
            if (hz5.inHotZone(touch)) {
                increaseDigit(4);
                delay(100);
            }
            if (hz6.inHotZone(touch)) {
                decreaseDigit(0);
                delay(100);
            }
            if (hz7.inHotZone(touch)) {
                decreaseDigit(1);
                delay(100);
            }
            if (hz8.inHotZone(touch)) {
                decreaseDigit(2);
                delay(100);
            }
            if (hz9.inHotZone(touch)) {
                decreaseDigit(3);
                delay(100);
            }
            if (hz10.inHotZone(touch)) {
                decreaseDigit(4);
                delay(100);
            }
        }
    }

    // Redraw the weather when button C is pressed
    if (screenState) {
        if (m5.BtnC.wasPressed()) {
            drawScreen();
        }
    }
}

/////////////////////////////////////////////////////////////////
// This method takes in a URL and makes a GET request to the
// URL, returning the response.
/////////////////////////////////////////////////////////////////
String httpGETRequest(const char* serverURL) {
    
    // Initialize client
    HTTPClient http;
    http.begin(serverURL);

    // Send HTTP GET request and obtain response
    int httpResponseCode = http.GET();
    String response = http.getString();

    // Check if got an error
    if (httpResponseCode > 0)
        Serial.printf("HTTP Response code: %d\n", httpResponseCode);
    else {
        Serial.printf("HTTP Response ERROR code: %d\n", httpResponseCode);
        Serial.printf("Server Response: %s\n", response.c_str());
    }

    // Free resources and return response
    http.end();
    return response;
}

/////////////////////////////////////////////////////////////////
// This method takes in an image icon string (from API) and a 
// resize multiple and draws the corresponding image (bitmap byte
// arrays found in EGR425_Phase1_weather_bitmap_images.h) to scale (for 
// example, if resizeMult==2, will draw the image as 200x200 instead
// of the native 100x100 pixels) on the right-hand side of the
// screen (centered vertically). 
/////////////////////////////////////////////////////////////////
void drawWeatherImage(String iconId, int resizeMult) {

    // Get the corresponding byte array
    const uint16_t * weatherBitmap = getWeatherBitmap(iconId);

    // Compute offsets so that the image is centered vertically and is
    // right-aligned
    int yOffset = -(resizeMult * imgSqDim - M5.Lcd.height()) / 2;
    int xOffset = sWidth - (imgSqDim*resizeMult*.8); // Right align (image doesn't take up entire array)
    //int xOffset = (M5.Lcd.width() / 2) - (imgSqDim * resizeMult / 2); // center horizontally
    
    // Iterate through each pixel of the imgSqDim x imgSqDim (100 x 100) array
    for (int y = 0; y < imgSqDim; y++) {
        for (int x = 0; x < imgSqDim; x++) {
            // Compute the linear index in the array and get pixel value
            int pixNum = (y * imgSqDim) + x;
            uint16_t pixel = weatherBitmap[pixNum];

            // If the pixel is black, do NOT draw (treat it as transparent);
            // otherwise, draw the value
            if (pixel != 0) {
                // 16-bit RBG565 values give the high 5 pixels to red, the middle
                // 6 pixels to green and the low 5 pixels to blue as described
                // here: http://www.barth-dev.de/online/rgb565-color-picker/
                byte red = (pixel >> 11) & 0b0000000000011111;
                red = red << 3;
                byte green = (pixel >> 5) & 0b0000000000111111;
                green = green << 2;
                byte blue = pixel & 0b0000000000011111;
                blue = blue << 3;

                // Scale image; for example, if resizeMult == 2, draw a 2x2
                // filled square for each original pixel
                for (int i = 0; i < resizeMult; i++) {
                    for (int j = 0; j < resizeMult; j++) {
                        int xDraw = x * resizeMult + i + xOffset;
                        int yDraw = y * resizeMult + j + yOffset;
                        M5.Lcd.drawPixel(xDraw, yDraw - 70, M5.Lcd.color565(red, green, blue));
                    }
                }
            }
        }
    }
}

// Custom functions for redrawing parts of the screen
void drawTemps(double tempMin, double tempNow, double tempMax, int primaryTextColor) {

    // This is where the conversions from Fahrenheit to Celcius takes place
    if (tempUnits) { // If units are in Fahrenheit
        M5.Lcd.setCursor(pad, pad);
        M5.Lcd.setTextColor(TFT_BLUE);
        M5.Lcd.setTextSize(3);
        M5.Lcd.printf("LO:%0.fF\n", tempMin);
        
        M5.Lcd.setCursor(pad, M5.Lcd.getCursorY());
        M5.Lcd.setTextColor(primaryTextColor);
        M5.Lcd.setTextSize(10);
        M5.Lcd.printf("%0.fF\n", tempNow);

        M5.Lcd.setCursor(pad, M5.Lcd.getCursorY());
        M5.Lcd.setTextColor(TFT_RED);
        M5.Lcd.setTextSize(3);
        M5.Lcd.printf("HI:%0.fF\n", tempMax);

    } else { // If units are in Celcius
        M5.Lcd.setCursor(pad, pad);
        M5.Lcd.setTextColor(TFT_BLUE);
        M5.Lcd.setTextSize(3);
        M5.Lcd.printf("LO:%0.fC\n", 5*(tempMin - 32)/9);
        
        M5.Lcd.setCursor(pad, M5.Lcd.getCursorY());
        M5.Lcd.setTextColor(primaryTextColor);
        M5.Lcd.setTextSize(10);
        M5.Lcd.printf("%0.fC\n", 5*(tempNow - 32)/9);

        M5.Lcd.setCursor(pad, M5.Lcd.getCursorY());
        M5.Lcd.setTextColor(TFT_RED);
        M5.Lcd.setTextSize(3);
        M5.Lcd.printf("HI:%0.fC\n", 5*(tempMax - 32)/9);
    }
}

// Custom function for drawing the screen given it's state
void drawScreen() {
    m5.update();
    if (m5.BtnB.wasPressed()) {
        screenState = !screenState;
    }

    if (screenState == 1) {

        // Go through the process of drawing the weather screen
        String serverURL = urlOpenWeather + "zip=" + zipCode +",us&units=imperial&appid=" + apiKey;

        String response = httpGETRequest(serverURL.c_str());
        
        const size_t jsonCapacity = 768+250;
        DynamicJsonDocument objResponse(jsonCapacity);

        DeserializationError error = deserializeJson(objResponse, response);
        if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
            return;
        }

        JsonArray arrWeather = objResponse["weather"];
        JsonObject objWeather0 = arrWeather.getElement(0);
        String strWeatherDesc = objWeather0["main"];
        String strWeatherIcon = objWeather0["icon"];
        String cityName = objResponse["name"];

        JsonObject objMain = objResponse["main"];
        double tempNow = objMain["temp"];
        double tempMin = objMain["temp_min"];
        double tempMax = objMain["temp_max"];
        Serial.printf("NOW: %.1f F and %s\tMIN: %.1f F\tMax: %.1f F\n", tempNow, strWeatherDesc, tempMin, tempMax);

        uint16_t primaryTextColor;
        if (strWeatherIcon.indexOf("d") >= 0) {
            M5.Lcd.fillScreen(TFT_CYAN);
            fillScreenColor = TFT_CYAN;
            primaryTextColor = TFT_BLACK;
        } else {
            M5.Lcd.fillScreen(TFT_NAVY);
            fillScreenColor = TFT_NAVY;
            primaryTextColor = TFT_WHITE;
        }
        
        // Calling the function to draw the weather image
        drawWeatherImage(strWeatherIcon, 2);
        
        // Calling the temperatures
        drawTemps(tempMin, tempNow, tempMax, primaryTextColor);

        M5.Lcd.setCursor(pad, M5.Lcd.getCursorY());
        M5.Lcd.setTextColor(primaryTextColor);
        M5.Lcd.printf("%s\n", cityName.c_str());

        M5.Lcd.setCursor(pad, M5.Lcd.getCursorY());
        M5.Lcd.setTextColor(primaryTextColor);

        timeClient.update();
        timeClient.getFormattedTime();
        long rawTime = timeClient.getEpochTime();
        long hours = (rawTime % 43200L)/3600;
        int timeofDay = (rawTime % 86400L)/3600; // We use millitary time to figure out if it's currently AM or PM.
        String timeHours = "0";
        String amPM = " XM";

        // This is pulling time from the UK, where they are 8 hours ahead of us.
        // If our hours roll into the negatives, then add 16 hours to the millitary time to adjust.
        if ((timeofDay-8) < 0) {
            timeofDay += 16;
        } else {
            timeofDay -= 8;
        }

        // If, in millitary time, the current hour is > 12, we set the time of day maker to PM.
        if ((timeofDay) >= 12) {
            amPM = " PM";
        } else {
            amPM = " AM";
        }

        // Here we convert the hours from millitary time to standard.
        if (hours == 0) {
            timeHours = "4";
        } else if (hours >= 9) {
            timeHours = String(hours-8);
        } else if (hours < 9) {
            timeHours = String(hours+4);
        }

        // Checking minutes
        long timeMinutes = (rawTime % 3600) / 60;
        String minutesString = "";

        // Setting up the seconds string.
        String secondsString = "";

        // If current minutes is less than 10, we add a 0 before the single-digit number to abide by convention.
        if (timeMinutes < 10) {
            minutesString += "0";
        }
        minutesString += String(timeMinutes);

        // Setting up the seconds for the timestamp.
        if (timeClient.getSeconds() < 10) {
            secondsString += "0";
        }
        secondsString += String(timeClient.getSeconds());

        // Pulling all of the elements of the timestamp together.
        String currentTime = timeHours + ":" + minutesString + ":" + secondsString + amPM;

        // M5.Lcd.printf(timeClient.getFormattedTime().c_str());           
        M5.Lcd.printf(currentTime.c_str());

    } else {
        // Set values for building the zipcode screen
        M5.Lcd.fillScreen(TFT_BLACK);
        M5.Lcd.setTextSize(4);
        M5.Lcd.setTextColor(TFT_WHITE);

        // Drawing the buttons for increasing and decreasing the digits of the zipcode.
        for (int i = 0; i <= zipCode.length() - 1; i++) {
            digits[i] = (int) zipCode.charAt(i);
            digits[i] -= 48;
            M5.Lcd.fillTriangle(sWidth*0.2*i + 20, sHeight*0.45 - 25, sWidth*0.2*i + 40, sHeight*0.45 - 25, sWidth*0.2*i + 30, sHeight*0.45 - 45, TFT_WHITE);
            M5.Lcd.fillTriangle(sWidth*0.2*i + 20, sHeight*0.45 + 50, sWidth*0.2*i + 40, sHeight*0.45 + 50, sWidth*0.2*i + 30, sHeight*0.45 + 70, TFT_WHITE);

            // Drawing the actual zipcode happens here.
            M5.Lcd.drawString(String(digits[i]), sWidth*0.2*i + 20, sHeight*0.45);
        }
    }
}

void increaseDigit(int index) {
    // We use this variable to edit the value which is at the passed in index.
    int returnDigit = digits[index];

    // If the next value for the digit is 10 then set the digit to be 0 (the digit cannot be > 9).
    // This rolls the digit over to 0 if the value goes out of bounds.
    if (returnDigit + 1 > 9) {
        returnDigit = 0;
    } else {
        returnDigit = digits[index] + 1;
    }

    // Putting the new digit back into the digits index after increasing by 1 or rolling back to 0.
    digits[index] = returnDigit;

    // Update the global zipcode variable in order to achieve persistence.
    String newZipString = "";
    for (int i = 0; i <= zipCode.length() - 1; i++) {
        newZipString += String(digits[i]);
    }

    // Setting the global zipcode
    zipCode = newZipString;

    drawScreen();
    // No need to return
}

void decreaseDigit(int index) {
    // We use this variable to edit the value which is at the passed in index.
    int returnDigit = digits[index];

    // If the next value for the digit is -1 then set the digit to be 9 (the digit cannot be < 0).
    // This rolls the digit over to 9 if the value goes out of bounds.
    if (returnDigit - 1 < 0) {
        returnDigit = 9;
    } else {
        returnDigit = digits[index] - 1;
    }

    // Putting the new digit back into the digits index after decreasing by 1 or rolling back to 9.
    digits[index] = returnDigit;

    // Update the global zipcode variable in order to achieve persistence.
    String newZipString = "";
    for (int i = 0; i <= zipCode.length() - 1; i++) {
        newZipString += String(digits[i]);
    }

    // Setting the global zipcode
    zipCode = newZipString;

    drawScreen();
    // No need to return
}

//////////////////////////////////////////////////////////////////////////////////
// For more documentation see the following links:
// https://github.com/m5stack/m5-docs/blob/master/docs/en/api/
// https://docs.m5stack.com/en/api/core2/lcd_api
//////////////////////////////////////////////////////////////////////////////////