//
// Created by onirii on 2021/2/23.
//

#ifndef EASYSERVER_INETADDRESS_H
#define EASYSERVER_INETADDRESS_H

#include "base/copyable.h"
#include <netinet/in.h>
#include <string>

namespace es {

class InetAddress : public copyable {
public:
  //本机ip 设置默认port
  explicit InetAddress(uint16_t port);
  
  InetAddress(std::string& ip, uint16_t port);
  
  InetAddress(const struct sockaddr_in& addr);
  
  std::string toHostPort() const;
  
  const sockaddr_in& getSockAddrInet() const;
  void setSockAddrInet(const struct sockaddr_in& addr);

private:
  struct sockaddr_in addr_;
};

}

#endif //EASYSERVER_INETADDRESS_H
