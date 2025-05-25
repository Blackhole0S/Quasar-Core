// include/utils.h

#ifndef UTILS_H
#define UTILS_H

#include <sys/types.h>

int file_exists(const char *path);
int is_executable(const char *path);
char *trim_whitespace(char *str);
int str_to_uid(const char *name, uid_t *uid);
int str_to_gid(const char *name, gid_t *gid);
char *read_file_to_string(const char *path);

#endif // UTILS_H
