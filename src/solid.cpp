#include "solid.h"
#include <math.h>


Solid::Solid(uint16_t size, Pixel color) : Generator::Generator(size), color(color) {
}

Pixel Solid::get(uint16_t i = 0) {
  if (end <= start) {
    if (i < end || i >= start) {
      return color;
    }
  } else if (i >= start && i < end) {
    return color;
  }
  return PIXEL_IGNORE;
}

Stack::Stack(uint16_t size, Pixel color) : Solid::Solid(size, color) {
}

Pixel Stack::get(uint16_t i) {
  for (int j = generators.size() - 1; j >= 0; j--) {
    Generator *g = generators.at(j);
    Pixel c = g->get(mapToRange(j, g->getStart(), g->getEnd(), g->getWidth()));
    if (c.getAlpha() > 0) {
      return c;
    }
  } 
  return Solid::get(i);
}

bool Stack::next() {
  if (Solid::next()) {
    for (int i = generators.size() - 1; i >= 0; i--) {
      Generator *g = generators.at(i);
      g->next();
    }
    return true;
  } else {
    return false;
  }
}

void Stack::push(Generator *g) {
  generators.push_back(g);
}

void Stack::pop() {
  if (!generators.empty()) {
    generators.pop_back();
  }
}

void Stack::clear() {
  generators.clear();
}

Chase::Chase(uint16_t size, Pixel color, uint16_t segment_size) : Generator::Generator(size), color(color), segment_size(segment_size) {}

Pixel Chase::get(uint16_t i) {
  uint16_t w = getWidth();
  uint16_t s = bounce ? size : 0;
  uint16_t sel = local_time % (w - s);
  uint16_t j = mapToRange(i, sel + size, sel + size, w);
  uint16_t bi = w - j - 1;

  if (i < OUT_OF_RANGE) {
    if (!bounce || (local_time / (w - size)) % 2 >= 1) {
      if (0 <= j && j < size) {
        return color;
      }
    } else if (0 <= bi && bi < size) {
      return color;
    }
  }
  return PIXEL_IGNORE;
}

void Chase::setSegmentCount(uint16_t count) {
  this->segment_count = count;
}

void Chase::setDirection(bool direction) {
  this->forward_direction = direction;
}

void Chase::setBounce(bool bounce) {
  this->bounce = bounce;
}

void Chase::setFade(bool fade) {
  this->fade = fade;
}

Rainbow::Rainbow(uint16_t size) : Generator::Generator(size) {}

Pixel Rainbow::get(uint16_t i) {
  return Pixel(255, 55, 0, 55);
}

Gradient::Gradient(uint16_t size, Pixel color1, Pixel color2) : Generator::Generator(size), color1(color1), color2(color2) {}

Pixel Gradient::get(uint16_t i) {
  uint16_t wi = getWidth();
  uint16_t v = i / (wi - 1);
  
  uint8_t r = v * color1.getRed() + (1 - v) * color2.getRed();
  uint8_t g = v * color1.getGreen() + (1 - v) * color2.getGreen();
  uint8_t b = v * color1.getBlue() + (1 - v) * color2.getBlue();
  uint8_t w = v * color1.getWhite() + (1 - v) * color2.getWhite();
  Pixel color = Pixel(r, g, b, w);

  return color;
}

Pulse::Pulse(uint16_t size, Pixel color) : Generator::Generator(size), color(color) {}

Pixel Pulse::get(uint16_t i) {
  float sine = std::sin(i / 100) / 5;
  uint8_t r = sine * color.getRed(), g = sine * color.getGreen(), b = sine * color.getBlue(), w = sine * color.getWhite();
  Pixel color = Pixel(r, g, b, w);

  return color;
}
