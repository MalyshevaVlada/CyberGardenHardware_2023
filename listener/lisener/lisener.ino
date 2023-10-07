#include <iarduino_RF433_Receiver.h>
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>


// Инициализация переменных и пинов
const int leds[] = {A0, A1, A2, A3, A4, A5, 3};
int data[2];
int ms = 1000;
iarduino_RF433_Receiver rx(2);


// логика
void setup() {
    Serial.begin(9600);
    rx.begin(1000);

    for (int i = 0; i < 7; i++) {
        pinMode(leds[i], OUTPUT);
        digitalWrite(leds[i], LOW);
    }

    mp3_set_serial(Serial);
    mp3_set_volume(20);

}

void loop() {
  if (rx.available()) {
    rx.read(&data[0], sizeof(data[0])); // 1 - номер фоторезистра для светодиода
    rx.read(&data[1], sizeof(data[1])); // 2 - номер фотодиода для говорилки
    
    trigger_led(data[0]);
    trigger_buzzer(data[1]);
  }
}


// функции
void trigger_buzzer(int number) {
    switch (number) {
      case 0:
        mp3_play(1); // влево
        delay(ms);
        break;
      case 1:
        mp3_play(2); // прямо
        delay(ms);
        break;
      case 2:
        mp3_play(3); // вправо 
        delay(ms);
        break;
      default:
        mp3_play(4); // ошибка
        delay(ms);
        break;
    }
}


void trigger_led(int number) {
    digitalWrite(leds[number], HIGH);
    delay(100);
    digitalWrite(leds[number], LOW);
}