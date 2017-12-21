/*
 * proc.c
 *
 *  Created on: Aug 25, 2017
 *      Author: hanchen
 */

int daemonize(const char *dir, int noclose) {
  int fd, ret;

  umask(0);

  if (fork()) {
    exit(0);
  }

  if (setsid() == -1) {
    return -3;
  }

  // close stdfd, STDIN_FILENO(0) / STDOUT_FILENO(1) / STDERR_FILENO(2).
  if (!noclose) {
    for (fd = 3; fd < NOFILE; fd++) {
      close(fd);
    }
  }

  if ((dir != NULL) && (strlen(dir) > 0)) {
    ret = chdir(dir);
    if (ret < 0) {
      return -4;
    }
  }

  return 0;
}

static int set_limit(int type, int size) {
  int ret = 0;
  struct rlimit limit;

  ret = getrlimit(type, &limit);
  if (ret < 0) {
    return -1;
  }

  if (size < 0) {
    limit.rlim_cur = limit.rlim_max;
  } else {
    limit.rlim_cur = size;
  }

  ret = setrlimit(type, &limit);
  if (ret < 0) {
    return -1;
  }

  return 0;
}

int set_core_dump() {
  return set_core_size(-1);
}

int set_core_size(int size) {
  return set_limit(RLIMIT_CORE, size);
}

int set_stack_size(int size) {
  return set_limit(RLIMIT_STACK, size);
}

int get_cpu_num() {
  return (int) sysconf(_SC_NPROCESSORS_CONF);
}

#if defined _GNU_SOURCE
int _bind_cpu(int cpu) {
#if defined(__linux__)
  cpu_set_t mask;
  CPU_ZERO(&mask);
  CPU_SET(cpu, &mask);
  if (sched_setaffinity(0, sizeof(mask), &mask) < 0) {
    return -1;
  }
  return 0;
#elif defined(__APPLE__) || defined(__CYGWIN__)
  return -1;
#else
  return -1;
#endif
}

ALIAS_FUNC_1(bind_cpu, _bind_cpu, int, int);
#endif

int get_pids_by_name(pid_t *pids, int max, const char *name) {
#if defined(__linux__)
  pid_t pid = -1, cpid = -1;
  DIR *dir = NULL;
  struct dirent *d = NULL;
  char filename[FILENAME_MAX], line[256];
  char pname[256], *p = NULL;
  FILE *fp = NULL;
  int num = 0;

  cpid = getpid();
  if (cpid < 0) {
    return -2;
  }

  dir = opendir("/proc");
  if (dir == NULL) {
    return -3;
  }
  while ((d = readdir(dir)) != NULL) {
    pid = (typeof(pid)) strtoul(d->d_name, NULL, 0);
    if ((pid <= 0) || (pid == cpid)) {  // avoid current
      continue;
    }
    snprintf(filename, sizeof(filename), "/proc/%s/status", d->d_name);
    fp = fopen(filename, "r");
    if (fp == NULL) {
      continue;
    }
    p = fgets(line, sizeof(line) - 1, fp);
    fclose(fp);
    if (p == NULL) {
      continue;
    }
    // line contain a string like "Name: binary_name"
    sscanf(line, "%*s %s", pname);
    if (name != NULL) {
      if (strcmp(pname, name) != 0) {
        continue;
      }
    }
    if (pids != NULL) {
      *pids++ = pid;
    }
    num++;
    if ((max >= 0) && (num >= max)) {
      break;
    }
  }
  closedir(dir);

  return num;
#elif defined(__APPLE__)
  static const int names[] = { CTL_KERN, KERN_PROC, KERN_PROC_ALL, 0 };
  int num = 0, ret = 0, pnum = 0, i = 0;
  struct kinfo_proc *procs = NULL;
  size_t length;
  pid_t cpid = -1;

  cpid = getpid();
  if (cpid < 0) {
    return -2;
  }

  ret = sysctl((int *) names, (sizeof(names) / sizeof(*names)) - 1, NULL,
               &length, NULL, 0);
  if (ret < 0) {
    return -3;
  }

  procs = malloc(length);
  if (procs == NULL) {
    return -4;
  }

  ret = sysctl((int *) names, (sizeof(names) / sizeof(*names)) - 1, procs,
               &length, NULL, 0);
  if (ret < 0) {
    free(procs);
    return -5;
  }

  pnum = length / sizeof(struct kinfo_proc);
  for (i = 0; i < pnum; i++) {
    if (procs[i].kp_proc.p_pid == cpid) {
      continue;
    }
    if (name != NULL) {
      if (strcmp(procs[i].kp_proc.p_comm, name) != 0) {
        continue;
      }
    }
    // DEBUGS("%s, %s", procs[i].kp_proc.p_comm, name);
    if (pids != NULL) {
      *pids++ = procs[i].kp_proc.p_pid;
    }
    num++;
    if ((max >= 0) && (num >= max)) {
      break;
    }
  }

  free(procs);

  return num;
#else
  return -1;
#endif
}

