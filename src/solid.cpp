#include "solid.h"

Solid::Solid(uint16_t size, Pixel color) : Generator::Generator(size), color(color) {
}

Pixel Solid::get(uint16_t index = 0) {
  if (end <= start) {
    if (index < end || index >= start) {
      return color;
    }
  } else if (index >= start && index < end) {
    return color;
  }
  return PIXEL_IGNORE;
}

Stack::Stack(uint16_t size, Pixel color) : Solid::Solid(size,color) {
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

void Stack::Push(Generator *g) {
  generators.push_back(g);
}

void Stack::Pop() {
  if (!generators.empty()) {
    generators.pop_back();
  }
}

Chase::Chase(uint16_t size, Pixel color) : Generator::Generator(size), color(color) {
}

Pixel Chase::get(uint16_t index) {
  // could do some precalc in next()
  uint16_t target = (start + (local_time % getWidth())) % size;

  return target == index ? color : PIXEL_IGNORE;
}

