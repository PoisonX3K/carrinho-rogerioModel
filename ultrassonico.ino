#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // Largura do display OLED
#define SCREEN_HEIGHT 64 // Altura do display OLED
#define OLED_RESET    -1 // Reset pin # 

// Definição dos pinos do sensor ultrassônico HC-SR04
const int pinoTrig = 4;
const int pinoEcho = 5;

// Instância do display OLED
Adafruit_SSD1306 displayLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(115200);

  // Configuração dos pinos do ultrassônico
  pinMode(pinoTrig, OUTPUT);
  pinMode(pinoEcho, INPUT);

  // Inicializa o display OLED no endereço I2C 0x3C
  if(!displayLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Falha ao alocar SSD1306"));
    for(;;);
  }
  
  displayLED.clearDisplay();
  displayLED.display();
}

void loop() {
  // Variáveis para cálculo da distância
  long duracao;
  float distancia;

  // Envia um pulso sonoro (Trigger)
  digitalWrite(pinoTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(pinoTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinoTrig, LOW);

  // Lê o tempo que o eco demorou para voltar (Echo)
  duracao = pulseIn(pinoEcho, HIGH);

  // Calcula a distância em centímetros (Velocidade do som = 0.0343 cm/us, dividido por 2 ida e volta)
  distancia = (duracao * 0.0343) / 2;

  // Atualiza o display OLED
  displayLED.clearDisplay();
  
  // Título (Parte superior / amarela)
  displayLED.setTextColor(SSD1306_WHITE);
  displayLED.setTextSize(1);
  displayLED.setCursor(0, 0); 
  displayLED.print("SENSOR ULTRASSONICO");

  // Valor da distância (Parte inferior / azul, em destaque)
  displayLED.setCursor(0, 25); 
  displayLED.setTextSize(2); 
  displayLED.print("Dist:");
  
  displayLED.setCursor(60, 25);
  displayLED.print(distancia, 1); // Mostra com 1 casa decimal
  
  displayLED.setTextSize(1);
  displayLED.setCursor(110, 30);
  displayLED.print("cm");

  // Envia para o display
  displayLED.display();
  
  delay(300); // Aguarda um breve momento antes da próxima medição
}