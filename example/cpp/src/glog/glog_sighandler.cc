/*
 * glog_sighandler_fix.cc
 *
 *  Created on: May 23, 2017
 *      Author: hanchen
 */

#include <glog/logging.h>
#include <glog/raw_logging.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

int g_terminated = 0;

static void sighander(int sig) {
    g_terminated = 1;
    // LOG(INFO) << "sig: " << sig << std::endl;
    RAW_LOG(INFO, "sig: %d", sig);  // rewrite this line
}

int main(int argc, char *argv[]) {
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::INFO);

    signal(SIGINT, sighander);

    while (!g_terminated) {
        LOG(INFO) << "Running";
    }

    LOG(INFO) << "Exit!";

    return EXIT_SUCCESS;
}
