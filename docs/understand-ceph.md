# Understand Ceph

## 总体结构

### MDS

MDS(Metadata server cluster)是Ceph的元数据服务器，只有在Cephfs的时候才需要。

MDS本身不存储数据，而是将数据存储到OSD节点中。创建Cephfs时，需要创建两个Pool，一个用来存放元数据，一个用来存放数据。当MDS收到请求需要读写元数据时，将会从OSD读取数据，然后缓存到本地内存中缓存。MDS承担了元数据接口、逻辑、缓存的功能。

### OSD

OSD(Object storage cluster)是对象存储服务。

每个OSD负责处理一块存储，存储可以是一个磁盘分区，或者是整个磁盘。

每个OSD监听4个端口，分别用于接收客户请求、传输数据、发送心跳、同步数据等操作。

OSD进程除了向Mon发送心跳外，还会和其他OSD互相发送心跳，以检查pg副本数是否正常。(随着集群增长会产生网络风暴？)

### Clients


### CMON

CMON(Cluster monitors)是集群监控服务，负责监控Ceph集群内的OSD状态，以及更新CRUSH MAP。


## 外部接口

### 对象存储

RGW(Rados Gateway)

LIBRADOS

### 块存储

RBD 是通过librbd库对应用提供块存储，主要面向云平台的虚拟机提供虚拟磁盘；RBD类似传统的SAN存储，提供数据块级别的访问；

目前 RBD 提供了两个接口，一种是直接在用户态实现， 通过 QEMU Driver 供 KVM 虚拟机使用。 另一种是在操作系统内核态实现了一个内核模块。通过该模块可以把块设备映射给物理主机，由物理主机直接访问。

### 文件存储

Ceph 文件系统服务提供了兼容 POSIX 的文件系统，可以直接挂载为用户空间文件系统。它跟传统的文件系统如Ext4是一个类型，区别在于分布式存储提供了并行化的能力；

## IO路径
file->pg->osd映射

### 文件存储

File切分成一个或多个Object，每个Object为2M-4M。
Object映射到PG(Placement Group)，一个Object只可能在一个PG，而每个PG有多个Object，PG的数量与集群OSD的数量有关。//TODO(来源)
PG通过CRUSH算法映射到一组(副本数量)OSD(Object Storage Device)，OSD即为磁盘进程，每个磁盘对应一个OSD。

PG提供了一致性hash的功能，通过一个固定的虚拟位置，保证了底层OSD增减时，只会影响相关OSD的映射。

### OSD MAP

要理解OSD MAP才能理解CRUSH算法的全部。
root-->bucket(datacenter, rack, host)-->dev(disk)

### CRUSH MAP

#### cluster map变化

cluster map的变化是以lazy的形式扩散的，Monitor并不会在cluster map更新后立刻同步给所有的OSD，而是在OSD向Monitor上报信息时，将cluster map回复给OSD。除此之外，如果OSD之间通信时，发现对方的cluster map版本较低，则把自己的OSD发送给对方，也就是更新push的模式。

Pool概念，副本概念；

Monitor

|         |                                          |
| ------- | ---------------------------------------- |
| ino     | File ID                                  |
| ono     | File切分成Object的编号，比如被切分成n个Object，no是[0,n)。 |
| oid     | {field_id, ono}                          |
| pgid    | hash(oid) & mask                         |
| osd set | CRUSH(pgid)                              |
|         |                                          |


## CRUSH算法

CRUSH算法是Ceph的一大创新，对于元数据的管理，通过计算替代了传统的索引结构。

从代码上看，ceph提供了5种算法，STRAW在实践中被发现存在问题，当节点出现增减时，可能会影响其他节点，被替换为STRAW2。

**src/crush/mapper.c**

```c++
static int crush_bucket_choose(const struct crush_bucket *in,
			                  struct crush_work_bucket *work,
			                  int x, int r,
                               const struct crush_choose_arg *arg,
                               int position)
{
	dprintk(" crush_bucket_choose %d x=%d r=%d\n", in->id, x, r);
	BUG_ON(in->size == 0);
	switch (in->alg) {
	case CRUSH_BUCKET_UNIFORM:
		return bucket_uniform_choose(
			(const struct crush_bucket_uniform *)in,
			work, x, r);
	case CRUSH_BUCKET_LIST:
		return bucket_list_choose((const struct crush_bucket_list *)in,
					  x, r);
	case CRUSH_BUCKET_TREE:
		return bucket_tree_choose((const struct crush_bucket_tree *)in,
					  x, r);
	case CRUSH_BUCKET_STRAW:
		return bucket_straw_choose(
			(const struct crush_bucket_straw *)in,
			x, r);
	case CRUSH_BUCKET_STRAW2:
		return bucket_straw2_choose(
			(const struct crush_bucket_straw2 *)in,
			x, r, arg, position);
	default:
		dprintk("unknown bucket %d alg %d\n", in->id, in->alg);
		return in->items[0];
	}
}
```

