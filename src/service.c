#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "service.h"
#include "logging.h"
#include "sandbox.h"
#include "utils.h"

#define MAX_SERVICES 64

static Service services[MAX_SERVICES];
static int service_count = 0;

int load_services(const char *config_path) {
    FILE *fp = fopen(config_path, "r");
    if (!fp) {
        log_error("Failed to open service config: %s", config_path);
        return -1;
    }

    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '#' || strlen(line) < 3) continue;

        Service s = {0};
        s.state = SERVICE_STOPPED;

        char *name = strtok(line, ":");
        char *exec = strtok(NULL, ":");
        char *type = strtok(NULL, ":");

        if (!name || !exec) continue;

        s.name = strdup(name);
        s.exec_start = strdup(exec);
        s.oneshot = (type && strcmp(type, "oneshot\n") == 0);

        if (service_count < MAX_SERVICES) {
            services[service_count++] = s;
            log_info("Loaded service: %s", s.name);
        }
    }

    fclose(fp);
    return 0;
}

static Service *find_service(const char *name) {
    for (int i = 0; i < service_count; i++) {
        if (strcmp(services[i].name, name) == 0)
            return &services[i];
    }
    return NULL;
}

int start_service(const char *name) {
    Service *s = find_service(name);
    if (!s) {
        log_error("Service not found: %s", name);
        return -1;
    }

    if (s->state == SERVICE_RUNNING) return 0;

    pid_t pid = fork();
    if (pid == 0) {
        apply_sandbox(s->name); // Apply sandbox profile
        execl("/bin/sh", "sh", "-c", s->exec_start, NULL);
        exit(1); // exec failed
    } else if (pid > 0) {
        s->pid = pid;
        s->state = s->oneshot ? SERVICE_STOPPED : SERVICE_RUNNING;
        log_info("Started service: %s (PID %d)", s->name, pid);

        if (s->oneshot) waitpid(pid, NULL, 0);
    } else {
        s->state = SERVICE_FAILED;
        log_error("Failed to fork for service: %s", s->name);
        return -1;
    }

    return 0;
}

int stop_service(const char *name) {
    Service *s = find_service(name);
    if (!s || s->state != SERVICE_RUNNING) return -1;

    if (s->exec_stop) {
        system(s->exec_stop); // Graceful stop
    } else {
        kill(s->pid, SIGTERM);
    }

    waitpid(s->pid, NULL, 0);
    s->state = SERVICE_STOPPED;
    log_info("Stopped service: %s", s->name);
    return 0;
}

int restart_service(const char *name) {
    stop_service(name);
    return start_service(name);
}

ServiceState get_service_state(const char *name) {
    Service *s = find_service(name);
    return s ? s->state : SERVICE_FAILED;
}

void list_services(void) {
    for (int i = 0; i < service_count; i++) {
        printf("Service: %s\tState: %d\n", services[i].name, services[i].state);
    }
}

void cleanup_services(void) {
    for (int i = 0; i < service_count; i++) {
        free(services[i].name);
        free(services[i].exec_start);
        free(services[i].exec_stop);
    }
    service_count = 0;
}
