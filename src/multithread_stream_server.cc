#include <iostream>
#include <memory>
#include <string>
#include <unistd.h>

#include <grpc++/grpc++.h>

#include "helloworld.grpc.pb.h"
#include "multithread_greeter_stream.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerReaderWriter;
using helloworld::HelloRequest;
using helloworld::HelloReply;
using helloworld::Greeter;

class MultiThreadGreeterStreamServiceImpl final : public Greeter::Service {

    Status SayHelloStream(ServerContext *context, ServerReaderWriter<HelloReply, HelloRequest> *stream) override {

        HelloRequest helloRequest;
        bool res = stream->Read(&helloRequest);

        if(!res) {
            std::cout << "MultiThreadStreamServer recv data err!" << std::endl;
            return Status::CANCELLED;
        }

        GreeterStream* greeterStream = new GreeterStream(helloRequest.name());
        greeterStream->run();
        while (true) {

            std::shared_ptr<Greet> greet = greeterStream->readGreet();
            if(greet == NULL) {
                greeterStream->sleep_ms(100);
                continue;
            }
            std::cout << "MultiThreadStreamServer rece queue data. Ready send: " << greet.get()->message << std::endl;
            HelloReply reply;
            std::string prefix;
            reply.set_message(greet.get()->message);
            grpc::WriteOptions writeOptions;
            res = stream->Write(reply, writeOptions);
            if(!res){
                std::cout << "MultiThreadStreamServer discover client closed." << std::endl;
                break;
            }
            if(greet.get()->is_end) {
                break;
            }
        }

        std::cout << "StreamServer finish once!" << std::endl;
        std::cout << "StreamServer client closed!" << std::endl;
        return Status::OK;
    }

};

void RunMultiThreadStreamServer() {
    std::string server_address("0.0.0.0:50053");
    MultiThreadGreeterStreamServiceImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "MultiThreadStreamServer listening on " << server_address << std::endl;
    server->Wait();
}

int main(int argc, char** argv) {
    RunMultiThreadStreamServer();
    return 0;
}
