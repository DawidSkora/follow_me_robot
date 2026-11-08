# Testy na sprzęcie

Brak testów automatycznych. Wszystkie testy ruchu robimy z **kołami uniesionymi nad podłożem**, a pierwszy test na podłodze w otwartej przestrzeni z baterią silników pod ręką.

## 1. Łącze UART
Wgraj firmware (z odpiętymi kablami RPi), podepnij kable (patrz `hardware.md`), potem na RPi w venv:

```python
import serial, time
s = serial.Serial("/dev/ttyAMA0", 115200)
for _ in range(20): s.write(b"l"); time.sleep(0.1)   # ok. 2 s skrętu w lewo
s.write(b"s")
```
`rpi/uart_test.py` wysyła tylko tekst i czeka na echo, którego ESP32 nie odsyła – nie nadaje się jako test ruchu.

## 2. Komendy i watchdog
Wysyłaj po 1 s `f`, `b`, `l`, `r` (przerwy 1,5 s bez komend). Oczekiwane (strony od tyłu autka):

| Komenda | Lewe koła | Prawe koła |
|---------|-----------|------------|
| `f` | przód | przód |
| `b` | tył | tył |
| `l` | tył | przód |
| `r` | przód | tył |

W przerwach, i po ostatniej komendzie wysłanej bez `s`, koła mają stanąć po ok. 0,5 s.

## 3. Blokada czujnika
Wysyłaj `f` i zasłoń czujnik dłonią (< 20 cm): koła stają, po odsłonięciu ruszają.

## 4. Kamera i detekcja
```bash
cd ~/workspace/follow_me_car/rpi && source ~/venv/bin/activate
python main.py --no-uart --no-display   # status co sekundę: kierunek, fps, liczba wykryć
```
Oczekiwane ok. 14 fps. Osoba po prawej stronie kadru → `RIGHT`, po lewej → `LEFT`, w środku → `FORWARD`, zajmuje ponad połowę kadru → `STOP_MIN_DIST`, brak osoby przez 10 klatek → `STOP_NO_OBJ`.

## 5. Pełna pętla
`python main.py --no-display` (koła uniesione). Sprawdź zgodność kierunków kół z położeniem osoby i zatrzymanie po jej zniknięciu.

## Logi ESP32
Kabel USB (`/dev/ttyUSB0`) + `idf.py monitor`. Logi `uart` i `sensor` są na poziomie debug, żeby je zobaczyć, zmień poziom logowania (np. `esp_log_level_set("uart", ESP_LOG_DEBUG)`).