参数说明：

| Param | Remark           |
| ----- | ---------------- |
| x     | object_id或者pg_id |
| r     | replica_num，副本数量 |
|       |                  |

具体算法结构如下：

src/crush/crush.h

```c++
/** @ingroup API
 * The weight of each item in the bucket when
 * __h.alg__ == ::CRUSH_BUCKET_UNIFORM.
 */
struct crush_bucket_uniform {
       struct crush_bucket h; /*!< generic bucket information */
	__u32 item_weight;  /*!< 16.16 fixed point weight for each item */
};

/** @ingroup API
 * The weight of each item in the bucket when
 * __h.alg__ == ::CRUSH_BUCKET_LIST.
 *
 * The weight of __h.items[i]__ is __item_weights[i]__ for i in
 * [0,__h.size__[. The __sum_weight__[i] is the sum of the __item_weights[j]__
 * for j in [0,i[.
 *
 */
struct crush_bucket_list {
        struct crush_bucket h; /*!< generic bucket information */
	__u32 *item_weights;  /*!< 16.16 fixed point weight for each item */
	__u32 *sum_weights;   /*!< 16.16 fixed point sum of the weights */
};

struct crush_bucket_tree {
	struct crush_bucket h;  /* note: h.size is _tree_ size, not number of
				   actual items */
	__u8 num_nodes;
	__u32 *node_weights;
};

struct crush_bucket_straw {
	struct crush_bucket h;
	__u32 *item_weights;   /* 16-bit fixed point */
	__u32 *straws;         /* 16-bit fixed point */
};

/** @ingroup API
 * The weight of each item in the bucket when
 * __h.alg__ == ::CRUSH_BUCKET_STRAW2.
 *
 * The weight of __h.items[i]__ is __item_weights[i]__ for i in
 * [0,__h.size__[.
 */
struct crush_bucket_straw2 {
        struct crush_bucket h; /*!< generic bucket information */
	__u32 *item_weights;   /*!< 16.16 fixed point weight for each item */
};
```

算法说明：

| Alg     | Remark                                   | Speed   |
| ------- | ---------------------------------------- | ------- |
| UNIFORM | 所有bucket有相同的权重，n为节点数，通过hash算法得到一个[0, n)的值k，选中第k个bucket。 | O(1)    |
| LIST    | 每个bucket可以有不同的权重，w为每个节点的权重，ws=sum([w1, w2, w3])，通过hash算法得到一个[0,ws)之间的值wk，然后选中wk所在的bucket，这也是常用的负载均衡算法。现在实现的算法复杂度为O(n)，实际上可以通过预计算ws优化到O(logn)。 | O(n)    |
| TREE    | TREE其实是LIST的变体，每个非叶子节点的w=w_left+w_right，如果wk<=w_left则再left中查找，否则再right中查找，直到叶子节点返回。 | O(logn) |
| STRAW   | 根据draw值                                  |         |
| STRAW2  |                                          |         |


### list算法

本质上是计算r个hash，然后得到r个结果。

```c++
/* list */
static int bucket_list_choose(const struct crush_bucket_list *bucket,
			      int x, int r)
{
	int i;

	for (i = bucket->h.size-1; i >= 0; i--) {
		__u64 w = crush_hash32_4(bucket->h.hash, x, bucket->h.items[i],
					 r, bucket->h.id);
		w &= 0xffff;
		dprintk("list_choose i=%d x=%d r=%d item %d weight %x "
			"sw %x rand %llx",
			i, x, r, bucket->h.items[i], bucket->item_weights[i],
			bucket->sum_weights[i], w);
		w *= bucket->sum_weights[i];
		w = w >> 16;
		/*dprintk(" scaled %llx\n", w);*/
		if (w < bucket->item_weights[i]) {
			return bucket->h.items[i];
		}
	}

	dprintk("bad list sums for bucket %d\n", bucket->h.id);
	return bucket->h.items[0];
}
```

