#include <iostream>
#include <memory>
#include <string>
#include <grpc++/grpc++.h>

#include "helloworld.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using helloworld::HelloRequest;
using helloworld::HelloReply;
using helloworld::Greeter;

class GreeterStreamClient {

    public:
        GreeterStreamClient(std::shared_ptr<Channel> channel): stub_(Greeter::NewStub(channel)) {}

        void SayHello(const std::string& name) {

            ClientContext context;
            std::unique_ptr<grpc::ClientReaderWriter<HelloRequest, HelloReply>> client;
            client = stub_->SayHelloStream(&context);

            HelloRequest request;
            request.set_name(name);
            grpc::WriteOptions options;
            options.set_last_message();
            client->Write(request, options);
            std::cout << "StreamClient send: " << name << std::endl;
            HelloReply reply;
            std::cout << "StreamClient ready read: " << name << std::endl;
            while (client->Read(&reply)) {
                std::cout << "StreamClient received: " << reply.message() << std::endl;
            }
            std::cout << "StreamClient send: " << name << std::endl;
        }

    private:
        std::unique_ptr<Greeter::Stub> stub_;
};

int main(int argc, char** argv) {
    GreeterStreamClient client(grpc::CreateChannel("localhost:50052", grpc::InsecureChannelCredentials()));
    std::string name(argv[1]);
    client.SayHello(name);
    std::cout << "Client closed" << std::endl;
    return 0;
}
