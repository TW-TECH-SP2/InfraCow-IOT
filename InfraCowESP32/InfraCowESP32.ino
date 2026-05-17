#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <ArduinoJson.h>

#define DURACAO_MS       10000
#define INTERVALO_MS     200

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  Serial.begin(115200);
  if(!mlx.begin()){
    Serial.println("Sensor com Mal Funcionamentoo");
  }
  mlx.begin();
}

void loop() {
  if (Serial.available() > 0) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();

    if (cmd == "MEDIR") {
      StaticJsonDocument<4096> doc;
      JsonArray leituras = doc.createNestedArray("leituras");

      unsigned long inicio = millis();
      while (millis() - inicio < DURACAO_MS) {
        JsonObject amostra = leituras.createNestedObject();
        amostra["objeto_C"]   = mlx.readObjectTempC();
        amostra["ambiente_C"] = mlx.readAmbientTempC();
        delay(INTERVALO_MS);
      }

      serializeJson(doc, Serial);
      Serial.println();
    }
  }
}
