#include "Display.h"


Display16x8::Display16x8(GameBoy* new_GB):GB_(new_GB){
  if(this->GB_){
    this->GB_->begin(10);  
    this->Clear();
  }else{
    Serial.println("Display16x8.GB* bad");
  }
  this->SetFPS(this->fps_);
}

void Display16x8::Draw(){
  for(int i = 0; i < this->heigth_; i++){
    for(int j = 0; j < this->width_; j++){
      //Serial.print(this->new_world_[i][j]);
      //Serial.print(this->drawed_world_[i][j]);
      //Serial.print(" ");
      if(this->new_world_[i][j] != this->drawed_world_[i][j]){
        this->drawed_world_[i][j] = this->new_world_[i][j];
        if(this->drawed_world_[i][j]){
          this->GB_->drawPoint(j, i);    
        }else{
          this->GB_->wipePoint(j, i);   
        }
      }
      //Serial.print(this->new_world_[i][j] != this->drawed_world_[i][j]);
      
      this->new_world_[i][j] = false;
    }
    //Serial.println();
  }
}

void Display16x8::SetPixel(uint8_t x, uint8_t y, bool is_on){
  if(y < this->heigth_ && x < this->width_){
    this->new_world_[y][x] = is_on;
  }
}

void Display16x8::Clear(){  
  for(int i = 0; i < this->heigth_; i++){
    for(int j = 0; j < this->width_; j++){
      this->new_world_[i][j] = false;
      this->drawed_world_[i][j] = false;
    }    
  }
  this->GB_->clearDisplay();
}

void Display16x8::SetFPS(uint8_t new_fps){
  if(new_fps > 0){
    this->fps_ = new_fps;
    this->time_unit_.SetDelayTime(1000 / this->fps_);
  }
}

void Display16x8::Tick(unsigned long now){
  if(this->time_unit_.IsTickTime(now)){
    this->Draw();
  }
}

bool Display16x8::IsTickTime(unsigned long now){
  return this->time_unit_.IsTickTime(now);
}
int Display16x8::GetWidth(){
  return this->width_;
}
int Display16x8::GetHeigth(){
  return this->heigth_;
}
