#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>

int colorArray[] = {100, 300, 20, 70, 100, 2};
int arraySize = sizeof(colorArray) / sizeof(colorArray[0]); // Calculate array size

const int rightBtn = 5;
const int leftBtn = 4;
const int switchPin = 7;
const int capPin = 6; // This is the A1 capacitance pad

volatile bool leftBtnFlag = false;
volatile bool rightBtnFlag = false;
volatile bool switchFlag = false;
volatile bool switchState = false;

int moistureLevel = 0;

bool canGoNextRound = true;

int numLEDs = 10;

int round_num = 1;

const int capPad_1 = 6;
const int capPad_2 = 9;
const int capPad_3 = 10;
const int capPad_4 = 3;
const int capPad_5 = 2;
const int capPad_6 = 0;
const int capPad_7 = 1;

AsyncDelay delayTimer;

// AsyncDelay round_1_delay;
// AsyncDelay round_2_delay;
// AsyncDelay round_3_delay;
// AsyncDelay round_4_delay;
// AsyncDelay round_5_delay;
// AsyncDelay round_6_delay;
// AsyncDelay round_7_delay;

// inputs:
// left btn
// right btn
// switch
// capacitance pads

// outputs: 
// sound, lights


void setup() {
  CircuitPlayground.begin();
  Serial.begin(9600);
  delay(1000); // make sure Serial Monitor is ready

  pinMode(switchPin, INPUT_PULLUP);
  pinMode(leftBtn, INPUT_PULLDOWN);
  pinMode(rightBtn, INPUT_PULLDOWN);

  attachInterrupt(switchPin, switchISR, CHANGE);
  attachInterrupt(leftBtn, leftBtnISR, FALLING);
  attachInterrupt(rightBtn, rightBtnISR, FALLING);

  // round_1_delay.start(2000, AsyncDelay::MILLIS);
  // round_2_delay.start(1800, AsyncDelay::MILLIS);
  // round_3_delay.start(1600, AsyncDelay::MILLIS);
  // round_4_delay.start(1400, AsyncDelay::MILLIS);
  // round_5_delay.start(1200, AsyncDelay::MILLIS);
  // round_6_delay.start(100, AsyncDelay::MILLIS);
  // round_7_delay.start(800, AsyncDelay::MILLIS);
}

void loop() {
  moistureLevel = CircuitPlayground.readCap(capPin); // Reading from the "A1" pin
  Serial.println(moistureLevel);
  if (canGoNextRound) {
    canGoNextRound = false;
    switch (round_num) {
      case 1:
        round_1_start();
        break;
      case 2:
        round_2_start();
        break;
      case 3:
        round_3_start();
        break;
      case 4:
        round_4_start();
        break;
      case 5:
        round_5_start();
        break;
      // case 6:                  // Extra two rounds (if necessary)
      //   round_6_start();
      //   break;
      // case 7:
      //   round_7_start();
      //   break;
    }
  }
}

// HELPER FUNCTIONS ------------------

void roundFailure()
{
  for (int j = 0; j < 10; j++)
  {
    CircuitPlayground.setPixelColor(j, CircuitPlayground.colorWheel(10));
    delay(100);
  }
  CircuitPlayground.clearPixels();
  for (int k = 10; k >= 0; k--)
  {
    CircuitPlayground.setPixelColor(k, CircuitPlayground.colorWheel(20));
    delay(100);
  }
  CircuitPlayground.clearPixels();
  Serial.println("INCORRECT!");
  CircuitPlayground.playTone(400, 100);
  CircuitPlayground.playTone(300, 100);
  CircuitPlayground.playTone(200, 100);
  delay(200);
  exit(1);
}

void flashing(int flashColors[], int size)
{
  Serial.println("Starting loop...");
  for (int l = 0; l < size; l++)
  {
    for(int q = 0; q < 10; q++)
    {
      CircuitPlayground.setPixelColor(q, CircuitPlayground.colorWheel(flashColors[l]*20 & 255));
    }
    delay(300);
    CircuitPlayground.clearPixels();
    delay(500);
  }
  Serial.println("Ending loop...");
}

// ------------------------------------

// ISR FUNCTIONS ----------

void switchISR() {
  switchFlag = 1;
}  


void leftBtnISR() {
  leftBtnFlag = 1;
}

void rightBtnISR() {
  rightBtnFlag = 1;
}

// --------------------------

// PROMPT FUNCTIONS --------------------

void capPadPrompt() {
  CircuitPlayground.playTone(400, 800);
  delay(100);
}

void switchPrompt() {
  for (int j = 0; j < 2; j++) {
    for (int i = 0; i < numLEDs; i++) {
      CircuitPlayground.setPixelColor(i, 255);  
    }
    delay(100);
  }
}

void rightButtonPrompt() {
  for (int i = 5; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, 100);
  }
  delay(500);
  CircuitPlayground.clearPixels();
}


void leftButtonPrompt() {
  for (int i = 0; i < 5; i++) {
    CircuitPlayground.setPixelColor(i, 100);
  }
  delay(100);
  CircuitPlayground.clearPixels();
}

// ---------------------------------------

// TEST FUNCTIONS ------------------------

