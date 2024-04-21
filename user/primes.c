#include "kernel/types.h"
#include "user.h"

#define MAX_LEN 36

void solve(int pipe_read) {
    int p[2];
    pipe(p);
    int r_size = 0, r_res = 0, first = 0;
    r_size = read(pipe_read, &r_res, sizeof(int));
    if (r_size > 0) {
        first = r_res;
        printf("prime %d\n", first);
    } else {
        exit(0);
    }

    int pid = fork();
    if (pid == 0) {
        close(p[1]);
        solve(p[0]);
    } else {
        while (r_size > 0) {
            if (r_res % first) {
                write(p[1], &r_res, sizeof(int));
            }
            r_size = read(pipe_read, &r_res, sizeof(int));
        }
        close(pipe_read);
        close(p[1]);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 1) {
        printf("primes: need not parameter\n");
    }

    int p[2];
    pipe(p);
    int pid = fork();
    if (pid == 0) {
        // 子进程，进入递归
        close(p[1]);
        solve(p[0]);
    } else {
        // 父进程，筛选，后传
        close(p[0]);
        printf("prime 2\n");
        for (int i = 2; i < MAX_LEN; i++) {
            if (i % 2) {
                write(p[1], &i, sizeof(int));
            }
        }
        close(p[1]);
    }
    wait(0);
    exit(0);
    return 0;
}