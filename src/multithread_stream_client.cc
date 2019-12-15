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

class MultiThreadGreeterStreamClient {

    public:
    MultiThreadGreeterStreamClient(std::shared_ptr<Channel> channel): stub_(Greeter::NewStub(channel)) {}

        void Hello(const std::string& name) {

            ClientContext context;

            std::unique_ptr<grpc::ClientReaderWriter<HelloRequest, HelloReply>> client;
            client = stub_->SayHelloStream(&context);

            HelloRequest request;
            request.set_name(name);
            grpc::WriteOptions options;
            options.set_last_message();
            client->Write(request, options);
            std::cout << "MultiThreadStreamClient send: " << name << std::endl;

            HelloReply reply;
            while (client->Read(&reply)) {
                std::cout << "MultiThreadStreamClient received: " << reply.message() << std::endl;
            }
        }

    private:
        std::unique_ptr<Greeter::Stub> stub_;
};

int main(int argc, char** argv) {
    MultiThreadGreeterStreamClient client(grpc::CreateChannel("localhost:50053", grpc::InsecureChannelCredentials()));
    std::string name(argv[1]);
    client.Hello(name);
    std::cout << "Client closed" << std::endl;
    return 0;
}
