
#include <Arduino.h>
#include <Adafruit_MLX90640.h>
#include <WiFiUdp.h>

#include "config.h"
#include "settings.hpp"
#include "web.hpp"

static paramsMLX90640 mlx90640_params;
static WiFiUDP udp;

static uint16_t mlx90640_frame[834];
static float mlx90640_temperature[768];

Adafruit_MLX90640 mlx;

bool FetchFrame() {
  if (mlx.MLX90640_GetFrameData(MLX90640_I2C_ADDR, mlx90640_frame) < 0) {
    if (State.DebugPrint()) {
      Serial.println("Failed to get frame from MLX90640.");
    }
    return false;
  }

  mlx.MLX90640_CalculateTo(mlx90640_frame, &mlx90640_params, 0.95,
                       mlx.MLX90640_GetTa(mlx90640_frame, &mlx90640_params) - 8,
                       mlx90640_temperature);

  mlx.MLX90640_BadPixelsCorrection(mlx90640_params.brokenPixels,
                               mlx90640_temperature, 1, &mlx90640_params);

  return true;
}

void setup() {
  Settings.Load();
  if (Settings.version_ != VERSION) {
    Settings = ThermalDetectorSettings();
    Settings.Save();
  }

  Serial.begin(Settings.serial_baud_rate_);

  Serial.printf("Setup WiFi: %s %s\n", Settings.ssid_, Settings.password_);
  if (Settings.mode_ == Mode::kAP) {
    WiFi.softAP(Settings.ssid_, Settings.password_);
  } else {
    WiFi.begin(Settings.ssid_, Settings.password_);
  }

  mlx.begin(MLX90640_I2CADDR_DEFAULT, &Wire);

  uint16_t mlx90640_eeprom[832];
  if (mlx.MLX90640_DumpEE(MLX90640_I2C_ADDR, mlx90640_eeprom) != 0) {
    Serial.println(F("Failed to dump eeprom of MLX90640."));
    ESP.restart();
  }

  int ret = mlx.MLX90640_ExtractParameters(mlx90640_eeprom, &mlx90640_params);
  if (ret == -4) {
    Serial.println(F("Too many outliers."));
  } else if (ret != 0) {
    Serial.println(F("Failed to extract params of MLX90640."));
    ESP.restart();
  }

  mlx.MLX90640_SetRefreshRate(MLX90640_I2C_ADDR, Settings.refresh_rate_level_);
 // MLX90640_I2CFreqSet(600);

  if (Settings.mode_ == Mode::kSTA) {
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println(F("Failed to connect WiFi."));
      ESP.restart();
    }

    Serial.print(F("WiFi connected. IP address: "));
    Serial.println(WiFi.localIP());
  }

  udp.begin(UDP_PORT);
  Web.Setup();
}

void loop() {
  static IPAddress remote_ip;
  static uint16_t remote_port;

  if (udp.parsePacket()) {
    auto r = udp.read();
    if (r == 0x0) {
      State.udp_client_attached_ = false;
      if (State.DebugPrint()) Serial.println(F("UDP client detached."));
    } else {
      remote_ip = udp.remoteIP();
      remote_port = udp.remotePort();
      State.udp_client_attached_ = true;
      if (State.DebugPrint()) {
        Serial.print(F("UDP client attached. IP address: "));
        Serial.println(remote_ip);
      }
    }
  }

  if (Serial.available() > 0) {
    auto c = Serial.read();
    if (c == 0x0 || c == '0') {
      State.serial_client_attached_ = false;
      if (State.DebugPrint()) Serial.println(F("Serial client detached."));
    } else {
      State.serial_client_attached_ = true;
    }
    Serial.flush();
  }

  if (State.udp_client_attached_ || State.serial_client_attached_) {
    if (!FetchFrame()) return;

    if (State.udp_client_attached_) {
      udp.beginPacket(remote_ip, remote_port);
      udp.write((uint8_t*)mlx90640_temperature, 768 * 4);
      if (udp.endPacket()) {
        delay(10);  // Prevent IP fragment loss.
      } else {
        State.udp_client_attached_ = false;
      }
    }

    if (State.serial_client_attached_) {
      static const uint8_t terminator[] = {0xF0, 0xF1};
      if (!Serial.write((uint8_t*)mlx90640_temperature, 768 * 4) ||
          !Serial.write(terminator, 2)) {
        State.serial_client_attached_ = false;
      }
    }
  }

  Web.server_.handleClient();
}