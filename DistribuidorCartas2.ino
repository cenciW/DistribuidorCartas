#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <Stepper.h>

#define MB1 9
#define MB2 10
#define MB3 11
#define MB4 12
#define LEFT 2
#define ENTER 3
#define RIGHT 4
#define MC 13

enum Estados {
  inicio = 0,
  truco = 1,
  presidente = 2,
  jogo_generico = 3,
  jogo_truco_func = 4,
  jogo_presidente_func = 5,
  jogo_generico_func = 6,
  definir_players = 7,
  definir_cartas = 8
};

Estados estado = inicio;  // Inicialize o estado como Inicio
// Variáveis
const int total_passos = 32 * 64;
int delayB = 20;
int delayC = 2000;
int qtdPlayers = 2;
int qtdCartas = 4;

boolean enterPressionado = false;  // Variável para rastrear o estado do botão ENTER
boolean leftPressionado = false;   // Variável para rastrear o estado do botão LEFT
boolean rightPressionado = false;  // Variável para rastrear o estado do botão RIGHT

//boolean enterPressionado = false;  // Variável para rastrear o estado do botão ENTER
//boolean leftPressionado = false;   // Variável para rastrear o estado do botão LEFT
//boolean rightPressionado = false;  // Variável para rastrear o estado do botão RIGHT

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(LEFT, INPUT_PULLUP);
  pinMode(ENTER, INPUT_PULLUP);
  pinMode(RIGHT, INPUT_PULLUP);
  pinMode(MB1, OUTPUT);
  pinMode(MB2, OUTPUT);
  pinMode(MB3, OUTPUT);
  pinMode(MB4, OUTPUT);
  pinMode(MC, INPUT);
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  estado = inicio;
}


void movimentarBase(int qtdPlayers) {
  for (int d = 0; d <= total_passos / qtdPlayers; d = d + 4) {
    int motorSpeed = 5;  //velocidade max do motor = 5
    // 1

    digitalWrite(MB4, HIGH);
    digitalWrite(MB3, LOW);
    digitalWrite(MB2, LOW);
    digitalWrite(MB1, LOW);
    delay(motorSpeed);
    // 3

    digitalWrite(MB4, LOW);
    digitalWrite(MB3, HIGH);
    digitalWrite(MB2, LOW);
    digitalWrite(MB1, LOW);
    delay(motorSpeed);

    // 5
    digitalWrite(MB4, LOW);
    digitalWrite(MB3, LOW);
    digitalWrite(MB2, HIGH);
    digitalWrite(MB1, LOW);
    delay(motorSpeed);

    // 7
    digitalWrite(MB4, LOW);
    digitalWrite(MB3, LOW);
    digitalWrite(MB2, LOW);
    digitalWrite(MB1, HIGH);
    delay(motorSpeed);
  }
  delay(delayC);
}

void distribuirDc(int atraso, int qtdCartas) {
  for (int i = 0; i < qtdCartas; i++) {
    delay(10);
    digitalWrite(MC, HIGH);
    delay(atraso);
    digitalWrite(MC, LOW);
    delay(2000);
  }
}

void clearMotorBase() {
  digitalWrite(MB4, LOW);
  digitalWrite(MB3, LOW);
  digitalWrite(MB2, LOW);
  digitalWrite(MB1, LOW);
}

void lcdProcessamentoInicio() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Bem-vindo!");
  //delay(500);
  lcd.setCursor(0, 1);
  lcd.print("Pressione Enter");
  delay(500);
}

void lcdProcessamentoTruco() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TRUCO:");
  lcd.setCursor(0, 1);
  lcd.print("Pressione Enter");
  Serial.println("Processamento Truco");
}

void lcdProcessamentoPresidente() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PRESIDENTE:");
  lcd.setCursor(0, 1);
  lcd.print("Pressione Enter");
  Serial.println("Processamento Presidente");
}

void lcdProcessamentoJogo_Generico() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("JOGO GENERICO:");
  lcd.setCursor(0, 1);
  lcd.print("Pressione Enter");
  Serial.println("Processamento Jogo Genérico");
}

void lcdProcessamentoJogando() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Distribuindo...");
  Serial.println("Processamento Jogando");
}

void lcdProcessamentoDefinir_Players(int qtd_player) {
  delay(300);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("- <- APERTE -> +");
  lcd.setCursor(0, 1);
  lcd.print("PLAYERS: ");
  lcd.setCursor(10, 1);
  lcd.print(qtd_player);
  Serial.println("Processamento Define Players");
}

void lcdProcessamentoDefinir_Carta(int qtd_cartas) {
  delay(300);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("- <- APERTE -> +");
  lcd.setCursor(0, 1);
  lcd.print("CARTAS: ");
  lcd.setCursor(9, 1);
  lcd.print(qtd_cartas);
  Serial.println("Processamento Define Carta");
}

