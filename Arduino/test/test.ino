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
  frequency = 250; 
  delay_time = 1000.0/frequency/2; //ms

  if (!isRotationComplete1) {
    // Code for rotation 1
    digitalWrite(barm_dirPin, HIGH); // Set the rotation direction
    digitalWrite(farm_dirPin, HIGH);
    digitalWrite(claw_dirPin, HIGH);
    digitalWrite(plate_dirPin, HIGH);

    barm_degree = 1000;
    barm_steps = barm_degree / 360.0 * barm_StepPerRevolution; // Number of steps for a full rotation;
    for(int i = 0; i < barm_steps; i++){
      // digitalWrite(barm_stepPin, HIGH); // Send step pulse
      // digitalWrite(farm_stepPin, HIGH); // Send step pulse
      // digitalWrite(claw_stepPin, HIGH); // Send step pulse
      digitalWrite(plate_stepPin, HIGH); // Send step pulse
      delay(delay_time); // Time interval between pulses

      // digitalWrite(barm_stepPin, LOW); // End step pulse
      // digitalWrite(farm_stepPin, LOW); // End step pulse
      // digitalWrite(claw_stepPin, LOW); // End step pulse
      digitalWrite(plate_stepPin, LOW); // End step pulse
      delay(delay_time); // Time interval between pulses
    }
    isRotationComplete1 = true;
  }
}