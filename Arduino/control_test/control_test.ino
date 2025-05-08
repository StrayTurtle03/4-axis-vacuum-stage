#include <AccelStepper.h>
#include <MultiStepper.h>

#define barm_dirPin 2 
#define barm_stepPin 3
#define farm_dirPin 4
#define farm_stepPin 5
#define claw_dirPin 7
#define claw_stepPin 6
# define plate_dirPin 8
# define plate_stepPin 9
# define barm_StepPerRevolution 400*50*2
# define farm_StepPerRevolution 400*10*2
# define claw_StepPerRevolution 400*10
# define plate_StepPerRevolution 400

AccelStepper barm(1, barm_stepPin, barm_dirPin);
AccelStepper farm(1, farm_stepPin, farm_dirPin);
AccelStepper claw(1, claw_stepPin, claw_dirPin);
AccelStepper plate(1, plate_stepPin, plate_dirPin);
MultiStepper group;

bool flag1 = 1, flag2 = 0, flag3 = 0, flag4 = 0;

void setup() {
    //usb.start();

    barm.setMaxSpeed(500.0);   
    barm.setAcceleration(500.0);  
    
    farm.setMaxSpeed(500.0);    
    farm.setAcceleration(500.0);  

    claw.setMaxSpeed(200.0);    
    claw.setAcceleration(500.0);  

    plate.setMaxSpeed(500.0);  
    plate.setAcceleration(100.0); 

    group.addStepper(barm);
    group.addStepper(farm);
    group.addStepper(claw);
    group.addStepper(plate);
    
    //farm.moveTo(512);

    //back to home position
    //barm.moveTo(0);
}

void loop() {
    int barm_degree = 0; //-back, +forward
    int farm_degree = 0; //-up, +down
    int claw_degree = 0; //-up, +down 
    if(flag1) {
        barm_degree = 17;
        farm_degree = 20;
        claw_degree = -3;
        barm.move(barm_degree*5*2048.0/90);
        farm.move(farm_degree*2048.0/90);
        claw.move(claw_degree*1024.0/90);
        plate.move(4096);
        barm.setSpeed(500.0);
        farm.setSpeed(100.0);
        //claw.setSpeed(50.0);
        plate.setSpeed(200.0);
        flag1 = 0;
        flag2 = 1;
    }
    group.runSpeedToPosition();

    if(flag2) {
        farm_degree = -3;
        claw_degree = 15;        
        //barm.move(barm_degree*5*2048.0/90);
        farm.move(farm_degree*2048.0/90);
        claw.move(claw_degree*1024.0/90);
        plate.move(4096);
        barm.setSpeed(500.0);
        farm.setSpeed(100.0);
        //claw.setSpeed(50.0);
        plate.setSpeed(200.0);
        flag2 = 0;
        flag3 = 1;
    }
    group.runSpeedToPosition();

    if(flag3) {
        farm_degree = -3;
        claw_degree = 5;        
        //barm.move(barm_degree*5*2048.0/90);
        farm.move(farm_degree*2048.0/90);
        claw.move(claw_degree*1024.0/90);
        plate.move(4096);
        // barm.setSpeed(500.0);
        // farm.setSpeed(100.0);
        //claw.setSpeed(50.0);
        plate.setSpeed(200.0);
        flag3 = 0;
        flag4 = 1;
    }
    group.runSpeedToPosition();

    if(flag4) {
        barm_degree = -17;
        farm_degree = -14;
        claw_degree = -17;
        barm.move(barm_degree*5*2048.0/90);
        farm.move(farm_degree*2048.0/90);
        claw.move(claw_degree*1024.0/90);
        //plate.move(4096);
        // barm.setSpeed(500.0);
        // farm.setSpeed(100.0);
        //claw.setSpeed(50.0);
        plate.setSpeed(200.0);
        flag4 = 0;
    }
    group.runSpeedToPosition();
    
}
