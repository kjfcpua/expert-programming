/*
 * io.h
 *
 *  Created on: Aug 25, 2017
 *      Author: hanchen
 */

#ifndef IO_H_
#define IO_H_

#ifndef BUF_UDP_MAX
#define BUF_UDP_MAX (65535)
#endif

#ifndef BUF_TCP_MAX
#define BUF_TCP_MAX (65535)
#endif

#ifndef BUF_NET_MAX
#define BUF_NET_MAX (65535)
#endif

#define FD_EV_RD  (1 << 0) // 1
#define FD_EV_WR  (1 << 1) // 2
#define FD_EV_ERR (1 << 2) // 4
#define FD_EV_ALL (FD_EV_RD | FD_EV_WR | FD_EV_ERR)

#define FD_FLAG_EOF     1 // read eof
#define FD_FLAG_CLOSED  2 // write bad file

/*
 * simple for select system call
 * ignore some signals except define in the params which end with 0
 *
 * about write:
 * if the socket has been closed by the other side, now you can to wirte
 * and without error event until you close write self
 *
 * On success, return the ev get
 * else, return -1
 **/
int WaitForFd(int fd, int ev, long msec, ...);

/*
 * read until count == len,
 * ignore some signals except define in the params which end with 0
 * timeo is max time when read, if < 0, will read until enough
 *
 * need to set fd to nonblock by yourself before called
 *
 * if read len or get some unknown errno, return the count readed
 * if read EOF, return -count
 **/
ssize_t ReadEx(int fd, void *buf, size_t len, long timeo, int *flags, ...);

/*
 * write until count == len,
 * ignore some signals except define in the params which end with 0
 * timeo is max time when write, if < 0, will write until read enough
 *
 * need to set fd to nonblock by yourself before called
 *
 * if no byte wrote, will return -1
 */
ssize_t WriteEx(int fd, const void *buf, size_t len, long timeo,
                int *flags, ...);

typedef struct {
    char      name[IFNAMSIZ];
    in_addr_t addr;
    in_addr_t broadaddr;
    in_addr_t netmask;
} eth_addr_t;

/*
 * 1.check ip, is localhost, broadcast or other resverse ip
 * 2.check broadcast, is 0.0.0.0 or 255.255.255.255
 *
 * return
 * 0, false
 * other, true
**/
int IsResvEth(const eth_addr_t *eth);
int IsLanEth(const eth_addr_t *eth);
int GetLanEths(eth_addr_t *eths, int num, const char *name);
int GetWanEths(eth_addr_t *eths, int num, const char *name);
/*
 * name, eth name, if NULL, will get all eths
 *
 * return
 * <  0, fail
 * >= 0, eth num
**/
int GetEths(eth_addr_t *eths, int num, const char *name);

int GetHostByName(in_addr_t *addrs, int num, const char *name);

/*
 * locaddr, local bind ip, host order, INADDR_ANY not bind
 * locport, local bind port, host order, 0 not bind
 * local host or port can be bind only one
 *
 * svraddr, remote connect ip, INADDR_ANY not connect
 * svrport, remote connect port, 0 not connect
 * remote host and port must be exist at same time and can be connect
 *
 * type, now support SOCK_DGRAM(udp) or SOCK_STREAM(tcp)
 *
 * timeo, connect timeout
 * if < 0, will block until connect system call terminate
 * if >= 0, will block timeo at most
 *
 * return
 * >=  0, success
 * == -1, bad params
 * == -2, create socket fail
 * == -3, local bind fail
 * == -4, remote connect fail
 * == -5, remote connect wait timeout, not connected until timeout
 * == -6, set socket opt fail
**/
int CreateSock(in_addr_t locaddr, int locport, in_addr_t svraddr, int svrport, int type, int timeo);
int CheckSockConnect(int sock, long msec);
int SetSockTimeout(int sock, int type, int msec);
int SetSockBlock(int sock, int block);
int SetNonBlock(int sock);
int GetSockType(int sock);
int UdpPing(uint32_t dwLocHost, uint32_t dwHost, int iPort, int timeo);

#endif /* IO_H_ */
