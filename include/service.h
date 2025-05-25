#ifndef SERVICE_H
#define SERVICE_H

#include <stdbool.h>

typedef enum {
    SERVICE_STOPPED,
    SERVICE_STARTING,
    SERVICE_RUNNING,
    SERVICE_FAILED
} ServiceState;

typedef struct {
    char *name;
    char *exec_start;
    char *exec_stop;
    bool oneshot;
    ServiceState state;
    pid_t pid;
} Service;

// Parse a config file and load all service definitions
int load_services(const char *config_path);

// Start, stop, restart services
int start_service(const char *name);
int stop_service(const char *name);
int restart_service(const char *name);

// Check status
ServiceState get_service_state(const char *name);

// Print all loaded services (for debug/UI)
void list_services(void);

// Shutdown and cleanup
void cleanup_services(void);

#endif // SERVICE_H
