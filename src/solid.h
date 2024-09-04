#pragma once

#include <vector>

#include "../generator.h"

class Stack : public Solid {
private:
  std::vector<Generator*> generators;
public:
  Stack(uint16_t, Pixel = PIXEL_CLEAR);
  Pixel get(uint16_t);
  bool next();
  void push(Generator*);
  void pop();
  void clear();
};

class Solid : public Generator {
protected:
  Pixel color;
public:
  Solid(uint16_t, Pixel);
  Pixel get(uint16_t);
};

class Chase : public Generator {
protected:
  Pixel color;
  bool bounce;
public:
  Chase(uint16_t, Pixel, bool = false);
  Pixel get(uint16_t);
};

class Gradient : public Generator {
protected:
  Pixel color1;
  Pixel color2;
public:
  Gradient(uint16_t, Pixel, Pixel);
  Pixel get(uint16_t);
};
