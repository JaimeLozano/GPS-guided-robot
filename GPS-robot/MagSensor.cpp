#include "MagSensor.h"
#include "TurnSensor.h"
#include <Zumo32U4.h>
#include <Wire.h>
#include <LIS3MDL.h>

LIS3MDL mag;
Zumo32U4LCD lcd;
extern Zumo32U4Motors motors;


int mx_max, mx_min, my_max, my_min;


void magSensorSetup(){
  float mx,my;
  
  Wire.begin();
  mag.init();
  mag.enableDefault();
  
  lcd.clear();
  lcd.gotoXY(0,0);
  lcd.print(F("Mag cal"));
  
  mag.read();
  mx_max = mag.m.x;
  my_max = mag.m.y;
  mx_min = mag.m.x;
  my_min = mag.m.y;

  while (angulo_corregido != -180){
    motors.setSpeeds(-speed,speed);
    mag.read();
    mx = mag.m.x;
    my = mag.m.y;
    angulo_corregido = (((int32_t)turnAngle >> 16) * 360) >> 16;
    if (mx > mx_max){
      mx_max = mx;
    }
    if (my > my_max){
      my_max = my;
    }
    if (mx < mx_min){
      mx_min = mx;
    }
    if (my < my_min){
      my_min = my;
    }
    turnSensorUpdate();
  }
  while (angulo_corregido != 0){
    motors.setSpeeds(-speed,speed);
    turnSensorUpdate();
    mag.read();
    mx = mag.m.x;
    my = mag.m.y;
    angulo_corregido = (((int32_t)turnAngle >> 16) * 360) >> 16;
    if (mx > mx_max){
      mx_max = mx;
    }
    if (my > my_max){
      my_max = my;
    }
    if (mx < mx_min){
      mx_min = mx;
    }
    if (my < my_min){
      my_min = my;
    }
  }
  motors.setSpeeds(0,0);
  lcd.clear();
}

void magSensorUpdate(){
  float mx,my;
  mag.read();
  mx = mag.m.x;
  my = mag.m.y;
  mx = map(mag.m.x,mx_min,mx_max,32767,-32768);
  my = map(mag.m.y,my_min,my_max,32767,-32768);
  angulo_a_norte = atan2(my,mx)*180/(2*PI);
  angulo_a_norte = map(angulo_a_norte,-90,90,180,-180);
}
