#include <Arduino.h>

// Define the segment pins for the 7-segment display (a to g, dp)
const int segA = 2;
const int segB = 3;
const int segC = 4;
const int segD = 5;
const int segE = 6;
const int segF = 7;
const int segG = 8;

// Define the digit pins for the 4-digit display
const int digit1 = 12;  // hour 1s
const int digit2 = 11;  // hour 2s
const int digit3 = 10;   // minute 1s
const int digit4 = 9;  // minute 2s

// Define the button pins
const int minuteButton = A0;
const int hourButton = A1;

unsigned long previousMillis = 0;
const long interval = 1000;
int hours = 0;
int minutes = 0;
int seconds = 0;

// Digit-to-segment encoding (0-9)
const byte digitToSegment[10] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111  // 9
};

void setup() {
  // Set segment pins as outputs
  pinMode(segA, OUTPUT);
  pinMode(segB, OUTPUT);
  pinMode(segC, OUTPUT);
  pinMode(segD, OUTPUT);
  pinMode(segE, OUTPUT);
  pinMode(segF, OUTPUT);
  pinMode(segG, OUTPUT);

  // Set digit pins as outputs
  pinMode(digit1, OUTPUT);
  pinMode(digit2, OUTPUT);
  pinMode(digit3, OUTPUT);
  pinMode(digit4, OUTPUT);

  // Set button pins as inputs with internal pull-up resistors
  pinMode(minuteButton, INPUT_PULLUP);
  pinMode(hourButton, INPUT_PULLUP);

  Serial.begin(9600);

  // Initialize digits to be off
  clearDigits();
}

void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    incrementTime();
      Serial.print(" sec-");
    Serial.print(seconds);
    
    Serial.print(" min-");
    Serial.print(minutes);
    Serial.println(" ");
  }
  
  // Check for button presses to set the time
  if (digitalRead(minuteButton) == 0) {
    Serial.print(" Minute-button is pressed");
    delay(50);  // Debounce delay
    if (digitalRead(minuteButton) == LOW) {
      setMinute();
    }
  }
  if (digitalRead(hourButton) == LOW) {
    Serial.print(" Hour-button is pressed");
    delay(50);  // Debounce delay
    if (digitalRead(hourButton) == LOW) {
      setHour();
    }
  }
  
  int displayValue = hours * 100 + minutes;
  displayTime(displayValue);
}

void incrementTime() {
  seconds++;
  if (seconds >= 60) {
    seconds = 0;
    minutes++;
    if (minutes >= 60) {
      minutes = 0;
      hours++;
      if (hours >= 24) {
        hours = 0;
      }
    }
  }
}

void setMinute() {
  minutes++;
  if (minutes >= 60) {
    minutes = 0;
  }
  delay(200);  // Delay to prevent multiple increments from a single press
}

void setHour() {
  hours++;
  if (hours >= 12) {
    hours = 1;
  }
  delay(200);  // Delay to prevent multiple increments from a single press
}

void displayTime(int value) {
  int digitArray[4] = {
    value / 1000,
    (value / 100) % 10,
    (value / 10) % 10,
    value % 10
  };
  
  for (int i = 0; i < 4; i++) {
    displayDigit(i, digitArray[i]);
    delay(5);
    clearDigits();
  }
  
    // Clear digits once after displaying all digits
}

void displayDigit(int digit, int number) {
  byte segments = digitToSegment[number];

  digitalWrite(segA, !(segments & 0b00000001));
  digitalWrite(segB, !(segments & 0b00000010));
  digitalWrite(segC, !(segments & 0b00000100));
  digitalWrite(segD, !(segments & 0b00001000));
  digitalWrite(segE, !(segments & 0b00010000));
  digitalWrite(segF, !(segments & 0b00100000));
  digitalWrite(segG, !(segments & 0b01000000));

  switch (digit) {
    case 0: digitalWrite(digit1, HIGH); 
    break;
    case 1: digitalWrite(digit2, HIGH); 
    break;
    case 2: digitalWrite(digit3, HIGH); 
    break;
    case 3: digitalWrite(digit4, HIGH); 
    break;
  }
}

void clearDigits() {
  digitalWrite(digit1, LOW);
  digitalWrite(digit2, LOW);
  digitalWrite(digit3, LOW);
  digitalWrite(digit4, LOW);
}
