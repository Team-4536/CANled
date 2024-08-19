#pragma once

#include <vector>

#include "../generator.h"

class Solid : public Generator {
protected:
  Pixel color;
public:
  Solid(uint16_t, Pixel);
  Pixel get(uint16_t);
};

class Stack : public Solid {
private:
  std::vector<Generator*> generators;
public:
  Stack(uint16_t, Pixel = PIXEL_CLEAR);
  Pixel get(uint16_t);
  bool next();
  void Push(Generator*);
  void Pop();
};


class Chase : public Generator {
protected:
  Pixel color;
  bool bounce;
public:
  Chase(uint16_t, Pixel, bool = false);
  Pixel get(uint16_t);
};
