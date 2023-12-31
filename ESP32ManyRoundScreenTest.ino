// Animated GIF with Round Display
//


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
#include "images/bb8.h"

// Adjust this value based on the number of displays
const int NUM_DISPLAYS = 6;
// Add more CS pins if you have more displays, each display must have a dedicated pin
const int CS_PINS[NUM_DISPLAYS] = { 19, 22, 21, 32, 33, 26 };
int currentScreenIndex = 0;

AnimatedGIF gif_1;
AnimatedGIF gif_2;
AnimatedGIF gif_3;
AnimatedGIF gif_4;
AnimatedGIF gif_5;
AnimatedGIF gif_6;


TFT_eSPI tft = TFT_eSPI();

unsigned long lastFrameSpeed = 0;

void setup() {
  Serial.begin(115200);

  tft.init();
  for (int i = 0; i < NUM_DISPLAYS; i++) {
    pinMode(CS_PINS[i], OUTPUT);
    digitalWrite(CS_PINS[i], LOW);  // select the display
    tft.fillScreen(TFT_BLACK);
    tft.setRotation(2);              // Adjust Rotation of your screen (0-3)
    digitalWrite(CS_PINS[i], HIGH);  // Deselect the display
  }
  openGif(&gif_1, hyperspace, sizeof(hyperspace));
  openGif(&gif_2, hud_6, sizeof(hud_6));
  openGif(&gif_3, nostromo, sizeof(nostromo));
  openGif(&gif_4, x_wing, sizeof(x_wing));
  openGif(&gif_5, hud_2, sizeof(hud_2));
  openGif(&gif_6, bb8, sizeof(bb8));
}
void loop() {
  playGif(&gif_1, 0);
  playGif(&gif_2, 1);
  playGif(&gif_3, 2);
  playGif(&gif_4, 3);
  playGif(&gif_5, 4);
  playGif(&gif_6, 5);
}

void openGif(AnimatedGIF *gif, const uint8_t *gifImage, int gifSize) {
  gif->begin(BIG_ENDIAN_PIXELS);
  if (!gif->open((uint8_t *)gifImage, gifSize, GIFDraw)) {
    Serial.printf("Could not open gif \n");
  }
}

void playGif(AnimatedGIF *gif, int screenIndex) {
  currentScreenIndex = screenIndex;
  int res = gif->playFrame(false, NULL);
  if (res == 0) {
    // If no more frames are available, reset the GIF to the beginning
    gif->reset();
    gif->playFrame(false, NULL);
  }
  if (res == -1) {
    Serial.printf("Gif Error = %d on screen %d\n", gif->getLastError(), screenIndex);
  }


  if (screenIndex == 0) {
    if (lastFrameSpeed == 0) {
      lastFrameSpeed = millis();
    } else {
      Serial.printf("Screen 0 FPS=%f\n", 1000.0f / (millis() - lastFrameSpeed));
      lastFrameSpeed = millis();
    }
  }
}
