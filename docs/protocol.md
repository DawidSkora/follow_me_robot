# Protokół UART RPi ↔ ESP32

Łącze: UART, 115200 baud, 8N1, bez flow control. Linie TX/RX skrzyżowane, wspólna masa.

- RPi: `/dev/ttyAMA0` (pyserial)
- ESP32: `UART_NUM_1`, RX = GPIO21, TX = GPIO22 (patrz `esp32/components/common/include/config.hpp`)

> Poziomy logiczne: RPi i ESP32 pracują na 3,3 V. Nie podłączać 5 V do GPIO.

## Komendy RPi → ESP32

Jeden bajt ASCII na komendę (obecna implementacja w `esp32/components/uart/uart.cpp`):

| Znak | Komenda | Znaczenie |
|------|---------|-----------|
| `f` | MoveForward | jedź do przodu |
| `b` | MoveBackward | jedź do tyłu |
| `l` | TurnLeft | skręć w lewo |
| `r` | TurnRight | skręć w prawo |
| `s` | Stop | zatrzymaj |

Nieznane bajty są ignorowane.

## Mapowanie kierunków z `rpi/main.py`

| `Direction` | Komenda |
|-------------|---------|
| `FORWARD` | `f` |
| `LEFT` | `l` |
| `RIGHT` | `r` |
| `STOP_NO_OBJ`, `STOP_MIN_DIST` | `s` |

(Mapowanie docelowe: `main.py` na razie nic nie wysyła po UART.)

## Do ustalenia

- Czy komendy powtarzać cyklicznie (watchdog po stronie ESP32: brak komendy przez X ms ⇒ stop)?
- Odpowiedzi ESP32 → RPi (ACK, odległość z czujnika)?
- Ewentualne przejście na ramki liniowe (`CMD\n`) zamiast pojedynczych bajtów.

Każda zmiana protokołu musi być zrobiona po obu stronach w jednym commicie
i odzwierciedlona w tym pliku.
