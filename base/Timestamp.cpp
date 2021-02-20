//
// Created by ∂≠∫£≈Ù on 2021/2/20.
//

#include "Timestamp.h"
#include <inttypes.h>

namespace es {

Timestamp Timestamp::now() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  int64_t microSecondFromEpoch = tv.tv_sec * kMicroSecondsPerSecond + tv.tv_usec;
  return Timestamp(microSecondFromEpoch);
}

Timestamp Timestamp::invalid() {
  return Timestamp();
}

std::string Timestamp::toString() const {
  char buf[32] = {0};
  int64_t seconds = microSecondFromEpoch_ / kMicroSecondsPerSecond;
  int64_t microseconds = microSecondFromEpoch_ % kMicroSecondsPerSecond;
  snprintf(buf, sizeof(buf), "%" PRId64 ".%06" PRId64 "", seconds, microseconds);
  return buf;
}

}