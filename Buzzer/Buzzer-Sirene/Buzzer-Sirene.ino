//Programa: Som no Arduino - Sirene

#define tempo 10
int frequencia = 0;
int pinBuzzer = 22;
void setup()
{
  pinMode(pinBuzzer,OUTPUT); //Pino do buzzer
}
void loop()
{
  for (frequencia = 150; frequencia < 1800; frequencia += 1) 
  {
    tone(pinBuzzer, frequencia, tempo); 
    delay(1);
  }
  for (frequencia = 1800; frequencia > 150; frequencia -= 1) 
  {
    tone(pinBuzzer, frequencia, tempo); 
    delay(1);
  }
}