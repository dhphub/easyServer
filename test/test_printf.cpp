//
// Created by onirii on 2021/2/6.
//


#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
  pid_t pid = getpid();
  printf("%5d\n", pid);
  printf("%10d\n", pid);
  return 0;
}