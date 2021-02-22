//
// Created by onirii on 2021/2/21.
//

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <syscall.h>

//三个线程顺序循环打印abc
int flag = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond;

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

void* print(void *data) {
  int num = *(int *)data;
//  pthread_mutex_lock(&mutex1);
//  printf("global pid %d, num %d\n", syscall(SYS_gettid), num);
//  pthread_mutex_unlock(&mutex1);
  for (int i = 0; i < 10; ++i) {
    pthread_mutex_lock(&mutex);
    while (num != flag) {
      pthread_cond_wait(&cond, &mutex);
    }
    if (num == 0) {
      printf("a");
    } else if (num == 1) {
      printf("b");
    } else {
      printf("c");
    }
    flag = (flag + 1) % 3;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
  }
  return NULL;
}

int main(int argc, char *argv[]) {
  pthread_cond_init(&cond, NULL);
  pthread_mutex_init(&mutex, NULL);
  pthread_mutex_init(&mutex1, NULL);
  
  pthread_t tid[3] = {0};
  int t[3] = {0, 1, 2};
  for (int i=0; i<3; ++i) {
    //这里的坑，注意不能把i的地址传给线程函数
    //因为有可能当线程函数取i的值得时候,i已经变了
    pthread_create(&tid[i], NULL, print, &t[i]);
  }
  
  for (int i = 0; i < 3; ++i) {
    pthread_join(tid[i], NULL);
  }
  
  pthread_cond_destroy(&cond);
  pthread_mutex_destroy(&mutex);
  
  return 0;
}

