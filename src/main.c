// src/main.c

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include "init.h"
#include "logging.h"
#include "signal.h"
#include "sandbox.h"
#include "service.h"
#include "utils.h"

static volatile sig_atomic_t running = 1;

static void shutdown_handler(int signo) {
    log_info("Signal %d received, initiating shutdown.", signo);
    running = 0;
}

int main(int argc, char *argv[]) {
    if (log_init() != 0) {
        fprintf(stderr, "Failed to initialize logging\n");
        return EXIT_FAILURE;
    }
    log_info("Quasar-Core starting...");

    if (signal_setup(shutdown_handler) != 0) {
        log_error("Failed to set up signal handlers");
        log_close();
        return EXIT_FAILURE;
    }

    if (sandbox_init() != 0) {
        log_error("Sandbox initialization failed");
        log_close();
        return EXIT_FAILURE;
    }

    if (service_start_all() != 0) {
        log_error("Failed to start services");
        sandbox_cleanup();
        log_close();
        return EXIT_FAILURE;
    }

    log_info("Initialization complete. Entering main loop.");

    while (running) {
        service_poll();
        sleep(1);
    }

    log_info("Shutting down services...");
    service_stop_all();

    log_info("Cleaning up sandbox...");
    sandbox_cleanup();

    log_info("Quasar-Core shutdown complete.");
    log_close();

    return EXIT_SUCCESS;
}
