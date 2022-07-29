#ifndef TetrisCar_h
#define TetrisCar_h

#include <stdint.h>
#include <Arduino.h>

#include "TimeUnit.h"
//----------------
class TetrisCar{
public:
  inline static const uint8_t sprite_imax_{4}, sprite_jmax_{3};
  inline static const int limit_x_min_{1}, limit_x_max_{13};
  //inline static const int limit_y_min_{-30}, limit_y_max_{10};
  inline static const int distance_y_max_{20};
  
protected:
  inline static float max_speed_{300.0f};
  
  bool sprite_[sprite_imax_][sprite_jmax_]{
    {0,1,0},
    {1,1,1},
    {0,1,0},
    {1,0,1}
    }; 

  bool alive_{true};

  float x_{}, y_{}, y_start_{};
  
  float speed_{};  
  float acceleration_{};
  
  float gas_pedal_{}, brake_pedal_{};//0-1
  float weight_{};
  float engine_power_MAX_{}, engine_power_{};
  float z_koef_{}, brake_koef_{};  


  bool direction_up_{true};

  float sterling_wheel_{};//0-1

  int draw_offset_x_{-1}, draw_offset_y_{-1};

  TimeUnit time_unit_speed_{}, time_unit_turn_{};
  
  void CalcAndApplySpeed();
  void ApplyTurn();
public:
  const bool freeze_y_{false};

  TetrisCar(int new_x, int new_y, bool new_direction_up = false, bool new_freeze_y = false);
  virtual ~TetrisCar(){}
  
  void Tick(unsigned long now);
  void Restore(int new_x, int new_y, bool new_direction_up = false);
  bool HasCollisionWith(TetrisCar* checking_car, int player_y);
  float GetSpeed();
  float GetMappedSpeed(float range_min, float range_max);
  float GetMappedTickDelayFromSpeed(float range_min, float range_max);
  bool TetrisCar::IsExceededDistance();

  void Draw(void (*DrawFunction)(int, int, bool));
  bool GetIsSpriteOnIJ(int i, int j);
  int GetX();
  int GetY();
  const float* const GetConstPtrX();
  int GetDrawOffsetX();
  int GetDrawOffsetY();
  bool IsAlive();
  void Kill();

  void SetSterlingWheel(float new_sterling_wheel);
  void SetGasPedal(float new_gas_pedal);
  void SetBrakePedal(float new_brake_pedal);

  void SetLimitsX(int new_limit_min, int new_limit_max);
};
//----------------
#endif
