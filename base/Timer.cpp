//
// Created by onirii on 2021/2/20.
//

#include "Timer.h"
namespace es {

void Timer::restart(Timestamp now) {
  if (repeat_) {
    expiration_ = addTime(now, interval_);
  } else {
    expiration_ = Timestamp::invalid();
  }
}

}