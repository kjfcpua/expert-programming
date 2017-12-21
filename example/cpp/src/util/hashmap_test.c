/*===============================================================                          
 *    File Name   : glb_test.c
 *    Author      : chenhan
 *    Date        : 2014.06.27
 *    Description : 
 *    Update Log  : 
 ===============================================================*/
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "gear.h"

static int count = 0;

int visit(void *pKey, void *pValue) {
    int key = *(int *) pKey;
    int value = *(int *) pValue;

    DEBUGS("key = %d, value = %d", key, value);

    return 0;
}

int count_visit(void *pKey, void *pValue) {
    count++;

    return 0;
}

int main(int argc, char *argv[]) {
    static char hashmap[1024 * 1024];
    HashMap *pstHashMap;
    int i, key, val, sk = 0;

    set_core_size(100 * 1000 * 1000);

    pstHashMap = (HashMap *) hashmap;
    if (HashMapInit(pstHashMap, sizeof(hashmap), 20, NULL, 1000, sizeof(int), sizeof(int), (stdhash) int_hash, (stdcmp) int_cmp) < 0) {
        return -1;
    }

    DEBUGS("**************************************init*************************************");
    DEBUGS("cap = %d", HashMapCap(pstHashMap));

    DEBUGS("**************************************test put*********************************");
    srandom(time(NULL));
    for (i = 0; i < 10; i++) {
        key = i;
        val = key + 10000;
        if (HashMapAdd(pstHashMap, &key, sizeof(key), &val, sizeof(val)) == 0) {
            // DEBUGS("put succ, key = %d, value = %d", key, val);
        } else {
            DEBUGS("put[%d] fail, key = %d", i, key);
            break;
        }
    }
    DEBUGS("size = %d", HashMapSize(pstHashMap));

    sk = 7;

    DEBUGS("*************************************test get**********************************");
    key = sk;
    if (HashMapGet(pstHashMap, &key, sizeof(key), &val, NULL) == 0) {
        DEBUGS("get key = %d, val = %d", key, val);
    }
    DEBUGS("size = %d", HashMapSize(pstHashMap));

    DEBUGS("*************************************test add**********************************");
    key = sk;
    val = 0;
    if (HashMapAdd(pstHashMap, &key, sizeof(key), &val, -1) == 0) {
        DEBUGS("add key = %d, val = %d", key, val);
    } else {
        DEBUGS("add fail, key = %d, val = %d", key, val);
    }
    DEBUGS("size = %d", HashMapSize(pstHashMap));

    DEBUGS("*************************************test remove*******************************");
    key = sk;
    if (HashMapRemove(pstHashMap, &key, sizeof(key), &val, NULL) == 0) {
        DEBUGS("remove key = %d, val = %d", key, val);
    }
    if (HashMapGet(pstHashMap, &key, sizeof(key), &val, NULL) == 0) {
        DEBUGS("get key = %d, val = %d", key, val);
    }
    DEBUGS("size = %d", HashMapSize(pstHashMap));

    DEBUGS("*************************************test visit********************************");
    HashMapTraverse(pstHashMap, count_visit);
    DEBUGS("count = %d", count);

    HashMapTraverse(pstHashMap, visit);

    exit(0);
}


