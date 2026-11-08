# Sprzęt i piny

Źródło prawdy: `esp32/components/common/include/config.hpp`. Ten plik trzeba aktualizować razem z nim.

## ESP32 (target `esp32`)

| Funkcja | GPIO |
|---------|------|
| Silnik lewy – przód | 18 |
| Silnik lewy – tył | 5 |
| Silnik prawy – przód | 16 |
| Silnik prawy – tył | 17 |
| HC-SR04 TRIG | 33 |
| HC-SR04 ECHO | 32 |
| Wbudowana dioda LED | 2 |
| UART1 RX (z TX RPi) | 21 |
| UART1 TX (do RX RPi) | 22 |

Lewa/prawa strona liczona od tyłu autka (zweryfikowane na sprzęcie, strony były zamienione względem pierwszego opisu).
UART nie używa GPIO16/17, bo kolidują z pinami silników.

Stała `MIN_FRONT_DISTANCE_CM = 20` – poniżej tej odległości autko się zatrzymuje.

## Raspberry Pi 5

- Kamera USB (`/dev/video*`, indeks wybrany w `cv2.VideoCapture`; `rpi/vdeo_search.py` pomaga go znaleźć).
- UART: `/dev/ttyAMA0` (musi być włączony w `raspi-config`).
- Dostęp po ssh: alias `pi-admin`; venv w `~/venv`.
