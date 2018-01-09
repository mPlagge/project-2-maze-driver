# define IRPin 8
void setup() {
  pinMode(IRPin,INPUT);
}

void loop() {
}

bool InfraRood(){
  if (digitalRead(IRPin) == HIGH){
    return true;
  } else {
    return false;
  }
}

