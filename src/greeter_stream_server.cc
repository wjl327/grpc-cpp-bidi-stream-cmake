#include <iostream>
#include <memory>
#include <string>
#include <grpc++/grpc++.h>

#include "helloworld.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerReaderWriter;
using helloworld::HelloRequest;
using helloworld::HelloReply;
using helloworld::Greeter;

class GreeterStreamServiceImpl final : public Greeter::Service {

    Status SayHelloStream(ServerContext *context, ServerReaderWriter<HelloReply, HelloRequest> *stream) override {

        HelloRequest helloRequest;
        while (stream->Read(&helloRequest)) {

            std::cout << "StreamServer revc: " << helloRequest.name() << std::endl;
            for (int i = 1; i <= 2; i++) {
                HelloReply reply;
                std::string prefix;
                prefix = i == 1 ? "Hi," : "Hello,";
                reply.set_message(prefix + helloRequest.name());
                grpc::WriteOptions writeOptions;
                bool res = stream->Write(reply, writeOptions);
                std::cout << "StreamServer result: " << res << std::endl;
            }
            std::cout << "StreamServer finish once!" << std::endl;
        }
        std::cout << "StreamServer client closed!" << std::endl;
        return Status::OK;
    }

};

void RunStreamServer() {
    std::string server_address("0.0.0.0:50052");
    GreeterStreamServiceImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "StreamServer listening on " << server_address << std::endl;
    server->Wait();
}

int main(int argc, char** argv) {
    RunStreamServer();
    return 0;
}
