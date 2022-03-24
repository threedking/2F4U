#ifndef Markup_h
#define Markup_h

#include <stdint.h>
#include <Arduino.h>

#include "TimeUnit.h"
//----------------
class Markup{
public:
  inline static const uint8_t sprite_imax_{20}, sprite_jmax_{17};
protected:
  inline static float max_speed_{300.0f};  
  
  bool sprite_[sprite_imax_][sprite_jmax_]{
    {1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1},
    {1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
    {1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1},
    {1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
    {1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1},
    {1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
    {1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}
    };

  const int display_high_{16};
  int line_top_pos_{0};
  int offset_x_{0};
  bool direction_down_{true};
  
  TimeUnit time_unit_{};
  
  void Markup::MoveIt();
public:
  Markup() = delete;
  Markup(int new_display_high);
  virtual ~Markup(){}

  bool Tick(unsigned long now, unsigned long world_update_time);
  void Draw(void (*DrawFunction)(int, int, bool));
  void SetOffsetX(int new_offset_x);
  void ResetTimer(unsigned long now);
};
//----------------
#endif
