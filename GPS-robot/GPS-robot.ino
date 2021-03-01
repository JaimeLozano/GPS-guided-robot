#include <Zumo32U4.h>
#include "TurnSensor.h"
#include "MagSensor.h"

Zumo32U4Motors motors;
Zumo32U4IMU imu;

int incomingByte = 0;
int speed = 100;
int32_t last_t;

int angulo_a_norte;
int angulo_a_objetivo;
int angulo_a_corregir;
int angulo_corregido;

void setup()
{
  Serial.begin(9600);
  turnSensorSetup();
  magSensorSetup();
  delay(1000);
  
  angulo_a_norte = 0;
  angulo_a_objetivo = 0;
}

void loop()
{
  if(Serial.available()){
    angulo_a_objetivo = Serial.parseInt();
    while(Serial.available()) Serial.read();
    angulo_a_corregir = angulo_a_norte - angulo_a_objetivo; 
  }
  
  if (angulo_corregido < angulo_a_corregir){
    while (angulo_corregido < angulo_a_corregir){
      motors.setSpeeds(-speed,speed);
      turnSensorUpdate();
      angulo_corregido = (((int32_t)turnAngle >> 16) * 360) >> 16;
    }
    motors.setSpeeds(0,0);
  }
  else if (angulo_corregido > angulo_a_corregir){
    while (angulo_corregido > angulo_a_corregir){
      motors.setSpeeds(speed,-speed);
      turnSensorUpdate();
      angulo_corregido = (((int32_t)turnAngle >> 16) * 360) >> 16;
    }
    motors.setSpeeds(0,0);
  }
}
