# Distributed Consensus

## 分布式一致性算法

### (multi-)Paxos

### Raft

### Paxos与Raft区别

## 主要问题

### 读一致性

#### 方案1-允许脏读
[Consistent reads are not consistent](https://github.com/coreos/etcd/issues/741)

可以从任意节点读取，很容易出现脏读

#### 方案2-部分条件下强一致
永远从leader读写，但如果网络发生分区，出现了两个leader，此时可能会产生脏读。(老leader不会自己回退)

#### 方案3-任何条件下强一致
任何时候多从多个节点读取，如果有超过1/2的节点数据一致，那么数据在集群内必然是强一致的。

### 脑裂 

## 开源实现

### coreos/etcd

### Tencent/phxpaxos

### osadalakmal/cpp-raft

### kanaka/raft.js
