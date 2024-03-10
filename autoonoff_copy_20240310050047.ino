const int optocouplerPin = 0;             // Pin connected to the optocoupler output
const int raspberryPiControlPin = 2;      // Pin to control Raspberry Pi
const int screenBrightnessPin = 6;        // PWM Pin to control screen brightness
const int buttonPin = 4;                  // Pin connected to the manual button
const int gl5528Pin = A1;                 // Analog pin connected to the GL5528 LDR

int lastOptoState = LOW;                  // Assuming optocoupler is de-energized initially
int optoStateChangedTime = 0;             // Time when optocoupler state last changed
int optoStateDuration = 3000;            // Duration for which optocoupler state should remain unchanged (in milliseconds)
int brightLevel = 64;                     // PWM value for approximately 75% brightness
int dimLevel = 150;                       // PWM value for approximately 25% brightness
int threshold = 600;                      // Fixed threshold for LDR

void setup() {
  pinMode(optocouplerPin, INPUT);
  pinMode(raspberryPiControlPin, OUTPUT);
  pinMode(screenBrightnessPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  // Initial state for the Raspberry Pi control pin (HIGH to start)
  digitalWrite(raspberryPiControlPin, HIGH);
}

void loop() {
  int currentOptoState = digitalRead(optocouplerPin);
  int buttonState = digitalRead(buttonPin);

  // Read the analog value from the GL5528 LDR
  int gl5528Value = analogRead(gl5528Pin);

  // Adjust screen brightness based on the GL5528 reading
  int brightness = gl5528Value > threshold ? brightLevel : dimLevel;

  analogWrite(screenBrightnessPin, brightness);

  // Check for state change of the optocoupler
  if (currentOptoState != lastOptoState) {
    // Optocoupler state change detected
    optoStateChangedTime = millis();  // Record the time of state change

    // Check if the optocoupler state has remained unchanged for a certain duration
    if (millis() - optoStateChangedTime >= optoStateDuration) {
      // Send momentary pulse to the Raspberry Pi control pin
      digitalWrite(raspberryPiControlPin, LOW);
      delay(600);  // Adjusted to 500 milliseconds
      digitalWrite(raspberryPiControlPin, HIGH);

      // Add any additional logic or commands if needed
    }

    // Update last state change time
    optoStateChangedTime = millis();

    // Delay for debounce (adjust as needed)
    delay(50);
  }

  // Check the manual button state
  if (buttonState == LOW) {
    // Button is pressed, send momentary pulse to the Raspberry Pi control pin
    digitalWrite(raspberryPiControlPin, LOW);
    delay(600);  // Adjusted to 500 milliseconds
    digitalWrite(raspberryPiControlPin, HIGH);

    // Add additional logic or commands as needed when the button is pressed
  }

  // Update the last state for the next iteration
  lastOptoState = currentOptoState;
}