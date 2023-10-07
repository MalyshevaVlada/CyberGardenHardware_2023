#include <AccelStepper.h>
#include <iarduino_RF433_Transmitter.h>

#define THRESHOLD_MIN 800
#define THRESHOLD_MAX 1100

iarduino_RF433_Transmitter tx(2);

// Инициализация переменных и пинов
const int photoResistors[] = {A0, A1, A2, A3, A4, A5, 3}; // фоторезисторы
const int irSensors[] = {7, 12, 13}; // t11838
const int stepperPins[] = {8, 9, 10, 11}; // моторчик

AccelStepper stepper(AccelStepper::FULL4WIRE, stepperPins[0], stepperPins[1], stepperPins[2], stepperPins[3]);


// логика
void setup() {
    Serial.begin(9600);
    tx.begin(1000);

    for (int i = 0; i < 7; i++)
        pinMode(photoResistors[i], INPUT);
    for (int i = 0; i < 3; i++)
        pinMode(irSensors[i], INPUT);

    stepper.setMaxSpeed(5000);
    stepper.setAcceleration(500);
}

void loop() {
    int detected_photo_sensor = detect_photo_sensor();

    if (detected_photo_sensor != -1) {
        int target_position = detected_photo_sensor * 20;  // 20 градусов разница в повороте
        stepper.moveTo(target_position);
        while (stepper.distanceToGo() != 0) {
            stepper.run();
        }
        if (detect_ir_sensor() != -1) {
            int detected_ir_sensor = detect_ir_sensor();
            send_photo_sensor(detected_photo_sensor);
            send_ir_sensor(detected_ir_sensor);
        }
    }
}


// функции
int detect_photo_sensor() {
    for (int i = 0; i < 7; i++) {
        int value = analogRead(photoResistors[i]);
        if (value >= THRESHOLD_MIN && value <= THRESHOLD_MAX)  // Предполагаемое значение порога
            return i;
    }
    return -1;  // Если вспышка не обнаружена
}


int detect_ir_sensor() {
    for (int i = 0; i < 3; i++)
        if (digitalRead(irSensors[i]) == HIGH)
            return i;
    return -1;  // Если вспышка не обнаружена
}


void send_photo_sensor(int photo_sensor) {
  tx.write(&photo_sensor, sizeof(photo_sensor));
}


void send_ir_sensor(int ir_sensor) {
  tx.write(&ir_sensor, sizeof(ir_sensor));
}