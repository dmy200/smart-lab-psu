#include "LbpWebInterface.h"
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include "Globals.h"
#include "Config.h"

// === HTML-страница главная ===
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang='ru'>
<head>
  <meta charset='UTF-8'>
  <title>Данные ЛБП</title>
  <style>
    body { font-family: monospace; background: #111; color: #0f0; padding: 1em 2em; font-size: 1.5em; }
    #status { position: fixed; top: 1em; right: 1em; width: 15px; height: 15px; border-radius: 50%; background: red; box-shadow: 0 0 5px red; }
    h1 { margin-bottom: 0.5em; }
    .reading { margin-bottom: 0.5em; }
    label { display: block; margin: 0.7em 0 0.3em 0; }
    input[type=number] {
      font-size: 1em; width: 100px; background: #222; border: 1px solid #0f0;
      color: #0f0; padding: 0.2em 0.4em; border-radius: 4px;
    }
    button.nav-btn {
      font-size: 1.5em; margin: 0.5em 0.5em 0 0; background: #222; color: #0f0;
      border: 1px solid #0f0; cursor: pointer; padding: 0.2em 0.5em; border-radius: 5px;
    }
    button.nav-btn:hover { background: #0f0; color: #111; }
    #modeBtn {
      font-size: 1em;
      margin-left: 0.5em;
      background: #222;
      border: 1px solid #0f0;
      color: #0f0;
      cursor: pointer;
      padding: 0.1em 0.5em;
      border-radius: 5px;
    }
    #modeBtn:hover {
      background: #0f0;
      color: #111;
    }
  </style>
</head>
<body>
  <div id="status" title="Статус WebSocket"></div>
  <h1>Данные ЛБП</h1>

  <div class="reading" id="v">Напряжение: -- В</div>
  <div class="reading" id="i">Ток: -- А</div>
  <div class="reading" id="q">Мощность: -- Вт</div>

  <hr>

  <label for="inputV">Установленное напряжение (V):</label>
  <input type="number" id="inputV" step="0.1" min="0" max="30" />

  <label for="inputIL">Ограничение по току (IL):</label>
  <input type="number" id="inputIL" step="0.01" min="0" max="10" />

  <label for="inputIF">Предохранитель (IF):</label>
  <input type="number" id="inputIF" step="0.01" min="0" max="10" />

  <hr>

  <div class="reading" id="wifi">Wi-Fi: ...</div>
  <div class="reading" id="ntcTemp">NTC Temp: -- °C</div>
  <div class="reading">Режим: <span id="modeText">--</span> <button id="modeBtn">Переключить</button></div>

  <button class="nav-btn" id="btnCharts" title="Графики">📈</button>
  <button class="nav-btn" id="btnSettings" title="Настройки">⚙️</button>
  <button class="nav-btn" id="btnSystem" title="Система">ℹ️</button>

