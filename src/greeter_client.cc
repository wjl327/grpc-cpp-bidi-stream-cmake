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

class GreeterClient {

    public:
        GreeterClient(std::shared_ptr<Channel> channel): stub_(Greeter::NewStub(channel)) {}

        std::string SayHello(const std::string& name) {
            HelloRequest request;
            request.set_name(name);

            HelloReply reply;
            ClientContext context;

            Status status = stub_->SayHello(&context, request, &reply);
            if (status.ok()) {
              return reply.message();
            } else {
              return "RPC failed";
            }
        }

    private:
        std::unique_ptr<Greeter::Stub> stub_;
};

int main(int argc, char** argv) {
    GreeterClient greeter(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    std::string name(argv[1]);
    std::string reply = greeter.SayHello(name);
    std::cout << "Greeter received: " << reply << std::endl;
    return 0;
}
