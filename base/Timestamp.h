//
// Created by onirii on 2021/2/20.
//

#ifndef EASYSERVER_TIMESTAMP_H
#define EASYSERVER_TIMESTAMP_H

#include <stdlib.h>
#include <sys/time.h>
#include <string>

namespace es {

class Timestamp {
public:
  Timestamp() : microSecondFromEpoch_(0) {}
  ~Timestamp() = default;
  explicit Timestamp(int64_t msFromEpoch) : microSecondFromEpoch_(msFromEpoch) {}
  int64_t microSecondFromEpoch() const {
    return microSecondFromEpoch_;
  }
  
  bool operator<(const Timestamp& rhs) const {
    return microSecondFromEpoch_ < rhs.microSecondFromEpoch();
  }
  
  bool operator<=(const Timestamp& rhs) const {
    return microSecondFromEpoch_ <= rhs.microSecondFromEpoch();
  }
  
  bool operator>(const Timestamp& rhs) const {
    return microSecondFromEpoch_ > rhs.microSecondFromEpoch();
  }
  
  bool operator>=(const Timestamp& rhs) const {
    return microSecondFromEpoch_ >= rhs.microSecondFromEpoch();
  }
  
  bool operator==(const Timestamp& rhs) const {
    return microSecondFromEpoch_ == rhs.microSecondFromEpoch();
  }
  
  bool operator!=(const Timestamp& rhs) const {
    return microSecondFromEpoch_ != rhs.microSecondFromEpoch();
  }
  
  bool valid() const {
    return microSecondFromEpoch_ > 0;
  }
  
  std::string toString() const;
  
  static Timestamp now();
  static Timestamp invalid();
  static const int kMicroSecondsPerSecond = 1000 * 1000;
private:
  int64_t microSecondFromEpoch_;
};

inline Timestamp addTime(Timestamp timestamp, double seconds) {
  int64_t delta = static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSecond);
  return Timestamp(delta + timestamp.microSecondFromEpoch());
}

}

#endif //EASYSERVER_TIMESTAMP_H
