#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define VIN 5
#define R 1000
#define DHTPIN 8
#define DHTTYPE DHT11
#define LDRPIN A0

int foto = A0;
int lux;
int sensorValor;
int lim1 = 350;
int lim2 = 600;

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
  sensorValor = analogRead(foto);
  lux = sensorRawToPhys(sensorValor);

  // Exibe o valor do sensor e a luminosidade calculada
  Serial.print(F("Valor do sensor= "));
  Serial.println(sensorValor); // Valor da leitura do sensor
  Serial.print(F("Valor físico do sensor = "));
  Serial.print(lux); // Valor da leitura do sensor em Lumens
  Serial.println(F(" lumen"));

  // LIMPA o LCD
  lcd.clear();

  // -------- CONTROLE DE LUZ AMBIENTE --------
  if (lux < lim1) {
    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_AMARELO, LOW);
    digitalWrite(LED_VERMELHO, LOW);
    digitalWrite(BUZZER, LOW);
    lcd.setCursor(0, 0);
    lcd.print("Ambiente Escuro");
    lcd.setCursor(0, 1);
    lcd.print("LED verde aceso");
  }
  else if (lux >= lim1 && lux < lim2) {
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_AMARELO, HIGH);
    digitalWrite(LED_VERMELHO, LOW);
    digitalWrite(BUZZER, LOW);
    lcd.setCursor(0, 0);
    lcd.print("Ambiente Meio Claro");
    lcd.setCursor(0, 1);
    lcd.print("LED amarelo aceso");
  }
  else {
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_AMARELO, LOW);
    digitalWrite(LED_VERMELHO, HIGH);
    digitalWrite(BUZZER, HIGH);
    lcd.setCursor(0, 0);
    lcd.print("Ambiente Claro");
    lcd.setCursor(0, 1);
    lcd.print("LED vermelho aceso");

    // Toque do buzzer
    tone(BUZZER, 440);
    delay(2000);
    noTone(BUZZER);
    delay(2000);
  }

  delay(5000); // Aguarda 5 segundos antes de atualizar
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

  delay(3000); // Aguarda 3 segundos antes de atualizar as informações no LCD
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
    lcd.clear();
}

// Função de conversão para luminosidade (Lux)
int sensorRawToPhys(int raw) {
  float Volt = float(raw) * (VIN / float(1024));
  float RLDR = (R * (VIN - Volt)) / Volt;
  int phys = 500 / (RLDR / 1000);  // Conversão para lux (ajuste de acordo com o seu sensor)
  return phys;
}
