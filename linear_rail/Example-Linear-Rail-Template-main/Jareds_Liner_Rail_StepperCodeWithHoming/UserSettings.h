/* Notes: 
 *  1. To turn on the serial monitor readouts/debugger, set the "DEBUG" value to 1.
 *  2. It is best to turn off the debugger when not using it. Since the motor will run smoother without it on. 
 *  3. Make sure test the code without the linear rail coupling attached to ensure your directions, speeds, and switches all work prior to running with the lead screw attached. 
 *  
 *  This code is not intended for industrial or commercial use and purely for personal use. 
 *  User accepts all liability.
 *  
 *  Parts used for origional setkup: 
 *  1. Linear rail: https://www.amazon.com/gp/product/B0784G53D4/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1
 *  2. Stepper driver: https://www.amazon.com/gp/product/B06Y5VPSFN/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1
 *  3. Limit switches: https://www.amazon.com/gp/product/B073SP7SXS/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1
 *  4. Arduino UNO: https://www.amazon.com/gp/product/B016D5KOOC/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1
 *  5. 24V Power Supply: https://www.amazon.com/gp/product/B0196PXMTU/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1
 *  
 */

//Serial Debugger:
        #define DEBUG 0 // This turns the serial debug feature ON/OFF. 0 = OFF, 1 = ON. Essentially makes it easier to debug issues. Turning off can sometimes speed up processing, but should have much effect on yours, so Id leave on. 

//Driver Settings and Limit Switch Pins
        
        // Define which pins you use for the limit switches
        // Inportant: The code only works if the limit switches are connected to Interrupt pins. On the Uno/Nano, pins 2 & 3 are the pins to use.
        #define forwardLimitSwitchPin 2
        #define rearLimitSwitchPin    3
        
        //Define which pins you used to wire your stepper motor driver to the Arduino.
        #define stepperDIRPin    7
        #define stepperPulsePin  8
        #define stepperEnablePin 9
        
        bool driverEnableSignalRequirement = true;  //If your stepper motor is enabled pin requires a HIGH signal, change this to false. 
        bool stepperDirectionSetting       = false; //If your stepper heads in opposite direction than expected, you can change this value to reverse its directions without editing the main code. 
                                                    //The two options are "true" and "false" try each and see which works best for you. 

//MAIN DISTANCE SETTINGS

        float stepsToGetOffLimitSwitchDuringHoming = 600;    //This is used to back off of the rear limit during homing. Test real world and adjust to the offset that you want.
        float homeTravelDistance = 30000;                    //This sets a high number for homing the device. If the homing sequence doesnt reach the rear limit switch, increase this number.
        float runDistance = 5000;                          //This set the length of the run so we dont have to use the limit switches (and they can be used as stops). Test this and adjust until the desired travel distance is reached.
        
//MAIN SPEED SETTINGS
        
        float stepperHomeSpeed = 2000;                       //Homing speed. Higher # = faster homing
        float stepperHomeAcceleration = 1000;                //Home Acceleration
        
        float stepperForwardRunSpeed = 1000;                 //This sets the speed at which the stepper will push the weight at. larger# = faster
        float stepperForwardRunAcceleration = 2000;          //Sets the acceleration for the forward movement (pushing the weights) larger = faster
        
        float stepperRearRunSpeed = 8000;                    //This sets the speed at which the stepper will return the sled to the home position at. 
        float stepperRearRunAcceleration = 5000;             //Sets the acceleration for the rear movement back to home position. larger = faster

//SECONDARY SETTINGS (NOT LIKELY TO BE CHANGE)
        
        float stepperMaxSpeed = 10000;                       //This is for future use. Not super useful now. But if you ever set your speed higher than this, adjust this to match.
