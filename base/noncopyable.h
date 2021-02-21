//
// Created by onirii on 2021/2/6.
//

#ifndef EASYSERVER_NONCOPYABLE_H
#define EASYSERVER_NONCOPYABLE_H

namespace es {

class noncopyable {
public:
  noncopyable() = default;
  noncopyable(const noncopyable&) = delete;
  noncopyable& operator=(const noncopyable&) = delete;
};

}

#endif //EASYSERVER_NONCOPYABLE_H
