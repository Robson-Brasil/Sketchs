#define pinBuzzer 22

void setup() {
  // put your setup code here, to run once:
  pinMode(pinBuzzer, OUTPUT);
}

void loop() {
/*
  Frequência das notas:
  Dó - 262 Hz
  Ré - 294 Hz
  Mi - 330 Hz
  Fá - 349 Hz
  Sol - 392 Hz
  Lá - 440 Hz
  Si - 494 Hz
  #Dó - 528 Hz
  */  
  // put your main code here, to run repeatedly:
  bool tipoBuzzer = 1;  //0=Ativo ou 1=Passivo

  if (tipoBuzzer == 0) {
    digitalWrite(pinBuzzer, HIGH);
    delay(500);
    digitalWrite(pinBuzzer, LOW);
    delay(500);
  } else {
    tone(pinBuzzer, 900);
    delay(500);
    noTone(pinBuzzer);
    delay(500);
  }
}
