/*
 * io.c
 *
 *  Created on: Aug 25, 2017
 *      Author: hanchen
 */

int WaitForFd(int fd, int ev, long msec, ...) {
  fd_set rfds, wfds, efds;
  int num, rev = 0;
  struct timeval tv;

  if (msec < 0) {
    msec = 0;
  }

  tv.tv_sec = msec / 1000;
  tv.tv_usec = (msec % 1000) * 1000;

  FD_ZERO(&rfds);
  if (ev & FD_EV_RD) {
    FD_SET(fd, &rfds);
  }

  FD_ZERO(&wfds);
  if (ev & FD_EV_WR) {
    FD_SET(fd, &wfds);
  }

  FD_ZERO(&efds);
  if (ev & FD_EV_ERR) {
    FD_SET(fd, &efds);
  }

  num = select(fd + 1, &rfds, &wfds, &efds, &tv);
  if (num < 0) {
    return -1;
  }

  if (FD_ISSET(fd, &rfds)) {
    rev = rev | FD_EV_RD;
  }

  if (FD_ISSET(fd, &wfds)) {
    rev = rev | FD_EV_WR;
  }

  if (FD_ISSET(fd, &efds)) {
    rev = rev | FD_EV_ERR;
  }

  // DEBUGS("ev: %d, rev: %d", ev, rev);

  return rev;
}

ssize_t ReadEx(int fd, void *buf, size_t len, long timeo, int *flags, ...) {
  void *p = buf;
  ssize_t n = 0;
  size_t sum = 0;
  uint32_t errs[32];  // 256 bit
  int err, po = 0;
  __attribute__ ((unused)) int bv;
  va_list ap;
  struct timeval s_tv, e_tv;
  long tm, left = timeo;
  int socktype, tms = 0, flgs = 0;

  va_start(ap, flags);
  while (1) {
    err = va_arg(ap, int);
    if (err <= 0 || err >= 256) {
      break;
    }
    bv = BITMAP_SET((void *) errs, 1, err, 0, 1);
  }
  va_end(ap);

  socktype = GetSockType(fd);

  if (timeo >= 0) {
    gettimeofday(&s_tv, NULL);
  }

  while (sum < len) {
    if (timeo >= 0) {  // if check timeout
      gettimeofday(&e_tv, NULL);
      tm = TimeDiff(&e_tv, &s_tv);
      if (tm < 0) {  // avoid cpu in different time
        tm = 0;
      }
      left = timeo - tm;
    }

    /*
     * check tms make sure at least 1 times check socket
     */
    if ((timeo >= 0) && (left < 0) && (tms > 0)) {
      break;
    }

    tms++;

    po = WaitForFd(fd, FD_EV_RD, left);
    if (po <= 0) {  // timeout
      if (timeo >= 0) {  // check timeout
        break;
      } else {  // don't check timeout
        continue;
      }
    }

    n = read(fd, p, len);
    if (n > 0) {
      p = OFFOF(p, n);
      sum += n;
    } else if (n == 0) {  // EOF
      flgs = FD_FLAG_EOF;
      break;
    } else {
      err = BITMAP_GET((void *) errs, 1, errno, 0);
      if (err == 0) {  // not mask
        break;
      }
    }

    if (socktype == SOCK_DGRAM) {
      break;
    }
  }

  if (sum <= 0) {  // read no data
    if ((po < 0) || (n < 0)) {  // first read fail
      return -1;
    }
  }

  if (flags != NULL) {
    *flags = flgs;
  }

  return sum;
}

ssize_t WriteEx(int fd, const void *buf, size_t len, long timeo, int *flags,
                ...) {
  void *p = (void *) buf;
  ssize_t n = 0;
  size_t sum = 0;
  uint32_t errs[32];  // 256 bit
  int err, po = 0;
  __attribute__ ((unused)) int bv = 0;  // only avoid warn in clang
  va_list ap;
  struct timeval s_tv, e_tv;
  long tm, left = 1000;
  int socktype, tms = 0, flgs = 0;

  va_start(ap, flags);
  while (1) {
    err = va_arg(ap, int);
    if (err <= 0 || err >= 256) {
      break;
    }
    bv = BITMAP_SET((void *) errs, 1, err, 0, 1);
  }
  va_end(ap);

  socktype = GetSockType(fd);

  if (timeo >= 0) {
    gettimeofday(&s_tv, NULL);
  }

  while (sum < len) {
    if (timeo >= 0) {  // if check timeout
      gettimeofday(&e_tv, NULL);
      tm = TimeDiff(&e_tv, &s_tv);
      left = timeo - tm;
    }

    if ((timeo >= 0) && (left < 0) && (tms > 0)) {
      break;
    }

    tms++;

    po = WaitForFd(fd, FD_EV_WR, left);
    // DEBUGS("po: %d", po);
    if (po <= 0) {  // timeout
      if (timeo >= 0) {  // check timeout
        break;
      } else {  // don't check timeout
        continue;
      }
    }

    n = write(fd, p, len);
    // DEBUGS("ret: %zd, errno: %d", n, errno);
    if (n > 0) {
      p = OFFOF(p, n);
      sum += n;
    } else {
      err = BITMAP_GET((void *) errs, 1, errno, 0);
      if (err == 0) {  // not mask
        break;
      }
    }

    if (socktype == SOCK_DGRAM) {
      break;
    }
  }

  if (sum <= 0) {  // write no data
    if (n < 0) {  // first write fail
      return -1;
    }
  }

  if (flags != NULL) {
    *flags = flgs;
  }

  return sum;
}

