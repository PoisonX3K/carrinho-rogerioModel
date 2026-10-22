#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // Largura do display OLED
#define SCREEN_HEIGHT 64 // Altura do display OLED
#define OLED_RESET    -1 // Reset pin # 

#define PINO_SENSOR 5

int valorSensor = 0;

Adafruit_SSD1306 displayLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(115200);

  // Inicializa o display OLED no endereço I2C 0x3C
  if(!displayLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Falha ao alocar SSD1306"));
    for(;;);
  }
  
  displayLED.clearDisplay();
  displayLED.display();

  pinMode(PINO_SENSOR, INPUT);
}

void loop() {
  // Lê o estado do sensor digital (HIGH ou LOW / 1 ou 0)
  valorSensor = digitalRead(PINO_SENSOR);
  

  // Limpa o display para a nova leitura
  displayLED.clearDisplay();
  
  // Configurações do texto
  displayLED.setTextColor(SSD1306_WHITE);
  displayLED.setTextSize(2); // Tamanho maior para facilitar a leitura
  
  // Parte amarela do display (topo)
  displayLED.setCursor(0, 0); 
  displayLED.print("SENSOR:");

  // Parte azul do display (abaixo)
  displayLED.setCursor(0, 30); 
  displayLED.print("Valor: ");
  displayLED.print(valorSensor);

  // Envia tudo para o display de uma vez só
  displayLED.display();
  
  delay(200); // Pequeno atraso para atualizar a leitura suavemente
}