#include <cstdint>
#include "generator.h"

Generator::Generator(uint16_t size) : size(size), end(size) {
  speed_numerator = 1;
  speed_denominator = 1;
};

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
  return Pixel(0,0,0);
}

uint16_t Generator::getSize() {
  return size;
}

uint16_t Generator::getStart() {
  return start;
}

uint16_t Generator::getWidth() {
  if (start > end) {
    return end + size - start;
  } else {
    return end - start;
  }
}

uint16_t Generator::getEnd() {
  return end;
}

void Generator::setStart(uint16_t start) {
  this->start = start;
}

void Generator::setWidth(uint16_t width) {
  if (width > size) {
    width = size;
  }
  end = (start + width) % size;
}

void Generator::setEnd(uint16_t end) {
  this->end = end > size ? size : end;
}

void Generator::setSpeed(uint8_t numerator, uint16_t denominator = 100) {
  if (numerator > denominator) {
    numerator = denominator;
  }
  speed_numerator = numerator;
  speed_denominator = denominator;
}
