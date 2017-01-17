// For the UNO.
const int right = 3;
const int front = 5;
const int left  = 6;
const int back  = 11;

// For the Bluetooth micro.
//const int right = A0;
//const int front = 10;
//const int left  = 11;
//const int back  = 6;

#include <math.h>

const char msg[] = "marisse_flat_disconnected_uni";
const bool directed = false;
const int16_t pulseLength = 25;

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

const int repeats = 1;
const int16_t strengths[] = {105, 155, 205, 255};
const float deltas[] = {0, 0, 0, 0, 22.5, 45, 90, 180};
const int16_t centers[] = {0, 45, 90, 145, 180, 225, 270, 315};
const int16_t nstim = NELEMS(centers) * NELEMS(deltas) * NELEMS(strengths);

int16_t stimSeq[nstim];

void createStim() {
  // Find a permutation of all stimuli via a Fisher-Yates shuffle.
  // https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle#The_modern_algorithm
  for(int16_t i = 0; i < nstim; i++) {
    stimSeq[i] = i;
  }

  for(int16_t i = 0; i < nstim - 1; i++) {
    int16_t j = (int16_t) random(i, nstim);
    
    // Swap the values.
    int16_t z = stimSeq[i];
    stimSeq[i] = stimSeq[j];
    stimSeq[j] = z;
  }
}

void oneStim(int16_t xscaled, int16_t yscaled) {
  if(directed) {
    for(int counter = 0; counter < pulseLength * 2; counter++) {
      if(counter < .5*pulseLength) {
        if(xscaled > 0) {
          analogWrite(right, xscaled);
          analogWrite(left , 0);
        } else {
          analogWrite(left, -xscaled);
          analogWrite(right , 0);
        }
      
        if(yscaled > 0) {
          analogWrite(front, yscaled);
          analogWrite(back , 0);
        } else {
          analogWrite(back, -yscaled);
          analogWrite(front, 0);
        }
      } else if(counter < pulseLength) {
        if(xscaled > 0) {
          analogWrite(right, xscaled);
          analogWrite(left , xscaled);
        } else {
          analogWrite(left, -xscaled);
          analogWrite(right , -xscaled);
        }
      
        if(yscaled > 0) {
          analogWrite(front, yscaled);
          analogWrite(back , yscaled);
        } else {
          analogWrite(back, -yscaled);
          analogWrite(front,-yscaled);
        }
      } else if(counter < 1.5*pulseLength) {
        if(xscaled > 0) {
          analogWrite(left, xscaled);
          analogWrite(right, 0);
        } else {
          analogWrite(right, -xscaled);
          analogWrite(left , 0);
        }
      
        if(yscaled > 0) {
          analogWrite(back , yscaled);
          analogWrite(front, 0);
        } else {
          analogWrite(front, -yscaled);
          analogWrite(back , 0);
        }
      } else {
        analogWrite(front, 0);
        analogWrite(back, 0);
        analogWrite(left, 0);
        analogWrite(right, 0);
      }
      digitalWrite(LED_BUILTIN, counter < pulseLength);
      delay(10);
    }
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
    if(xscaled > 0) {
      analogWrite(right, xscaled);
      analogWrite(left , 0);
    } else {
      analogWrite(left, -xscaled);
      analogWrite(right , 0);
    }
    
    if(yscaled > 0) {
      analogWrite(front, yscaled);
      analogWrite(back , 0);
    } else {
      analogWrite(back, -yscaled);
      analogWrite(front, 0);
    }
    delay(pulseLength * 10);
    digitalWrite(LED_BUILTIN, LOW);

    analogWrite(front, 0);
    analogWrite(back, 0);
    analogWrite(left, 0);
    analogWrite(right, 0);
  }
}


// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  // 
  pinMode(right, OUTPUT);
  pinMode(front, OUTPUT);
  pinMode(left, OUTPUT);
  pinMode(right, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  delay(1000);
  randomSeed(analogRead(0));
  createStim();
  Serial.println("message,trialNum,strength,center,delta,direction,answerSame,answerCorrect");
}

int16_t trialNum = 0;

void loop() {
  // Run a same - different paradigm
  if(trialNum > nstim) {
    delay(1000);
    return;
  }
  int16_t stimNum = stimSeq[trialNum];

  int16_t strength = strengths[stimNum % NELEMS(strengths)];
  stimNum /= NELEMS(strengths);
  
  int16_t delta = deltas[stimNum % NELEMS(deltas)];
  stimNum /= NELEMS(deltas);

  int16_t center = centers[stimNum % NELEMS(centers)];
  int8_t direction = random(0, 1) < .5 ? -1 : 1;

  //center = 270;
  //direction = 0;
  //delta = 0;
  
  long theta1 = float(center ) / 180.0 * 3.1416;
  long theta2 = float(center + direction * delta) / 180.0 * 3.1416;

  oneStim(cos(theta1) * strength, sin(theta1) * strength);
  delay(500);
  oneStim(cos(theta2) * strength, sin(theta2) * strength);
  bool answerSame;
  
  while(1) {
    while(Serial.available() == 0) {
      
    }
    byte incomingByte = Serial.read();
    if(incomingByte == (byte) 's') {
      answerSame = true;
      break;
    } else if(incomingByte == (byte) 'd') {
      answerSame = false;
      break;
    }
  }
  
  Serial.print(msg);
  Serial.print(",");
  Serial.print(trialNum);
  Serial.print(",");
  Serial.print(strength);
  Serial.print(",");
  Serial.print(center);
  Serial.print(",");
  Serial.print(delta);
  Serial.print(",");
  Serial.print(direction);
  Serial.print(",");
  Serial.print(answerSame);
  Serial.print(",");
  Serial.println(answerSame == (delta == 0));
  trialNum++;
}


