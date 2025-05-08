#define barm_dirPin 2 
#define barm_stepPin 3
#define farm_dirPin 4
#define farm_stepPin 5
#define claw_dirPin 7
#define claw_stepPin 6
#define plate_dirPin 8
#define plate_stepPin 9

bool isRotationComplete1 = false; // Used to control whether rotation 1 is completed
bool isRotationComplete2 = false; // Used to control whether rotation 2 is completed
bool isRotationComplete3 = false; // Used to control whether rotation 3 is completed

int barm_StepPerRevolution = 400 * 50 * 2;
int farm_StepPerRevolution = 2000 * 10 * 2;
int claw_StepPerRevolution = 4000 * 10;
int plate_StepPerRevolution = 400;

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
  float frequency, delay_time; // Frequency of the stepper motor
  int barm_degree, barm_steps;
  int farm_degree, farm_steps;
  int claw_degree, claw_steps;
  int plate_degree, plate_steps;
  frequency = 1000/4; 
  delay_time = 1000/frequency; //ms

  if (!isRotationComplete1) {
    // Code for rotation 1
    barm_degree = 15; // Degree of rotation for the back arm
    barm_steps = barm_degree / 360.0 * barm_StepPerRevolution; // Number of steps for a full rotation
    farm_degree = 15; // Degree of rotation for the forward arm
    farm_steps = farm_degree / 360.0 * farm_StepPerRevolution;

    digitalWrite(barm_dirPin, HIGH); // Set the rotation direction
    digitalWrite(farm_dirPin, HIGH); // Set the rotation direction

    tone(wave,frequency,t);
    
    for(int i = 0; i < barm_steps; i++){
      digitalWrite(barm_stepPin, HIGH); // Send step pulse
      digitalWrite(farm_stepPin, HIGH);
      delay(delay_time); // Time interval between pulses

      digitalWrite(barm_stepPin, LOW); // End step pulse
      digitalWrite(farm_stepPin, LOW);
      
      delay(delay_time); // Time interval between pulses
    }

    delay(2000);
    isRotationComplete1 = true; // Mark rotation 1 as complete
  }

  // Continuously rotate the plate while executing rotation 2
  while (!isRotationComplete2) {
    // Rotate the plate continuously
    digitalWrite(plate_dirPin, LOW); // Set the rotation direction
    digitalWrite(plate_stepPin, HIGH); // Send step pulse
    delay(delay_time); // Time interval between pulses
    digitalWrite(plate_stepPin, LOW); // End step pulse
    delay(delay_time); // Time interval between pulses
  }

  if (!isRotationComplete2) {
    // Code for rotation 2
    claw_degree = 30;
    claw_steps = claw_degree / 360.0 * claw_StepPerRevolution;
    
    digitalWrite(claw_dirPin, HIGH); // Set the rotation direction
    
    for(int i = 0; i < claw_steps; i++){
      digitalWrite(claw_stepPin, HIGH); // Send step pulse
      delay(delay_time); // Time interval between pulses

      digitalWrite(claw_stepPin, LOW); // End step pulse
      delay(delay_time); // Time interval between pulses
    }

    delay(5000);
    digitalWrite(claw_dirPin, LOW);
    
    for(int i = 0; i < claw_steps*2; i++){
      digitalWrite(claw_stepPin, HIGH); // Send step pulse
      delay(delay_time); // Time interval between pulses

      digitalWrite(claw_stepPin, LOW); // End step pulse
      delay(delay_time); // Time interval between pulses
    }

    delay(5000);
    digitalWrite(plate_stepPin, LOW);
    isRotationComplete2 = true; // Mark rotation 2 as complete
  }

  if (!isRotationComplete3) {
    // Code for rotation 3
    claw_degree = 30;
    claw_steps = claw_degree / 360.0 * claw_StepPerRevolution;
    barm_degree = 15; // Degree of rotation for the back arm
    barm_steps = barm_degree / 360.0 * barm_StepPerRevolution; // Number of steps for a full rotation
    farm_degree = 15; // Degree of rotation for the forward arm
    farm_steps = farm_degree / 360.0 * farm_StepPerRevolution;

    digitalWrite(claw_dirPin, HIGH); // Set the rotation direction
    digitalWrite(barm_dirPin, LOW); // Set the rotation direction
    digitalWrite(farm_dirPin, LOW); // Set the rotation direction
    
    for(int i = 0; i < claw_steps; i++){
      digitalWrite(claw_stepPin, HIGH); // Send step pulse
      delay(delay_time); // Time interval between pulses

      digitalWrite(claw_stepPin, LOW); // End step pulse
      delay(delay_time); // Time interval between pulses
    }

    delay(1000);

    for(int i = 0; i < barm_steps; i++){
      digitalWrite(barm_stepPin, HIGH); // Send step pulse
      digitalWrite(farm_stepPin, HIGH);
      delay(delay_time); // Time interval between pulses

      digitalWrite(barm_stepPin, LOW); // End step pulse
      digitalWrite(farm_stepPin, LOW);
      
      delay(delay_time); // Time interval between pulses
    }

    delay(2000);

    isRotationComplete3 = true; // Mark rotation 3 as complete
  }
}
