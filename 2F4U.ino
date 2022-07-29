#include <SoftwareSerial.h>
#include <GameBoy.h>
#include "Display.h"
#include "TetrisCar.h"
#include "Markup.h"
#include "Camera.h"

SoftwareSerial blue_tooth(4, 3);

GameBoy gb;
Display16x8* gb_display;

TetrisCar* player_car;
const int cars_amount{2};
TetrisCar* cars[cars_amount]{};
Markup* markup;
Camera* camera;

TimeUnit world_time_unit{};
unsigned long world_update_time{100};
unsigned long world_y{-1};

void setup() {
  Serial.begin(9600);
  blue_tooth.begin(9600);
  randomSeed(analogRead(A0));
  player_car = new TetrisCar(13, 0, true, true);
  gb_display = new Display16x8(&gb);
  markup = new Markup(gb_display->GetHeigth());
  camera = new Camera(player_car->GetConstPtrX(), TetrisCar::limit_x_min_, TetrisCar::limit_x_max_, 8, 1, 13);
  cars[0] = player_car;
  for(int i = 1; i < cars_amount; i++){
    int new_x = FindFreeXPlaceOtherwiseMinus1();
    cars[i] = new TetrisCar(new_x != -1 ? new_x : 1, world_y - 16, new_x >= 9, false );
  }
}

void loop() {
  static unsigned long now{}, last_time{};
  last_time = now;
  now = millis();

  ApplyControl();

  for(auto &car : cars){
    car->Tick(now);
    if(CheckCarIsLeaveDisplay(car)){
      car->Kill();
    }
    for(auto &checking_car : cars){
      if(car == checking_car){
        continue;
      }
     
      if( car->IsAlive() && checking_car->IsAlive() && car->HasCollisionWith(checking_car, GetPlayerY())){
        car->Kill();
        checking_car->Kill();
      }
    }

    if(!car->IsAlive()){
      if(car != player_car){
        int new_x = FindFreeXPlaceOtherwiseMinus1();
        if( new_x != -1 ){
          car->Restore(new_x, world_y - 16, new_x >= 9 );
        }
      }else{
        gb.testMatrix(5);
        StartNewGame();
        return;
      }
    }
  }
  
  camera->Tick(now);

  int markup_offset = - 2 + player_car->limit_x_min_ + camera->GetOffsetX();
  //Serial.println(String("carX= ")+String(player_car->GetX())+String(" camX= ")+ String(camera_offset_x) +String(" offs= ") +  String(markup_offset));
  markup->SetOffsetX( markup_offset );
  if(markup->Tick(now, player_car->GetMappedTickDelayFromSpeed(33, 1000))){
    Serial.println(player_car->GetMappedTickDelayFromSpeed(33, 1000));
    world_y--;
  }
  
  Draw(now);
}

void Draw(unsigned long now){
  if(gb_display->IsTickTime(now)){

    for(auto &car : cars){
      for(int i = 0; i < TetrisCar::sprite_imax_; i++){
        for(int j = 0; j < TetrisCar::sprite_jmax_; j++){
          if(car->GetIsSpriteOnIJ(i, j)){
            gb_display->SetPixel( j + car->GetX() + car->GetDrawOffsetX() + camera->GetOffsetX(), 
                                  i + car->GetY() -(car->freeze_y_ ? 0 : world_y) + car->GetDrawOffsetY() + camera->GetOffsetY(), 
                                  car->GetIsSpriteOnIJ(i, j));
          }
        }
      }
    }
    
    markup->Draw(&DrawPixelOnDisplay16x8);
            
    gb_display->Draw(); 
  }
}

void DrawPixelOnDisplay16x8(int x, int y, bool is_on){
  gb_display->SetPixel(x , y, is_on);
}

bool CheckCarIsLeaveDisplay(TetrisCar* car){
  if( car == player_car ){
    return false;
  }
  int y = car->GetY() - world_y + car->GetDrawOffsetY() + camera->GetOffsetY();
  return y < -8 || y > 16;
}

int FindFreeXPlaceOtherwiseMinus1(){
  bool is_place_free[4]{true, true, true, true};
  for(auto &car : cars){
    if( car == player_car || !car->IsAlive() ){
      continue;
    }
    is_place_free[ (car->GetX() - 1) / 4 ] = false;
  }
  int count{};
  for(int i = 0; i < 4; i++){
    if(is_place_free[i]){
      count++;
    }
  }
  if( count > 0 ){
    int free_place{};
    do{
      free_place = random(4);
    }while( !is_place_free[free_place] );
    return 1 + 4 * free_place;
  }
  return -1;
}

int GetPlayerY(){
  unsigned long dy{-1};
  return int(-(dy - world_y)) ;  
}
void StartNewGame(){
  world_y = -1;
  player_car->Restore(13, 0, true);
  for(int i = 1; i < cars_amount; i++){
    int new_x = FindFreeXPlaceOtherwiseMinus1();
    cars[i]->Restore(new_x != -1 ? new_x : 1, world_y - 16, new_x >= 9);
  }
  gb_display->Clear();
  markup->ResetTimer(millis());
}

void ApplyButtons(){
  int key = gb.getKey();
  player_car->SetSterlingWheel( key == 4 ? -1.0f : key == 5 ? 1.0f : 0.0f );
  player_car->SetGasPedal( key == 1 ? 1.0f : 0.0f );
  player_car->SetBrakePedal( key == 2 ? 1.0f : 0.0f );  
}

void ApplyBluetooth(){
  struct InputData{
    char control{' '};
    int force{};
  };
  InputData input_data{};
    
  if(blue_tooth.available()){
    input_data.control = blue_tooth.read();
    Serial.print(String("Blue: ")+String(input_data.control));
    if(input_data.control == 'G' || input_data.control == 'B' || input_data.control == 'T'){
      input_data.force = blue_tooth.parseInt();
      input_data.force = constrain( input_data.force, input_data.control == 'T' ? -100 : 0 , 100 );
      Serial.print(input_data.force);
      if(input_data.control == 'G'){
        player_car->SetGasPedal( static_cast<float>(input_data.force) / 100 );
      }else if(input_data.control == 'B'){
        player_car->SetBrakePedal( static_cast<float>(input_data.force) / 100 );
      }else if(input_data.control == 'T'){
        player_car->SetSterlingWheel( static_cast<float>(input_data.force) / 100 );
      }
    }
  }
}

void ApplyControl(){
  ApplyButtons();
  ApplyBluetooth();
}

  
