
#include <Wire.h>
#include <Adafruit_MLX90614.h>

#define TEMPO_MEDICAO_MS 3000   
#define INTERVALO_MS      250 

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
bool sensorOk = false;

void setup() {
  Serial.begin(115200);
  sensorOk = mlx.begin();
  Serial.println(sensorOk ? "PRONTO" : "ERRO: sensor MLX90614 nao encontrado");
}

void medir() {
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
      while (Serial.available() > 0) Serial.read();
    }
  }
}
