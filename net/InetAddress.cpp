//
// Created by onirii on 2021/2/23.
//

#include "InetAddress.h"
#include "net/SocketsOps.h"
#include <strings.h>

static const in_addr_t kInaddrAny = INADDR_ANY;

namespace es {

InetAddress::InetAddress(uint16_t port) {
  bzero(&addr_, sizeof(addr_));
  addr_.sin_family = AF_INET;
  addr_.sin_addr.s_addr = sockets::hostToNetwork32(kInaddrAny);
  addr_.sin_port = sockets::hostToNetwork16(port);
}

InetAddress::InetAddress(std::string &ip, uint16_t port) {
  bzero(&addr_, sizeof(addr_));
  sockets::fromHostPort(ip.c_str(), port, &addr_);
}

InetAddress::InetAddress(const sockaddr_in &addr) {
  addr_ = addr;
}

std::string InetAddress::toHostPort() const {
  char buf[32] = {0};
  sockets::toHostPort(buf, sizeof(buf), addr_);
  return buf;
}

const sockaddr_in &InetAddress::getSockAddrInet() const {
  return addr_;
}

void InetAddress::setSockAddrInet(const sockaddr_in &addr) {
  addr_ = addr;
}

}