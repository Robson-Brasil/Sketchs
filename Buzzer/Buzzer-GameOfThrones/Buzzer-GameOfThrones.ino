/*
Código que toca um pedaço da música
de abertura de Game of Thrones
*/
#define pinBuzzer 22

void tom(char pino, int frequencia, int duracao);

void setup(){
  pinMode(pinBuzzer,OUTPUT);
}
void loop(){
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
  delay(1000);
  int tempo = 400;
  tom(pinBuzzer,440,tempo); //LA
  delay(tempo);
  tom(pinBuzzer,294,tempo); //RE
  delay(tempo);
  tom(pinBuzzer,349,tempo/2); //FA
  delay(tempo/2);
  tom(pinBuzzer,392,tempo/2); //SOL
  delay(tempo/2);
  tom(pinBuzzer,440,tempo); //LA
  delay(tempo);
  tom(pinBuzzer,294,tempo); //RE
  delay(tempo);
  tom(pinBuzzer,349,tempo/2); //FA
  delay(tempo/2);
  tom(pinBuzzer,392,tempo/2); //SOL
  delay(tempo/2);
  tom(pinBuzzer,330,tempo); //MI
  delay(tempo);
}

void tom(char pino, int frequencia, int duracao){
  float periodo = 1000.0/frequencia; //Periodo em ms
  for (int i = 0; i< duracao/(periodo);i++){ //Executa a rotina de dentro o tanta de vezes que a frequencia desejada cabe dentro da duracao
    digitalWrite(pino,HIGH);
    delayMicroseconds(periodo*500); //Metade do periodo em ms
    digitalWrite(pino, LOW);
    delayMicroseconds(periodo*500);
  }
}