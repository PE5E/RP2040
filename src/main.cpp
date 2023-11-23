/* ====================================================================
 * RP2040-zero 
 * 
 * Author:
 * Jaap van der Leest
 * 
 * License:
 * MIT License
 * 
 * Copyright (c) 2023 Jaap van der Leest
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


// INCLUDES:
#include <Arduino.h>

#include <FastLED.h>

// PINOUT
// Noepixel or WS2812B
// Neopixels for RPM indication
#define pin_rpm_leds    16
#define CHIP_SET        WS2812B
#define COLOR_CODE      GRB

const int total_leds = 1; 
CRGB leds[total_leds]; 
int brightness    =  50; // 0 - 255

unsigned long last_time = 0;
double rainbow_round_time = 5.0; // seconds

void color_rainbow(int angle_int, int &red, int &green, int &blue);

void setup() {
    FastLED.addLeds<CHIP_SET, pin_rpm_leds, COLOR_CODE>(leds, total_leds);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 50);
    FastLED.setBrightness(brightness);
}

void loop() {
    /*
    leds[0] = CRGB::Green;
    FastLED.show();
    delay(1000);

    leds[0] = CRGB::Blue;
    FastLED.show();
    delay(1000);
    
    leds[0] = CRGB::Magenta;
    FastLED.show();
    delay(1000);

    leds[0] = CRGB::Violet;
    FastLED.show();
    delay(1000);

    leds[0] = CRGB::Red;
    FastLED.show();
    delay(1000);

    leds[0] = CRGB::Orange;
    FastLED.show();
    delay(1000);

    leds[0] = CRGB::Yellow;
    FastLED.show();
    delay(1000);
    */

    unsigned long now = millis();
    unsigned long period_time = now - last_time;
    double time_ratio = period_time / (rainbow_round_time * 1000.0);

    if(time_ratio >= 1.0) {
        last_time = now;
    }
    while(time_ratio >= 1.0) {
        time_ratio -= 1.0;
    }
    int angle = time_ratio * 360;

    int red = 0;
    int green = 0;
    int blue = 0;

    color_rainbow(angle, red, green, blue);
    leds[0] = CRGB(red, green, blue);

    FastLED.show();
    delay(10);
}

void color_rainbow(int angle_int, int &red, int &green, int &blue) {
    int angle = angle_int;
    while(angle >= 360) {
        angle -= 360;
    }

    if      (angle < 60)  {red = 255; green = round(angle * 4.25 - 0.01); blue = 0;}          // from red to orange to yellow
    else if (angle < 120) {red = round((120 - angle) * 4.25 - 0.01); green = 255; blue = 0;}  // from yellow to green
    else if (angle < 180) {red = 0, green = 255; blue = round((angle - 120) * 4.25 - 0.01);}  // from green to aqua
    else if (angle < 240) {red = 0, green = round(( 240 - angle) * 4.25 - 0.01); blue = 255;} // from aqua to blue
    else if (angle < 300) {red = round((angle - 240) * 4.25 - 0.01), green = 0; blue = 255;}  // from blue to magenta
    else                  {red = 255, green = 0; blue = round((360 - angle) * 4.25 - 0.01);}  // from magenta to red
}

