#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int eyeRadius = 30;
int pupilRadius = 15;
int eyeX = SCREEN_WIDTH / 2;
int eyeY = SCREEN_HEIGHT / 2;
int pupilX = eyeX;
int pupilY = eyeY;

unsigned long previousMillis = 0;
const long blinkInterval = 2000; // Time between blinks
bool isBlinking = false;

int pirPin = 3;

void setup() {
  // Initialize the display with the I2C address (usually 0x3C for 128x64 OLEDs)
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Loop forever if initialization fails
  }
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(pirPin, INPUT); // Set PIR pin as input
  display.clearDisplay();
  display.display();
  delay(1000);
}

void Eye() {
  display.clearDisplay();
  display.drawCircle(eyeX, eyeY, eyeRadius, SSD1306_WHITE);
  display.fillCircle(pupilX, pupilY, pupilRadius, SSD1306_WHITE);
  display.display();
}

void blinkEye() {
  display.clearDisplay();
  display.drawLine(eyeX - eyeRadius, eyeY, eyeX + eyeRadius, eyeY, SSD1306_WHITE); // Draw a closed eye
  display.display();
}

void movePupil() {
  pupilX = eyeX + random(-eyeRadius / 2, eyeRadius / 2);
  pupilY = eyeY + random(-eyeRadius / 2, eyeRadius / 2);
}

void loop() {
  int pirState = digitalRead(pirPin); // Read the PIR sensor state

  if (pirState == HIGH) {
    // When motion is detected, move the pupil
    movePupil();
    Eye();
    digitalWrite(LED_BUILTIN, HIGH); // Draw the eye with the moving pupil
  } else {
    // If no motion detected, keep the eyes closed
    blinkEye(); 
    digitalWrite(LED_BUILTIN, LOW);// Draw the closed eye
  }

  delay(200); // A short delay to prevent rapid updates
}