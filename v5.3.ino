#include <Dabble.h>
#include <Adafruit_MotorShield.h>
#include <Wire.h>
#include <Arduino.h>
#define CUSTOM_SETTINGS //Configuração para uso da aplicação para controle do LED
#define INCLUDE_LEDCONTROL_MODULE

#define sensorEsquerda 8
#define sensorDireita 7
#define sensorCentral 6

// Ultrassônico HC-SR04
#define trigPin 10
#define echoPin 11
#define DISTANCIA_MINIMA 25 //Distância mínima para considerar o obstáculo

int AnalogicoEsquerda = A0;
int valorDigitalEsquerda;
int valorAnalogicoEsquerda;

int AnalogicoDireita = A2;
int valorDigitalDireita;
int valorAnalogicoDireita;

int AnalogicoCentral = A1;
int valorDigitalCentral;
int valorAnalogicoCentral;

float duration, distance;

int leituraVerde = 0, leituraPreto = 0;
int cont = 0;
int verificao = 0;
int verde_min = 89, verde_max = 325;

bool BRANCO = 0, PRETO = 1;

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *MotorEsquerdaFrente = AFMS.getMotor(1);
Adafruit_DCMotor *MotorEsquerdaTras = AFMS.getMotor(2);
Adafruit_DCMotor *MotorDireitaFrente = AFMS.getMotor(3);
Adafruit_DCMotor *MotorDireitaTras = AFMS.getMotor(4);

// Funções
void frente(int d);
void Parar(int d);
void tras();
void correcaoEsquerda();
void correcaoDireita();
void leituraSensorInfra();
void leituraSensorUltrassonico();
void tresBrancos(int d);
void CurvaEsquerda();
void CurvaDireita(int d);
void Cruzamento();
void curva180();
void desvioObstaculo();
void girarEsquerda45();
void girarDireita45();
void girarDireita90();
void girarEsquerda90();

// MOVIMENTO PARA FRENTE
void frente(int d)
{
  MotorEsquerdaFrente->run(BACKWARD);
  MotorEsquerdaTras->run(BACKWARD);

  MotorDireitaFrente->run(BACKWARD);
  MotorDireitaTras->run(BACKWARD);

  delay(d);
}

// MOVIMENTO PARA PARAR
void Parar(int d)
{
  MotorEsquerdaFrente->run(RELEASE);
  MotorEsquerdaTras->run(RELEASE);

  MotorDireitaFrente->run(RELEASE);
  MotorDireitaTras->run(RELEASE);
  delay(d);
}

// MOVIMENTO DE RÉ
void tras()
{
  MotorEsquerdaFrente->run(FORWARD);
  MotorEsquerdaTras->run(FORWARD);

  MotorDireitaFrente->run(FORWARD);
  MotorDireitaTras->run(FORWARD);
}

// TRÊS BRANCOS
void tresBrancos(int d)
{
  MotorDireitaFrente->setSpeed(60);
  MotorEsquerdaFrente->setSpeed(60);

  MotorEsquerdaFrente->run(BACKWARD);
  MotorEsquerdaTras->run(BACKWARD);

  MotorDireitaFrente->run(BACKWARD);
  MotorDireitaTras->run(BACKWARD);

  delay(d);
}

// CORREÇÃO PARA ESQUERDA
void correcaoEsquerda()
{
  MotorEsquerdaTras->run(BACKWARD);
  MotorEsquerdaFrente->run(BACKWARD);

  MotorDireitaFrente->run(RELEASE);
  MotorDireitaTras->run(RELEASE);
  delay(100);

  leituraSensorInfra();
}

// CORREÇÃO PARA DIREITA
void correcaoDireita()
{
  MotorDireitaFrente->run(BACKWARD);
  MotorDireitaTras->run(BACKWARD);

  MotorEsquerdaFrente->run(RELEASE);
  MotorEsquerdaTras->run(RELEASE);
  delay(100);

  leituraSensorInfra();
}

// CURVA ESQUERDA
void CurvaEsquerda()
{
  Parar(250);

  frente(970);

  MotorDireitaFrente->run(BACKWARD);
  MotorDireitaTras->run(BACKWARD);

  MotorEsquerdaFrente->run(FORWARD);
  MotorEsquerdaTras->run(FORWARD);

  delay(2000);
}

