// include/service.h

#ifndef SERVICE_H
#define SERVICE_H

int service_add(const char *name, const char *exec_path);
int service_start(const char *name);
int service_stop(const char *name);
int service_restart(const char *name);
void service_cleanup(void);

#endif // SERVICE_H
