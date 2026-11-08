# follow_me_car

Hobbystyczne autko, które jeździ za człowiekiem. Składa się z dwóch modułów:

| Moduł | Sprzęt | Rola | Katalog |
|-------|--------|------|---------|
| Sterownik | ESP32 | silniki, czujnik ultradźwiękowy, odbiór komend po UART | [`esp32/`](esp32/) |
| "Mózg" | Raspberry Pi 5 + kamera | wykrywanie ludzi (YOLOv8n), wybór kierunku jazdy | [`rpi/`](rpi/) |

```
kamera ─► RPi 5 (YOLO, OpenCV) ──UART 115200──► ESP32 ─► silniki
                                                  ▲
                                   czujnik HC-SR04 (stop przy < 20 cm)
```

RPi wykrywa osobę, wylicza kierunek (`LEFT`/`RIGHT`/`FORWARD`/`STOP`) i wysyła
jednoznakowe komendy do ESP32. ESP32 wykonuje je na silnikach i niezależnie
zatrzymuje autko, gdy przeszkoda jest za blisko.

## Struktura repo

```
esp32/   firmware (ESP-IDF, C++): components/{common,motor_driver,robot,uart,ultrasonic_sensor}, main/
rpi/     kod Pythona (OpenCV + ultralytics), modele, requirements.txt
docs/    protokół UART, opis sprzętu i pinów
```

## ESP32

Wymaga zainstalowanego i załadowanego ESP-IDF (`. $IDF_PATH/export.sh`), target `esp32`.

```bash
cd esp32
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
```

## Raspberry Pi 5

Środowisko wirtualne (na RPi: `~/venv`):

```bash
cd rpi
python -m venv ~/venv          # tylko za pierwszym razem
source ~/venv/bin/activate
pip install -r requirements.txt
python main.py                 # detekcja + sterowanie autkiem (okno podglądu, `q` kończy)
python main.py --no-uart       # tylko detekcja, bez wysyłania komend
python main.py --no-display    # bez okna podglądu (np. przez ssh), status w konsoli
python uart_test.py            # test łącza UART (/dev/ttyAMA0)
python vdeo_search.py          # sprawdzenie, które /dev/video* działają
```

## Dokumentacja

- [`docs/protocol.md`](docs/protocol.md) – protokół komunikacji RPi ↔ ESP32
- [`docs/hardware.md`](docs/hardware.md) – piny ESP32 i połączenia

## Status

Work in progress. Szczegóły i znane braki: sekcja "Stan prac" w [`AGENTS.md`](AGENTS.md).

## Praca z agentami AI

Instrukcje dla agentów (Claude Code, Codex itp.) są w [`AGENTS.md`](AGENTS.md).
