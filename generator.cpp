#include <cmath>
#include <cstdint>

#include "generator.h"

Generator::Generator(uint16_t size) : size(size), start(0), end(0), duration(-1) {
  speed_numerator = 1;
  speed_denominator = 1;
};

uint16_t Generator::mapToRange(uint16_t i, uint16_t start, uint16_t end, uint16_t width) {
  if (i == OUT_OF_RANGE) {
    return OUT_OF_RANGE;
  } else if (
    start == end ||
    (start < end && start <= i && i < end) ||
    (start > end && (start <= i || i < end))
  ) {
    return (size + i - start) % width;
  } else {
    return OUT_OF_RANGE;
  }
}

Pixel Generator::HSVtoRGB(uint8_t H, uint8_t S, uint8_t V, uint8_t W = 0, uint8_t A = 0) {
  float S_norm = S / 255.0f;
  float V_norm = V / 255.0f;
  
  float C = V_norm * S_norm;
  float H_prime = fmod(H / 60.0f, 6.0f);
  float X = C * (1 - fabs(fmod(H_prime, 2.0f) - 1));
  float M = V_norm - C;

  float R, G, B;

  if (H_prime < 1) {
    R = C;
    G = X;
    B = 0;
  } else if (H_prime < 2) {
    R = X;
    G = C;
    B = 0;
  } else if (H_prime < 3) {
    R = 0;
    G = C;
    B = X;
  } else if (H_prime < 4) {
    R = 0;
    G = X;
    B = C;
  } else if (H_prime < 5) {
    R = X;
    G = 0;
    B = C;
  } else {
    R = C;
    G = 0;
    B = X;
  }

  R = (R + M) * 255;
  G = (G + M) * 255;
  B = (B + M) * 255;

  return Pixel(R, G, B, W, A);
}

void Generator::reset() {
  world_time = 0;
  local_time = 0;
}

bool Generator::next() {
  world_time++;
  int diff =
    ( (world_time    * speed_numerator) / speed_denominator) -
    (((world_time-1) * speed_numerator) / speed_denominator);

  local_time += diff;

  return (bool)diff;
}

Pixel Generator::get(uint16_t) { 
  return Pixel(0, 0, 0);
}

uint16_t Generator::getSize() {
  return size;
}

uint16_t Generator::getStart() {
  return start;
}

uint16_t Generator::getWidth() {
  if (start == end) {
    return size;
  } else if (start < end) {
    return end - start;
  } else {
    return size + end - start;
  }
}

uint16_t Generator::getEnd() {
  return end;
}

void Generator::setStart(uint16_t s) {
  this->start = s;
}

void Generator::setWidth(uint16_t w) {
  if (w > size) {
    w = size;
  }
  end = (start + w) % size;
}

void Generator::setEnd(uint16_t e) {
  this->end = e > size ? size : e;
}

void Generator::setDuration(float duration) {
  this->duration = duration;
}

void Generator::setSpeed(uint8_t numerator, uint16_t denominator = 255) {
  if (numerator > denominator) {
    numerator = denominator;
  }
  speed_numerator = numerator;
  speed_denominator = denominator;
}
