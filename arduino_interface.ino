/*******************************************************************************************************
DESCRIPTION:      This code is set up to act as an interface between the pi and other components.
                  This was done because I'm a noob and its easier for me to control these functions
                  using a microcontroller than it is an SBC with a whole os running. The code assumes 
                  that the ardiuno will always be powered, and I'm hoping power draw is low enough that
                  regular driving will maintain a charged battery. (this should probably change).
                  It controls the waveshare screen brightness by monitoring ambient light with an ldr
                  (gl5528) and dimming the screen at a certain ambient light threshold. 
                  It also monitors the status of the ignition via an optocoupler. changes state based
                  on ignition on/off. This is, in conjuction with a manual input, draws GPIO3 on the pi
                  low momentarily. This is intepreted by the pi as a wake/halt command, allowing for the
                  pi to turn on and off automatically and safely depending on ignition state.

EXTRA SETUP:      I connected the grounds of all the devices being interacted with to the arduino 
                  because as I understand it thats the right move.
                  The gl5528 ldr is powered by 5v on one leg, and has a voltage divider between the 
                  itself and the analog input on the arduino. Voltage divider uses a 10k ohm resistor
                  The optocoupler has 3.3v connected to nc, input pin connected to com, and ground 
                  connected to no.
                  Right now I'm planning on powering the arduino with the cars 12v supply going straight
                  to vin. This is likely a bad idea but its whats in my budget so its what I'm doing 
                  for now. 
*******************************************************************************************************/

const int optocouplerPin = 0;             // Pin connected to the optocoupler signal
const int raspberryPiControlPin = 2;      // Pin to control Raspberry Pi GPIO3
const int screenBrightnessPin = 6;        // PWM Pin to control screen brightness
const int buttonPin = 4;                  // Pin connected to the manual button (I just used a tactile button)
const int gl5528Pin = A1;                 // Analog pin connected to the GL5528 LDR, after volt divider (5v is dangerous as a potential input btw)

int lastOptoState = LOW;                  // Assuming optocoupler is de-energized initially
int optoStateChangedTime = 0;             // Time when optocoupler state last changed
int optoStateDuration = 3000;             // Duration for which optocoupler state should remain unchanged (in milliseconds)
int brightLevel = 64;                     // PWM for bright (lower simulated voltage/duty cycle)
int dimLevel = 150;                       // PWM for dimmed (higher voltage/duty cycle)
int threshold = 600;                      // threshold for brightness change based on ambient light reading (change as needed, i started with 600 as a good number)

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

  // set screen brightness based on ldr value vs thershold
  int brightness = gl5528Value > threshold ? brightLevel : dimLevel;

  analogWrite(screenBrightnessPin, brightness);

  // Check for optocoupler state change
  if (currentOptoState != lastOptoState) {
    // Optocoupler state change detected
    optoStateChangedTime = millis();  // store time of state change

    // check if optocoupler is holding state change
    if (millis() - optoStateChangedTime >= optoStateDuration) {
      // Send momentary pulse to the Raspberry Pi control pin
      digitalWrite(raspberryPiControlPin, LOW);
      delay(600);  // hold low for 600 milliseconds
      digitalWrite(raspberryPiControlPin, HIGH);
    }

    // Update last state change time
    optoStateChangedTime = millis();

    // Delay for debounce
    delay(50);
  }

  // Check the manual button state. a way to manually power cycle, just in case.
  if (buttonState == LOW) {
    // Button is pressed, send momentary pulse to the Raspberry Pi control pin
    digitalWrite(raspberryPiControlPin, LOW);
    delay(600);  // hold low for 600 milliseconds
    digitalWrite(raspberryPiControlPin, HIGH);
  }

  // Update the last state for the next iteration
  lastOptoState = currentOptoState;
}
