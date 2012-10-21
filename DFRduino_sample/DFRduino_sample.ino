int S1 = 5;
int S2 = 6;
int M1 = 4;
int M2 = 7;

int OFF_BUTTON = 2;
int ON_BUTTON = 3;

void halt(){
  digitalWrite(S1, LOW);
  digitalWrite(S2, LOW);
}

void foward(int a, int b){
  analogWrite(S1, a);
  digitalWrite(M1, HIGH);
  analogWrite(S2, b);
  digitalWrite(M2, HIGH);
}

void setup(){
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  
  pinMode(OFF_BUTTON, INPUT);
  pinMode(ON_BUTTON, INPUT);
}

void loop(){
  if (digitalRead(ON_BUTTON) == 0){
    foward(200, 100);
  }
  
  if (digitalRead(OFF_BUTTON) == 0){
     halt();
  }
}
