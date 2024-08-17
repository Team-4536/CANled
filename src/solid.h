#pragma once

#include <vector>

#include "../generator.h"

class Solid : public Generator {
protected:
  Pixel color;
public:
  Solid(uint32_t, Pixel);
  Pixel get(uint32_t);
};


class Stack : public Solid {
private:
  std::vector<Generator*> generators;
public:
  Stack(uint32_t, Pixel = PIXEL_CLEAR);
  Pixel get(uint32_t);
  void Push(Generator*);
  void Pop();
};
