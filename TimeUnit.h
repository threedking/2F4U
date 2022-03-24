#ifndef TimeUnit_h
#define TimeUnit_h

//----------------
class TimeUnit{
protected:
  unsigned long delay_time_{100};
  unsigned long tick_time_{0};  
public:
  TimeUnit();
  TimeUnit(unsigned long new_delay_time);
  virtual ~TimeUnit(){}
    
  bool IsTickTime(unsigned long &now);
  void SetDelayTime(unsigned long new_delay_time);
  void ForceTick(unsigned long &now);
};
//----------------
#endif
