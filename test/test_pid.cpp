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
 * pthread_self���ص���ͬһ�������и����߳�֮��ı�ʶ�ţ����������������Ψһ�ģ�
 * ����ͬ�����У�ÿ���̷߳��ص�pthread_self���ܷ��ص���һ���ġ�
 * ��gettid������ϵͳ�ڸ����̼߳�ı�ʶ��������linux��������������ʵ�ֵģ�����ʵ���ص�Ӧ����pid��
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