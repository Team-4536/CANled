#pragma once

#include <vector>

#include "../generator.h"

class Stack : public Solid {
private:
  std::vector<Generator*> generators;
public:
  Stack(uint16_t size, Pixel color = PIXEL_CLEAR);
  Pixel get(uint16_t i);
  bool next();
  void push(Generator *g);
  void pop();
  void clear();
};

class Solid : public Generator {
protected:
  Pixel color;
public:
  Solid(uint16_t size, Pixel color);
  Pixel get(uint16_t i);
};

class Chase : public Generator {
protected:
  Pixel color;
  uint16_t segment_size;
  uint16_t segment_count;
  bool bounce;
  bool fade;
public:
  Chase(uint16_t size, Pixel color, uint16_t segment_size, uint16_t segment_count, bool bounce = false, bool fade = false);
  Pixel get(uint16_t i);
};

class Rainbow : public Generator {
public:
  Rainbow(uint16_t size);
  Pixel get(uint16_t i);
};

class Gradient : public Generator {
protected:
  Pixel color1;
  Pixel color2;
public:
  Gradient(uint16_t size, Pixel color1, Pixel color2);
  Pixel get(uint16_t i);
};