int testCapPad(int currentRoundDelay) {
  delayTimer.start(currentRoundDelay, AsyncDelay::MILLIS);
  while (!(delayTimer.isExpired())) {
    moistureLevel = CircuitPlayground.readCap(capPin); // Reading from the "A1" pin
    if (moistureLevel > 1100) {
      Serial.println("Correct!");
      delay(100);
      return 0;
    }
  }
  if (delayTimer.isExpired() && (moistureLevel < 1100)) {
    roundFailure();
  }
}

int testSwitch(int currentRoundDelay) {
  delayTimer.start(currentRoundDelay, AsyncDelay::MILLIS);
  while (!(delayTimer.isExpired())) {
    if (switchFlag) {
      Serial.println("Correct!");
      switchFlag = false;
      delay(100);
      return 0;
    }
  }
  if (delayTimer.isExpired() && !switchFlag) {
    roundFailure();
  }
}

int testLeftBtn(int currentRoundDelay) {
  delayTimer.start(currentRoundDelay, AsyncDelay::MILLIS);
  while (!(delayTimer.isExpired())) {
    if (leftBtnFlag) {
      Serial.println("Correct!");
      leftBtnFlag = false;
      delay(200);
      return 0;
    }
  }
  if (delayTimer.isExpired() && !leftBtnFlag) {
    roundFailure();
  }
}

int testRightBtn(int currentRoundDelay) {
  delayTimer.start(currentRoundDelay, AsyncDelay::MILLIS);
  while (!(delayTimer.isExpired())) {
    if (rightBtnFlag) {
      Serial.println("Correct!");
      rightBtnFlag = false;
      delay(200);
      return 0;
    }
  }
  if (delayTimer.isExpired() && !rightBtnFlag) {
    roundFailure();
  }
}

// --------------------------------------------

// ROUND FUNCTIONS ----------------------

void round_1_start() {
  Serial.println("FIRST ROUND...");
  Serial.println("RULES: ");
  Serial.println("All LEDs lighting up on one side of the board means 'hit the button on that side,' (all LEDs must light up on that side)\n");
  Serial.println("Two flashes of green LEDs lighting up the pixels means switch the switch pin once!\n");
  Serial.println("A loud, 1 second tone means 'hold the A1 capacitance pad for 2 seconds!'\n");
  Serial.println("If you fail, board LEDs will go in a forwards and backwards pattern and the game will end!\n");
  Serial.println("There are 5 pre-defined rounds... BEGIN!");
  delay(15000); // delay 15 seconds

  rightButtonPrompt();
  testRightBtn(4000);

  leftButtonPrompt();
  testLeftBtn(4000);

  switchPrompt();
  testSwitch(4000);

  capPadPrompt();
  testCapPad(4000);

  canGoNextRound = true;
  round_num++;
}

void round_2_start() {
  Serial.println("WELCOME TO THE SECOND ROUND!");

  rightButtonPrompt();
  testRightBtn(3500); 

  rightButtonPrompt();
  testRightBtn(3500); 

  capPadPrompt();
  testCapPad(3500);

  switchPrompt();
  testSwitch(3500);

  capPadPrompt();
  testCapPad(3500);

  rightButtonPrompt();
  testRightBtn(3500); 

  canGoNextRound = true;
  round_num++;
}

void round_3_start() {
  Serial.println("WELCOME TO THE THIRD ROUND!");

  capPadPrompt();
  testCapPad(3000);
  
  switchPrompt();
  testSwitch(3000);

  switchPrompt();
  testSwitch(3000);

  rightButtonPrompt();
  testRightBtn(3000); 

  rightButtonPrompt();
  testRightBtn(3000); 

  leftButtonPrompt();
  testLeftBtn(3000);

  leftButtonPrompt();
  testLeftBtn(3000);

  switchPrompt();
  testSwitch(3000);

  canGoNextRound = true;
  round_num++;
}

void round_4_start() {
  Serial.println("WELCOME TO THE FOURTH ROUND!");

  switchPrompt();
  testSwitch(2500);

  switchPrompt();
  testSwitch(2500);

  leftButtonPrompt();
  testLeftBtn(2500);
  
  rightButtonPrompt();
  testRightBtn(2500); 

  capPadPrompt();
  testCapPad(2500);

  capPadPrompt();
  testCapPad(2500);

  rightButtonPrompt();
  testRightBtn(2500); 
  
  canGoNextRound = true;
  round_num++;
}

void round_5_start() {
  Serial.println("WELCOME TO THE FIFTH ROUND!");

  switchPrompt();
  testSwitch(2000);

  leftButtonPrompt();
  testLeftBtn(2000);

  leftButtonPrompt();
  testLeftBtn(2000);

  capPadPrompt();
  testCapPad(2000);

  leftButtonPrompt();
  testLeftBtn(2000);

  capPadPrompt();
  testCapPad(2000);

  rightButtonPrompt();
  testRightBtn(2500); 

  switchPrompt();
  testSwitch(2500);
  
  rightButtonPrompt();
  testRightBtn(2500); 

  Serial.println("CONDRADULATIONS! YOU HAVE WON THIS GAME. YOU ARE A GOD AMONG THESE MORTALS!"); 
  delay(100);
  flashing(colorArray, arraySize);
}

// ---------------------------------------s


