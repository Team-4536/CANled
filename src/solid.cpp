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

Chase::Chase(uint16_t size, Pixel color, bool bounce) : Generator::Generator(size), color(color), bounce(bounce) {}

Pixel Chase::get(uint16_t index) { // with the soon-to-be implemented mapToRange helper function, this will likely break
  uint16_t width = getWidth();
  uint16_t sel = local_time % width;
  uint16_t target;

  if (bounce && ((local_time / width) % 2 == 1)) {
    target = (start + width - sel) % size;
  } else {
    target = (start + sel) % size;
  }
  return target == index ? color : PIXEL_IGNORE;
}

Gradient::Gradient(uint16_t size, Pixel color1, Pixel color2) : Generator::Generator(size), color1(color1), color2(color2) {}

Pixel Gradient::get(uint16_t index) {
  start = mapToRange(start);
  end = mapToRange(end);
}
