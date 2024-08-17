#include <cstdint>
#include "generator.h"

Generator::Generator(uint32_t size) {
  this->size = size;
  this->end = size;
};

void Generator::reset() {
  tick = 0;
}

void Generator::next() {
  tick++;
}

Pixel Generator::get(uint32_t) {
  return Pixel(0,0,0);
}

uint32_t Generator::getSize() {
  return size;
}

uint32_t Generator::getStart() {
  return start;
}

uint32_t Generator::getEnd() {
  return end;
}

void Generator::setStart(uint32_t start) {
  this->start = start;
}

void Generator::setEnd(uint32_t end) {
  this->end = end;
}



