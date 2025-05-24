// src/sandbox.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "sandbox.h"
#include "logging.h"

// Example: flags to track sandbox state
static int sandbox_active = 0;

// Helper: run a shell command and return exit status
static int run_command(const char *cmd) {
    log_debug("Running command: %s", cmd);
    int ret = system(cmd);
    if (ret == -1) {
        log_error("Failed to execute command: %s", cmd);
        return -1;
    }
    int exit_status = WEXITSTATUS(ret);
    if (exit_status != 0) {
        log_warn("Command exited with status %d: %s", exit_status, cmd);
    }
    return exit_status;
}

int sandbox_init(void) {
    log_info("Initializing sandbox environment...");

    // Example: Load AppArmor profile
    if (run_command("apparmor_parser -r /etc/apparmor.d/quasar-core-profile") != 0) {
        log_warn("AppArmor profile loading failed or not available");
        // Not fatal, continue
    }

    // Example: Prepare Firejail environment (check if firejail exists)
    if (access("/usr/bin/firejail", X_OK) == 0) {
        // You could preload firejail, or setup defaults here
        log_info("Firejail available and ready");
    } else {
        log_warn("Firejail not installed or not executable");
    }

    sandbox_active = 1;
    log_info("Sandbox environment initialized successfully.");
    return 0;
}

void sandbox_cleanup(void) {
    if (!sandbox_active) {
        log_debug("Sandbox cleanup called but sandbox not active.");
        return;
    }
    log_info("Cleaning up sandbox environment...");

    // Example: Unload AppArmor profile if needed (optional)
    // apparmor_parser -R /etc/apparmor.d/quasar-core-profile
    // Usually AppArmor profiles stay loaded, so this can be skipped or done on shutdown

    sandbox_active = 0;
    log_info("Sandbox environment cleanup complete.");
}