/*
 * notice pidmap should be save data by 1,
 * pid == 0, is current process group
 * pid == -1, is perm can send signal, but not include process init(pid == 1)
 * pid < -1, is current process group and pid is (-pid)
 **/
int kill_procs(pid_t *pids, int num, int signo, int wait) {
  pid_t pidmap[num];
  int ret = 0, eperm = 0, exist = 0, i;

  memcpy(pidmap, pids, num * sizeof(pid_t));

  for (;;) {
    for (i = 0; i < num; i++) {
      ret = kill(pids[i], signo);
      if (ret < 0) {
        if (errno == EINVAL) {  // invalid signo
          return -1;
        } else if (errno == EPERM) {  // no auth to kill
          eperm = 1;
          pidmap[i] = 1;
        } else if (errno == ESRCH) {  // pid not exist, it's ok
          pidmap[i] = 1;
        }
      }
    }

    if (!wait) {  // not wait
      return 0;
    }

    exist = 0;
    for (i = 0; i < num; i++) {
      if (pidmap[i] != 1) {  // exist
        exist = 1;
      }
    }
    if (!exist) {  // no pid exist
      if (eperm) {
        return 1;
      } else {
        return 0;
      }
    }

    if (g_terminated) {  // current process killed, avoid unless loop
      return 1;  // kill but not all
    }

    /* first loop is real signo
     * the next is check process only
     * just kill(pid, 0)
     **/
    if (signo != 0) {
      signo = 0;
    }
  }

  return 0;
}

WEAK_FUNC_1( terminate, _, void, int signo) {
  DEBUGS("pid: %d, signo: %d", getpid(), signo);
  switch (signo) {
    default:
      g_terminated = 1;  // program will be exit when next call loop
      break;
  }
}

ALIAS_FUNC_1( terminate, _terminate, void, int);

int sig_ignore(int sig) {
  return sig_reg(SIG_IGN, sig, 0);
}

WEAK_FUNC_0( RegSignal, _, void) {
  sig_reg(terminate, SIGINT, SIGQUIT, SIGTERM, 0);
  sig_reg(SIG_IGN, SIGHUP, 0);
}

ALIAS_FUNC_0( RegSignal, _RegSignal, void);

int sig_reg_s(void (*handler)(int), int *sigs, int num) {
  struct sigaction act;
  int i;

  if ((sigs == NULL) || (num < 0)) {
    return -1;
  }

  memset(&act, 0, sizeof(act));
  sigemptyset(&act.sa_mask);
  act.sa_flags = SA_RESTART;
  act.sa_handler = handler;

  for (i = 0; i < num; i++) {
    sigaction(sigs[i], &act, NULL);
  }

  return 0;
}

int sig_reg(void (*handler)(int), ...) {
  int sig, sigs[256], num = 0;
  va_list ap;

  va_start(ap, handler);
  while (num < (int) ARRLEN(sigs)) {
    sig = va_arg(ap, int);
    if (sig <= 0 || sig >= 256) {
      break;
    }
    sigs[num] = sig;
    num++;
  }
  va_end(ap);

  return sig_reg_s(handler, sigs, num);
}

int get_shm_num(void) {
#if defined(__linux__)
  struct shmid_ds sd;
  return shmctl(0, SHM_INFO, &sd) + 1;
#else
  return -1;
#endif
}

ssize_t get_shm_size(key_t key) {
  struct shmid_ds sd;
  int ret, id;
  id = shmget(key, 0, 0);
  ret = shmctl(id, IPC_STAT, &sd);
  if (ret < 0) {
    // DEBUGS("shmctl fail, ret: %d, errno: %d", ret, errno);
    return -1;
  }
  return sd.shm_segsz;
}

static int _get_shm(void **shm, key_t key, ssize_t size, int flag) {
  int id;
  void *s = NULL;

  if ((shm == NULL) || (key < 0) || (size < 0)) {
    DEBUGS("invalid param");
    return -1;
  }
  if ((id = shmget(key, size, flag)) < 0) {
    /*
     DEBUGS("shmget fail, ret: %d, errno: %d, key: 0x%x, size: %zd, flag: 0%o",
     id, errno, key, size, flag);
     */
    return -2;
  }
  if ((s = shmat(id, NULL, 0)) == (void *) -1) {
    DEBUGS("shmat fail, flag: 0%o", flag);
    return -3;
  }
  if (s == NULL) {
    return -4;
  }
  *shm = s;

  return 0;
}

int get_shm(void **shm, key_t key, ssize_t size, int flag) {
  ssize_t sz = get_shm_size(key);
  if (sz >= 0) {  // if found by key
    if (size < 0) {  // if not special size use all shm size
      size = sz;
    }
    if (_get_shm(shm, key, size, flag & (~IPC_CREAT)) == 0) {
      return 0;
    }
  } else {  // not found try to create if flag has IPC_CREAT
    if (_get_shm(shm, key, size, flag) == 0) {
      return 1;
    }
  }
  return -1;
}

