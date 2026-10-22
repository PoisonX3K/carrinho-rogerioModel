#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // Largura do display OLED
#define SCREEN_HEIGHT 64 // Altura do display OLED
#define OLED_RESET    -1 // Reset pin # 

Adafruit_SSD1306 displayLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  // Inicializa o display OLED no endereço I2C 0x3C
  if(!displayLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Falha ao alocar SSD1306"));
    for(;;);
  }
  
  displayLED.clearDisplay();
}

void loop() {
  displayLED.clearDisplay();
  displayLED.setTextColor(WHITE);
  displayLED.setTextSize(1);
  
  // Posicionado na parte AZUL (abaixo da linha 16)
  displayLED.setCursor(10, 20); 
  displayLED.print("TE AMO!!!");
  displayLED.display();
    //parte amarela
  displayLED.setCursor(0, 8); 
  displayLED.print("  NN !");
  displayLED.display();
  
  delay(2000);
  
  displayLED.clearDisplay();
  
  // Posicionado na parte AMARELA (no topo, entre 0 e 15) 
  // eu coloquei no azul pq estava dando conflito com a cor amarela
  displayLED.setCursor(10, 20); 
  displayLED.print("Mais que tudo!!"); 
  displayLED.display();
    //parte amarela
  displayLED.setCursor(0, 8); 
  displayLED.print("  NN !");
  displayLED.display(); 
  
  delay(2000);
  
  displayLED.clearDisplay();
  displayLED.setCursor(10, 20); 
  displayLED.print("Meu Amor !!!");
  displayLED.display();
  

  //parte amarela
  displayLED.setCursor(0, 8); 
  displayLED.print("  NN !");
  displayLED.display();
  
  delay(2000);
}