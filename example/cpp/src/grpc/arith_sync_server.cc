/*
 * arith_sync_server.cc
 *
 *  Created on: Nov 4, 2017
 *      Author: hanchen
 */

#include <iostream>
#include <memory>
#include <string>

#include <grpc++/grpc++.h>

#include "arith.grpc.pb.h"

class ArithServiceImpl final : public proto::Arith::Service {
  ::grpc::Status Multiply(::grpc::ServerContext* context,
                          const proto::MultiplyRequest* request,
                          proto::MultiplyResponse* response) override {
    response->set_result(request->x() * request->y());
    return grpc::Status::OK;
  }
};

void RunServer() {
  std::string server_address("0.0.0.0:8080");
  ArithServiceImpl service;

  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();

  return 0;
}

