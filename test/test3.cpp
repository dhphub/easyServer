//
// Created by onirii on 2021/2/20.
//

#include "net/EventLoop.h"

void print(const char *msg) {
  printf("%s\n", msg);
}

int main(int argc, char **argv) {
  es::EventLoop loop;
  loop.runAfter(1.0, std::bind(print, "after 1.0s"));
  loop.runAfter(2.0, std::bind(print, "after 2.0s"));
  loop.runAfter(1.5, std::bind(print, "after 1.5s"));
  loop.runEvery(1.0, std::bind(print, "every 1.0s"));
  loop.runEvery(3.0, std::bind(print, "every 3.0"));
  loop.loop();
  
  print("main loop exists\n");
  sleep(1);
}