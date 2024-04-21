#include <kernel/types.h>
#include "user.h"

int main(int argc, char *argv[]) {
    // 创建一个管道
    int p[2];
    char text[10];
    pipe(p);
    int pid = fork();
    if (pid == 0) {
        // 子进程
        read(p[0], text, 10); // 要是父进程还没写入会进入阻塞
        printf("%d: received %s\n", getpid(), text);
        write(p[1], "pong", 10);
        exit(0);
    } else {
        // 父进程
        write(p[1], "ping", 10);
        wait(0); // 父进程阻塞，等待子进程读取和写入
        read(p[0], text, 10);
        printf("%d: received %s\n", getpid(), text);
        exit(0);
    }
    return 0;
}
