//
// Created by onirii on 2021/2/6.
//

#ifndef EASYSERVER_NONCOPYABLE_H
#define EASYSERVER_NONCOPYABLE_H

namespace es {

class noncopyable {

/*
 * ����ϣ���ⲿ�û����������󣬶��ǹ��������������ʱ
 * ͨ�����԰ѹ��� ��������Ϊprotected
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
