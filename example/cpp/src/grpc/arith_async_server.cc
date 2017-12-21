/*
 * arith_sync_server.cc
 *
 *  Created on: Nov 4, 2017
 *      Author: hanchen
 */

#include <memory>
#include <iostream>
#include <string>
#include <thread>

#include <grpc++/grpc++.h>
#include <grpc/support/log.h>

#include "arith.grpc.pb.h"

class ServerImpl final {
 public:
  ~ServerImpl() {
    server_->Shutdown();
    cq_->Shutdown();
  }

  void Run() {
    std::string server_address("0.0.0.0:8080");

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service_);
    cq_ = builder.AddCompletionQueue();
    server_ = builder.BuildAndStart();

    HandleRpcs();
  }

 private:
  class CallData {
   public:
    CallData(proto::Arith::AsyncService* service, grpc::ServerCompletionQueue* cq)
        : service_(service), cq_(cq), responder_(&ctx_), status_(CREATE) {
      Proceed();
    }

    void Proceed() {
      if (status_ == CREATE) {
        status_ = PROCESS;
        service_->RequestMultiply(&ctx_, &request_, &responder_, cq_, cq_,
                                  this);
      } else if (status_ == PROCESS) {
        new CallData(service_, cq_);
        response_.set_result(request_.x() * request_.y());
        status_ = FINISH;
        responder_.Finish(response_, grpc::Status::OK, this);
      } else {
        GPR_ASSERT(status_ == FINISH);
        delete this;
      }
    }

   private:
    proto::Arith::AsyncService* service_;
    grpc::ServerCompletionQueue* cq_;
    grpc::ServerContext ctx_;

    proto::MultiplyRequest request_;
    proto::MultiplyResponse response_;

    grpc::ServerAsyncResponseWriter<proto::MultiplyResponse> responder_;

    enum CallStatus { CREATE, PROCESS, FINISH };
    CallStatus status_;  // The current serving state.
  };

  void HandleRpcs() {
    new CallData(&service_, cq_.get());
    void* tag;
    bool ok;
    while (true) {
      GPR_ASSERT(cq_->Next(&tag, &ok));
      GPR_ASSERT(ok);
      static_cast<CallData*>(tag)->Proceed();
    }
  }

  std::unique_ptr<grpc::ServerCompletionQueue> cq_;
  proto::Arith::AsyncService service_;
  std::unique_ptr<grpc::Server> server_;
};

int main(int argc, char** argv) {
  ServerImpl server;
  server.Run();

  return 0;
}

