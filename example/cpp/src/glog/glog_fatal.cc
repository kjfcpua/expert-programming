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

int main(int argc, char *argv[]) {
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::INFO);

    LOG(FATAL) << "fatal error!";

    LOG(INFO) << "Exit!";

    return EXIT_SUCCESS;
}
