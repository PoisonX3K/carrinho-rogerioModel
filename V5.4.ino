#include <Wire.h>
#include <Arduino.h>
#include "carrinhoFunc.h"

#define sensorEsquerda 8
#define sensorDireite 7
#define sensorCentral 6

// Ultrassônico HC-SR04
#define trigPin 10
#define echoPin 11

// Distância mínima para considerar obstáculo
#define DISTANCIA_MINIMA 25

carrinhoFunc carro;

int AnalogicoEsquerda = A0;
int valorDigitalEsquerda;
int valorAnalogicoEsquerda;

int AnalogicoCentral = A1;
int valorDigitalCentral;
int valorAnalogicoCentral;

int AnalogicoDireita = A2;
int valorDigitalDireita;
int valorAnalogicoDireita;

float duration, distance;

int leituraVerde = 0, leituraPreto = 0;
int cont = 0;
int verificao = 0;
int verde_min = 89, verde_max = 325;

bool BRANCO = 0, PRETO = 1;

// Declaração das funções que ficaram no sketch
void leituraSensorInfra();
void leituraSensorUltrassonico();
void desvioObstaculo();

void setup()
{
  carro.begin();
  Serial.begin(9600);

  pinMode(sensorEsquerda, INPUT);
  pinMode(AnalogicoEsquerda, INPUT);

  pinMode(sensorDireite, INPUT);
  pinMode(AnalogicoDireita, INPUT);

  pinMode(sensorCentral, INPUT);
  pinMode(AnalogicoCentral, INPUT);

  // Ultrassônico
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}


void loop()
{
  // LEITURA DO ULTRASSÔNICO
  leituraSensorUltrassonico();

  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");

  // VERIFICA OBSTÁCULO
  if (distance > 0 && distance <= DISTANCIA_MINIMA)
  {
    // Obstáculo encontrado
    carro.Parar(100);
    desvioObstaculo();

    Serial.println("OBSTACULO DETECTADO!");

    // Fica parado enquanto houver obstáculo
    while (true)
    {
      leituraSensorUltrassonico();

      Serial.print("Distancia: ");
      Serial.print(distance);
      Serial.println(" cm");

      if (distance > DISTANCIA_MINIMA || distance <= 0)
      {
        break;
      }

      carro.Parar(50);
    }

    Serial.println("Caminho livre!");
  }

  // LEITURA DOS SENSORES INFRAVERMELHOS
  leituraSensorInfra();

  Serial.print("Esquerda: ");
  Serial.println(valorAnalogicoEsquerda);

  Serial.print("\nDireita: ");
  Serial.println(valorAnalogicoDireita);

  delay(100);

  // LÓGICA DO SEGUE-LINHA
  if ((valorDigitalEsquerda == BRANCO) && (valorDigitalCentral == PRETO) && (valorDigitalDireita == BRANCO))
  {
    carro.frente(100);
    carro.Parar(0);
    leituraSensorInfra();
  }

  else if ((valorDigitalEsquerda == BRANCO) && (valorDigitalCentral == BRANCO) && (valorDigitalDireita == PRETO))
  {
    carro.Parar(500);
    carro.correcaoEsquerda();
    leituraSensorInfra();
  }

  else if ((valorDigitalEsquerda == PRETO) && (valorDigitalCentral == BRANCO) && (valorDigitalDireita == BRANCO))
  {
    carro.Parar(500);
    carro.correcaoDireita();
    leituraSensorInfra();
  }

  else if ((valorDigitalEsquerda == BRANCO) && (valorDigitalCentral == BRANCO) && (valorDigitalDireita == BRANCO))
  {
    carro.Parar(20);
    carro.tresBrancos(20);
    leituraSensorInfra();
  }

  else if ((valorDigitalEsquerda == PRETO) && (valorDigitalCentral == PRETO) && (valorDigitalDireita == BRANCO))
  {
    carro.Parar(700);
    carro.CurvaEsquerda();
  }

  else if ((valorDigitalEsquerda == BRANCO) && (valorDigitalCentral == PRETO) && (valorDigitalDireita == PRETO))
  {
    while ((valorDigitalEsquerda != BRANCO) && (valorDigitalCentral == PRETO))
    {
      carro.CurvaDireita();
      carro.Parar(200);
      leituraSensorInfra();
      carro.Parar(200);
    }
  }

  else if ((valorDigitalEsquerda == PRETO) && (valorDigitalCentral == PRETO) && (valorDigitalDireita == PRETO))
  {
    carro.Parar(1000);

    carro.frente(100);

    carro.Parar(500);

    leituraSensorInfra();

    if ((valorDigitalEsquerda == BRANCO) && (valorDigitalCentral == PRETO) && (valorDigitalDireita == BRANCO))
    {
      carro.frente(450);
    }

    else if ((valorDigitalEsquerda == BRANCO) && (valorDigitalCentral == BRANCO) && (valorDigitalDireita == BRANCO))
    {
      carro.curva180();
    }
  }

  else
  {
    leituraSensorInfra();
  }
}


// ======================================================
// SENSORES INFRAVERMELHOS
// ======================================================

void leituraSensorInfra()
{
  valorDigitalEsquerda = digitalRead(sensorEsquerda);
  valorDigitalCentral = digitalRead(sensorCentral);
  valorDigitalDireita = digitalRead(sensorDireite);

  valorAnalogicoEsquerda = analogRead(AnalogicoEsquerda);
  valorAnalogicoCentral = analogRead(AnalogicoCentral);
  valorAnalogicoDireita = analogRead(AnalogicoDireita);
}

// ULTRASSÔNICO HC-SR04
void leituraSensorUltrassonico()
{

  // Garante que o TRIG comece desligado
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Pulso de 10 microssegundos
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Mede o tempo do ECHO
  duration = pulseIn(echoPin, HIGH, 30000);

  // Se não recebeu eco
  if (duration == 0) {
    distance = 999;
  }
  else {
    distance = (duration * 0.0343) / 2;
  }
}

// DESVIO DE OBSTÁCULO
void desvioObstaculo()
{
  float distanciaEsquerda;
  float distanciaDireita;

  // 1. Gira aproximadamente 45 graus para a esquerda e mede.
  Serial.println("Verificando esquerda...");
  carro.girarEsquerda45();

  leituraSensorUltrassonico();
  distanciaEsquerda = distance;

  Serial.print("Distancia esquerda: ");
  Serial.println(distanciaEsquerda);

  // 2. Da posicao a esquerda, gira 90 graus para a direita.
  // Agora o carrinho aponta aproximadamente 45 graus para a direita.
  Serial.println("Verificando direita...");
  carro.girarDireita90();

  leituraSensorUltrassonico();
  distanciaDireita = distance;

  Serial.print("Distancia direita: ");
  Serial.println(distanciaDireita);

  // 3. Escolhe o lado com maior distancia medida.
  if (distanciaEsquerda > distanciaDireita) {
    Serial.println("Escolhendo esquerda");

    // Sai da orientacao de 45 graus a direita e vai para 45 graus a esquerda.
    carro.girarEsquerda90();
  } else {
    Serial.println("Escolhendo direita");
    // Ja esta apontado aproximadamente 45 graus para a direita.
  }

  // 4. Avanca na direcao escolhida.
  carro.frente(600);
  carro.Parar(100);
}
