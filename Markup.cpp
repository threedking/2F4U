#include "Markup.h"

Markup::Markup(int new_display_high):display_high_(new_display_high){

}

bool Markup::Tick(unsigned long now, unsigned long world_update_time){
  if(this->time_unit_.IsTickTime(now)){

    this->MoveIt();
    
    this->time_unit_.SetDelayTime(world_update_time);
    
    return true;
  }
  return false;
}

void Markup::MoveIt(){
  this->line_top_pos_++;
  if(this->line_top_pos_ >= this->display_high_){
    this->line_top_pos_ = sprite_imax_ < this->display_high_ ? 0 : - sprite_imax_ + this->display_high_;
  }
}
  
void Markup::Draw(void (*DrawFunction)(int, int, bool)){
  for(int sprite_draw_times = 0; sprite_draw_times < ( sprite_imax_ < this->display_high_ ? this->display_high_ / this->sprite_imax_ : 1 ) + 1; sprite_draw_times++){
    int current_line_top = this->line_top_pos_ + sprite_draw_times * this->sprite_imax_;
    if(current_line_top >= this->display_high_){
      current_line_top = this->line_top_pos_ - (1 + (current_line_top - this->display_high_) / this->sprite_imax_) * this->sprite_imax_;
    }
    for(int i = 0; i < Markup::sprite_imax_; i++){
      for(int j = 0; j < Markup::sprite_jmax_; j++){
        if( this->sprite_[i][j] && 
            j + this->offset_x_ >= 0 && 
            j + this->offset_x_ < Markup::sprite_jmax_ ){
          DrawFunction(j + this->offset_x_, i + current_line_top, this->sprite_[i][j]);
        }
      }
    }
  }
}
void Markup::SetOffsetX(int new_offset_x){
  //Serial.println(new_offset_x);
  this->offset_x_ = new_offset_x;
}
void Markup::ResetTimer(unsigned long now){
  this->time_unit_.ForceTick(now);
}
