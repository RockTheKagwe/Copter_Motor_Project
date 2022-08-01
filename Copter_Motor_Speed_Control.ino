/*
EEE310 : Control Systems 1: Copter Motor Project.

This is a Control Systems 1 project which will emulate an Aeropendulum.
The following components are used:
1. A2212 Copter Motor, and propellers.
2. JYQD V8.3 Brushless motor driver.
3. 10k potentiometer.
4. Arduino UNO R3 microcontroller board.
5. A 16*2 LCD to display progress(duty cycle, and angle subtended by the arm) of the system.

We use the potentiometer for reading the angle, whereas the microcontroller 
will be used to write voltages in PWM form to the motor driver for speed control.
*/

#include <LiquidCrystal_I2C.h>     //Import the I2C LCD library for use with the LCD.

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display

const int motorPin = 5;           //We connect motor to pin 5 since it can produce a PWM signal.
const int potPin = A1;            //We connect the potentiometer to an analog pin, A1, since the potentiometer is an analog device.

int potentiometer_value  = 0; //This value will be anything between 0 and 1023 depending on the displacement of the potentiometer from its initial position
int swing_angle = 0;          //This is the angle swept by the arm of the aeropendulum
int reference_angle = 90;     //This is the starting angle, with reference to the x-axis
int display_angle = 0;        //This is the angle to be displayed by the LCD. The angle θ as required in the question

int min_duty = 0;        //Minimum duty cycle in percentage
int max_duty = 100;      //Maximum duty cycle in percentage

int min_pwm_value = 0;   //Minimum pwm value.
int max_pwm_value = 130; //Maximum pwm value. Ideal maximum is 255. But in our case, writing a 255 as maximum was not possible since the setup would power off at 160. 

int pwm_value = 0;       //This value will be anything between the min and max pwm_value

void setup() 
{
  pinMode(motorPin, OUTPUT);  //Setting the PWM pin for the motor as an output pin
  pinMode(potPin, INPUT);     //Setting the pin for the potentiometer as an input pin

  //Initializing the LCD.
  lcd.begin();        
  lcd.backlight();      
}


//This is the function for lifting the motor from rest to maximum angle.
void increase_motor_speed()
{
  for (int duty_cycle = min_duty; duty_cycle <= max_duty; duty_cycle = duty_cycle+10)
  {    
    pwm_value = map(duty_cycle, min_duty, max_duty, min_pwm_value, max_pwm_value);
    analogWrite(motorPin, pwm_value); //Write the PWM value to the motor pin 
    
    lcd.clear();
    lcd.setCursor(2,0);   //Move cursor to character 2 on line 1
    lcd.print("Copter Motor");
    lcd.setCursor(3,1);   //Move cursor to character 3 on line 1
    lcd.print("Reading...");
    
    delay(3000);    //Wait for the system to settle before reading the values.
    
    potentiometer_value = analogRead(potPin);
  
    swing_angle=map(potentiometer_value, 0, 290, 0, 90); //Mapping the angle

    display_angle=reference_angle-(swing_angle);

    lcd.clear();
    lcd.setCursor(4,0);   //Move cursor to character 4 on line 0
    lcd.print("Duty: ");
    lcd.print(duty_cycle);
    lcd.setCursor(3,1);   //Move cursor to character 3 on line 1
    lcd.print("Angle: "); 
    lcd.print(display_angle);
    
    delay(3000);    //Allow enough time for reading the values displayed.
  }  
}

//This is the function for returning the motor back to the initial resting position.
void decrease_motor_speed()
{
  for (int duty_cycle = max_duty; duty_cycle >= min_duty; duty_cycle = duty_cycle-10)
  {    
    pwm_value = map(duty_cycle, min_duty, max_duty, min_pwm_value, max_pwm_value);
    analogWrite(motorPin, pwm_value); //Write the PWM value to the motor pin 
    
    lcd.clear();
    lcd.setCursor(2,0);   //Move cursor to character 2 on line 1
    lcd.print("Copter Motor");
    lcd.setCursor(3,1);   //Move cursor to character 3 on line 1
    lcd.print("Reading...");
    
    delay(3000);    //Wait for the system to settle before reading the values.
    
    potentiometer_value = analogRead(potPin);
  
    swing_angle=map(potentiometer_value, 0, 290, 0, 90);

    display_angle=reference_angle-swing_angle;

    lcd.clear();
    lcd.setCursor(4,0);   //Move cursor to character 4 on line 0
    lcd.print("Duty: ");
    lcd.print(duty_cycle);
    lcd.setCursor(3,1);   //Move cursor to character 3 on line 1
    lcd.print("Angle: "); 
    lcd.print(display_angle);
    
    delay(3000);    //Allow enough time for reading the values displayed.
  }
}

void loop()
{
  lcd.clear(); 
  
  // Print the opening message on both lines of the LCD.
  
  lcd.setCursor(1,0);   //Set cursor to character 2 on line 0
  lcd.print("Control System");
  
  lcd.setCursor(2,1);   //Move cursor to character 2 on line 1
  lcd.print("Copter Motor");
  delay(2000);

  //Calling the function to increase the speed of the motor to maximum
  increase_motor_speed();
  
  //Calling the function to decrease the speed of the motor to minimum
  decrease_motor_speed();
}