/*****************************SOCKET****************************/
int IsResvEth(const eth_addr_t *eth) {
  static const char resv_addrs[][20] = { "0.0.0.0", "10.0.0.0", "127.0.0.1",
      "169.254.0.0", "172.16.0.0", "192.0.0.0", "192.0.2.0", "192.88.99.0",
      "192.168.0.0", "198.18.0.0", "198.51.100.0", "203.0.113.0", "224.0.0.0",
      "240.0.0.0", "255.255.255.255" };
  static const char resv_brdaddrs[][20] = { "0.0.0.0", "255.255.255.255" };
  int i;

  for (i = 0; i < (int) ARRLEN(resv_addrs); i++) {
    if (inet_addr(resv_addrs[i]) == eth->addr) {
      return 1;  // true
    }
  }

  for (i = 0; i < (int) ARRLEN(resv_brdaddrs); i++) {
    if (inet_addr(resv_brdaddrs[i]) == eth->broadaddr) {
      return 1;  // true
    }
  }

  return 0;
}

int IsLanEth(const eth_addr_t *eth) {
  uint8_t cNet;

  cNet = (uint8_t) (eth->addr & 0xff);  // cut off
  if (cNet == 10 || cNet == 128 || cNet == 172 || cNet == 192) {
    return 1;  // true
  }

  return 0;  // false
}

int GetLanEths(eth_addr_t *eth, int num, const char *name) {
  eth_addr_t es[32];
  int i, n = 0, en;

  en = GetEths(es, ARRLEN(es), name);
  if (en <= 0) {
    return -1;
  }

  for (i = 0; i < en; i++) {
    if (n >= num) {
      break;
    }
    if (IsResvEth(&es[i])) {
      continue;
    }
    if (IsLanEth(&es[i])) {
      eth[n++] = es[i];
    }
  }

  return n;
}

int GetWanEths(eth_addr_t *eth, int num, const char *name) {
  eth_addr_t es[32];
  int i, n = 0, en;

  en = GetEths(es, ARRLEN(es), NULL);
  if (en <= 0) {
    return -1;
  }

  for (i = 0; i < en; i++) {
    if (n >= num) {
      break;
    }
    if (IsResvEth(&es[i])) {
      continue;
    }
    if (!IsLanEth(&es[i])) {
      eth[n++] = es[i];
    }
  }

  return n;
}

/**
 * eths, the eths match with name
 * num,  the num of eths to get
 * name, the eth name to get, if NULL, match all eths
 */
