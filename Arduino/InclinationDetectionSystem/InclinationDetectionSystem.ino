//Notes: L'accelerometre s'ha d'alimentar amb la sortida de tensió de 3.3V de la placa Arduino. Mai més de 3.6V. Grounds interconectats. 

  // analog in pins, they can read voltages. They connect to the Arduino ADC. 
  const int accelerationSensorPin = A0;
  const int potenciometerPin = A1;

  // digital pins. ON/OFF
  const int buttonInputPin = 2;
  const int redLedPin = 7; 
  const int greenLedPin = 8;
  const int motorPwmPin = 10;

  // state variables  
  int motorState = 0;
  int previousButtonState = 0;

  // ---------------- CALIBRATION PARAMETERS (to measure in lab) ----------------
  // These are placeholders until you do 2-point calibration
  float zeroGVoltage = 2.5;   // voltage when flat (≈1g on ADXL335 axis)
  float sensitivity = 0.3;    // V per g (typical value for ADXL335)
  // ---------------------------------------------------------------------------

void setup() {
  pinMode(buttonInputPin, INPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(motorPwmPin, OUTPUT);

  Serial.begin(9600);
}

void loop() {

  int rawValue = analogRead(accelerationSensorPin); // from 0 to 1023 aka 10bits resolution: 2^10

  // convert ADC → voltage (0–5V)
  float voltage = rawValue * (5.0 / 1023.0); 

  // CONVERT TO REAL ACCELERATION (g) 
  float acceleration_g = (voltage - zeroGVoltage) / sensitivity;

  // LED LOGIC (REAL UNITS) 
  if (acceleration_g < 0.9) {
    digitalWrite(redLedPin, HIGH);
    digitalWrite(greenLedPin, LOW);
  } 
  else {
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, HIGH);
  }

  // -------- BUTTON LOGIC --------
  int currentButtonState = digitalRead(buttonInputPin);

  if (currentButtonState == HIGH && previousButtonState == LOW) {
    motorState = !motorState;
    delay(50);
  }

  previousButtonState = currentButtonState;

  // -------- POTENTIOMETER → PWM --------
  int potentiometerValue = analogRead(potenciometerPin);
  int pwmValue = map(potentiometerValue, 0, 1023, 0, 255);

  if (motorState == 1) {
    analogWrite(motorPwmPin, pwmValue);
  } else {
    analogWrite(motorPwmPin, 0);
  }

  // -------- SERIAL OUTPUT (REAL PHYSICS VALUE) --------
  Serial.println(acceleration_g);

  delay(50);
}