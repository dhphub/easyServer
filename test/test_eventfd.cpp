//
// Created by onirii on 2021/2/21.
//

#include <pthread.h>
#include <inttypes.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/eventfd.h>
#include "base/Mutex.h"

#define CONSUMER_NUM 2
#define PRODUCER_NUM 4
#define MAX_EVENT_SIZE 10
#define MAX_LOG_SIZE 1024

es::MutexLock gLock;

typedef struct thread_info {
  pthread_t thread_id;
  int rank;
  int epfd;
} thread_info_t ;

//防止多线程打印出问题，加锁
void log(const char *format) {
  es::MutexLockGuard lockGuard(gLock);
  printf(format);
}

/*
 * 用eventfd epoll来实现一个生产者消费者模型
 * 除了打印log的地方外,没有用到锁
 */

static void* consumer_routine(void *data) {
  thread_info_t *threadInfo = (thread_info_t *)data;
  int epfd = threadInfo->epfd;
  int rank = threadInfo->rank;
  struct epoll_event *events = (struct epoll_event*)calloc(MAX_EVENT_SIZE, sizeof(struct epoll_event));
  char buf[MAX_LOG_SIZE] = {0};
  snprintf(buf, MAX_LOG_SIZE, "consumer %d is runing\n", rank);
  log(buf);
  while (1) {
    int nfds = epoll_wait(epfd, events, MAX_EVENT_SIZE, 1000);
    for(int i=0; i<nfds; ++i) {
      if (events[i].events & EPOLLIN) {
        int fd = events[i].data.fd;
        char buf[MAX_LOG_SIZE] = {0};
        snprintf(buf, MAX_LOG_SIZE, "consumer %d get event from fd %d\n", rank, fd);
        log(buf);
        uint64_t res;
        read(fd, &res, sizeof(res));
        close(fd);
      }
    }
  }
}

static void* producer_routine(void *data) {
  thread_info_t *threadInfo = (thread_info_t*)data;
  int epfd = threadInfo->epfd;
  int rank = threadInfo->rank;
  struct epoll_event event;
  char buf[MAX_EVENT_SIZE] = {0};
  snprintf(buf, MAX_LOG_SIZE, "producer %d is running\n", rank);
  log(buf);
  while (1) {
    sleep(1);
    //使用eventfd进行线程异步唤醒的好处有(相比于pipe)
    //一是只需要一个fd,二是不必管理缓冲区
    //原因:pipe只能单向读写, eventfd的缓冲区就是一个uint64_t
    int evefd = eventfd(1, EFD_CLOEXEC | EFD_NONBLOCK);
    event.data.fd = evefd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(epfd, EPOLL_CTL_ADD, evefd, &event);
    
    //触发
    write(evefd, (void*)0xffffffff, sizeof(uint64_t));
  }
}


int main(int argc, char **argv) {
  
  thread_info_t *plist = nullptr;
  thread_info_t *clist = nullptr;
  plist = (thread_info_t*)calloc(PRODUCER_NUM, sizeof(thread_info_t));
  clist = (thread_info_t*)calloc(CONSUMER_NUM, sizeof(thread_info_t));
  int epfd = epoll_create1(EPOLL_CLOEXEC);
  
  for (int i=0; i<CONSUMER_NUM; ++i) {
    clist[i].epfd = epfd;
    clist[i].rank = i;
    pthread_create(&clist[i].thread_id, NULL, consumer_routine, &clist[i]);
  }
  
  for (int i=0; i<PRODUCER_NUM; ++i) {
    plist[i].epfd = epfd;
    plist[i].rank = i;
    pthread_create(&plist[i].thread_id, NULL, producer_routine, &plist[i]);
  }
  
  
  for (int i=0; i<CONSUMER_NUM; ++i) {
    pthread_join(clist[i].thread_id, NULL);
  }
  
  for (int i=0; i<PRODUCER_NUM; ++i) {
    pthread_join(plist[i].thread_id, NULL);
  }
  
  free(clist);
  free(plist);
  return 0;
}