const int Buzzer = 22;
int x = 0;
int y = 0;

void setup() {
  pinMode(Buzzer, OUTPUT);
}

void loop(){
  while (x < 2) {
    tone(Buzzer, 392);
    delay(500);
    tone(Buzzer, 440);
    delay(250);
    tone(Buzzer, 468);
    delay(400);
    tone(Buzzer, 589);
    delay(1000);
    noTone(Buzzer);
    delay(400);

    tone(Buzzer, 663);
    delay(400);
    tone(Buzzer, 701);
    delay(250);
    tone(Buzzer, 589);
    delay(400);
    tone(Buzzer, 663);
    delay(250);
    tone(Buzzer, 526);
    delay(400);
    tone(Buzzer, 589);
    delay(250);
    tone(Buzzer, 440);
    delay(1000);
    noTone(Buzzer);
    delay(800);
    x++;
  }

  while (y <  1) {
    tone(Buzzer, 440);
    delay(600);
    tone(Buzzer, 468);
    delay(1000);
    y++;
  }

  noTone(Buzzer);
}