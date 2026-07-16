#include "app.hpp"

static App app;

extern "C" void app_main(void) {
    app.run();
}
