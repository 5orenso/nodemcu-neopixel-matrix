// #include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_GFX.h>
// #include <Fonts/FreeMono9pt7b.h>

#include <math.h>
#include <string.h>

#define NUMPIXELS_1 24
#define PIN_1 D5
#define TOTAL_EFFECTS 1
#define DELAY_BETWEEN_EFFECTS 10000

// MATRIX DECLARATION:
// Parameter 1 = width of NeoPixel matrix
// Parameter 2 = height of matrix
// Parameter 3 = pin number (most are valid)
// Parameter 4 = matrix layout flags, add together as needed:
//   NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
//     Position of the FIRST LED in the matrix; pick two, e.g.
//     NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
//   NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs are arranged in horizontal
//     rows or in vertical columns, respectively; pick one or the other.
//   NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns proceed
//     in the same order, or alternate lines reverse direction; pick one.
//   See example below for these values in action.
// Parameter 5 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)


// Example for NeoPixel Shield.  In this application we'd like to use it
// as a 5x8 tall matrix, with the USB port positioned at the top of the
// Arduino.  When held that way, the first pixel is at the top right, and
// lines are arranged in columns, progressive order.  The shield uses
// 800 KHz (v2) pixels that expect GRB color data.
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(60, 8, PIN_1,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS    + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);

Adafruit_NeoPixel strip = Adafruit_NeoPixel(480, PIN_1, NEO_GRB + NEO_KHZ800);

 // Current animation effect
int  mode = 0;
 // Current color
int  r = 100,
     g = 100,
     b = 100;
// Timers
int prevTime,
    lastLoop1;


    void scrollText (char myScrollText[], uint16_t color, int wait, int startPos) {
        matrix.setBrightness(120);
        matrix.setTextColor(color);
        int x = matrix.width() - startPos;
        int pass = 0;
        int scrollLength = (strlen(myScrollText) * 6) + x;

        int colorCycle = 255;
    //    printInt(scrollLength, x-18, 0);
    //    delay(2000);

        matrix.setTextSize(1);
        for(uint16_t i=0; i<scrollLength; i++) {
            matrix.fillScreen(0);
            matrix.setCursor(x, 7);

    //        colorCycle--;
    //        if (colorCycle <= 0) {
    //            colorCycle = 255;
    //        }
    //        matrix.setTextColor(textWheel(colorCycle));
            matrix.print(myScrollText);
            if (--x < -scrollLength) {
                x = matrix.width();
            }
            matrix.show();
            delay(wait);
        }
    }


    void makeItSnow(int numberOfSnowFlakes, uint16_t color, int loops, int speed) {
        int width =  matrix.width();
        int height = matrix.height() + 5;
        float x [numberOfSnowFlakes];
        float y [numberOfSnowFlakes];
            float x2 [numberOfSnowFlakes];
            float y2 [numberOfSnowFlakes];
            float x3 [numberOfSnowFlakes];
            float y3 [numberOfSnowFlakes];
    //        float x4 [numberOfSnowFlakes];
    //        float y4 [numberOfSnowFlakes];

        int c [numberOfSnowFlakes];
        float s [numberOfSnowFlakes];
        // Make initial snowflakes.
        for (int i=0; i<numberOfSnowFlakes; i++) {
            x[i] = (float) random(0, width);
            y[i] = (float) random(-5,1);
            c[i] = random(10, 255);
            s[i] = (float) random(5,15)/10;
            matrix.drawPixel(round(x[i]), round(y[i]), matrix.Color(c[i], c[i], c[i]));
        }
        matrix.show();
        delay(speed);

        for (int j=0; j<loops; j++) {
            for (int i=0; i<numberOfSnowFlakes; i++) {
                matrix.drawPixel(round(x[i]), round(y[i]), matrix.Color(0,0,0));
                matrix.drawPixel(round(x2[i]), round(y2[i]), matrix.Color(0,0,0));
                matrix.drawPixel(round(x3[i]), round(y3[i]), matrix.Color(0,0,0));
    //            matrix.drawPixel(round(x4[i]), round(y4[i]), matrix.Color(0,0,0));
    //            x4[i] = x3[i];
    //            y4[i] = y3[i];
                x3[i] = x2[i];
                y3[i] = y2[i];
                x2[i] = x[i];
                y2[i] = y[i];
                x[i] = x[i] + (sin(y[i]) / 2);
                y[i] = y[i] + s[i];
                if (y[i] > (float) height) {
                    x[i] = (float) random(1, width);
                    y[i] = (float) random(-3,1);
                    c[i] = random(10, 255);
                    s[i] = (float) random(5,15)/10;
                }
    //            matrix.drawPixel(round(x4[i]), round(y4[i]), matrix.Color(c[i]/8, c[i]/8, c[i]/8));
                matrix.drawPixel(round(x3[i]), round(y3[i]), matrix.Color(c[i]/4, c[i]/4, c[i]/4));
                matrix.drawPixel(round(x2[i]), round(y2[i]), matrix.Color(c[i]/2, c[i]/2, c[i]/2));
                matrix.drawPixel(round(x[i]), round(y[i]), matrix.Color(c[i], c[i], c[i]));
            }
            matrix.show();
            delay(speed);
        }

        matrix.show();
    }

void setup() {
    Serial.begin(115200);
    prevTime = millis();

    strip.begin();
    strip.show(); // Initialize all pixels to 'off'

    matrix.begin();
    // matrix.setFont(&FreeMono9pt7b);
    matrix.setTextWrap(false);
}

void loop() {
    int t = 0;
    long now = millis();

    switch(mode) {
        case 0: {
            // if (now - lastLoop1 > 10) {
            //     lastLoop1 = now;
            //     neopixelSet1.bouncing(255, 20, 147, 4, 0);
            // }

            scrollText("Hundekjøring pågår :)", matrix.Color(r, g, b), 60, 0);
            matrix.clear();
            makeItSnow(30, matrix.Color(r, g, b), 500, 100);
            break;
        }
    }

    t = millis();
    if ((t - prevTime) > DELAY_BETWEEN_EFFECTS) {
        mode++;
        if (mode >= TOTAL_EFFECTS) {
            mode = 0;
        }
        Serial.print("mode = "); Serial.println(mode);
        prevTime = t;
    }
}
