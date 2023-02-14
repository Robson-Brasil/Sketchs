

// Curso de Arduino para Iniciantes - Aula 06 - Buzzer Projeto Sirene - Aldeir Moreira

// Canal Aldeir Moreira no YouTube: https://www.youtube.com/AldeirMoreira

//Mais detalhes sobre o comando tone(): https://www.arduino.cc/reference/pt/language/functions/advanced-io/tone
//Mais detalhes sobre  comando for(): https://www.arduino.cc/reference/pt/language/structure/control-structure/for

// Conheça o Curso de Arduino para Modelismo e dê vida aos seus Modelos,
// Miniaturas e Projetos: https://arduinoparamodelismo.com
#define pinBuzzer 22

int freq;// declarando variável global para armazernar a frequencia do tom

void setup() {

  pinMode(pinBuzzer, OUTPUT); // pino digital 6 como saída para o buzzer

}

void loop() {

  for (freq = 200; freq < 2000; freq++) {
    tone(pinBuzzer, freq, 10);
    delay(1);
  }

  for (freq = 2000; freq > 200; freq--) {
    tone(pinBuzzer, freq, 10);
    delay(1);
  }
}
