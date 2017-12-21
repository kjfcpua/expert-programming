/*===============================================================                          
 *    File Name   : channel_test.c
 *    Author      : chenhan
 *    Date        : 2014.06.27
 *    Description : 
 *    Update Log  : 
 ===============================================================*/
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <getopt.h>
#include <libgen.h>
#include "gear.h"

#define CMD_TEST   1
#define CMD_SHOW   2
#define CMD_CLEAR  3
#define CMD_PUT    4
#define CMD_GET    5
#define CMD_PEEK   6
#define CMD_UNLOCK 7

#define OPTV_HELP           1
#define OPTV_VERSION        2
#define OPTV_VERBOSE        'v'
#define OPTV_CMD            'c'
#define OPTV_HOST           'h'
#define OPTV_PORT           'p'
#define OPTV_TIMEO          't'
#define OPTV_TIME           6
#define OPTV_REPEAT         7
#define OPTV_SHMKEY         10
#define OPTV_SBUF           11

typedef struct {
    int   verbose;
	int   cmd;
	int   timeo;
	key_t shmkey;
	void  *sbuf;
	ssize_t slen;
} Config;

static Config config[1];

int init_shm_channel(channel_t **channel, key_t key)
{
    int ret = 0;

    channel_t *chnl = NULL;
    // ssize_t chsz = 1000 * 1000;
    // ssize_t chmsz = ChannelMemSize(chsz);
    ret = get_shm(&chnl, key, -1, IPC_CREAT | 0666);
    if (ret < 0)
    {
        DEBUGS("get shm fail, ret=%d, key=0x%x", ret, key);
        return -1;
    }

    ret = ChannelInit(chnl, -1);
    if (ret < 0)
    {
    	DEBUGS("init channel fail, ret=%d", ret);
        return -1;
    }

    *channel = chnl;

    return 0;
}

int test(void *args) {
    char sbuf[1024 * 1024]; 
    channel_t *channel = (channel_t *) sbuf;
    char *strs[] = { "Hello", "Channel", "Welecom", "This is a test", "by rabbihan", "apple",
        "coca cola", "Tencent", "HTC", "Desire", "Cisco", "HP", "Linux", "www.ao.com", "end of test" };
    char str[32];
    int ret, num, i;
    ssize_t len;

    RegSignal();

    ret = ChannelInit(channel, 128);
    if (ret < 0) {
        DEBUGS("channel init fail, ret: %d", ret);
        return -1;
    }

    num = ARRLEN(strs);
    for (i = 0; i < num; /* not add */) {
        if (g_terminated) {
            break;
        }
        len = ChannelPutBuffer(channel, strs[i], strlen(strs[i]) + 1, CHANNEL_OPT_CAS | CHANNEL_OPT_SPIN);
        // DEBUGS("put: %zd", len);
        if (len >= 0) {
        	DEBUGS("put succ, str = %s[%d], channel size = %zd", strs[i], (int) (strlen(strs[i]) + 1), ChannelSize(channel));
            i++;
        } else if (len == CHANNEL_SHORT_OF_WR) {
            len = ChannelGetBuffer(channel, str, sizeof(str), CHANNEL_OPT_CAS | CHANNEL_OPT_SPIN);
            // DEBUGS("get: %zd", len);
            if (len >= 0) {
            	DEBUGS("get succ, str = %s, channel size = %zd", str, ChannelSize(channel));
            } else if (len == CHANNEL_EMPTY) {
            	DEBUGS("empty");
            } else {
            	DEBUGS("get fail, channel size = %zd", ChannelSize(channel));
            }       
        } else {
        	DEBUGS("put fail, ret: %zd, str = %s[%d], channel size = %zd", len, strs[i], (int) (strlen(strs[i]) + 1), ChannelSize(channel));
        }
    } // while

    return 0;
}

int show(void *args) {
	int ret = 0;
	channel_t *channel = NULL;
	ret = init_shm_channel(&channel, config->shmkey);
	if (ret < 0) {
		DEBUGS("init shm channel fail, ret: %d", ret);
		return -1;
	}
	DEBUGS("capacity: %zd", ChannelCapacity(channel));
	DEBUGS("size: %zd", ChannelSize(channel));
	long rltm = 0, wltm = 0;
	ChannelGetOpt(channel, CHANNEL_OPT_LOCK_TIME | CHANNEL_OPT_RD, &rltm, sizeof(rltm));
	DEBUGS("read_lock_time: %ldms", rltm);
	ChannelGetOpt(channel, CHANNEL_OPT_LOCK_TIME | CHANNEL_OPT_WR, &wltm, sizeof(wltm));
	DEBUGS("write_lock_time: %ldms", wltm);
	return 0;
}

