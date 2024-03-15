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

INPUTS:           GL5528 LDR analog input goes to pin a1, after 10kohm voltage divider
                  OPTOCOUPLER com line goes to digital pin 0. NC and NO are wired to drive the com
                  pin high and low
                  PI STATUS reads 3.3v state on digital pin 1 (WIP)
                  BUTTON PIN digital input on pin 4, connects to momentary tactile button. Manual trigger
                  for output signal on pin 2
                  
OUTPUTS:          PI ON PIN digital output on pi 2 to wake and halt the pi. wake only is (WIP)
                  PI OFF PIN digital output on pin 3 to halt the pi (WIP)
                  SCREEN BRIGHTNESS PWM output on pin 6 to set brightness on screen

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

const int optocouplerPin = 0;             // Pin connected to the optocoupler output
const int piStatus = 1;                   // to read the 3.3v line on the pi that is powered when the pi is awake
const int piOnPin = 2;                    // Pin to control Raspberry Pi
const int piOffPin = 3;                   // pin to be used for only power off with custom dtoverlay=gpio-shutdown pin = x. maybe set to gpio5?
const int buttonPin = 4;                  // Pin connected to the manual button
const int screenBrightnessPin = 6;        // PWM Pin to control screen brightness
const int gl5528Pin = A1;                 // Analog pin connected to the GL5528 LDR

int lastOptoState = LOW;                  // Assuming optocoupler is de-energized initially
int brightLevel = 64;                     // PWM value for approximately 75% brightness
int dimLevel = 150;                       // PWM value for approximately 25% brightness
int threshold = 600;                      // Fixed threshold for LDR
long optoStateChangedTime = 0;            // Time when optocoupler state last changed
bool signalSent = true;                   // stores if the signal has been sent per state change

void setup() {
  pinMode(optocouplerPin, INPUT);
  pinMode(piOnPin, OUTPUT);
  pinMode(screenBrightnessPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  // Initial state for the Raspberry Pi control pin (HIGH to start)
  digitalWrite(piOnPin, HIGH);

  Serial.begin(9600);
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
    lastOptoState = currentOptoState;
    // Delay for debounce (adjust as needed)
    delay(50);
    signalSent = false;
  }

  // Check if the optocoupler state has remained unchanged for a certain duration
  if (millis() - optoStateChangedTime >= 5000UL && !signalSent) {
    // Send momentary pulse to the Raspberry Pi control pin
    digitalWrite(piOnPin, LOW);
    delay(600);  // Adjusted to 500 milliseconds
    digitalWrite(piOnPin, HIGH);
    // Add any additional logic or commands if needed
    signalSent = true;
  }

  // Check the manual button state
  if (buttonState == LOW) {
    // Button is pressed, send momentary pulse to the Raspberry Pi control pin
    digitalWrite(piOnPin, LOW);
    delay(600);  // Adjusted to 500 milliseconds
    digitalWrite(piOnPin, HIGH);

    // Add additional logic or commands as needed when the button is pressed
  }
}
