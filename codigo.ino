#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 8
#define DHTTYPE DHT22
#define LDRPIN A0

// LEDs e Buzzer
#define LED_VERDE 2
#define LED_AMARELO 3
#define LED_VERMELHO 4
#define BUZZER 5

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.init();
  lcd.backlight();

  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(BUZZER, OUTPUT);
}

void loop() {
  float somaTemp = 0, somaUmid = 0, somaLuz = 0;

  // 5 leituras
  for (int i = 0; i < 5; i++) {
    somaTemp += dht.readTemperature();
    somaUmid += dht.readHumidity();
    somaLuz += analogRead(LDRPIN);
    delay(500); // pequeno intervalo entre leituras
  }

  float temp = somaTemp / 5;
  float umid = somaUmid / 5;
  float luz = somaLuz / 5;

  // LIMPA o LCD
  lcd.clear();

  // -------- CONTROLE DE LUZ AMBIENTE --------
  if (luz < 300) {
    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_AMARELO, LOW);
    digitalWrite(LED_VERMELHO, LOW);
    digitalWrite(BUZZER, LOW);
    // LCD: não precisa mostrar nada específico
  }
  else if (luz >= 300 && luz < 700) {
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_AMARELO, HIGH);
    digitalWrite(LED_VERMELHO, LOW);
    digitalWrite(BUZZER, LOW);
    lcd.setCursor(0, 0);
    lcd.print("Ambiente a");
    lcd.setCursor(0, 1);
    lcd.print("meia luz");
  }
  else if (luz > 700) { // luz forte
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_AMARELO, LOW);
    digitalWrite(LED_VERMELHO, HIGH);
    digitalWrite(BUZZER, HIGH);
    lcd.setCursor(0, 0);
    lcd.print("Ambiente muito");
    lcd.setCursor(0, 1);
    lcd.print("claro");
  }

  delay(5000);
  lcd.clear();

  // -------- CONTROLE DE TEMPERATURA --------
  if (temp >= 10 && temp <= 15) {
    lcd.setCursor(0, 0);
    lcd.print("Temperatura OK");
    lcd.setCursor(0, 1);
    lcd.print(temp);
    lcd.print(" C");
    digitalWrite(LED_AMARELO, LOW);
    digitalWrite(BUZZER, LOW);
  } else {
    digitalWrite(LED_AMARELO, HIGH);
    digitalWrite(BUZZER, HIGH);
    lcd.setCursor(0, 0);
    if (temp > 15) {
      lcd.print("Temp. Alta");
    } else {
      lcd.print("Temp. Baixa");
    }
    lcd.setCursor(0, 1);
    lcd.print(temp);
    lcd.print(" C");
  }

  delay(3000);
  lcd.clear();

  // -------- CONTROLE DE UMIDADE --------
  if (umid >= 50 && umid <= 70) {
    lcd.setCursor(0, 0);
    lcd.print("Umidade OK");
    lcd.setCursor(0, 1);
    lcd.print(umid);
    lcd.print(" %");
    digitalWrite(LED_VERMELHO, LOW);
    digitalWrite(BUZZER, LOW);
  } else {
    digitalWrite(LED_VERMELHO, HIGH);
    digitalWrite(BUZZER, HIGH);
    lcd.setCursor(0, 0);
    if (umid > 70) {
      lcd.print("Umidade Alta");
    } else {
      lcd.print("Umidade Baixa");
    }
    lcd.setCursor(0, 1);
    lcd.print(umid);
    lcd.print(" %");
  }

  delay(5000); // Espera 5s antes de repetir tudo
}