### straw算法

```c++
/* straw */

static int bucket_straw_choose(const struct crush_bucket_straw *bucket,
			       int x, int r)
{
	__u32 i;
	int high = 0;
	__u64 high_draw = 0;
	__u64 draw;

	for (i = 0; i < bucket->h.size; i++) {
		draw = crush_hash32_3(bucket->h.hash, x, bucket->h.items[i], r);
		draw &= 0xffff;
		draw *= bucket->straws[i];
		if (i == 0 || draw > high_draw) {
			high = i;
			high_draw = draw;
		}
	}
	return bucket->h.items[high];
}
```

### straw2算法

```c++
static int bucket_straw2_choose(const struct crush_bucket_straw2 *bucket,
				int x, int r, const struct crush_choose_arg *arg,
                                int position)
{
	unsigned int i, high = 0;
	unsigned int u;
	__s64 ln, draw, high_draw = 0;
        __u32 *weights = get_choose_arg_weights(bucket, arg, position);
        __s32 *ids = get_choose_arg_ids(bucket, arg);
	for (i = 0; i < bucket->h.size; i++) {
                dprintk("weight 0x%x item %d\n", weights[i], ids[i]);
		if (weights[i]) {
			u = crush_hash32_3(bucket->h.hash, x, ids[i], r);
			u &= 0xffff;

			/*
			 * for some reason slightly less than 0x10000 produces
			 * a slightly more accurate distribution... probably a
			 * rounding effect.
			 *
			 * the natural log lookup table maps [0,0xffff]
			 * (corresponding to real numbers [1/0x10000, 1] to
			 * [0, 0xffffffffffff] (corresponding to real numbers
			 * [-11.090355,0]).
			 */
			ln = crush_ln(u) - 0x1000000000000ll;

			/*
			 * divide by 16.16 fixed-point weight.  note
			 * that the ln value is negative, so a larger
			 * weight means a larger (less negative) value
			 * for draw.
			 */
			draw = div64_s64(ln, weights[i]);
		} else {
			draw = S64_MIN;
		}

		if (i == 0 || draw > high_draw) {
			high = i;
			high_draw = draw;
		}
	}

	return bucket->h.items[high];
}
```


优缺点：

1. 优点：写入数据时，所有对象在集群的节点和磁盘内均匀分布。

2. 缺点：无法做到本地优先。

http://www.openstack.cn/?p=4270

[crush: straw is dead, long live straw2](http://www.spinics.net/lists/ceph-devel/msg21635.html)


## 一致性


Ceph中的RADOS采用强一致性设计，即Write-All-Read-One，写入时必须等待所有的副本全部写入完毕才算是写入成功，读取时任意读取一个副本。

副本之间的扩散，主要有两种思路：
1. 提供一个client lib，让client发起多副本的写入，这样client将会产生多倍流量；
2. client只向master发起请求，master收到请求后，扩散到其他follower，这样client只有一份流量，而master将有多倍流量；

Ceph采用的是方案2。



### scurb


src/osd/osd_types.h

```c++
// placement group id
struct pg_t {
  uint64_t m_pool;
  uint32_t m_seed;
  int32_t m_preferred;

  pg_t() : m_pool(0), m_seed(0), m_preferred(-1) {}
  pg_t(ps_t seed, uint64_t pool, int pref=-1) :
    m_pool(pool), m_seed(seed), m_preferred(pref) {}
  // cppcheck-suppress noExplicitConstructor
  pg_t(const ceph_pg& cpg) :
    m_pool(cpg.pool), m_seed(cpg.ps), m_preferred((__s16)cpg.preferred) {}

  // cppcheck-suppress noExplicitConstructor
  pg_t(const old_pg_t& opg) {
    *this = opg.v;
  }

  old_pg_t get_old_pg() const {
    old_pg_t o;
    assert(m_pool < 0xffffffffull);
    o.v.pool = m_pool;
    o.v.ps = m_seed;
    o.v.preferred = (__s16)m_preferred;
    return o;
  }
};
```

## 扩容

增加每个pool的pg_num数量时,每个PG分裂成半,但他们都保持到它们的父OSD的映射。一般情况下，pg_num总是保持为2整数幂。