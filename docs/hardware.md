# Sprzęt i piny

Źródło prawdy dla pinów ESP32: `esp32/components/common/include/config.hpp`. Ten plik trzeba aktualizować razem z nim.

## ESP32 (moduł ESP32-WROOM-32D, target `esp32`)

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

- Lewa/prawa strona liczona **od tyłu autka** (zweryfikowane na sprzęcie).
- UART nie używa GPIO16/17, bo kolidują z pinami silników.
- `MIN_FRONT_DISTANCE_CM = 20` – poniżej tej odległości jazda do przodu jest blokowana.

### Uwagi
- **GPIO1/GPIO3 (`TX`/`RX` na płytce)** to UART0 (flash i logi). Na płytkach typu DevKit leżą tuż obok IO21/IO22. Nie podłączaj tam przewodów od RPi.
- **Flashowanie: odepnij przewody od RPi** (przynajmniej TX RPi → ESP32). Przy podłączonych kablach flash kończył się błędem `no sync reply: the serial TX path seems to be down`.
- GPIO2 (LED) i GPIO5 (silnik) to piny strapping. Przy resecie GPIO5 może dać krótki impuls na silnik, więc **koła unieś nad podłoże przy flashowaniu i testach**.
- HC-SR04 daje na ECHO 5 V; ESP32 toleruje 3,3 V (dzielnik napięcia lub moduł 3,3 V).
- Czujnik widzi stożek ok. 15°. Przeszkoda bliżej niż 20 cm (także element stojaka czy kabel) blokuje jazdę do przodu; miękkie ubrania słabo odbijają ultradźwięki.

## Połączenie RPi ↔ ESP32

| RPi 5 (złącze GPIO) | ESP32 | Uwagi |
|---------------------|-------|-------|
| pin 8, GPIO14 (TXD) | GPIO21 (RX, `IO21`) | komendy RPi → ESP32 |
| pin 10, GPIO15 (RXD) | GPIO22 (TX, `IO22`) | na razie nieużywane (ESP32 nic nie odsyła) |
| pin 6 (GND) | GND | wspólna masa, konieczna |

Poziomy logiczne 3,3 V po obu stronach; nie łącz pinów 5 V ani 3,3 V między płytkami. Silniki mają własne zasilanie z baterii, a jego masa musi być wspólna z masą ESP32.

## Raspberry Pi 5

- Kamera USB (UVC, Xiongmai): `/dev/video0` (indeks 0 w `cv2.VideoCapture`); `/dev/video1` to węzeł metadanych. `rpi/vdeo_search.py` pomaga znaleźć właściwy indeks. Czasem przy starcie zgłasza błąd USB (`uvcvideo ... probe control -110`); kolejne uruchomienie zwykle działa.
- UART: `/dev/ttyAMA0` (GPIO14/15), włączony przez `dtparam=uart0=on` w `/boot/firmware/config.txt`. `/dev/serial0` wskazuje na `ttyAMA10` (złącze debug) – nie używać.
- Użytkownik musi należeć do grupy `dialout`.
- Dostęp po ssh: alias `pi-admin` (host `badgerhause`, użytkownik `adminbadger`); venv w `~/venv`; kod w `~/workspace/follow_me_car/rpi`.
