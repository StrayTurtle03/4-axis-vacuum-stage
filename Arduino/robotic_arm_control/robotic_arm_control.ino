#define barm_dirPin 2 
#define barm_stepPin 3
#define farm_dirPin 7
#define farm_stepPin 6
#define claw_dirPin 8
#define claw_stepPin 9
# define plate_dirPin 12
# define plate_stepPin 11
# define barm_StepPerRevolution 400*50*2
# define farm_StepPerRevolution 400*10*2
# define claw_StepPerRevolution 400*10
# define plate_StepPerRevolution 400

bool isRotationComplete1 = false; // Used to control whether rotation 1 is completed
bool isRotationComplete2 = false; // Used to control whether rotation 2 is completed
 // Used to control whether rotation 2 is completed
bool isRotationComplete3 = false;

void setup() {
  pinMode(claw_dirPin, OUTPUT);
  pinMode(claw_stepPin, OUTPUT);
  pinMode(barm_dirPin, OUTPUT);
  pinMode(barm_stepPin, OUTPUT);
  pinMode(farm_dirPin, OUTPUT);
  pinMode(farm_stepPin, OUTPUT);
  pinMode(plate_dirPin, OUTPUT);
  pinMode(plate_stepPin, OUTPUT);
}


void loop() {
  int frequency, delay_time; // Frequency of the stepper motor
  int barm_degree, barm_steps;
  int farm_degree, farm_steps;
  int claw_degree, claw_steps;
  int plate_degree, plate_steps;
  frequency = 500; 
  delay_time = 1000/frequency/2; //ms

  if (!isRotationComplete1) {
    // Code for rotation 1
    digitalWrite(barm_dirPin, HIGH); // Set the rotation direction
    digitalWrite(farm_dirPin, HIGH);

    barm_degree = 5;
    barm_steps = barm_degree / 360.0 * barm_StepPerRevolution; // Number of steps for a full rotation;
    for(int i = 0; i < barm_steps; i++){
      digitalWrite(barm_stepPin, HIGH); // Send step pulse
      delay(delay_time); // Time interval between pulses
      digitalWrite(barm_stepPin, LOW); // End step pulse
      delay(delay_time); // Time interval between pulses
    }


    barm_degree = 7; // Degree of rotation for the back arm
    farm_degree = 15; // Degree of rotation for the forward arm
    plate_degree = 720;

    barm_steps = barm_degree / 360.0 * barm_StepPerRevolution; // Number of steps for a full rotation
    farm_steps = farm_degree / 360.0 * farm_StepPerRevolution;
    plate_steps = plate_degree / 360.0 * plate_StepPerRevolution;

    for (int i = 0; i < max(barm_steps, farm_steps); i++) {
      if (i < barm_steps) digitalWrite(barm_stepPin, HIGH); // Back arm step pulse
      if (i < farm_steps) digitalWrite(farm_stepPin, HIGH); // Forward arm step pulse
      delay(delay_time); // Time interval between pulses
      
      if (i < barm_steps) digitalWrite(barm_stepPin, LOW); // End back arm step pulse
      if (i < farm_steps) digitalWrite(farm_stepPin, LOW); // End forward arm step pulse
      delay(delay_time); // Time interval between pulses
    }

    barm_degree = 5;
    farm_degree = 5;
    barm_steps = barm_degree / 360.0 * barm_StepPerRevolution; // Number of steps for a full rotation;
    farm_steps = farm_degree / 360.0 * farm_StepPerRevolution;
    for(int i = 0; i < barm_steps; i++){
      digitalWrite(barm_stepPin, HIGH); // Send step pulse
      if (i < farm_steps) digitalWrite(farm_stepPin, HIGH);
      delay(delay_time); // Time interval between pulses

      digitalWrite(barm_stepPin, LOW); // End step pulse
      if (i < farm_steps) digitalWrite(farm_stepPin, LOW);
      delay(delay_time); // Time interval between pulses
    }

    delay(1000);
    isRotationComplete1 = true; // Mark rotation 1 as complete
  }



  if (!isRotationComplete2) {
    // Code for rotation 2
    digitalWrite(plate_dirPin, HIGH);

    for(int i = 0; i < plate_steps; i++){ // Send step pulse
      digitalWrite(plate_stepPin, HIGH);
      delay(delay_time); // Time interval between pulses
      digitalWrite(plate_stepPin, LOW);
      delay(delay_time);
    }
    

    digitalWrite(claw_dirPin, HIGH); // down
    claw_degree = 15; //down 7 degree
    claw_steps = claw_degree / 360.0 * claw_StepPerRevolution;
    
    for(int i = 0; i < claw_steps; i++){
      digitalWrite(claw_stepPin, HIGH); // Send step pulse
      digitalWrite(plate_stepPin, HIGH);
      delay(delay_time); // Time interval between pulses

      digitalWrite(claw_stepPin, LOW); // End step pulse
      digitalWrite(plate_stepPin, LOW);
      delay(delay_time); // Time interval between pulses
    }

    delay(100); //for safety

    for(int i = 0; i < 2*plate_steps; i++){ // Send step pulse
      digitalWrite(plate_stepPin, HIGH);
      delay(delay_time); // Time interval between pulses
      digitalWrite(plate_stepPin, LOW);
      delay(delay_time);
    }

    delay(100);

    digitalWrite(claw_dirPin, LOW); //up
    digitalWrite(farm_dirPin, HIGH); //down
    
    farm_degree = 3; //

    farm_steps = farm_degree / 360.0 * farm_StepPerRevolution;

    for(int i = 0; i < claw_steps*2; i++){
      if(i < farm_steps) digitalWrite(farm_stepPin, HIGH); // Send step pulse
      digitalWrite(claw_stepPin, HIGH); // Send step pulse
      digitalWrite(plate_stepPin, HIGH);
      delay(delay_time); // Time interval between pulses

      if(i < farm_steps) digitalWrite(farm_stepPin, LOW); // End step pulse
      digitalWrite(claw_stepPin, LOW); // End step pulse
      digitalWrite(plate_stepPin, LOW);
      delay(delay_time); // Time interval between pulses
    }

    delay(100);

    for(int i = 0; i < 2*plate_steps; i++){ // Send step pulse
      digitalWrite(plate_stepPin, HIGH);
      delay(delay_time); // Time interval between pulses
      digitalWrite(plate_stepPin, LOW);
      delay(delay_time);
    }

    delay(100);

    digitalWrite(claw_dirPin, HIGH); //down
    digitalWrite(farm_dirPin, LOW); //up

    for(int i = 0; i < claw_steps; i++){
      if(i < farm_steps) digitalWrite(farm_stepPin, HIGH);
      digitalWrite(claw_stepPin, HIGH); // Send step pulse
      digitalWrite(plate_stepPin, HIGH);
      delay(delay_time); // Time interval between pulses

      if(i < farm_steps) digitalWrite(farm_stepPin, LOW);
      digitalWrite(claw_stepPin, LOW); // End step pulse
      digitalWrite(plate_stepPin, LOW);
      delay(delay_time); // Time interval between pulses
    }

    delay(1000);
    isRotationComplete2 = true; // Mark rotation 2 as complete
  }
  
  if (!isRotationComplete3) {
    // Code for rotation 3
    digitalWrite(barm_dirPin, LOW); // Set the rotation direction
    digitalWrite(farm_dirPin, LOW); // Set the rotation direction
  
    barm_degree = 5;
    barm_steps = barm_degree / 360.0 * barm_StepPerRevolution; // Number of steps for a full rotation;
    for(int i = 0; i < barm_steps; i++){
      digitalWrite(barm_stepPin, HIGH); // Send step pulse
      delay(delay_time); // Time interval between pulses
      digitalWrite(barm_stepPin, LOW); // End step pulse
      delay(delay_time); // Time interval between pulses
    }

    barm_degree = 7; // Degree of rotation for the back arm
    farm_degree = 10; // Degree of rotation for the forward arm
    plate_degree = 720;

    barm_steps = barm_degree / 360.0 * barm_StepPerRevolution; // Number of steps for a full rotation
    farm_steps = farm_degree / 360.0 * farm_StepPerRevolution;
    plate_steps = plate_degree / 360.0 * plate_StepPerRevolution;

    for (int i = 0; i < max(barm_steps, farm_steps); i++) {
      if (i < barm_steps) digitalWrite(barm_stepPin, HIGH); // Back arm step pulse
      if (i < farm_steps) digitalWrite(farm_stepPin, HIGH); // Forward arm step pulse
      delay(delay_time); // Time interval between pulses
      
      if (i < barm_steps) digitalWrite(barm_stepPin, LOW); // End back arm step pulse
      if (i < farm_steps) digitalWrite(farm_stepPin, LOW); // End forward arm step pulse
      delay(delay_time); // Time interval between pulses
    }

    barm_degree = 5;
    farm_degree = 10;
    barm_steps = barm_degree / 360.0 * barm_StepPerRevolution; // Number of steps for a full rotation;
    farm_steps = farm_degree / 360.0 * farm_StepPerRevolution;
    for(int i = 0; i < barm_steps; i++){
      digitalWrite(barm_stepPin, HIGH); // Send step pulse
      if (i < farm_steps) digitalWrite(farm_stepPin, HIGH);
      delay(delay_time); // Time interval between pulses
      
      digitalWrite(barm_stepPin, LOW); // End step pulse
      if (i < farm_steps) digitalWrite(farm_stepPin, LOW);
      delay(delay_time); // Time interval between pulses
    }

    delay(1000);
    isRotationComplete3 = 1;
  }
}