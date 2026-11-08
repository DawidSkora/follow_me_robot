# AGENTS.md

Autko jeżdżące za człowiekiem: **ESP32** (silniki, czujnik, firmware C++/ESP-IDF) + **Raspberry Pi 5** (kamera, YOLOv8n, Python). Monorepo; przegląd w [README.md](README.md).

## Układ repo
- `esp32/` – firmware ESP-IDF (target `esp32`), komponenty w `esp32/components/`, punkt wejścia `esp32/main/`.
- `rpi/` – Python (OpenCV, ultralytics, pyserial), modele `yolov8n*`, `requirements.txt`.
- `docs/` – `protocol.md` (UART RPi↔ESP32), `hardware.md` (piny).

## Komendy
- ESP32 (wymaga załadowanego ESP-IDF): `cd esp32 && idf.py build`; flash: `idf.py -p <port> flash monitor`.
- RPi: Python **zawsze w venv** (`source ~/venv/bin/activate`), potem `pip install -r rpi/requirements.txt`.
- Brak automatycznych testów. Weryfikacja = build firmware'u + uruchomienie na sprzęcie.

## Raspberry Pi
- Dostęp: `ssh pi-admin` (host `badgerhause`, użytkownik `adminbadger`), venv: `~/venv`.
- Źródłem prawdy jest to repo. Zmiany robimy lokalnie, na RPi `git pull`. Nie edytuj plików na RPi na boku bez commita.
- Stary katalog `~/workspace/opencv` na RPi to kopia sprzed migracji; nie traktuj go jako aktualnego.
- `main.py` otwiera okno podglądu (`cv2.imshow`) – wymaga środowiska graficznego; przez ssh bez DISPLAY nie zadziała.

## Konwencje
- Protokół UART jest wspólny dla obu stron: zmianę rób po obu stronach w jednym commicie i aktualizuj `docs/protocol.md`.
- Piny ESP32 tylko w `esp32/components/common/include/config.hpp` + aktualizacja `docs/hardware.md`.
- Firmware: C++ z klasami na komponent, zadania FreeRTOS, komunikacja między zadaniami przez kolejki (`components/common/queues.cpp`). Trzymaj ten styl.
- Nie commituj `esp32/build/`, `sdkconfig`, `venv`. Duże modele (kilkadziesiąt MB i więcej) – Git LFS lub skrypt pobierający.
- Komunikaty commitów po angielsku, krótkie, w trybie rozkazującym (`Add UartCommand queue`).

## Bezpieczeństwo (robot fizycznie jeździ)
- Domyślnym stanem silników jest **stop**. Błąd, utrata łączności lub brak detekcji ⇒ stop.
- Nie usuwaj ani nie osłabiaj zatrzymania przy przeszkodzie (`MIN_FRONT_DISTANCE_CM`) bez wyraźnej prośby.
- Nie flashuj ani nie uruchamiaj jazdy na sprzęcie bez zgody użytkownika.

## Stan prac
- ESP32: `Uart::listen()` parsuje bajty `f/b/l/r/s`, ale komenda nie trafia jeszcze do kolejki ani do `Robot` (`TODO` w `uart.cpp`). Kolejka `uart_command_queue` istnieje (`UartCommand { char command[32]; }`), a `UartCommandType` jest zdefiniowany osobno w `uart.hpp`.
- RPi: `main.py` wylicza kierunek, ale **nie wysyła** go po UART (tylko rysuje na podglądzie). `uart_test.py` to tylko echo-test łącza. Model ładowany jako `yolov8n.pt` (eksport do NCNN jest zakomentowany).
- Do ustalenia: watchdog komend, odpowiedzi ESP32→RPi (patrz `docs/protocol.md`).
