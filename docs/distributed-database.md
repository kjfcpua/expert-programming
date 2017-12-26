# database

## 典型数据库产品

### LevelDB

#### 关键技术
- 使用Snappy库压缩存储；

### hbase

### mysql

### h-store

### Percona

### Terark

### TiDB

主要模块

| 模块   | 功能                                       |
| ---- | ---------------------------------------- |
| TiDB | 主要是负责 SQL 的解析器和优化器，它相当于计算执行层，同时也负责客户端接入和交互。 |
| TiKV | 是一套分布式的 Key-Value 存储引擎，它承担整个数据库的存储层，数据的水平扩展和多副本高可用特性都是在这一层实现。TiKV是可替换的，早期的版本中使用的是HBase。 |
| PD   | 根据数据分布状况以及各个存储节点的负载来采取合适的调度策略，维持整个系统的平衡与稳定。 |

#### 关键技术
- Google F1/Spanner的实现；
- 协议与MySQL兼容；
- 内部通信使用gRPC；
- 使用Raft保证一致性;

### PostgreSQL

#### PostgreSQL-XC

#### PostgreSQL-XZ

## 存储结构

### B+ Tree

### LSM Tree

### Succinct Trie

## 压缩算法

## 性能测试

### YCSB



关系型和非关系型，混合型
分布式架构

行存储与列存储