boolean debounce(int pinoBotao, boolean &estadoDoBotao) {
  boolean estadoAtual = digitalRead(pinoBotao);
  if (estadoAtual != estadoDoBotao) {
    delay(5);  // Delay de debounce para estabilidade
    estadoAtual = digitalRead(pinoBotao);
    if (estadoAtual != estadoDoBotao) {
      estadoDoBotao = estadoAtual;
      return estadoAtual == LOW;
    }
  }
  return false;
}

void processamentoInicio() {
  delay(200);
  lcdProcessamentoInicio();

  if ((debounce(ENTER, enterPressionado))) {
    estado = truco;
  } else {
    estado = inicio;
  }
}

void processamentoTruco() {
  delay(200);
  lcdProcessamentoTruco();

  if ((debounce(ENTER, enterPressionado))) {
    estado = jogo_truco_func;
  } else if ((debounce(RIGHT, rightPressionado))) {
    estado = presidente;
  } else if ((debounce(LEFT, leftPressionado))) {
    estado = jogo_generico;
  } else {
    estado = truco;
  }
}

void processamentoPresidente() {
  delay(200);
  lcdProcessamentoPresidente();

  if ((debounce(ENTER, enterPressionado))) {
    estado = jogo_presidente_func;
  } else if ((debounce(RIGHT, rightPressionado))) {
    estado = jogo_generico;
  } else if ((debounce(LEFT, leftPressionado))) {
    estado = truco;
  } else {
    estado = presidente;
  }
}

void processamentoJogo_Generico() {
  delay(200);
  lcdProcessamentoJogo_Generico();

  if ((debounce(ENTER, enterPressionado))) {
    estado = definir_players;
  } else if ((debounce(RIGHT, rightPressionado))) {
    estado = truco;
  } else if ((debounce(LEFT, leftPressionado))) {
    estado = presidente;
  } else {
    estado = jogo_generico;
  }
}


void processamentoJogo_Truco_Func() {
  delay(200);
  lcdProcessamentoJogando();

  for (int j = 0; j < 3; j++) {
    delay(300);
    distribuirDc(350, 3);
    movimentarBase(4);  //+ 8 PASSOS
    clearMotorBase();
    delay(delayB);
  }

  distribuirDc(350, 3);
  delay(delayB);
  estado = inicio;
}

void processamentoJogo_Presidente_Func() {
  delay(200);
  lcdProcessamentoJogando();

  for (int j; j < 3; j++) {
    delay(300);
    distribuirDc(350, 12);
    movimentarBase(4);  //+ 8 PASSOS
    clearMotorBase();
    delay(delayB);
  }
  distribuirDc(350, 12);
  delay(delayB);

  estado = inicio;
}

void processamentoJogo_Generico_Func() {
  delay(200);
  lcdProcessamentoJogando();

  for (int i = 0; i < (qtdPlayers - 1); i++) {
    delay(200);
    distribuirDc(250, qtdCartas);
    delay(500);
    movimentarBase(qtdPlayers);
    clearMotorBase();
    delay(1700);
  }
  delay(300);
  distribuirDc(250, qtdCartas);

  estado = inicio;
}

void processamentoDefinir_Player() {

  lcdProcessamentoDefinir_Players(qtdPlayers);

  if ((debounce(ENTER, enterPressionado))) {
    estado = definir_cartas;
  } else if ((debounce(RIGHT, rightPressionado))) {
    qtdPlayers++;
    if (qtdPlayers > 6) {
      qtdPlayers = 6;
    }
  } else if ((debounce(LEFT, leftPressionado))) {
    qtdPlayers--;
    if (qtdPlayers < 0) {
      qtdPlayers = 0;
    }
  } else {
    estado = definir_players;
  }

  lcdProcessamentoDefinir_Players(qtdPlayers);
}

void processamentoDefinir_Carta() {

  lcdProcessamentoDefinir_Carta(qtdCartas);

  if ((debounce(ENTER, enterPressionado))) {
    estado = jogo_generico_func;
  } else if ((debounce(RIGHT, rightPressionado))) {
    qtdCartas++;
    if (qtdCartas > 8) {
      qtdCartas = 8;
    }
  } else if ((debounce(LEFT, leftPressionado))) {
    qtdCartas--;
    if (qtdCartas <= 0) {
      qtdCartas = 0;
    }
  } else {
    estado = definir_cartas;
  }
}


void loop() {
  switch (estado) {
    case inicio:
      processamentoInicio();
      break;
    case truco:
      processamentoTruco();
      break;
    case presidente:
      processamentoPresidente();
      break;
    case jogo_generico:
      processamentoJogo_Generico();
      break;
    case jogo_truco_func:
      processamentoJogo_Truco_Func();
      break;
    case jogo_presidente_func:
      processamentoJogo_Presidente_Func();
      break;
    case jogo_generico_func:
      processamentoJogo_Generico_Func();
      break;
    case definir_players:
      processamentoDefinir_Player();
      break;
    case definir_cartas:
      processamentoDefinir_Carta();
      break;
  }
}
