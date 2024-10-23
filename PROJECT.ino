#define BLYNK_TEMPLATE_ID "TMPL3Q73RHSEp"
#define BLYNK_TEMPLATE_NAME "PROJECT"
#define BLYNK_AUTH_TOKEN "BafWaLR2bXGypUaoDJtqeAAJecd3j2LN"
#define BLYNK_PRINT Serial 

#include <BlynkSimpleEsp32.h>
#include <HX711.h>
#include <ESP32Servo.h>

// Blynk Authorization Token
char auth[] = BLYNK_AUTH_TOKEN;

// HX711 pins
#define LOADCELL_DOUT_PIN  4
#define LOADCELL_SCK_PIN   5

HX711 scale;

// Servo pins
Servo servo1;
Servo servo2;
Servo servo3;
int servo1Pin = 12;
int servo2Pin = 13;
int servo3Pin = 14;

// Color sensor pins
#define S0 15
#define S1 16
#define S2 17
#define S3 18
#define sensorOut 19

// Frequency variables
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

// Color values
int redColor = 0;
int greenColor = 0;
int blueColor = 0;

float weight = 0; // Variable to store the random weight

void setup() {
    Serial.begin(115200);
    Blynk.begin(auth, "vivo T2x 5G", "123456789sai");
    
    // HX711 setup
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    scale.set_scale(2280.f);
    scale.tare();

    // Servo setup
    servo1.attach(servo1Pin);
    servo2.attach(servo2Pin);
    servo3.attach(servo3Pin);

    // Color sensor setup
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(sensorOut, INPUT);
    
    // Set frequency scaling
    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);
    
    randomSeed(analogRead(0)); // Seed random number generator
}

void loop() {
    Blynk.run();

    if (Blynk.connected()) {
            weight = random(0, 2000) / 1000.0; // Generate random weight between 2.0 kg and 5.0 kg
            Serial.print("Weight: ");
            Serial.println(weight); // Send weight to Serial Monitor

        // Read color data
        readColor();

        // Send color data to Blynk
        Serial.print("Red: ");
        Serial.print(redColor);
        Serial.print(", Green: ");
        Serial.print(greenColor);
        Serial.print(", Blue: ");
        Serial.println(blueColor);
        delay(2000);
    }
}

// Function to read the color
void readColor() {
    // RED
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
    redFrequency = pulseIn(sensorOut, LOW);
    redColor = map(redFrequency, 96, 302, 239, 0);

    // GREEN
    digitalWrite(S2, HIGH);
    digitalWrite(S3, HIGH);
    greenFrequency = pulseIn(sensorOut, LOW);
    greenColor = map(greenFrequency, 631, 694, 608, 0);

    // BLUE
    digitalWrite(S2, LOW);
    digitalWrite(S3, HIGH);
    blueFrequency = pulseIn(sensorOut, LOW);
    blueColor = map(blueFrequency, 364, 182, 96, 0);

    // Detect and print the color
    if (redColor > greenColor && redColor > blueColor) {
        Serial.println(" - RED detected!");
    }
    if (greenColor > redColor && greenColor > blueColor) {
        Serial.println(" - GREEN detected!");
    }
    if (blueColor > redColor && blueColor > greenColor) {
        Serial.println(" - BLUE detected!");
    }
}

// Blynk button callback (optional)
BLYNK_WRITE(V0) {
    int value = param.asInt();
    servo1.write(value);
}

BLYNK_WRITE(V1) {
    int value = param.asInt();
    servo2.write(value);
}

BLYNK_WRITE(V2) {
    int value = param.asInt();
    servo3.write(value);
}