int clear(void *args) {
	int ret = 0;
	channel_t *channel = NULL;
	ret = init_shm_channel(&channel, config->shmkey);
	if (ret < 0) {
		DEBUGS("init shm channel fail, ret: %d", ret);
		return -1;
	}
	// DEBUGS("size: %zd", ChannelSize(channel));
	ret = ChannelClear(channel);
	if (ret < 0) {
		DEBUGS("channel clear fail, ret: %d", ret);
		return -1;
	}
	DEBUGS("buffer clear succ");
	return 0;
}

int put(void *args) {
	int ret = 0;
	channel_t *channel = NULL;
	ret = init_shm_channel(&channel, config->shmkey);
	if (ret < 0) {
		DEBUGS("init shm channel fail, ret: %d", ret);
		return -1;
	}
	ret = ChannelPutBuffer(channel, config->sbuf, config->slen, CHANNEL_OPT_CAS | CHANNEL_OPT_SPIN);
	if (ret < 0) {
		DEBUGS("put buffer fail, ret: %d", ret);
		return -1;
	}
	DEBUGS("put buffer %s[%zd] succ", (char *) config->sbuf, config->slen);
	return 0;
}

int get(void *args) {
	int ret = 0;
	channel_t *channel = NULL;
	ret = init_shm_channel(&channel, config->shmkey);
	if (ret < 0) {
		DEBUGS("init shm channel fail, ret: %d", ret);
		return -1;
	}
	char rbuf[65536];
	ssize_t rlen = sizeof(rbuf) - 1;
	rlen = ChannelGetBuffer(channel, rbuf, rlen, CHANNEL_OPT_CAS | CHANNEL_OPT_SPIN);
	if (rlen < 0) {
		DEBUGS("get buffer fail, ret: %d", (int) rlen);
		return -1;
	}
	rbuf[rlen] = '\0';
	DEBUGS("get buffer: %s[%zd]", rbuf, rlen);
	return 0;
}

int peek(void *args) {
	int ret = 0;
	channel_t *channel = NULL;
	ret = init_shm_channel(&channel, config->shmkey);
	if (ret < 0) {
		DEBUGS("init shm channel fail, ret: %d", ret);
		return -1;
	}
	char rbuf[65536];
	ssize_t rlen = sizeof(rbuf) - 1;
	rlen = ChannelGetBuffer(channel, rbuf, rlen, CHANNEL_OPT_CAS | CHANNEL_OPT_SPIN | CHANNEL_OPT_PEEK);
	if (rlen < 0) {
		DEBUGS("peek buffer fail, ret: %d", ret);
		return -1;
	}
	rbuf[rlen] = '\0';
	DEBUGS("peek buffer: %s[%zd]", rbuf, rlen);
	return 0;
}

int unlock(void *args) {
	int ret = 0;
	channel_t *channel = NULL;
	ret = init_shm_channel(&channel, config->shmkey);
	if (ret < 0) {
		DEBUGS("init shm channel fail, ret: %d", ret);
		return -1;
	}
	ret = ChannelUnlock(channel, CHANNEL_OPT_CAS | CHANNEL_OPT_WR | CHANNEL_OPT_RD, config->timeo);
	if (ret < 0) {
		DEBUGS("channel unlock fail, ret: %d", ret);
		return -1;
	}
	DEBUGS("unlock succ");
	return 0;
}

int PrintVersion(int argc, char *argv[]) {
#if defined _DATE
    printf("version: %s\n", MACRO_STR(_DATE));
#else
    printf("version: %s\n", "undefined");
#endif
    return 0;
}

int PrintUsage(int argc, char *argv[]) {
	printf("Usage: %s [OPTION]...\n", argv[0]);
	printf("  %3s %-10s %s\n", "-c,", "--cmd",         "command, test, show, clear, put, get, peek, unlock");
	printf("  %3s %-10s %s\n", "",    "--timeo",       "timeout, default: 0ms, if < 0, block");
	// printf("  %3s %-10s %s\n", "-v,", "--verbose",     "verbose, 0: none; 1: opts; 2: time; 255/none: all;");
	printf("  %3s %-10s %s\n", "",    "--shmkey",         "shm key");
	printf("  %3s %-10s %s\n", "",    "--sbuf",        "set buffer");
	printf("  %3s %-10s %s\n", "",    "--help",        "help");
	printf("  %3s %-10s %s\n", "",    "--version",     "version");

	return 0;
}

static int ProcOpts(Config *config) {
    eth_addr_t eth;
    GetLanEths(&eth, 1, NULL);
    return 0;
}

