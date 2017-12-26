# Distributed Storage System

## 概要

### 分布式系统的挑战
- 单机硬件或系统不可靠；
- 大规模服务需要大量水平扩展；

### 概念
RT，响应时间，是指系统对请求作出响应的时间；
Throughput，吞吐量，是指系统在单位时间内处理请求的数量；
PV(Page View)；
QPS(Query Per Second)，每秒查询次数，比如执行了select操作，相应的QPS会增加；
TPS(Transactions Per Second)，每秒事务数，比如执行了dml操作，那么相应的TPS会增加；
RPS(Requests Per Second)，每秒请求次数，RPS=并发数/平均响应时间；


## 存储类型

| 存储类型 | 英文全称                 | 英文缩写 |
| ---- | -------------------- | ---- |
| 块存储  | Block Storage        | BS   |
| 对象存储 | Object-Based Storage | OS  |
| 文件存储 | File Storage         | FS   |
| 键值存储 | Key-Value Storage    | KV   |

### 块存储

### 对象存储

### 文件存储

### 键值存储

## 主流分布式存储系统

| 文件系统                                   | 存储类型     | 语言     | 开发者                        | 开源协议        |
| -------------------------------------- | -------- | ------ | -------------------------- | ----------- |
| GFS                                    | OS       | C++    | Google                     |             |
| HDFS                                   | OS       | Java   | Apache                     |             |
| AFS                                    | FS       | C++    | IBM                        |             |
| GlusterFS                              | FS       | C++    | Z RESEARCH                 |             |
| Lustre                                 | OS       | C++    | Cluster File Systems  Inc. | GNU GPL 2.0 |
| [Ceph](http://ceph.com/)               | BS/FS/OS | C++    | Ceph                       |             |
| OpenStack  Swift                       | OS       | Python | OpenStack                  | Apache 2.0  |
| Riak                                   | KV       | Erlang | Amazon                     |             |
| [LeoFS](http://leo-project.net/leofs/) | OS       | C++    |                            |             |
| Haystack                               | OS       | C++    | Facebook                   |             |
| [TFS](http://tfs.taobao.org/)          | OS       | C++    |                            |             |
| MogileFS                               | KV       | Perl   |                            |             |
| FastDFS                                | KV       | C      |                            |             |
| GridFS                                 | OS       |        |                            |             |
| KFS                                    |          | C++    |                            |             |

## Alibaba系

### 组件体系

| 组件   | 作用   |
| ---- | ---- |
| 夸父   | 网络   |
| 女娲   | 协同   |
| 伏羲   | 调度   |
| 盘古   | 存储   |
| 神农   | 监控   |

### 盘古
| 组件     | 作用   |
| ------ | ---- |
| Client | 客户端  |
| Master | 元数据  |
| Chunk  | 真实数据 |

#### Volume

Volume，文件？

#### 一致性

三副本强一致。

#### 完整性
- 端到端的数据校验，主要是CRC校验；
- 静默数据检查；

#### 性能
读:150K QPS
写: 50K QPS
如何高性能？

#### 成本
纠删码

#### 面向容错的设计思想
- Checksum和数据自愈；
- 服务监控、环境检查、故障报警；
- 误删保护，比如回收站和Read-Only属性；

#### Chunk
- SSD作为Cache，然后转储到机械盘；
- 随机写转为顺序写；

#### 协程
- C++的协程是单线程的，不能使用Sleep这样的阻塞操作；
- C++的异常可能会导致协程出现问题；

#### 工程实践
- 持续集成，问题代码回退；
- 没有专职测试，端到端的全栈研发；

### Tair

#### ConfigServer
ConfigServer获取数据分布的对照表，类似于Ceph的CRUSH算法，Client将会获取和缓存对照表。
对照表有一个版本号，当数据节点发生增删时，对照表更新，版本号增加，然后通过心跳同步给数据节点。当客户端请求数据节点或者数据节点之间通信时，如果发现版本号不相同，则会用新版本的对照表覆盖旧版本。

#### 存储引擎
Tair默认包含两个存储引擎：mdb和fdb。
mdb是一个高效的缓存存储引擎，它有着和memcached类似的内存管理方式。mdb支持使用share memory，这使得我们在重启Tair数据节点的进程时不会导致数据的丢失，从而使升级对应用来说更平滑，不会导致命中率的较大波动。
fdb是一个简单高效的持久化存储引擎，使用树的方式根据数据key的hash值索引数据，加快查找速度。索引文件和数据文件分离，尽量保持索引文件在内存中，以便减小IO开销。使用空闲空间池管理被删除的空间。

## GFS

## Ceph
真正的分布式架构，无中心查询节点。

## TFS
海量小文件。

## MogileFS
Web中处理海量小图片，有依赖模块的问题。

## FastDFS
MogileFS改进后的文件系统，只能通过Client API方式访问，不支持posix方式访问。

## GridFS
MongoDB内置功能，不支持大文件存储。

## KFS
主要应用图片存储、搜索引擎、网络计算、数据挖掘等方面。
