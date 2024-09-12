#pragma once

#include <vector>

#include "../generator.h"

class Solid : public Generator {
protected:
  Pixel color;
public:
  Solid(uint16_t size, Pixel color);
  Pixel get(uint16_t i);
};

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

class Chase : public Generator {
protected:
  Pixel color;
  uint16_t segment_size;
  uint16_t segment_count = 1;
  bool forward_direction = true;
  bool bounce = false;
  bool fade = false;
public:
  Chase(uint16_t size, Pixel color, uint16_t segment_size);

  Pixel get(uint16_t i);

  void setSegmentCount(uint16_t segment_count);
  void setDirection(bool forward);
  void setBounce(bool bounce);
  void setFade(bool fade);
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

class Pulse : public Generator {
protected:
  Pixel color;
public:
  Pulse(uint16_t size, Pixel color);
  Pixel get(uint16_t i);
};
