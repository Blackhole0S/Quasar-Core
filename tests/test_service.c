#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/service.h"

#define TEST_CONFIG_FILE "/tmp/test_services.conf"

void create_mock_config() {
    FILE *fp = fopen(TEST_CONFIG_FILE, "w");
    if (!fp) {
        perror("Failed to create test config");
        exit(1);
    }

    // Basic oneshot service and a sleep-based long-running one
    fprintf(fp,
        "echo_test:/bin/echo Quasar test:oneshot\n"
        "sleep_test:/bin/sleep 1:forking\n"
    );

    fclose(fp);
}

void test_loading_services() {
    if (load_services(TEST_CONFIG_FILE) != 0) {
        printf("FAILED: Could not load services\n");
        exit(1);
    }

    printf("PASSED: Service config loaded\n");
}

void test_start_service(const char *name) {
    if (start_service(name) == 0) {
        printf("PASSED: Service '%s' started successfully\n", name);
    } else {
        printf("FAILED: Service '%s' failed to start\n", name);
    }
}

void test_service_state(const char *name, ServiceState expected) {
    ServiceState state = get_service_state(name);
    if (state == expected) {
        printf("PASSED: State of '%s' is as expected (%d)\n", name, expected);
    } else {
        printf("FAILED: State of '%s' is %d, expected %d\n", name, state, expected);
    }
}

void test_stop_service(const char *name) {
    if (stop_service(name) == 0) {
        printf("PASSED: Service '%s' stopped successfully\n", name);
    } else {
        printf("FAILED: Service '%s' failed to stop\n", name);
    }
}

int main() {
    create_mock_config();
    test_loading_services();

    test_start_service("echo_test");
    test_service_state("echo_test", SERVICE_STOPPED); // oneshot returns to STOPPED

    test_start_service("sleep_test");
    test_service_state("sleep_test", SERVICE_RUNNING);

    test_stop_service("sleep_test");
    test_service_state("sleep_test", SERVICE_STOPPED);

    cleanup_services();
    unlink(TEST_CONFIG_FILE);

    return 0;
}
