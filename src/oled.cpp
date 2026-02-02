/*
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

void setup()
{
    display.begin(SSD1306_SWITCHCAPVCC, 0x3c); // initialize I2C addr 0x3c
    display.clearDisplay();                    // clears the screen and buffer
    display.drawPixel(127, 63, WHITE);

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(15, 0);
    display.println("OLED 0.96 TESTER");

    display.setTextColor(BLACK, WHITE);
    display.setTextSize(2);
    display.setCursor(9, 16);
    display.println("Cybertice");

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 40);
    display.println("128 x 64  Pixels 0.96");

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(12, 50);
    display.println("www.cybertice.com");

    display.display();
}

void loop()
{
}
*/
