// src/utils.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include "utils.h"

int file_exists(const char *path) {
    struct stat st;
    return (stat(path, &st) == 0);
}

int is_executable(const char *path) {
    return access(path, X_OK) == 0;
}

char *trim_whitespace(char *str) {
    if (!str) return NULL;

    // Trim leading
    while (*str == ' ' || *str == '\t' || *str == '\n') str++;

    if (*str == '\0') return str;

    // Trim trailing
    char *end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t' || *end == '\n')) end--;

    *(end + 1) = '\0';
    return str;
}

int str_to_uid(const char *name, uid_t *uid) {
    struct passwd *pwd = getpwnam(name);
    if (!pwd) return -1;
    *uid = pwd->pw_uid;
    return 0;
}

int str_to_gid(const char *name, gid_t *gid) {
    struct group *grp = getgrnam(name);
    if (!grp) return -1;
    *gid = grp->gr_gid;
    return 0;
}

char *read_file_to_string(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char *buf = calloc(1, size + 1);
    if (!buf) {
        fclose(f);
        return NULL;
    }

    fread(buf, 1, size, f);
    fclose(f);
    return buf;
}
