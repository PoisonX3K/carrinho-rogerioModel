#include "carrinhoFunc.h"

carrinhoFunc::carrinhoFunc()
{
    _afms = Adafruit_MotorShield();
    MotorEsquerdaFrente = nullptr;
    MotorEsquerdaTras   = nullptr;
    MotorDireitaFrente  = nullptr;
    MotorDireitaTras    = nullptr;
}

// Chamar no setup(), no lugar de AFMS.begin() e dos getMotor/setSpeed
void carrinhoFunc::begin()
{
    _afms.begin();

    MotorEsquerdaFrente = _afms.getMotor(1);
    MotorEsquerdaTras   = _afms.getMotor(2);
    MotorDireitaFrente  = _afms.getMotor(3);
    MotorDireitaTras    = _afms.getMotor(4);

    MotorEsquerdaFrente->setSpeed(VELOCIDADE_PADRAO);
    MotorEsquerdaTras->setSpeed(VELOCIDADE_PADRAO);
    MotorDireitaFrente->setSpeed(VELOCIDADE_PADRAO);
    MotorDireitaTras->setSpeed(VELOCIDADE_PADRAO);
}

void carrinhoFunc::frente(int d)
{
    MotorEsquerdaFrente->run(BACKWARD);
    MotorEsquerdaTras->run(BACKWARD);
    MotorDireitaFrente->run(BACKWARD);
    MotorDireitaTras->run(BACKWARD);

    delay(d);
}

void carrinhoFunc::Parar(int d)
{
    MotorEsquerdaFrente->run(RELEASE);
    MotorEsquerdaTras->run(RELEASE);
    MotorDireitaFrente->run(RELEASE);
    MotorDireitaTras->run(RELEASE);

    delay(d);
}

void carrinhoFunc::tras()
{
    MotorEsquerdaFrente->run(FORWARD);
    MotorEsquerdaTras->run(FORWARD);
    MotorDireitaFrente->run(FORWARD);
    MotorDireitaTras->run(FORWARD);
}

void carrinhoFunc::tresBrancos(int d)
{
    MotorDireitaFrente->setSpeed(60);
    MotorEsquerdaFrente->setSpeed(60);

    MotorEsquerdaFrente->run(BACKWARD);
    MotorEsquerdaTras->run(BACKWARD);
    MotorDireitaFrente->run(BACKWARD);
    MotorDireitaTras->run(BACKWARD);

    delay(d);
}

void carrinhoFunc::correcaoEsquerda()
{
    MotorEsquerdaTras->run(BACKWARD);
    MotorEsquerdaFrente->run(BACKWARD);
    MotorDireitaFrente->run(RELEASE);
    MotorDireitaTras->run(RELEASE);

    delay(100);
}

void carrinhoFunc::correcaoDireita()
{
    MotorDireitaFrente->run(BACKWARD);
    MotorDireitaTras->run(BACKWARD);
    MotorEsquerdaFrente->run(RELEASE);
    MotorEsquerdaTras->run(RELEASE);

    delay(100);
}

void carrinhoFunc::CurvaEsquerda()
{
    Parar(250);
    frente(970);

    MotorDireitaFrente->run(BACKWARD);
    MotorDireitaTras->run(BACKWARD);
    MotorEsquerdaFrente->run(FORWARD);
    MotorEsquerdaTras->run(FORWARD);

    delay(2000);
}

void carrinhoFunc::CurvaDireita()
{
    Parar(250);
    frente(950);

    MotorDireitaFrente->run(FORWARD);
    MotorDireitaTras->run(FORWARD);
    MotorEsquerdaFrente->run(BACKWARD);
    MotorEsquerdaTras->run(BACKWARD);

    delay(2000);
}

void carrinhoFunc::Cruzamento()
{
    Parar(250);
    frente(2000);
}

void carrinhoFunc::girarEsquerda45()
{
    MotorDireitaFrente->run(BACKWARD);
    MotorDireitaTras->run(BACKWARD);
    MotorEsquerdaFrente->run(FORWARD);
    MotorEsquerdaTras->run(FORWARD);

    delay(TEMPO_GIRO_45);
    Parar(150);
}

void carrinhoFunc::girarDireita45()
{
    MotorDireitaFrente->run(FORWARD);
    MotorDireitaTras->run(FORWARD);
    MotorEsquerdaFrente->run(BACKWARD);
    MotorEsquerdaTras->run(BACKWARD);

    delay(TEMPO_GIRO_45);
    Parar(150);
}

void carrinhoFunc::girarEsquerda90()
{
    MotorDireitaFrente->run(BACKWARD);
    MotorDireitaTras->run(BACKWARD);
    MotorEsquerdaFrente->run(FORWARD);
    MotorEsquerdaTras->run(FORWARD);

    delay(TEMPO_GIRO_45 * 2);
    Parar(150);
}

void carrinhoFunc::girarDireita90()
{
    MotorDireitaFrente->run(FORWARD);
    MotorDireitaTras->run(FORWARD);
    MotorEsquerdaFrente->run(BACKWARD);
    MotorEsquerdaTras->run(BACKWARD);

    delay(TEMPO_GIRO_45 * 2);
    Parar(150);
}

void carrinhoFunc::curva180()
{
    Parar(250);
    frente(700);

    MotorDireitaFrente->run(BACKWARD);
    MotorDireitaTras->run(BACKWARD);
    MotorEsquerdaFrente->run(FORWARD);
    MotorEsquerdaTras->run(FORWARD);

    delay(4500);
}