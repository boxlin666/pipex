#include <stdio.h>
#include <string.h>
#include <errno.h>

int main() {
    // 错误码示例
    int error_codes[] = {EACCES, ENOENT, EINVAL, ENOMEM, 9999}; // 9999 是无效错误码
    int num_errors = sizeof(error_codes) / sizeof(int);

    for (int i = 0; i < num_errors; i++) {
        int errnum = error_codes[i];
        printf("Error %d: %s\n", errnum, strerror(errnum));
    }

    return 0;
}