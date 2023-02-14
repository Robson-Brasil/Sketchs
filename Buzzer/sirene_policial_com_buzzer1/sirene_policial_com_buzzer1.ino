int buzzer = 22; //Atribui o pino digital D1 (GPIO5) à variável buzzer
int azul = 36; //Atribui o pino digital D2 (GPIO4) à variável azul
int vermelho = 37; //Atribui o pino digital D3 (GPIO0) à variável vermelho
int tempo = 100; //variável usada para definir o tempo de acionamento do buzzer
int frequencia = 0; //variável usada para armazenar a frequencia que será usada no acionamento do buzzer
unsigned long millisTarefa1 = millis(); //variável que recebe o tempo atual em milissegundos
int temp = 250; //variável usada para definir o tempo de acionamento do led


void setup() {
  //configura buzzer, azul e vermelho como saída (OUTPUT)
  pinMode(buzzer, OUTPUT);
  pinMode(azul, OUTPUT);
  pinMode(vermelho, OUTPUT);
}
//Função de acionamento dos LEDs
void led() {
  if (millis() - millisTarefa1 > temp) {//Se o resultado da subtração de millis() - millisTarefa1 for maior que temp (250 milissegundo)
    digitalWrite(azul, HIGH);//Liga o LED azul
    digitalWrite(vermelho, LOW);//Desliga o LED vermelho
  } else {//Senão
    digitalWrite(azul, LOW);//Desliga o azul
    digitalWrite(vermelho, HIGH);//Liga o LED vermelho
  }
  if ((millis() - millisTarefa1) > (2 * temp)) {//Se o resultado da subtração de millis() - millisTarefa1 for maior que 2 vezes temp (500 milissegundo)
    millisTarefa1 = millis();//Atribui a millisTarefa1 o valor de millis()
  }
}

void loop() {
  for (frequencia = 150; frequencia < 1800; frequencia += 1) { //Define frequencia igual a 150; verifica se frequencia é menor que 1800; realiza a soma frequencia = frequencia + 1
    led(); //Chama a função led
    tone(buzzer, frequencia, tempo); //Aciona o led com frequência definida pelo for e tempo igual a 100 milissegundos
    delay(1); //delay de 1 milissegundos
  }
  for (frequencia = 1800; frequencia > 150; frequencia -= 1) { //Define frequencia igual a 1800; verifica se frequencia é maior que 150; realiza a subtração frequencia = frequencia - 1
    led(); //Chama a função led
    tone(buzzer, frequencia, tempo); //Aciona o led com frequência definida pelo for e tempo igual a 100 milissegundos
    delay(1); //delay de 1 milissegundos
  }
}
