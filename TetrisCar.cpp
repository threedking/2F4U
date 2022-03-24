#include "TetrisCar.h"

TetrisCar::TetrisCar(int new_x, int new_y, bool new_direction_up = false, bool new_freeze_y = false):x_(static_cast<float>(new_x)),y_(static_cast<float>(new_y)), y_start_(y_), direction_up_(new_direction_up), freeze_y_(new_freeze_y){
  this->time_unit_speed_.SetDelayTime(10);
  this->time_unit_turn_.SetDelayTime(50);
}

void TetrisCar::Tick(unsigned long now){
  if(!this->alive_){
    return;
  }
  if(this->time_unit_speed_.IsTickTime(now)){
    
    this->CalcAndApplySpeed();    

    if(!this->freeze_y_){
      this->time_unit_speed_.SetDelayTime(this->GetMappedTickDelayFromSpeed(33, 1000));
    }
  }
  if(this->time_unit_turn_.IsTickTime(now)){

    this->ApplyTurn();    
    
  }
}


void TetrisCar::Restore(int new_x, int new_y, bool new_direction_up = false){
  this->x_ = new_x;
  this->y_ = new_y;
  this->y_start_ = this->y_;
  this->direction_up_ = new_direction_up;
  this->alive_ = true;
}


void TetrisCar::Kill(){
  this->alive_ = false;
}

bool TetrisCar::HasCollisionWith(TetrisCar* checking_car, int player_y){
  int y_this{}, y_checking{};
  y_this = this->freeze_y_ ? player_y : this->GetY();
  y_checking = checking_car->freeze_y_ ? player_y : checking_car->GetY();

  return 
    ( this->GetX() - 1 <= checking_car->GetX() + 1 && this->GetX() - 1 >= checking_car->GetX() - 1 ||
    this->GetX() + 1 <= checking_car->GetX() + 1 && this->GetX() + 1 >= checking_car->GetX() - 1 )
    &&    
    ( y_this - 1 <= y_checking + 1 && y_this - 1 >= y_checking - 1 ||
    y_this + 1 <= y_checking + 1 && y_this + 1 >= y_checking - 1 );
}

void TetrisCar::CalcAndApplySpeed(){

  if(!this->freeze_y_){
    this->y_+= direction_up_ ? -1.0f : 1.0f;
  }

  this->speed_ = this->freeze_y_ ? this->gas_pedal_ > 0.5f ? 250 : 125 : 50;

  float abs_difference = this->y_ - this->y_start_;
  abs_difference *= abs_difference < - 1E-5 ? -1 : 1;
  
  if( abs_difference > distance_y_max_ + 1E-5 ){
    //this->y_ = this->y_start_;
    this->alive_ = false;
  }
}
float TetrisCar::GetSpeed(){
  return this->speed_ > 1E-5 ? this->speed_ < this->max_speed_ - 1E-5 ? this->speed_ : this->max_speed_ : 0.0f;
}

float TetrisCar::GetMappedSpeed(float range_min, float range_max){
  if( range_min > range_max - 1E-5 ||
  this->max_speed_ * (range_max - range_min) + range_min < 1E-5 ){
    return 0.0f;
  }
  return this->GetSpeed() / this->max_speed_ * (range_max - range_min) + range_min;
}
float TetrisCar::GetMappedTickDelayFromSpeed(float range_min, float range_max){
  float mapped_speed = this->GetMappedSpeed( 0, this->max_speed_ / 10);
  return mapped_speed > 1E-5 ? 1000 / mapped_speed : 0.0f;
}
void TetrisCar::ApplyTurn(){
    this->x_ += this->sterling_wheel_;
    if( this->x_ < static_cast<float>(limit_x_min_) - 1E-5 ){
      this->x_ = limit_x_min_;
    }
    if( this->x_ > static_cast<float>(limit_x_max_) + 1E-5 ){
      this->x_ = limit_x_max_;
    }
    this->sterling_wheel_ = 0.0f;
}
void TetrisCar::Draw(void (*DrawFunction)(int, int, bool)){
  for(int i = 0; i < TetrisCar::sprite_imax_; i++){
    for(int j = 0; j < TetrisCar::sprite_jmax_; j++){
      if(this->sprite_[i][j]){
        DrawFunction(this->x_ + j, this->y_ + i, this->sprite_[i][j]);
      }
    }
  }
}

bool TetrisCar::GetIsSpriteOnIJ(int i, int j){
  if(i < 0 || j < 0 || i >= TetrisCar::sprite_imax_ || j >= TetrisCar::sprite_jmax_){
    return false;
  }
  return this->sprite_[this->direction_up_ ? i : TetrisCar::sprite_imax_ - i - 1][j];
}

int TetrisCar::GetX(){
  return static_cast<int>(this->x_);
}
int TetrisCar::GetY(){
  return static_cast<int>(this->y_);  
}
const float* const TetrisCar::GetConstPtrX(){
  return &this->x_;
}
int TetrisCar::GetDrawOffsetX(){
  return this->draw_offset_x_;
}
int TetrisCar::GetDrawOffsetY(){
  return this->draw_offset_y_;
}
bool TetrisCar::IsAlive(){
  return this->alive_;
}

void TetrisCar::SetSterlingWheel(float new_sterling_wheel){
  if(new_sterling_wheel > -1 - 1E-5 && new_sterling_wheel < 0 - 1E-5 ||
     new_sterling_wheel > 0 + 1E-5 && new_sterling_wheel < 1 + 1E-5){
    this->sterling_wheel_ = new_sterling_wheel;
  }
}
void TetrisCar::SetGasPedal(float new_gas_pedal){
  if(new_gas_pedal > 0 - 1E-5 && new_gas_pedal < 1 + 1E-5){
    this->gas_pedal_ = new_gas_pedal;
  }
}
