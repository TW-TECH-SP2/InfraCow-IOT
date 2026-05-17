# InfraCow-IOT

## Pré-requisitos

- [Arduino IDE](https://www.arduino.cc/en/software) instalado
- ESP32 conectado ao computador via USB
- Sensor MLX90614

---

## 1. Instalando o suporte à placa ESP32 (Expressif Systems)

O Arduino IDE não vem com suporte ao ESP32 por padrão. É necessário adicionar o repositório da Expressif Systems manualmente.

### 1.1 Abrir as preferências

- No Windows/Linux: vá em **File → Preferences**
- No macOS: vá em **Arduino IDE → Preferences**

### 1.2 Adicionar a URL do pacote ESP32

No campo **"Additional boards manager URLs"**, cole a URL abaixo:

```
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

Se já houver outra URL no campo, clique no ícone à direita do campo para abrir a lista e adicione a URL em uma nova linha. Clique em **OK** para salvar.

### 1.3 Abrir o Boards Manager

- Vá em **Tools → Board → Boards Manager...**

### 1.4 Instalar o pacote ESP32

- Na barra de busca, digite `esp32`
- Localize o pacote chamado **"esp32"** com o autor **Espressif Systems**
- Clique em **Install** e aguarde o download concluir (pode demorar alguns minutos)

### 1.5 Selecionar a placa correta

- Vá em **Tools → Board → esp32 → ESP32 Dev Module**

### 1.6 Selecionar a porta serial

- Conecte o ESP32 via USB
- Vá em **Tools → Port** e selecione a porta correspondente ao seu ESP32
  - No Windows aparece como `COM3`, `COM4`, etc.
  - No Linux/macOS aparece como `/dev/ttyUSB0` ou `/dev/tty.usbserial-...`

---

## 2. Instalando as bibliotecas necessárias

### 2.1 Abrir o Library Manager

- Vá em **Sketch → Include Library → Manage Libraries...**
- Aguarde o índice de bibliotecas carregar

### 2.2 Bibliotecas a instalar

Instale as duas bibliotecas abaixo buscando exatamente pelo nome indicado:

---

#### Adafruit MLX90614 Library
- **Busque por:** `MLX90614`
- **Nome exato:** `Adafruit MLX90614 Library`
- **Autor:** `Adafruit`
- Clique em **Install**
- Caso o Arduino IDE pergunte sobre dependências, clique em **Install All**

---

#### ArduinoJson
- **Busque por:** `ArduinoJson`
- **Nome exato:** `ArduinoJson`
- **Autor:** `Benoit Blanchon`
- Clique em **Install**

---

## 3. Conexões do hardware

| MLX90614 | ESP32       |
|----------|-------------|
| VCC      | 3.3V        |
| GND      | GND         |
| SDA      | GPIO 21     |
| SCL      | GPIO 22     |

---

## 4. Como usar

1. Faça o upload do código para o ESP32
2. Abra o **Serial Monitor** em **Tools → Serial Monitor**
3. Configure o baud rate para **115200**
4. Configure o terminador de linha para **Newline**
5. Digite `MEDIR` e pressione **Enter**
6. O ESP32 coletará leituras por 10 segundos e retornará o resultado em JSON

### Exemplo de saída

```json
{"leituras":[{"objeto_C":36.25,"ambiente_C":25.12},{"objeto_C":36.30,"ambiente_C":25.14}]}
```

---

## 5. Configurações do código

No início do arquivo `.ino` há duas constantes que podem ser ajustadas:

| Constante       | Padrão   | Descrição                          |
|-----------------|----------|------------------------------------|
| `DURACAO_MS`    | `10000`  | Tempo total de coleta (ms)         |
| `INTERVALO_MS`  | `200`    | Intervalo entre leituras (ms)      |
