#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>
#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>

int colorArray[] = {100, 300, 20, 70, 100, 2};
int arraySize = sizeof(colorArray) / sizeof(colorArray[0]); // Calculate array size

const int rightBtn = 4;
const int leftBtn = 5;
const int switchPin = 7;

volatile bool leftBtnFlag = false;
volatile bool rightBtnFlag = false;
volatile bool switchFlag = false;

int rightBtnCounter = 0;
int leftBtnCounter = 0;
bool rightSideAccurate = false;
bool leftSideAccurate = false;

int current_score = 0;
bool didPassRound = true;
bool canGoNextRound = true;

const int capPad_1 = 6;
const int capPad_2 = 9;
const int capPad_3 = 10;
const int capPad_4 = 3;
const int capPad_5 = 2;
const int capPad_6 = 0;
const int capPad_7 = 1;

int toneList[7] = {65, 75, 85, 95, 100, 105, 110}; // MIDI Pitches
const int lenOfToneList = 7;
const int numLEDs = 10;
float midi[127];
int A_four = 440; // variable for calculating frequency based on pitches in the indexes of the song array(s)... A_four = 440Hz 

int numLEDsRightSide = 0;
int numLEDsLeftSide = 0;

int capPadToneMapping[][2] = 
{
  {capPad_1, toneList[0]},
  {capPad_2, toneList[1]},
  {capPad_3, toneList[2]},
  {capPad_4, toneList[3]},
  {capPad_5, toneList[4]},
  {capPad_6, toneList[5]},
  {capPad_7, toneList[6]},
};

int randToneOrder[7];
int randLEDOrder[7];

AsyncDelay round_delay;

// range for tones (with MIDI) is between 0 and 127 <---- !!!!

// Function prototypes
void generateMIDI();
void leftButtonISR();
void rightButtonISR();
void generateRandomSound(int toneOrder[]);
void generateRandomLight(int lightOrder[]);
void displayLights(int lightOrder[]);
void endRoundSuccess();
void endRoundFailure();

void setup() {
  CircuitPlayground.begin();
  pinMode(rightBtn, INPUT_PULLDOWN);
  pinMode(leftBtn, INPUT_PULLDOWN);
  pinMode(switchPin, INPUT_PULLDOWN)
  Serial.begin(9600);
  generateMIDI();

  attachInterrupt(leftBtn, leftButtonISR, FALLING);
  attachInterrupt(rightBtn, rightButtonISR, FALLING);

  round_delay.start(9000, AsyncDelay::MILLIS);
}


void loop() {

  if (canGoNextRound) {
    generateRandomSound(randToneOrder); 
    generateRandomLight(randLEDOrder);
    displayLights(randLEDOrder);
    Serial.println("Starting round");
    Serial.print("Current Score/Round = ");
    Serial.println(current_score);
    canGoNextRound = false;
    round_delay.repeat();
  }

  if (rightSideAccurate && leftSideAccurate) { // if you were accurate for both sides
      endRoundSuccess();
    } else if (round_delay.isExpired()) { // if time is up 
      endRoundFailure();
    } else if ((rightBtnCounter >= 6) || (leftBtnCounter >= 6)) { //if you hit the button too many times 
      endRoundFailure();
    }

  if (rightBtnFlag) {
    delay(40);
    rightBtnCounter ++;
    Serial.println("+1 right side. Current right score: ");
    Serial.println(rightBtnCounter);
    rightBtnFlag = false;
  }

  if (leftBtnFlag) {
    delay(40);
    leftBtnCounter ++;
    Serial.println("+1 left side. Current left score:");
    Serial.println(leftBtnCounter);
    leftBtnFlag = false;
  } 

  if ( (rightBtnCounter == numLEDsRightSide) && (rightBtnCounter != 0) ) {
    Serial.println("Right side is accurate"); 
    rightSideAccurate = true;
  }

  if ( (leftBtnCounter == numLEDsLeftSide) && (leftBtnCounter != 0) ) { 
    Serial.println("Left side is accurate");
    leftSideAccurate = true;
  }
}


void endRoundFailure() {
  flashing(colorArray, arraySize);
  delay(100);
  exit(1);
}

void endRoundSuccess() {
  CircuitPlayground.clearPixels();
  rightBtnCounter = 0;
  leftBtnCounter = 0;

  numLEDsRightSide = 0;
  numLEDsLeftSide = 0;
  current_score += 1;

  canGoNextRound = true;
  rightSideAccurate = false;
  leftSideAccurate = false;
  delay(200);
}

void flashing(int flashColors[], int size)
{
  for (int l = 0; l < size - 2; l++)
  {
    for(int q = 0; q < 10; q++)
    {
      CircuitPlayground.setPixelColor(q, CircuitPlayground.colorWheel(flashColors[l]*20 & 255));
    }
    delay(300);
    CircuitPlayground.clearPixels();
    delay(500);
  }
}

void generateRandomSound(int toneOrder[]) {
  for (int i = 0; i < lenOfToneList; i++) {
    int k = random(0, lenOfToneList);
    toneOrder[i] = toneList[k]; 
  }
} 

void displayLights(int lightOrder[]) {
  for (int i = 0; i < numLEDs; i++) {
    int kth_pin = lightOrder[i]; 
    CircuitPlayground.setPixelColor(kth_pin, CircuitPlayground.colorWheel(20*current_score));
    delay(200);
  }
}

void generateRandomLight(int lightOrder[]) {
  numLEDsLeftSide = 0;
  numLEDsRightSide = 0;
  for (int i = 0; i < lenOfToneList; i++) {
    int k = random(0, numLEDs);
    lightOrder[i] = k; 
    if (lightOrder[i] < 6) { // if on the left side (left side of board has LEDs 1-5) 
      numLEDsLeftSide += 1;
    } else if (lightOrder[i] >= 6) { // if on the right side (right side of board has LEDs 6-10)
      numLEDsRightSide += 1;
    }
  }
}

void leftButtonISR() {
  leftBtnFlag = true;
}

void rightButtonISR() {
  rightBtnFlag = true;
}

void generateMIDI()
{
  for (int x = 0; x < 127; ++x)
  {
    midi[x] = (A_four / 32.0) * pow(2.0, ((x - 9.0) / 12.0));
    Serial.println(midi[x]);
  }
}
