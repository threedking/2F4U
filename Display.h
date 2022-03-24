#ifndef Display_h
#define Display_h

#include <stdint.h>
#include <Arduino.h>
#include <GameBoy.h>


#include "TimeUnit.h"

class Display16x8{
  protected:
    static const int width_{8};
    static const int heigth_{16};
    
    bool new_world_[heigth_][width_];
    bool drawed_world_[heigth_][width_];

    uint8_t fps_{30};
    TimeUnit time_unit_;
    
    GameBoy* GB_;
  public:
    Display16x8(GameBoy* new_GB);
    virtual ~Display16x8(){}

    void SetPixel(uint8_t x, uint8_t y, bool is_on);
    void Draw();
    void Clear();
    void SetFPS(uint8_t new_fps);
    void Tick(unsigned long now);
    bool IsTickTime(unsigned long now);
    int GetWidth();
    int GetHeigth();
};

#endif
