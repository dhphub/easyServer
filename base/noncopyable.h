//
// Created by onirii on 2021/2/6.
//

#ifndef EASYSERVER_NONCOPYABLE_H
#define EASYSERVER_NONCOPYABLE_H

namespace es {

class noncopyable {

/*
 * 当不希望外部用户构造此类对象，而是构造此类的子类对象时
 * 通常可以把构造 析构声明为protected
 */
protected:
  noncopyable() = default;
  ~noncopyable() = default;

public:
  noncopyable(const noncopyable&) = delete;
  noncopyable& operator=(const noncopyable&) = delete;
  
};

}

#endif //EASYSERVER_NONCOPYABLE_H
