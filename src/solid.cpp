#include "solid.h"

Solid::Solid(uint32_t size, Pixel color) : Generator::Generator(size), color(color) {
}

Pixel Solid::get(uint32_t index = 0) {
  if (end <= start) {
    if (index < end || index >= start) {
      return color;
    }
  } else if (index >= start && index < end) {
    return color;
  }
  return PIXEL_IGNORE;
}

Stack::Stack(uint32_t size, Pixel color) : Solid::Solid(size,color) {
}

Pixel Stack::get(uint32_t index) {
  for (int i = generators.size()-1; i >= 0; i--) {
    Generator *g = generators.at(i);
    Pixel c = g->get(index);
    if (c.getAlpha() > 0) {
      return c;
    }
  }
    
  return Solid::get(index);
}

void Stack::Push(Generator *g) {
  generators.push_back(g);
}


