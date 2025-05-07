#include <stdio.h>
#include <errno.h>
#include <string.h>

int main() {
    FILE *file = fopen("nonexistent_file.txt", "r");
    if (!file) {
        printf("Error: %s\n", strerror(errno));
    }
    return 0;
}