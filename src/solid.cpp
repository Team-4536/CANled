#include "solid.h"

Stack::Stack(uint16_t size, Pixel color) : Solid::Solid(size, color) {
}

Pixel Stack::get(uint16_t index) {
  for (int i = generators.size()-1; i >= 0; i--) {
    Generator *g = generators.at(i);
    Pixel c = g->get(index);
    if (c.getAlpha() > 0) {
      return c;
    }
  } 
  return Solid::get(index);
}

bool Stack::next() {
  if (Solid::next()) {
    for (int i = generators.size()-1; i >= 0; i--) {
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

Chase::Chase(uint16_t size, Pixel color, bool bounce, bool fade) : Generator::Generator(size), color(color), bounce(bounce), fade(fade) {}

Pixel Chase::get(uint16_t index) { // implement fade
  uint16_t w = getWidth();
  uint16_t s = bounce ? size : 0;
  uint16_t sel = local_time % w - s;
  uint16_t i = mapToRange(index, sel + size, sel + size, w); // redo using i rather than index

  if (!bounce) {
    if (sel <= index && index < sel + size) {
      return color;
    } else if (index < sel + size - w) {
      return color;
    }
  } else  {
    uint16_t i = ((local_time / (w - size)) % 2 >= 1) ? index : w - index - 1;
    if (sel <= i && i < sel + size) {
      return color;
    }
  }
  return PIXEL_IGNORE;
}

Gradient::Gradient(uint16_t size, Pixel color1, Pixel color2) : Generator::Generator(size), color1(color1), color2(color2) {}

Pixel Gradient::get(uint16_t index) {
  uint16_t w = getWidth();
  uint16_t v = index / (w - 1);
  
  uint8_t r = v * color1.getRed() + (1 - v) * color2.getRed();
  uint8_t g = v * color1.getGreen() + (1 - v) * color2.getGreen();
  uint8_t b = v * color1.getBlue() + (1 - v) * color2.getBlue();
  uint8_t w = v * color1.getWhite() + (1 - v) * color2.getWhite();
  Pixel color = Pixel(r, g, b, w);

  return color;
}
