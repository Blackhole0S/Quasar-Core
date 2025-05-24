// src/service.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "service.h"
#include "logging.h"
#include "utils.h"

// Simple linked list for services
typedef struct service {
    char *name;
    char *exec_path;
    pid_t pid;
    int running;
    struct service *next;
} service_t;

static service_t *services_head = NULL;

// Find service by name
static service_t *find_service(const char *name) {
    service_t *current = services_head;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Add a new service
int service_add(const char *name, const char *exec_path) {
    if (find_service(name)) {
        log_warn("Service '%s' already exists", name);
        return -1;
    }

    service_t *svc = malloc(sizeof(service_t));
    if (!svc) {
        log_error("Failed to allocate memory for service '%s'", name);
        return -1;
    }
    svc->name = strdup(name);
    svc->exec_path = strdup(exec_path);
    svc->pid = 0;
    svc->running = 0;
    svc->next = services_head;
    services_head = svc;

    log_info("Added service '%s' with exec path '%s'", name, exec_path);
    return 0;
}

// Start a service
int service_start(const char *name) {
    service_t *svc = find_service(name);
    if (!svc) {
        log_error("Service '%s' not found", name);
        return -1;
    }
    if (svc->running) {
        log_warn("Service '%s' is already running (PID %d)", name, svc->pid);
        return 0;
    }

    pid_t pid = fork();
    if (pid < 0) {
        log_error("Failed to fork for service '%s'", name);
        return -1;
    } else if (pid == 0) {
        // Child: execute the service binary
        execl(svc->exec_path, svc->exec_path, (char *)NULL);
        // If execl returns, exec failed
        log_error("Failed to exec service '%s'", svc->exec_path);
        _exit(EXIT_FAILURE);
    } else {
        // Parent
        svc->pid = pid;
        svc->running = 1;
        log_info("Started service '%s' with PID %d", name, pid);
        return 0;
    }
}

// Stop a service
int service_stop(const char *name) {
    service_t *svc = find_service(name);
    if (!svc || !svc->running) {
        log_warn("Service '%s' not running or not found", name);
        return -1;
    }

    if (kill(svc->pid, SIGTERM) != 0) {
        log_error("Failed to send SIGTERM to service '%s' PID %d", name, svc->pid);
        return -1;
    }

    // Wait for process to terminate
    int status;
    waitpid(svc->pid, &status, 0);
    svc->running = 0;
    svc->pid = 0;
    log_info("Stopped service '%s'", name);
    return 0;
}

// Restart a service
int service_restart(const char *name) {
    if (service_stop(name) == 0) {
        return service_start(name);
    }
    return -1;
}

// Cleanup all services
void service_cleanup(void) {
    service_t *current = services_head;
    while (current) {
        if (current->running) {
            service_stop(current->name);
        }
        service_t *tmp = current;
        current = current->next;
        free(tmp->name);
        free(tmp->exec_path);
        free(tmp);
    }
    services_head = NULL;
}
