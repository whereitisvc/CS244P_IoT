#include <Servo.h>
Servo myservo;
unsigned long currentMilli = 0;
unsigned long previousMilli1 = 0;
unsigned long previousMilli3 = 0;
unsigned long previousMilli5 = 0;
unsigned long previousMilli7 = 0;
unsigned long previousMilli10 = 0;
unsigned long previousMilli15 = 0;

const unsigned long period_1sec = 1000;
const unsigned long period_3sec = 3000;
const unsigned long period_5sec = 5000;
const unsigned long period_7sec = 7000;
const unsigned long period_10sec = 10000;
const unsigned long period_15sec = 15000;

int led_blink = 0;
int led_status = 0;
int light_calibration = 1;
int calibrarion_time_counter = 10;
int current_motor_degree = 0;
int previous_motor_degree = 0;
unsigned long light_value = 0;
unsigned long light_val_max = 0;
unsigned long light_val_min = 0;

void setup() {
  Serial.begin(9600);
  myservo.attach(3);
  pinMode(LED_BUILTIN, OUTPUT);
  if(light_calibration == 1) {
    Serial.print("Sart light calibration for 10 sec...\n");
    led_blink = 1;
  }
}

void loop() {
  currentMilli = millis();
  light_value = analogRead(0);

  // Light sensor calibration
  if(light_calibration == 1) {
    if(light_val_max < light_value) {
      light_val_max = light_value;
    }
    if(light_val_min > light_value) {
      light_val_min = light_value;
    }
  }
  if(currentMilli - previousMilli10 > period_10sec) {
    previousMilli10 = currentMilli;
    if(light_calibration == 1) {
      light_calibration = 0;
      led_blink = 0;
      Serial.print("Stop light calibration\n");
      Serial.print("Result:\n");
      Serial.print("Maximum_light value: ");
      Serial.print(light_val_max);
      Serial.print(", ");
      Serial.print("Minimun light value : ");
      Serial.print(light_val_min);
      Serial.print("\n");
    }
  }
  
  // blinking or rotate servo motor
  if(currentMilli - previousMilli1 > period_1sec) {
    previousMilli1 = currentMilli;
    
    if(led_blink == 1) {
      Serial.print(--calibrarion_time_counter);
      Serial.print(" sec...\n");
      if(led_status == 0) {
        digitalWrite(LED_BUILTIN, HIGH);
        led_status = 1;
      }else{
        digitalWrite(LED_BUILTIN, LOW);
        led_status = 0;
      }
    }

    if(light_calibration == 0) {
      current_motor_degree = map(light_value, light_val_min, light_val_max, 0, 179);
      
      if(previous_motor_degree != current_motor_degree) {
        previous_motor_degree = current_motor_degree;
        Serial.print("current motor degree: ");
        Serial.print(current_motor_degree);
        Serial.print("\n");
        myservo.write(current_motor_degree);
      }else{
        Serial.print("current motor degree: ");
        Serial.print(current_motor_degree);
        Serial.print("\n");
      }

    }
  }

  // milli() will overflow (go back to zero), 
  // after approximately 50 days.
  if((currentMilli - previousMilli1 < 0) 
    || (currentMilli - previousMilli3 < 0) 
    || (currentMilli - previousMilli7 < 0) 
    || (currentMilli - previousMilli15 < 0))
  {
    Serial.print("MILLI COUNTER RESET!");
    previousMilli1 = 0;
    previousMilli3 = 0;
    previousMilli7 = 0;
    previousMilli15 = 0;
  }
  
  if(currentMilli - previousMilli3 >= period_3sec) {
    previousMilli3 = currentMilli;
//    Serial.print("It's 3 seconds");
//    Serial.print(" (current milli: ");
//    Serial.print(previousMilli3);
//    Serial.print(")");
//    Serial.print("\n");
  }
  
  if(currentMilli - previousMilli7 >= period_7sec) {
    previousMilli7 = currentMilli;
//    Serial.print("It's 7 seconds");
//    Serial.print(" (current milli: ");
//    Serial.print(previousMilli7);
//    Serial.print(")");
//    Serial.print("\n");
  }
  
  if(currentMilli - previousMilli15 >= period_15sec) {
    previousMilli15 = currentMilli;
//    Serial.print("It's 15 seconds");
//    Serial.print(" (current milli: ");
//    Serial.print(previousMilli15);
//    Serial.print(")");
//    Serial.print("\n");
  }
}