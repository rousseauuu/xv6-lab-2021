#include "kernel/types.h"
#include "user/user.h"

#define RD 0
#define WR 1
#define stdin 0
#define stdout 1
#define stderr 2

int main(int argc, char const* argv[]) {
  int p[2];    // 管道两端的文件描述符，其中p[0]对应输入文件描述符，p[1]对应输出文件描述符
  char buf[2]; // 读信息的缓冲区
  char* child_msg = "a";
  char* parent_msg = "b";

  pipe(p);

  if (fork() == 0) {
    // child process
    if (read(p[RD], buf, 1) != 1) {
      fprintf(stderr, "Can't read from the parent!\n");
      exit(1);
    }
    int pid = getpid();
    printf("%d: received ping\n", pid);
    close(p[RD]);

    if (write(p[WR], child_msg, 1) != 1) {
      fprintf(stderr, "Can't write to the parent!\n");
    }
    close(p[WR]);

    exit(0);
  } else {
    // parent process
    if (write(p[WR], parent_msg, 1) != 1) {
      fprintf(stderr, "Can't write to the chile!\n");
      exit(0);
    }
    close(p[WR]);

    wait(0);

    if (read(p[RD], buf, 1) != 1) {
      fprintf(stderr, "Can't read from the child!\n");
      exit(1);
    }
    close(p[RD]);
    printf("%d: received pong\n", getpid());

    exit(0);
  }
}