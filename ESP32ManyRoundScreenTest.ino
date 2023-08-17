// Animated GIF with Multiple Displays
//
// ESP32 40MHz SPI single frame rendering performance

#include <SPI.h>
#include <TFT_eSPI.h>    // Install this library with the Arduino IDE Library Manager
#include <AnimatedGIF.h> // Install this library with the Arduino IDE Library Manager
#include "images/darthvader.h"
#include "images/x_wing.h"
#include "images/nostromo.h"
#include "images/hud_6.h"

const int NUM_DISPLAYS = 3; // Adjust this value based on the number of displays
AnimatedGIF gifs[NUM_DISPLAYS];

// Define CS pins for all displays
const int CS_PINS[NUM_DISPLAYS] = {19,22,21}; // Add more pins if you have more displays

// Define the images for all displays
const uint8_t *GIF_IMAGES[NUM_DISPLAYS] = {x_wing,darthvader,hud_6};      // Add more images if needed
const size_t GIF_SIZES[NUM_DISPLAYS] = {sizeof(x_wing),sizeof(darthvader),sizeof(hud_6)}; // Add more sizes if needed

TFT_eSPI tft;

void setup()
{
  Serial.begin(115200);

  tft.init();
  for (int i = 0; i < NUM_DISPLAYS; i++)
  {
    pinMode(CS_PINS[i], OUTPUT);
    digitalWrite(CS_PINS[i], HIGH); // Deselect the display
  }

  for (int i = 0; i < NUM_DISPLAYS; i++)
  {
    digitalWrite(CS_PINS[i], LOW); // Select the display
    tft.fillScreen(TFT_BLACK);
    tft.setRotation(2); // Adjust Rotation of your screen (0-3)
    gifs[i].begin(BIG_ENDIAN_PIXELS);
    if (!gifs[i].open((uint8_t *)GIF_IMAGES[i], GIF_SIZES[i], GIFDraw))
    {
      Serial.printf("Failed to open GIF%d\n", i + 1);
      Serial.printf("Last Error=%d", gifs[i].getLastError());
      Serial.printf("Image size=%d", sizeof(GIF_IMAGES[i]));
    }
    digitalWrite(CS_PINS[i], HIGH); // Deselect the display
  }
}

void loop()
{
  for (int i = 0; i < NUM_DISPLAYS; i++)
  {
    digitalWrite(CS_PINS[i], LOW); // Select the display

    tft.startWrite();

    int res = gifs[i].playFrame(false, NULL);
    if (res == 0)
    {
      // If no more frames are available, reset the GIF to the beginning
      gifs[i].reset();
      gifs[i].playFrame(false, NULL);
    }

    tft.endWrite();

    digitalWrite(CS_PINS[i], HIGH); // Deselect the display
  }
}