static int PraseOpts(Config *config, int argc, char *argv[]) {
    static struct option long_options[] = {
//    	{ "verbose", optional_argument, NULL, OPTV_VERBOSE }, // 'v'
        { "cmd", required_argument, NULL, OPTV_CMD }, // 'c'
//        { "host", required_argument, NULL, OPTV_HOST }, // 'h'
//        { "port", required_argument, NULL, OPTV_PORT }, // 'p'
        { "timeo", required_argument, NULL, OPTV_TIMEO }, // 't'
//        { "repeat", required_argument, NULL, OPTV_REPEAT },
        { "shmkey", required_argument, NULL, OPTV_SHMKEY },
        { "sbuf", required_argument, NULL, OPTV_SBUF },
    	{ "help", no_argument, NULL, OPTV_HELP },
    	{ "version", no_argument, NULL, OPTV_VERSION },
        { NULL, 0, NULL, 0 }
    };
    int ret = 0, optv;
    if (argc < 2) {
        return -1;
    }
    while (1) {
        int option_index = 0;
        optv = getopt_long(argc, argv, "c:h:p:t:v::", long_options, &option_index);
        if (optv == -1) {
            break;
        }
        switch (optv) {
        case OPTV_HELP: // help
            ret = 1;
        	break;
        case OPTV_VERSION: // version
        	ret = 2;
        	break;
        case OPTV_VERBOSE: // 'v'
        	if (optarg != NULL) {
        		int v = atoi(optarg);
        		if (v == 0) {
        			config->verbose = 0;
        		} else {
        			config->verbose |= v;
        		}
        	} else {
                // config->verbose = VERBOSE_ALL;
        	}
        	break;
        case OPTV_CMD: // 'c'
            if (strcasecmp(optarg, "test") == 0) {
        		config->cmd = CMD_TEST;
            } else if (strcasecmp(optarg, "show") == 0) {
                config->cmd = CMD_SHOW;
			} else if (strcasecmp(optarg, "clear") == 0) {
				config->cmd = CMD_CLEAR;
			} else if (strcasecmp(optarg, "put") == 0) {
				config->cmd = CMD_PUT;
			} else if (strcasecmp(optarg, "get") == 0) {
				config->cmd = CMD_GET;
			} else if (strcasecmp(optarg, "peek") == 0) {
				config->cmd = CMD_PEEK;
			} else if (strcasecmp(optarg, "unlock") == 0) {
				config->cmd = CMD_UNLOCK;
			}
        	break;
        case OPTV_TIMEO:
        	config->timeo = strtoul(optarg, NULL, 0);
        	break;
        case OPTV_SHMKEY:
        	config->shmkey = strtoul(optarg, NULL, 0);
        	break;
        case OPTV_SBUF:
        	config->sbuf = optarg;
        	config->slen = strlen(optarg);
        	break;
        default:
        	ret = -1;
            break;
        }
        if (ret != 0) {
        	break;
        }
        // OptVal(config, optv, 1);
    }
    return ret;
}

int main(int argc, char *argv[]) {
	int ret = 0;

    sig_reg(terminate, SIGINT, SIGQUIT, SIGTERM, 0);
    sig_reg(SIG_IGN, SIGHUP, SIGPIPE, 0);

    set_core_size(100 * 1024 * 1024);

    pid_t pids[1];
    int pidnum = get_pids_by_name(pids, ARRLEN(pids), basename(argv[0]));
    if (pidnum > 0) {
        DEBUGS("been running with pid[%d]", pids[0]);
        return -1;
    }

    memset(config, 0, sizeof(config[0]));

	ret = PraseOpts(config, argc, argv);
	if (ret < 0) {
        PrintUsage(argc, argv);
		return -1;
	} else if (ret == 1) { // help
		PrintUsage(argc, argv);
		return 0;
	} else if (ret == 2) { // version
		PrintVersion(argc, argv);
		return 0;
	}

    ProcOpts(config);
    // PrintOpts(config);

	int_ptr_pair_t handles[] = {
			{ CMD_TEST, test },
			{ CMD_SHOW, show },
			{ CMD_CLEAR, clear },
			{ CMD_PUT, put },
			{ CMD_GET, get },
			{ CMD_PEEK, peek },
			{ CMD_UNLOCK, unlock },
	};

    int hidx = MAP_GET(handles, config->cmd);
	if (hidx < 0) {
	    DEBUGS("cmd not found: %d", config->cmd);
		return -2;
	}

	int_ptr_pair_t *handle = &handles[hidx];
	int (*func)(void *) = (typeof(func)) handle->value;

	ret = func(config);
	if (ret < 0) {
		DEBUGS("handle fail, ret: %d", ret);
	}

    exit(0);
}

