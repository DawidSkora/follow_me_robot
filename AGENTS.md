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
- Sterowanie: RPi wysyła co klatkę jednobajtową komendę (`rpi/main.py` → `rpi/car_link.py`), ESP32 parsuje ją w `Uart::listen()` i wrzuca do `uart_command_queue` (`xQueueOverwrite`), a `Robot` ją wykonuje.
- Zabezpieczenia w `Robot`: brak świeżej komendy przez `COMMAND_TIMEOUT_MS` ⇒ stop; jazda do przodu tylko przy świeżym odczycie czujnika > `MIN_FRONT_DISTANCE_CM` (błąd czujnika ⇒ stop). Autonomiczne omijanie przeszkód zostało usunięte.
- Przetestowane na sprzęcie (koła uniesione): komendy `f/b/l/r/s`, watchdog, blokada czujnika, pełna pętla `main.py` z kamerą (około 13,8 fps na `yolov8n.pt`). Jazda po podłodze jeszcze nie testowana.
- Debug: `ESP_LOGD` dla odebranych bajtów (`uart`) i odległości (`sensor`); żeby je zobaczyć, podnieś poziom logów w `sdkconfig`/`esp_log_level_set`.
- `main.py` ma flagi `--port`, `--no-uart` (sama detekcja, bez jazdy) i `--no-display` (bez okna, np. przez ssh). Detekcja traci osobę dopiero po `MAX_FRAMES_WITHOUT_DETECTION` klatkach, do tego czasu jedzie ostatnim kierunkiem.
- Do ustalenia: odpowiedzi ESP32→RPi (patrz `docs/protocol.md`).
