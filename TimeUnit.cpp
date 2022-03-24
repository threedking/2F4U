#include "TimeUnit.h"

TimeUnit::TimeUnit(){
}
TimeUnit::TimeUnit(unsigned long new_delay_time){
  this->SetDelayTime(new_delay_time);
}
    
bool TimeUnit::IsTickTime(unsigned long &now){
  if( this->tick_time_ <= now ){
    this->tick_time_ += this->delay_time_;
    return true;
  }
  return false;
}
void TimeUnit::SetDelayTime(unsigned long new_delay_time){
  this->delay_time_ = new_delay_time;
}
void TimeUnit::ForceTick(unsigned long &now){
  do{
    this->tick_time_ += this->delay_time_;
  }while( this->tick_time_ <= now );
}
