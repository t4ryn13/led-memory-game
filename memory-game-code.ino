// assign LEDs and buttons to pins
const int buzzer = 7;
const int LEDList[4] = {6, 5, 4, 3}; // red LED, yellow LED, green LED, blue LED
const int toneList[4] = {262, 330, 392, 523}; // tones for each light
const int buttonList[4] = {13, 12, 11, 10}; // button 1, button 2, button 3, button 4

// declare general variables
int level = 0; // will keep incrementing until limit to fill each spot of the sequence array
int limit = 15;
int sequence[15];
int displaySpeed;

// declare variables for debounce functionality
unsigned long currentTime;
unsigned long lastDebounce = 0;
int debounceDelay = 200;



void setup(){
  // set pin modes and initiate all the LEDs as turned off
  for (int i = 0; i <= 3; i++){
    pinMode(LEDList[i], OUTPUT);
    pinMode(buttonList[i], INPUT);
    digitalWrite(LEDList[i], LOW);
  }
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);

  // get initial sequence
  getSequence();

  // intro lights and tune
  for (int i = 0; i <= 3; i++){
    digitalWrite(LEDList[i], HIGH);
    tone(buzzer, toneList[i], 300);
    delay(300);
    digitalWrite(LEDList[i], LOW);
    delay(100);
  }
  for (int i = 0; i <= 3; i++){
    digitalWrite(LEDList[i], HIGH);
    tone(buzzer, toneList[i], 50);
    delay(50);
    digitalWrite(LEDList[i], LOW);
    delay(50);
  }

  // wait until game begins
  delay(700);
}



void loop() {
  // check if game is over
  if (level == limit){
    while (true){
      for (int i = 0; i <= 3; i++){
        digitalWrite(LEDList[i], HIGH);
      }
      delay(400);
      for (int i = 0; i <= 3; i++){
        digitalWrite(LEDList[i], LOW);
      }
      delay(400);
    }
  }

  delay(300);
  displaySequence();
  
  for (int i = 0; i <= level; i++){
    // declare variables for this specific round
    int currentIndex = sequence[i];
    int expectedButton = buttonList[currentIndex];
    int actualButton = buttonPress();

    // wrong button pressed (game over)
    if (expectedButton != actualButton){
      for (int i = 0; i <= 2; i++){
        digitalWrite(LEDList[currentIndex], HIGH);
        tone(buzzer, toneList[currentIndex], 300);
        delay(300);
        digitalWrite(LEDList[currentIndex], LOW);
        delay(300);
      }
      tone(buzzer, 294, 400);
      delay(400);
      tone(buzzer, 208, 650);
      gameOver();
    }

    // correct button was pressed (game continues)
    delay(300);
    tone(buzzer, toneList[currentIndex], displaySpeed);
    
  }
  delay(1000);
  level++;
}



void getSequence(){
  // adds random values from 0 to 3 to the sequence array (length = limit)
  for (int i = 0; i <= (limit - 1); i++) {
    int index = random(4);
    sequence[i] = index;
  }
}



void displaySequence(){
  // determine speed of display depending on level
  if (level < 10){
    displaySpeed = 500;
  }
  else {
    displaySpeed = 250;
  }
  
  // shows the expected LEDs of the sequence up to the level
  for (int i = 0; i <= level; i++){
    int currentIndex = sequence[i];
    int currentLED = LEDList[currentIndex];
    int currentTone = toneList[currentIndex];
    digitalWrite(currentLED, HIGH);
    tone(buzzer, currentTone, displaySpeed);
    delay(displaySpeed);
    digitalWrite(currentLED, LOW);
    delay(displaySpeed);
  }
}



int buttonPress(){
  // considers deboucing feature to avoid unintentional button presses
  int pressedButton = 0;
  currentTime = millis();
  while (pressedButton == 0){
    for (int i = 0; i <= 3; i++){
      if ((digitalRead(buttonList[i]) == HIGH) && 
      ((currentTime - lastDebounce) > debounceDelay)){
        pressedButton = buttonList[i];
        lastDebounce = currentTime;
      }
    }
  }
  return pressedButton;
}



void gameOver(){
  while (true){
    for (int i = 0; i <= 3; i++){
      digitalWrite(LEDList[i], HIGH);
      delay(300);
    }
    for (int i = 0; i <= 3; i++){
      digitalWrite(LEDList[i], LOW);
      delay(300);
    }
  }
}