<script>
  const status = document.getElementById('status');
  const v = document.getElementById('v');
  const i = document.getElementById('i');
  const q = document.getElementById('q');
  const inputV = document.getElementById('inputV');
  const inputIL = document.getElementById('inputIL');
  const inputIF = document.getElementById('inputIF');
  const wifi = document.getElementById('wifi');
  const ntcTemp = document.getElementById('ntcTemp');
  const modeText = document.getElementById('modeText');
  const modeBtn = document.getElementById('modeBtn');
  let ws;

  function connectWS() {
    ws = new WebSocket("ws://" + location.hostname + "/ws");
    ws.onopen = () => {
      status.style.background = '#0f0';
      status.style.boxShadow = '0 0 8px #0f0';
    };
    ws.onclose = () => {
      status.style.background = 'red';
      status.style.boxShadow = '0 0 5px red';
      setTimeout(connectWS, 2000);
    };
    ws.onmessage = e => {
      if (e.data.startsWith("V:")) {
        v.textContent = "Напряжение: " + e.data.slice(2) + " В";
      } else if (e.data.startsWith("I:")) {
        i.textContent = "Ток: " + e.data.slice(2) + " А";
      } else if (e.data.startsWith("Q:")) {
        q.textContent = "Мощность: " + e.data.slice(2) + " Вт";
      } else if (e.data.startsWith("VSET:")) {
        if (document.activeElement !== inputV) inputV.value = parseFloat(e.data.slice(5));
      } else if (e.data.startsWith("IL:")) {
        if (document.activeElement !== inputIL) inputIL.value = parseFloat(e.data.slice(3));
      } else if (e.data.startsWith("IF:")) {
        if (document.activeElement !== inputIF) inputIF.value = parseFloat(e.data.slice(3));
      } else if (e.data.startsWith("TEMP:")) {
        ntcTemp.textContent = "NTC Temp: " + e.data.slice(5) + " °C";
      } else if (e.data.startsWith("MODE:")) {
        modeText.textContent = e.data.slice(5) === "auto" ? "Авто" : "Ручной";
      }
    };
  }

  function sendSetting(prefix, value) {
    if (ws && ws.readyState === WebSocket.OPEN) {
      ws.send(`SET ${prefix}:${value}`);
    }
  }

  inputV.addEventListener('change', () => sendSetting('V', inputV.value));
  inputIL.addEventListener('change', () => sendSetting('IL', inputIL.value));
  inputIF.addEventListener('change', () => sendSetting('IF', inputIF.value));

  modeBtn.addEventListener('click', () => {
    const newMode = modeText.textContent === "Авто" ? "manual" : "auto";
    sendSetting('MODE', newMode);
  });

  document.getElementById('btnCharts').addEventListener('click', () => alert("Раздел 'Графики' пока не реализован."));
  document.getElementById('btnSettings').addEventListener('click', () => location.href = "/settings");
  document.getElementById('btnSystem').addEventListener('click', () => location.href = "/system");

  connectWS();

  fetch('/wifi').then(r => r.text()).then(txt => { wifi.textContent = txt; });
</script>
</body>
</html>
)rawliteral";

static AsyncWebServer server(80);
static AsyncWebSocket ws("/ws");

namespace LbpWebInterface {

  void begin() {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
      request->send_P(200, "text/html", index_html);
    });

    server.on("/wifi", HTTP_GET, [](AsyncWebServerRequest* request) {
      String info = "Wi-Fi: " + WiFi.SSID() + " (" + String(WiFi.RSSI()) + " dBm)";
      request->send(200, "text/plain", info);
    });

    ws.onEvent([](AsyncWebSocket *server, AsyncWebSocketClient *client,
                  AwsEventType type, void *arg, uint8_t *data, size_t len) {
      if (type == WS_EVT_DATA) {
        AwsFrameInfo *info = (AwsFrameInfo*)arg;
        if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
          String msg = String((char*)data);
          if (msg.startsWith("SET ")) {
            msg = msg.substring(4);
            int sep = msg.indexOf(":");
            if (sep > 0) {
              String key = msg.substring(0, sep);
              String valStr = msg.substring(sep + 1);
              if (key == "V") labV_set = valStr.toFloat();
              else if (key == "IL") labI_limit = valStr.toFloat();
              else if (key == "IF") labI_fuse = valStr.toFloat();
              else if (key == "MODE") {
                modeAuto = (valStr == "auto");
              }
            }
          }
        }
      }
    });

    server.addHandler(&ws);
    server.begin();
  }

  void update() {
    if ((millis() - lastSend) > WEBSOCKET_SEND_INTERVAL) {
      lastSend = millis();
      ws.textAll("V:" + String(labV_meas, 3));
      ws.textAll("I:" + String(labI_meas, 3));
      ws.textAll("Q:" + String(labQ_meas, 3));
      ws.textAll("VSET:" + String(labV_set, 2));
      ws.textAll("IL:" + String(labI_limit, 2));
      ws.textAll("IF:" + String(labI_fuse, 2));
      ws.textAll("TEMP:" + String(labTemp_ntc, 1));
      ws.textAll("MODE:" + String(modeAuto ? "auto" : "manual"));
    }
  }

}  // namespace
