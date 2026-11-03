// InfraCow - ESP32 + MLX90614 (versão simples)
//
// Como funciona:
//   1. O app manda o texto "MEDIR" pelo cabo USB.
//   2. O ESP32 lê o sensor durante TEMPO_MEDICAO_MS (3 s) e tira a média.
//   3. Responde UMA linha JSON, no formato que o app já entende:
//      {"leituras":[{"objeto_C":36.52,"ambiente_C":27.10}]}
//
// Ligação do sensor (ESP32 DevKit):
//   VIN -> 3V3      GND -> GND      SDA -> GPIO 21      SCL -> GPIO 22
//
// Biblioteca necessária: "Adafruit MLX90614 Library" (Gerenciador de Bibliotecas).
//
// Teste sem o app: abra o Monitor Serial em 115200, "Nova linha" (Newline),
// digite MEDIR e envie.

#include <Wire.h>
#include <Adafruit_MLX90614.h>

#define TEMPO_MEDICAO_MS 3000   // quanto tempo medindo antes de dar o valor final (3000 = 3 s, 5000 = 5 s)
#define INTERVALO_MS      250   // intervalo entre leituras dentro desse tempo

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
bool sensorOk = false;

void setup() {
  Serial.begin(115200);
  sensorOk = mlx.begin();
  Serial.println(sensorOk ? "PRONTO" : "ERRO: sensor MLX90614 nao encontrado");
}

void medir() {
  // Se o sensor não foi achado na inicialização (solda ruim, fio solto...),
  // tenta de novo agora, sem precisar reiniciar a placa.
  if (!sensorOk) sensorOk = mlx.begin();
  if (!sensorOk) {
    Serial.println("ERRO: sensor indisponivel");
    return;
  }

  double somaObjeto = 0, somaAmbiente = 0;
  int validas = 0;
  unsigned long inicio = millis();

  while (millis() - inicio < TEMPO_MEDICAO_MS) {
    double obj = mlx.readObjectTempC();
    double amb = mlx.readAmbientTempC();
    if (!isnan(obj) && !isnan(amb)) {
      somaObjeto += obj;
      somaAmbiente += amb;
      validas++;
    }
    delay(INTERVALO_MS);
  }

  if (validas == 0) {
    Serial.println("ERRO: leitura invalida do sensor");
    return;
  }

  Serial.print("{\"leituras\":[{\"objeto_C\":");
  Serial.print(somaObjeto / validas, 2);
  Serial.print(",\"ambiente_C\":");
  Serial.print(somaAmbiente / validas, 2);
  Serial.println("}]}");
  Serial.flush();
}

void loop() {
  if (Serial.available() > 0) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();

    if (cmd == "MEDIR") {
      medir();
      // Descarta comandos que chegaram durante a medição (o app reenvia
      // "MEDIR" se demorar), para não disparar uma medição fantasma.
      while (Serial.available() > 0) Serial.read();
    }
  }
}
