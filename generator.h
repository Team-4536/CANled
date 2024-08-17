#pragma once

#include "pixel.h"

class Generator {
  uint32_t size; // size of the strip
protected:
  uint32_t start; // start index
  uint32_t end; // end index
  unsigned long tick;
public:
  Generator(uint32_t size);
  virtual uint32_t getSize();
  virtual uint32_t getStart();
  virtual uint32_t getEnd();

  virtual void setStart(uint32_t);
  virtual void setEnd(uint32_t);

  virtual void next();
  virtual void reset();
  virtual Pixel get(uint32_t index);
};
