const int ledPins[] = {8, 9, 10, 11};  // Tableau des numéros de broches
int potentiometerPin = A1;           
int potentiometerValue = 0;          
int ledIndex = 0;                   // Index du DEL allume
int pinButton = 2;

unsigned long currentTime = 0;
unsigned long serialPrevious = 0;
int serialDelay = 1000;

int mappedIndex = 0;
int mappedSymbol = 0;
int mappedPourcentage = 0;

void setup() {
  Serial.begin(9600);
  pinMode(pinButton, INPUT_PULLUP);
  for (int i = 0; i < 4; i++) {
    // Initialisation des DEL en sortie
    pinMode(ledPins[i], OUTPUT); 
  }
}


int estClic(unsigned long ct) {
  static unsigned long lastTime = 0;
  static int lastState = HIGH;
  const int rate = 50;
  int clic = 0;

  if (ct - lastTime < rate) {
    return clic; // Trop rapide
  }

  lastTime = ct;

  int state = digitalRead(pinButton);

  if (state == LOW) {
    if (lastState == HIGH) {
      clic = 1;
    }
  }

  lastState = state;

  return clic;
}



void loop() {
  currentTime = millis();

  potentiometerValue = analogRead(potentiometerPin);
  mappedIndex = map (potentiometerValue, 0, 1023, 0, 3);
  mappedSymbol = map (potentiometerValue, 0, 1023, 0, 19);
  mappedPourcentage = map (potentiometerValue, 0, 1023, 0, 100);

  
  int valueButton = digitalRead(pinButton);
  

  for (int i = 0; i < 4; i++) {
    if (i == mappedIndex) {
      digitalWrite(ledPins[i], HIGH);
    }
    else {
      digitalWrite(ledPins[i], LOW);
    }
  }


  if (currentTime - serialPrevious >= serialDelay) {
    serialPrevious = currentTime;
    if (valueButton == 0) {

      for (int i = 0; i < 20; i++) {
        if (i < mappedSymbol) {
          Serial.print("#");
        }
        else {
          Serial.print(".");
        }
      }   
      Serial.print(mappedPourcentage);    
      Serial.println(" % ");
    }
  }  
}