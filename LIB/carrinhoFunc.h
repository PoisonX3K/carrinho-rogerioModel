#ifndef carrinhoFunc_h
#define carrinhoFunc_h

#include <Arduino.h>
#include <Adafruit_MotorShield.h>

// Tempo de giro para ~45 graus (ajuste conforme carrinho, bateria e piso)
#define TEMPO_GIRO_45 300
#define VELOCIDADE_PADRAO 100

class carrinhoFunc
{
  public:
    carrinhoFunc();
    void begin();

    void frente(int d);
    void Parar(int d);
    void tras();
    void tresBrancos(int d);

    void correcaoEsquerda();
    void correcaoDireita();
    void CurvaEsquerda();
    void CurvaDireita();
    void Cruzamento();
    void curva180();

    void girarEsquerda45();
    void girarDireita45();
    void girarEsquerda90();
    void girarDireita90();

  private:
    Adafruit_MotorShield _afms;
    Adafruit_DCMotor *MotorEsquerdaFrente;
    Adafruit_DCMotor *MotorEsquerdaTras;
    Adafruit_DCMotor *MotorDireitaFrente;
    Adafruit_DCMotor *MotorDireitaTras;
};

#endif
