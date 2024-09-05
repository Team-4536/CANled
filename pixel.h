#pragma once

#include <cstdint>

class Pixel {
protected:
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  uint8_t white;
  uint8_t alpha;

public:
  Pixel(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t w = 0, uint8_t a = 255) {
    this->red = r;
    this->green = g;
    this->blue = b;
    this->white = w;
    this->alpha = a;
  }
  uint8_t getRed() {
    return red;
  }
  uint8_t getGreen() {
    return green;
  }
  uint8_t getBlue() {
    return blue;
  }
  uint8_t getWhite() {
    return white;
  }
  uint8_t getAlpha() {
    return alpha;
  }
};

const Pixel PIXEL_CLEAR = Pixel();
const Pixel PIXEL_IGNORE = Pixel(0,0,0,0,0);
const Pixel PIXEL_RED = Pixel(255);
const Pixel PIXEL_GREEN = Pixel(0,255);
const Pixel PIXEL_BLUE = Pixel(0,0,255);
