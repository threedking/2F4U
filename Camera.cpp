#include "Camera.h"

Camera::Camera(const float* new_pursued_x, int new_pursued_x_min, int new_pursued_x_max, int new_screen_width, int new_offset_x, int new_offset_y)
  :time_unit_(75), pursued_x_(new_pursued_x), pursued_x_min_(new_pursued_x_min), pursued_x_max_(new_pursued_x_max), screen_width_(new_screen_width), offset_x_(new_offset_x), offset_y_(new_offset_y){

}
    
void Camera::Tick(unsigned long now){
  //Serial.println(String("carX= ") + String(*this->pursued_x_) + String("camX= ") + String(this->offset_x_));
  if( static_cast<int>(*this->pursued_x_) + this->offset_x_ < 1 ){
    this->offset_x_ = 1 - static_cast<int>(*this->pursued_x_);
    this->time_unit_.ForceTick(now);
    return;
  }
  if( static_cast<int>(*this->pursued_x_) + this->offset_x_ > this->screen_width_ - 2 ){
    this->offset_x_ = this->screen_width_ - 2 - static_cast<int>(*this->pursued_x_);
    this->time_unit_.ForceTick(now);
    return;
  }
  if(this->time_unit_.IsTickTime(now)){      
    if( static_cast<int>(*this->pursued_x_) + this->offset_x_ < 3 &&
        static_cast<int>(*this->pursued_x_) >= this->pursued_x_min_ + 1 ||
        static_cast<int>(*this->pursued_x_) + this->offset_x_ < 2){
      this->offset_x_++;
    }
    
    if( static_cast<int>(*this->pursued_x_) + this->offset_x_ > this->screen_width_ - 4 &&
        static_cast<int>(*this->pursued_x_) <= this->pursued_x_max_ - 1 ||
        static_cast<int>(*this->pursued_x_) + this->offset_x_ > this->screen_width_ - 3){
      this->offset_x_--;
    }
  }
}
int Camera::GetOffsetX(){
  return this->offset_x_;
}
int Camera::GetOffsetY(){
  return this->offset_y_;
}