int GetEths(eth_addr_t *eths, int num, const char *name) {
  struct ifconf ifc;
  struct ifreq *ifr, ifreq;
  char ifcb[65536], pname[256];
  eth_addr_t eth;
  int sock = -1, ret, cnt = 0;
  size_t off = 0, len = 0;

  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0) {
    return -1;
  }

  memset(ifcb, 0, sizeof(ifcb));
  memset(&ifc, 0, sizeof(ifc));
  ifc.ifc_len = sizeof(ifcb);
  ifc.ifc_buf = (caddr_t) ifcb;

  ret = ioctl(sock, SIOCGIFCONF, (char *) &ifc);
  if (ret < 0) {
    DEBUGS("ioctl fail, ret: %d", ret);
    close(sock);
    return -2;
  }

  memset(pname, 0, sizeof(pname));
  while (off < ifc.ifc_len) {
    ifr = OFFOF(ifc.ifc_req, off);

#if defined(__APPLE__)
    len = IFNAMSIZ + ifr->ifr_addr.sa_len;
#else
    len = sizeof(*ifr);
#endif

    off += len;

    if (strcmp(pname, ifr->ifr_name) == 0) {
      continue;
    }

    // DEBUGS("%s", ifr->ifr_name);

    strncpy_v(pname, ifr->ifr_name, sizeof(pname));

    if ((name != NULL) && (strcmp(ifr->ifr_name, name) != 0)) {
      continue;
    }

    memset(&eth, 0, sizeof(eth));

    // name
    strncpy_v(eth.name, ifr->ifr_name, sizeof(eth.name));
    strncpy_v(ifreq.ifr_name, ifr->ifr_name, IFNAMSIZ);

    // addr
    ret = ioctl(sock, SIOCGIFADDR, &ifreq);
    if (ret == 0) {
      eth.addr = ((struct sockaddr_in *) &ifreq.ifr_addr)->sin_addr.s_addr;
      // DEBUGS("addr: %s", inet_ntoa(*(struct in_addr *) &eth.addr));
    } else {
      // DEBUGS("ret: %d, errno: %d", ret, errno);
    }

    // broadaddr
    ret = ioctl(sock, SIOCGIFBRDADDR, &ifreq);
    if (ret == 0) {
      eth.broadaddr = ((struct sockaddr_in *) &ifreq.ifr_broadaddr)->sin_addr
          .s_addr;
      // DEBUGS("broadaddr: %s", inet_htoa(*(struct in_addr *) &eth.broadaddr));
    }

    // netmask
    ret = ioctl(sock, SIOCGIFNETMASK, &ifreq);
    if (ret == 0) {
#if defined(__APPLE__)
      eth.netmask = ((struct sockaddr_in *) &ifreq.ifr_addr)->sin_addr.s_addr;
#else
      eth.netmask = ((struct sockaddr_in *) &ifreq.ifr_netmask)->sin_addr.s_addr;
#endif
      // DEBUGS("netmask: %s", inet_htoa(*(struct in_addr *) &eth.netmask));
    }

    eths[cnt++] = eth;
    if (cnt >= num) {
      break;
    }
  }

  close(sock);

  return cnt;
}

int GetHostByName(in_addr_t *addrs, int num, const char *name) {
  struct addrinfo hints, *res, *reses;
  int ret, n = 0;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = PF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  ret = getaddrinfo(/*"www.zheezes.com"*/name, "http", &hints, &reses);
  if (ret != 0) {
    DEBUGS("getaddrinfo fail, ret: %d, error: %s", ret, gai_strerror(ret));
    return -1;
  }

  for (res = reses; res != NULL; res = res->ai_next) {
    if (n >= num) {
      break;
    }
    addrs[n] = ntohl(((struct sockaddr_in *) res->ai_addr)->sin_addr.s_addr);
    n++;
  }

  return n;
}

int CreateSock(in_addr_t locaddr, int locport, in_addr_t svraddr, int svrport,
               int type, int timeo) {
  struct sockaddr_in sa;
  int sock = -1, reuse = 1, ret = 0, nonb = 0;

#define __AT_EXIT__(ret) \
({ \
  typeof(ret) _ret = ret; \
  close(sock); \
  _ret; \
})

  if ((sock = socket(AF_INET, type, 0)) < 0) {
    return __AT_EXIT__(-2);
  }

  ret = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
  if (ret < 0) {
    return __AT_EXIT__(-6);
  }

  if (timeo >= 0) {  // set to nonblock for a short time
    ret = SetSockBlock(sock, 0);
    if (ret < 0) {
      return __AT_EXIT__(-6);
    }
    nonb = 1;
  }

  if ((locaddr != INADDR_ANY) || (locport != 0)) {
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = /*htonl(*/locaddr/*)*/;
    sa.sin_port = htons(locport);
    ret = bind(sock, (struct sockaddr *) &sa, sizeof(sa));
    if (ret < 0) {
      DEBUGS("bind, ret: %d, errno: %d, addr: %s, port: %d", ret, errno,
             inet_ntoa(*(struct in_addr *) &locaddr), locport);
      return __AT_EXIT__(-3);
    }
  }

  if ((svraddr != INADDR_ANY) && (svrport != 0)) {
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = /*htonl(*/svraddr/*)*/;
    sa.sin_port = htons(svrport);
    ret = connect(sock, (struct sockaddr *) &sa, sizeof(sa));
    if (ret < 0) {
      if (nonb) {
        if (errno != EINPROGRESS) {
          return __AT_EXIT__(-4);
        }
        ret = CheckSockConnect(sock, timeo);
        if (ret < 0) {  // timeout
          return __AT_EXIT__(-5);
        }
      } else {
        DEBUGS("connect fail, ret: %d, errno: %d, error: %s", ret, errno,
               strerror(errno));
        return __AT_EXIT__(-4);
      }
    }
  }

  if (nonb) {  // restore to block
    ret = SetSockBlock(sock, 1);
    if (ret < 0) {
      return __AT_EXIT__(-6);
    }
  }

#undef  __AT_EXIT__

  return sock;
}