// CURVA DIREITA
void CurvaDireita(int d)
{
  Parar(250);

  frente(950);

  MotorDireitaFrente->run(FORWARD);
  MotorDireitaTras->run(FORWARD);

  MotorEsquerdaFrente->run(BACKWARD);
  MotorEsquerdaTras->run(BACKWARD);

  delay(d);
}

// CRUZAMENTO
void Cruzamento()
{
  Parar(250);
  frente(2000);
}

// CURVA 180°
void curva180()
{
  Parar(250);

  frente(700);

  MotorDireitaFrente->run(BACKWARD);
  MotorDireitaTras->run(BACKWARD);

  MotorEsquerdaFrente->run(FORWARD);
  MotorEsquerdaTras->run(FORWARD);

  delay(4500);
}

void setup()
{
  AFMS.begin();

  MotorEsquerdaFrente->setSpeed(100);
  MotorEsquerdaTras->setSpeed(100);
  MotorDireitaFrente->setSpeed(100);
  MotorDireitaTras->setSpeed(100);

  Serial.begin(9600);

  pinMode(sensorEsquerda, INPUT);
  pinMode(AnalogicoEsquerda, INPUT);

  pinMode(sensorDireita, INPUT);
  pinMode(AnalogicoDireita, INPUT);

  pinMode(sensorCentral, INPUT);
  pinMode(AnalogicoCentral, INPUT);

  // Ultrassônico
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  
  Dabble.begin(9600); //Inicia a comunicação via Bluetooth com o Dabble
}

