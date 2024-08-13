//These are the libraries used
#include "UserSettings.h"
#include <AccelStepper.h> //version 1.61

//This is the debugger setup. Notes on this are on the UserSettings.h tab
#if DEBUG == 1
        #define debug(x) Serial.print(x);
        #define debug_delay(x) delay(x);
        #define debugln(x) Serial.println(x)
#else
        #define debug(x)
        #define debug_delay(x)
        #define debugln(x)
#endif

//this assigns the pins to the accelStepper library
AccelStepper stepper(1, stepperPulsePin, stepperDIRPin); // Driver; PUL pin 6; DIR pin 9

//define the homing states and set to notHomed initially. Use the "error" as a kill status
const int notHomed = 0;
const int homed = 1;
const int error = 3;
const int pressedDuringHome = 4;
int homeStatus = notHomed;

//this is used so some of the debug readings dont run continiously
const int runSequence = 4;
int debugStatus = notHomed;

//--------------(Interrupt Functions)-----------------------//

//Interrupt function for the forward limit switch
void rearLimitHit (){

        //If 
        if(homeStatus != pressedDuringHome){    
                
                delay(20);//delay used as a debouce. To help reduce the chance of false readings 
                
                //Check again after the slight delay to see if the limit switch is still pressed
                if(digitalRead(rearLimitSwitchPin) == LOW){
                        //if it is homing and trigger the rear limit, set to homed
                        if(homeStatus == notHomed){
                                homeStatus = pressedDuringHome;
                                debugln("rear limit hit #2");
                        }
                        //if its already homed and triggers the rear limit, something is wrong, stop and require a reset.
                        if(homeStatus == homed){
                             digitalWrite(stepperEnablePin, HIGH);   
                             homeStatus = error;
                             debugln("rear limit hit #3");
                        }
                }
        }
  
}
//Interrupt function for the forward limit switch
void forwardLimitHit (){

        delay(20);//delay used as a debouce. To help reduce the chance of false readings 
        
        //Check again after the slight delay to see if the limit switch is still pressed
        if(digitalRead(forwardLimitSwitchPin) == LOW){
                    //this is set to "error" since this limit should never be hit. If it is, it will stop and require a reset of the Arduino. 
                    digitalWrite(stepperEnablePin, HIGH);
                    homeStatus = error;
                    debugln("forward limit hit");
        }
}
//------------------------(Setup )-----------------------------//
void customSetup() {
  
        //These turn on the serial monitor, prints to let the user know the device is running and that they need to turn on the debug feature to use it.
        Serial.begin(9600);
        Serial.println("");
        Serial.println("System ON, ensure you set DEBUG = 0 in the UserSettings.h tab to use the serail debug readouts");
        Serial.println("");
        
        delay(500);
        
        // Setup the limit switches as Input Pullups. 
        pinMode(forwardLimitSwitchPin, INPUT_PULLUP);
        pinMode(rearLimitSwitchPin, INPUT_PULLUP);
        
        // Attach interrupt pins to handlers (functions can be found on this page above)
        attachInterrupt(digitalPinToInterrupt(forwardLimitSwitchPin), forwardLimitHit, FALLING);
        attachInterrupt(digitalPinToInterrupt(rearLimitSwitchPin), rearLimitHit, FALLING);

        //AccelStepper setup stuff
        stepper.setEnablePin(stepperEnablePin); // assign which pin was used for the the Enable motor pin
        stepper.setPinsInverted(stepperDirectionSetting, false, driverEnableSignalRequirement); //Here I am telling the Accel Stepper Library that my motor's driver uses for it enable signal (high/low) and setting the direction
        //Set the max speed/acceleration
        stepper.setMaxSpeed(stepperMaxSpeed); // 60RPM = 1 rev/sec = microsteps setting/maxspeed
        stepper.setAcceleration(stepperHomeAcceleration); // (was 6400)1 rev to maxspeed = microsteps setting/2 = acceleration; 1 rev =microsteps setting/2 = moveto
        stepper.setSpeed(stepperHomeSpeed);
}
