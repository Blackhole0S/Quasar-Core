#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../include/utils.h"

void test_trim() {
    char input1[] = "  hello world  ";
    trim(input1);
    assert(strcmp(input1, "hello world") == 0);

    char input2[] = "\tfoo\n";
    trim(input2);
    assert(strcmp(input2, "foo") == 0);

    printf("PASSED: trim()\n");
}

void test_file_exists() {
    const char *tmpfile = "/tmp/test_utils_file.tmp";

    FILE *fp = fopen(tmpfile, "w");
    assert(fp);
    fclose(fp);

    assert(file_exists(tmpfile) == 1);
    unlink(tmpfile);
    assert(file_exists(tmpfile) == 0);

    printf("PASSED: file_exists()\n");
}

int main() {
    test_trim();
    test_file_exists();

    printf("All utils tests passed!\n");
    return 0;
}
