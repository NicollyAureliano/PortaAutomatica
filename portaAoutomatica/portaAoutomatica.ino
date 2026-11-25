
#include <Servo.h>

// Definindo os pinos dos componentes
const int TRIG = 2; // Trig do sensor
const int ECHO = 3; // echo do sensor 
const int SENSOR_IR = 4;
const int BUZZER = 6;
const int MOTOR = 9;

// Frequencias do som Ding-Dong
const int DING = 2000;
const int DONG = 1500;
const int TEMPO_FECHAMENTO = 3000;
const int TEMPO_SOM = 150;

// Criando o servo motor
Servo motor;

bool portaAberta = false;
unsigned long tempoInicial = 0;
unsigned long tempoBuzzer = 0;
bool buzzerLigado = false;

int etapaBuzzer = 0;

// Funcao para iniciar o som Ding-Dong
void iniciarDingDong() {
  tone(BUZZER, DING);
  buzzerLigado = true;
  etapaBuzzer = 1;
  tempoBuzzer = millis();
}

void setup() {
  // Configurando os pinos
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(SENSOR_IR, INPUT);
  pinMode(BUZZER, OUTPUT);

  // Iniciando os componentes
  motor.attach(MOTOR);
  motor.write(0);

  digitalWrite(BUZZER, LOW);
}

void loop() {

  // Medindo a distancia
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG, LOW);

  long tempo = pulseIn(ECHO, HIGH, 30000);
  float distancia = tempo * 0.0343 / 2;

  // Verificando o sensor infravermelho
  bool obstaculo = digitalRead(SENSOR_IR) == LOW;

  // Verificando se alguem esta perto
  bool pessoaPerto = distancia > 0 && distancia <= 5;

  // Se detectar alguem, abre a porta
  if (pessoaPerto || obstaculo) {

    if (!portaAberta) {
      motor.write(90);
      portaAberta = true;

  // Toca o Ding-Dong na abertura
     iniciarDingDong();
    }

    // Reinicia a contagem do fechamento
    tempoInicial = millis();
  }

  // Se a porta estiver aberta e nao houver obstaculos
  else if (portaAberta) {

    // Espera 3 segundos antes de fechar
    if (millis() - tempoInicial >= TEMPO_FECHAMENTO) {

      // Verifica novamente se existe obstaculo
      if (digitalRead(SENSOR_IR) == HIGH) {

        motor.write(0);
        portaAberta = false;

     // Toca o Ding-Dong no fechamento
       iniciarDingDong();
      }
    }
  }

  // Controle do som Ding-Dong
if (buzzerLigado) {

  // Primeira nota: DING
  if (etapaBuzzer == 1 && millis() - tempoBuzzer >= 180) {
    noTone(BUZZER);
    etapaBuzzer = 2;
    tempoBuzzer = millis();
  }

  // Intervalo entre as notas
  else if (etapaBuzzer == 2 && millis() - tempoBuzzer >= 100) {
    tone(BUZZER, DONG);
    etapaBuzzer = 3;
    tempoBuzzer = millis();
  }

  // Segunda nota: DONG
  else if (etapaBuzzer == 3 && millis() - tempoBuzzer >= 350) {
    noTone(BUZZER);
    buzzerLigado = false;
    etapaBuzzer = 0;
  }
}
}
