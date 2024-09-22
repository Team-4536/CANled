#pragma once

#include "pixel.h"

#define OUT_OF_RANGE 0xffff

class Generator {
protected:
  uint16_t size;
  uint16_t start;
  uint16_t end;
  float duration;

  // uint16_t mapToRange(uint16_t i, uint16_t start, uint16_t end, uint16_t width);
  Pixel HSVtoRGB(uint8_t H, uint8_t S, uint8_t V, uint8_t W, uint8_t A);

  unsigned long world_time;
  uint8_t speed_numerator;
  uint16_t speed_denominator;
  unsigned long local_time;
public:
  Generator(uint16_t size);
  virtual uint16_t getSize();

  uint16_t getStart();
  uint16_t getWidth();
  uint16_t getEnd();

  void setStart(uint16_t start);
  void setWidth(uint16_t width);
  void setEnd(uint16_t end);
  void setDuration(float duration);


  uint16_t mapToRange(uint16_t i, uint16_t start, uint16_t end, uint16_t width);


  virtual void setSpeed(uint8_t numerator, uint16_t denominator);

  virtual bool next();
  virtual void reset();
  virtual Pixel get(uint16_t index);
};
