#ifndef Camera_h
#define Camera_h

#include <stdint.h>
#include <Arduino.h>

#include "TimeUnit.h"
//----------------
class Camera{
protected:

  int offset_x_{1}, offset_y_{13};
  int screen_width_{8};

  const float* pursued_x_{};
  int pursued_x_min_{}, pursued_x_max_{};

  TimeUnit time_unit_{};

public:
  Camera() = delete;
  Camera(const float* new_pursued_x, int new_pursued_x_min, int new_pursued_x_max, int new_screen_width,int new_offset_x, int new_offset_y);
  virtual ~Camera(){}
    
  void Tick(unsigned long now);
  int GetOffsetX();
  int GetOffsetY();
};
//----------------
#endif
