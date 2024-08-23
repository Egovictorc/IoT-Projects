
#include <AccelStepper.h>

AccelStepper stepper(1, 17, 16); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

void setup()
{  
   stepper.setMaxSpeed(1000);
   stepper.setSpeed(100);	
   stepper.moveTo(500);
}

void loop()
{  
   stepper.run();
}