int CheckSockConnect(int sock, long msec) {
  struct timeval begin, end;
  long tm = 0, left = 0;
  int ret, serr = 0, ts = 0;
  socklen_t serrlen = sizeof(serr);

  gettimeofday(&begin, NULL);
  for (ts = 0; ts < 70 * 1000; ts++) {  // block sock just wait 70s
    if (ts > 0) {  // allow wait once at least
      gettimeofday(&end, NULL);
      tm = TimeDiff(&end, &begin);
      if (tm > msec) {  // timeout
        break;
      }
    }
    left = msec - tm;
    ret = WaitForFd(sock, FD_EV_WR, left);
    if (ret <= 0) {
      continue;
    }
    /* when get the error, kernel will reset the error,
     * so you can't get error twice
     */
    ret = getsockopt(sock, SOL_SOCKET, SO_ERROR, &serr, &serrlen);
    // The relevant values are:
    // #define  ETIMEDOUT 110 /* Connection timed out */
    // #define  ECONNREFUSED  111 /* Connection refused */
    // #define  EHOSTDOWN 112 /* Host is down */
    // #define  EHOSTUNREACH  113 /* No route to host */
    // #define  EALREADY  114 /* Operation already in progress */
    // #define  EINPROGRESS 115 /* Operation now in progress */
    if (ret < 0) {
      return -1;
    }
    if (serr != 0) {
      continue;
    }
    return 0;
  }

  return -1;
}

int __SetSockTimeout(int sock, int type, int msec) {
  struct timeval tv;

  if (sock < 0) {
    return -1;
  }

  if ((type != SO_SNDTIMEO) && (type != SO_RCVTIMEO)) {
    return -2;
  }

  if (msec < 0) {
    return -3;
  }

  tv.tv_sec = msec / 1000;
  tv.tv_usec = (msec % 1000) * 1000;

  return setsockopt(sock, SOL_SOCKET, type, &tv, sizeof(tv));
}

ALIAS_FUNC_3( SetSockTimeout, __SetSockTimeout, int, int, int, int);

WEAK_FUNC_2( SetSockBlock, _, int, int sock, int block) {
  int flags;

  if (sock < 0) {
    return -1;
  }

  flags = fcntl(sock, F_GETFL, 0);
  if (flags < 0) {
    return -2;
  }

  if (block == 0) {
    flags = flags | O_NONBLOCK | O_NDELAY;
  } else {
    flags = flags & ~(O_NONBLOCK | O_NDELAY);
  }

  flags = fcntl(sock, F_SETFL, flags);
  if (flags < 0) {
    return -2;
  }

  return 0;
}

ALIAS_FUNC_2( SetSockBlock, _SetSockBlock, int, int, int);

WEAK_FUNC_1( SetNonBlock, _, int, int sock) {
  return SetSockBlock(sock, 0);
}

ALIAS_FUNC_1( SetNonBlock, _SetNonBlock, int, int);

int GetSockType(int sock) {
  int ret, socktype = -1;
  socklen_t optlen = 0;

  optlen = sizeof(socktype);
  ret = getsockopt(sock, SOL_SOCKET, SO_TYPE, &socktype, &optlen);
  if (ret < 0) {
    return -1;
  }

  return socktype;
}

int __UdpPing(uint32_t dwLocHost, uint32_t dwHost, int iPort, int timeo) {
  char sbuf[75];
  ssize_t len;
  int iSock, err = 0;

  iSock = CreateSock(dwLocHost, 0, dwHost, iPort, SOCK_DGRAM, -1);
  if (iSock < 0) {
    return -1;
  }

  len = send(iSock, sbuf, sizeof(sbuf), 0);
  if (len < 0) {
    err = errno;
  } else {
    if (WaitForFd(iSock, FD_EV_RD | FD_EV_ERR, timeo) > 0) {
      len = send(iSock, sbuf, sizeof(sbuf), 0);  // greate then 45
      if (len < 0) {
        err = errno;
      }
    }
  }
  close(iSock);

  if ((len < 0) || (err != 0)) {
    return -1;
  }

  if (err == ECONNREFUSED) {
    return 1;  // icmap fail
  }

  return 0;
}

ALIAS_FUNC_4( UdpPing, __UdpPing, int, uint32_t, uint32_t, int, int);
