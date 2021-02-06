//
// Created by dhp on 2021/2/6.
//

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

void *func(void *) {
  printf("getpid: %d\n", ::getpid());
  printf("pthread_self: %d\n", pthread_self());
  printf("gettid: %d\n", syscall(SYS_gettid));
  return NULL;
}

/*
 * pthread_self返回的是同一个进程中各个线程之间的标识号，对于这个进程内是唯一的，
 * 而不同进程中，每个线程返回的pthread_self可能返回的是一样的。
 * 而gettid是用来系统内各个线程间的标识符，由于linux采用轻量级进程实现的，它其实返回的应该是pid号
 */

int main(int argc, char **argv) {
  pthread_t pid;
  pthread_create(&pid, NULL, func, NULL);
  sleep(5);
  printf("main_thread pid: %d\n", pid);
  printf("main_thread getpid: %d\n", ::getpid());
  pthread_join(pid, NULL);
  return 0;
}