void loop()
{
  Dabble.processInput();

/* -------------------- ARRUMA AQUI MAYKEL -------------------------
  int valorLidoAnalogicoDireita = analogRead(AnalogicoDireita);

  Terminal.print("Valor do Sensor IV: ");
  Terminal.println(valor);

  // Imprime também no monitor serial do PC para acompanhar
  Serial.print("Sensor IV: ");
  Serial.println(valorSensor);

  delay(500);
 ----------------------------------------------------------------*/

  // LEITURA DO ULTRASSÔNICO
  leituraSensorUltrassonico();

  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println("cm");

  // VERIFICA OBSTÁCULO
  if (distance > 0 && distance <= DISTANCIA_MINIMA)
  {
    Parar(100);
    desvioObstaculo();
    Serial.println("OBSTACULO DETECTADO!");

    // Fica parado enquanto houver obstáculo
    while (true)
    {
      leituraSensorUltrassonico();

      Serial.print("Distancia: ");
      Serial.print(distance);
      Serial.println("cm");

      if (distance > DISTANCIA_MINIMA || distance <= 0)
      {
        break;
      }
      Parar(50);
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

  // LÓGICA DO SEGUIDOR DE LINHA (COMPLETO)

  if ((valorDigitalEsquerda == BRANCO) && (valorDigitalCentral == PRETO) && (valorDigitalDireita == BRANCO))
  {
    frente(100);
    Parar(0);
    leituraSensorInfra();
  }
  else if ((valorDigitalEsquerda == BRANCO) && (valorDigitalCentral == BRANCO) && (valorDigitalDireita == PRETO))
  {
    Parar(500);
    correcaoEsquerda();
    leituraSensorInfra();
  }
  else if ((valorDigitalEsquerda == PRETO) && (valorDigitalCentral == BRANCO) && (valorDigitalDireita == BRANCO))
  {
    Parar(500);
    correcaoDireita();
    leituraSensorInfra();
  }
  else if ((valorDigitalEsquerda == BRANCO) && (valorDigitalCentral == BRANCO) && (valorDigitalDireita == BRANCO))
  {
    Parar(20);
    tresBrancos(20);
    leituraSensorInfra();
  }
  else if ((valorDigitalEsquerda == PRETO) && (valorDigitalCentral == PRETO) && (valorDigitalDireita == BRANCO))
  {
    Parar(700);
    CurvaEsquerda();
  }
  else if ((valorDigitalEsquerda == BRANCO) && (valorDigitalCentral == PRETO) && (valorDigitalDireita == PRETO))
  {
    while ((valorDigitalEsquerda != BRANCO) && (valorDigitalCentral == PRETO))
    {
      CurvaDireita(200);
      Parar(200);
      leituraSensorInfra();
      Parar(200);
    }
  }
  else if ((valorDigitalEsquerda == PRETO) && (valorDigitalCentral == PRETO) && (valorDigitalDireita == PRETO))
  {
    Parar(1000);

    frente(100);

    Parar(500);

    leituraSensorInfra();

    if ((valorDigitalEsquerda == BRANCO) && (valorDigitalCentral == PRETO) && (valorDigitalDireita == BRANCO))
    {
      frente(450);
    }

    else if ((valorDigitalEsquerda == BRANCO) && (valorDigitalCentral == BRANCO) && (valorDigitalDireita == BRANCO))
    {
      curva180();
    }
  }
  else
  {
    leituraSensorInfra();
  }
}

// SENSORES INFRAVERMELHOS
void leituraSensorInfra()
{
  valorDigitalEsquerda = digitalRead(sensorEsquerda);
  valorDigitalCentral = digitalRead(sensorCentral);
  valorDigitalDireita = digitalRead(sensorDireita);

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
  if (duration == 0)
  {
    distance = 999;
  }
  else
  {
    distance = (duration * 0.0343) / 2;
  }
}


/*============================
  VERIFICAÇÃO ANGULAR ULTRASSONICO
============================*/

// Tempo inicial de calibracao para aproximadamente 45 graus.
// Deve ser ajustado conforme o carrinho, bateria e piso.
const int TEMPO_GIRO_45 = 800;

void girarEsquerda45()
{
  MotorDireitaFrente->run(BACKWARD);
  MotorDireitaTras->run(BACKWARD);
  MotorEsquerdaFrente->run(FORWARD);
  MotorEsquerdaTras->run(FORWARD);

  delay(TEMPO_GIRO_45);
  Parar(150);
}
void girarDireita45()
{
  MotorDireitaFrente->run(FORWARD);
  MotorDireitaTras->run(FORWARD);
  MotorEsquerdaFrente->run(BACKWARD);
  MotorEsquerdaTras->run(BACKWARD);

  delay(TEMPO_GIRO_45);
  Parar(150);
}
void girarDireita90()
{
  MotorDireitaFrente->run(FORWARD);
  MotorDireitaTras->run(FORWARD);
  MotorEsquerdaFrente->run(BACKWARD);
  MotorEsquerdaTras->run(BACKWARD);

  delay(TEMPO_GIRO_45 * 2);
  Parar(150);
}
void girarEsquerda90()
{
  MotorDireitaFrente->run(BACKWARD);
  MotorDireitaTras->run(BACKWARD);
  MotorEsquerdaFrente->run(FORWARD);
  MotorEsquerdaTras->run(FORWARD);

  delay(TEMPO_GIRO_45 * 2);
  Parar(150);
}

void desvioObstaculo()
{
  float distanciaEsquerda;
  float distanciaDireita;

  // 1. Gira aproximadamente 45 graus para a esquerda e mede.
  Serial.println("Verificando esquerda...");
  girarEsquerda45();

  leituraSensorUltrassonico();
  distanciaEsquerda = distance;

  Serial.print("Distancia esquerda: ");
  Serial.println(distanciaEsquerda);

  // 2. Da posicao a esquerda, gira 90 graus para a direita.
  // Agora o carrinho aponta aproximadamente 45 graus para a direita.
  Serial.println("Verificando direita...");
  girarDireita90();

  leituraSensorUltrassonico();
  distanciaDireita = distance;

  Serial.print("Distancia direita: ");
  Serial.println(distanciaDireita);

  // 3. Escolhe o lado com maior distancia medida.
  if (distanciaEsquerda > distanciaDireita) {
    Serial.println("Escolhendo esquerda");

    // Sai da orientacao de 45 graus a direita e vai para 45 graus a esquerda.
    girarEsquerda90();
  } else {
    Serial.println("Escolhendo direita");
    // Ja esta apontado aproximadamente 45 graus para a direita.
  }

  // 4. Avanca na direcao escolhida.
  frente(600);
  Parar(100);
}