// src/signal.c

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include "signal.h"
#include "logging.h"
#include "service.h"  // to stop services on shutdown

static volatile sig_atomic_t shutdown_requested = 0;

void handle_sigchld(int signo) {
    (void)signo; // unused

    // Reap all dead children
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        log_info("Reaped child process PID %d", pid);
        // Could notify service manager here about stopped child if needed
    }
}

void handle_termination(int signo) {
    log_info("Received termination signal (%d), shutting down...", signo);
    shutdown_requested = 1;

    // Attempt to stop all services cleanly
    service_cleanup();
}

void handle_sighup(int signo) {
    log_info("Received SIGHUP (%d), reload configuration not implemented yet", signo);
    // TODO: implement config reload
}

void setup_signal_handlers(void) {
    struct sigaction sa;

    // Handle SIGCHLD to reap zombies
    sa.sa_handler = handle_sigchld;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction SIGCHLD");
        exit(EXIT_FAILURE);
    }

    // Handle SIGTERM and SIGINT for graceful shutdown
    sa.sa_handler = handle_termination;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("sigaction SIGTERM");
        exit(EXIT_FAILURE);
    }
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction SIGINT");
        exit(EXIT_FAILURE);
    }

    // Handle SIGHUP for reload
    sa.sa_handler = handle_sighup;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGHUP, &sa, NULL) == -1) {
        perror("sigaction SIGHUP");
        exit(EXIT_FAILURE);
    }

    log_info("Signal handlers installed");
}

int is_shutdown_requested(void) {
    return shutdown_requested;
}
