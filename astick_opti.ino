#include <LiquidCrystal_I2C.h>
#include <WiFi.h>

#define MOVE_BUTTON_PIN 4
#define SELECT_BUTTON_PIN 15

LiquidCrystal_I2C lcd(0x27, 16, 2);

int mode = 0;
unsigned long lastMoveButtonPress = 0;
unsigned long debounceDelay = 500;

void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(MOVE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(SELECT_BUTTON_PIN, INPUT_PULLUP);
  lcd.setCursor(0, 0);
  lcd.print("AStick by uzum");
  lcd.setCursor(0, 1);
  lcd.print("");
  delay(200);
}

void displayDefault() {
  unsigned long milliseconds = millis();
  unsigned long seconds = milliseconds / 1000;
  lcd.setCursor(0, 0);
  lcd.print("AStick v0.1     ");
  lcd.setCursor(0, 1);
  lcd.print(seconds);
  lcd.print("                ");
}

void displayIPAddress() {
  lcd.setCursor(0, 0);
  lcd.print("IP Address:        ");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
}

void displayWifiScan() {
  int numNetworks = WiFi.scanNetworks();
  if (numNetworks == 0) {
    lcd.setCursor(0, 0);
    lcd.print("No networks found");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("WiFi Networks:");
    for (int i = 0; i < numNetworks; ++i) {
      lcd.setCursor(0, 1);
      lcd.print(WiFi.SSID(i));
      delay(2000); // Attendre 2 secondes avant de passer au prochain réseau
    }
  }
}

void loop() {
  int move_button = digitalRead(MOVE_BUTTON_PIN);

  if (move_button == HIGH && (millis() - lastMoveButtonPress) > debounceDelay) {
    mode = (mode + 1) % 2;
    lastMoveButtonPress = millis();
  }

  switch (mode) {
    case 0:
      displayDefault();
      break;
    case 1:
      displayIPAddress();
    case 2:
      displayWifiScan();
      mode = 0; // Revenir au menu par défaut après le scan
      break;
    default:
      mode = 0;
      break;
  }
}
