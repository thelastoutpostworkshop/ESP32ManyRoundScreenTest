// Animated GIF with Round Display
//
// ESP32 40MHz SPI single frame rendering performance
// Note: no DMA performance gain on smaller images or transparent pixel GIFs
//
#define USE_DMA      // ESP32 ~1.25x single frame rendering performance boost for badgers.h
                     // Note: Do not use SPI DMA if reading GIF images from SPI SD card on same bus as TFT
#define NORMAL_SPEED // Comment out for rame rate for render speed test

#include <SPI.h>
#include <TFT_eSPI.h>
#include "AnimatedGIF.h"

// Examples images
#include "images/hyperspace.h"
#include "images/nostromo.h"
#include "images/hud_1.h"
#include "images/hud_2.h"
#include "images/hud_5.h"
#include "images/hud_6.h"
#include "images/hud_7.h"
#include "images/darthvader.h"
#include "images/x_wing.h"

 // Adjust this value based on the number of displays
const int NUM_DISPLAYS = 6;                    
// Add more CS pins if you have more displays, each display must have a dedicated pin
const int CS_PINS[NUM_DISPLAYS] = {19, 22, 21,32,33,25}; 

AnimatedGIF gif_1;
AnimatedGIF gif_2;
AnimatedGIF gif_3;
AnimatedGIF gif_4;
AnimatedGIF gif_5;
AnimatedGIF gif_6;

TFT_eSPI tft = TFT_eSPI();

void setup()
{
  Serial.begin(115200);

  tft.init();
  for (int i = 0; i < NUM_DISPLAYS; i++)
  {
    pinMode(CS_PINS[i], OUTPUT);
    digitalWrite(CS_PINS[i], LOW); // select the display
    tft.fillScreen(TFT_BLACK);
    tft.setRotation(2);             // Adjust Rotation of your screen (0-3)
    digitalWrite(CS_PINS[i], HIGH); // Deselect the display
  }

  openGif(&gif_1, hyperspace, sizeof(hyperspace));
  openGif(&gif_2, hud_6, sizeof(hud_6));
  openGif(&gif_3, nostromo, sizeof(nostromo));
  openGif(&gif_4, x_wing, sizeof(x_wing));
  openGif(&gif_5, hud_2, sizeof(hud_2));
  openGif(&gif_6, hud_7, sizeof(hud_7));
}
void loop()
{
  playGif(&gif_1, 0);
  playGif(&gif_2, 1);
  playGif(&gif_3, 2);
  playGif(&gif_4, 3);
  playGif(&gif_5, 4);
  playGif(&gif_6, 5);
}

void openGif(AnimatedGIF *gif, const uint8_t *gifImage, int gifSize)
{
  gif->begin(BIG_ENDIAN_PIXELS);
  if (!gif->open((uint8_t *)gifImage, gifSize, GIFDraw))
  {
    Serial.printf("Could not open gif \n");
  }
}

void playGif(AnimatedGIF *gif, int screenIndex)
{
  digitalWrite(CS_PINS[screenIndex], LOW); // Select the display
  tft.startWrite();
  int res = gif->playFrame(false, NULL);
  if (res == 0)
  {
    // If no more frames are available, reset the GIF to the beginning
    gif->reset();
    gif->playFrame(false, NULL);
  }
  tft.endWrite();
  digitalWrite(CS_PINS[screenIndex], HIGH); // Deselect the display
}
