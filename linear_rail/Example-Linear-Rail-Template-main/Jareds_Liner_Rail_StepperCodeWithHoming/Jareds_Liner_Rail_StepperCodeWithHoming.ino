#include "Setup.h"

void setup() {
        customSetup(); //See the Setup.h tabe for this function
}


//--------------------(Main Code)---------------------------//
void loop() {

        //if the system has not been homed, home system and save the home position
        if(homeStatus == notHomed  || homeStatus == pressedDuringHome){
                if(homeStatus == notHomed){
                        debugln("Homing system, stay clear of machine");
                        digitalWrite(stepperEnablePin, LOW);//Enable the motor
                        
                        //run the stepper reverse until the rear limit switch is reached
                        stepper.moveTo(homeTravelDistance);
                        stepper.setMaxSpeed(stepperHomeSpeed);
                        while(homeStatus == notHomed){
                                stepper.run();
                        }
                        //set position as 0
                        stepper.setCurrentPosition(0);
                }
                if(homeStatus == pressedDuringHome){  
                        debugln("Rear limit hit during homing, backing off switch");
                        digitalWrite(stepperEnablePin, LOW);//Enable the motor
                        
                        //run the stepper reverse until the rear limit switch is reached
                        stepper.moveTo(-stepsToGetOffLimitSwitchDuringHoming);
                        debugln("susped");
                        stepper.runToPosition();
                        debugln("@");
                        debugln("done backing off limit, ready to enter run");
                        //set position as 0
                        stepper.setCurrentPosition(0);
                        homeStatus = homed;       
                }

        }

        //while homingStatus == homed, continiously run back-n-forth between the limits
        while(homeStatus == homed){
              
                //On the first run, write to the serial monitor so we know
                if(debugStatus != runSequence){
                       debugln("Entered run sequence for the first time");   
                       debugStatus = runSequence;
                }

                
                //set the stepper speed and acceleration to the forward settings
                stepper.moveTo(-runDistance);
                stepper.setMaxSpeed(stepperForwardRunSpeed);
                stepper.setAcceleration(stepperForwardRunAcceleration);
                  
                //run stepper until the runDistance has been traveled (UNLESS either limit is hit)
                while (stepper.distanceToGo() != 0 & homeStatus == homed){
                        stepper.run();
                }

                //this stops the motor as quickly as possible while using accel+decell (May take trial and error during setup to find a setting that doesnt hit the limit switch)
                stepper.stop(); // Stop as fast as possible: sets new target
                stepper.runToPosition(); 
               
                //Write to the serial monitor (see DEBUG.h tab)
                debugln("Forward movement complete, reversing");

                // Now, tell the stepper to reverse back to the home position
                stepper.moveTo(0);
                stepper.setMaxSpeed(stepperRearRunSpeed);
                stepper.setAcceleration(stepperRearRunAcceleration);

                // Full speed basck to 0 (UNLESS either limit switch is hit)
                while (stepper.distanceToGo() != 0 & homeStatus == homed){ 
                        stepper.run();
                }
                               
                //this stops the motor as quickly as possible while using accel+decell
                stepper.stop(); // Stop as fast as possible: sets new target
                stepper.runToPosition();               

                //write to the serial monitor 
                debugln("Reverse movement complete, pausing for a second, then sending forward");

        }

        //if an error occurs, disable the motor and wait for operator to check/reset
        while(homeStatus == error){
              debugln("An error has occured, check machine and reset when ready");
              digitalWrite(12, HIGH);//disable the motor
              delay(5000);
        }
       
}
