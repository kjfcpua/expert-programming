# Linux IO

## Cache

### Page Cache
VFS的Cache，大小为4K。

### Buffer Cache

### Page和Buffer区别
- Page和Buffer处于IO不同的阶段，write先到Page，刷新磁盘的时候会到Buffer；
- Buffer会复用Page的内存，但inode不属于文件内容，并不会在Page中，则是独立的Buffer；

## Direct IO

## AIO

### 标准IO的不足
Posix的`write`在非阻塞模式下，无法获取IO完成事件，如果需要处理`write`完成事件，则必须要使用阻塞模式。
在协程库libco中，如果`write`是非阻塞模式，则不会进行协程处理。但如果`write`是阻塞模式，磁盘IO可能会导致协程阻塞，并且协程无法切换，这样会对性能造成一定影响。(备注：不过这与协程库实现有关，在golang的协程中，阻塞的系统调用也会触发协程切换。)
```c++
if (!lp || (O_NONBLOCK & lp->user_flag)) {
  ssize_t ret = g_sys_write_func(fd, buf, nbyte);
  return ret;
}
```
如果需要使用`write`非阻塞模式并处理完成事件，则可以使用AIO。

### glibc AIO
多线程模型。

```c++
#include <aio.h>

int aio_read(struct aiocb *aiocbp);
int aio_write(struct aiocb *aiocbp);
int lio_listio(int mode, struct aiocb *const aiocb_list[], int nitems, 
               struct sigevent *sevp);
```

### Linux Kernel Native AIO
真正实现内核异步通知。
但要注意只能使用DIRECT_IO。

```c++
#include <libaio.h>

struct iocb {
  void     *data;
  unsigned key;
  short    aio_lio_opcode;
  short    aio_reqprio;
  int      aio_fildes;
};

typedef void (*io_callback_t)(io_context_t ctx, struct iocb *iocb, long res, long res2);

inline void io_prep_pwrite(struct iocb *iocb, int fd, void *buf, size_t count, long long offset);
inline void io_prep_pread(struct iocb *iocb, int fd, void *buf, size_t count, long long offset);
static inline void io_prep_fsync(struct iocb *iocb, int fd);
static inline void io_set_callback(struct iocb *iocb, io_callback_t cb);
int io_submit(io_context_t ctx, long nr, struct iocb *iocbs[]);
```

## 参考文档

- [Linux Kernel AIO](http://www.wzxue.com/linux-kernel-aio%E8%BF%99%E4%B8%AA%E5%A5%87%E8%91%A9/)
