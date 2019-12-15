#ifndef GRPC_HELLOWORLD_MULTITHREAD_GREETER_STREAM_H
#define GRPC_HELLOWORLD_MULTITHREAD_GREETER_STREAM_H

#include <string>
#include <thread>
#include <iostream>
#include <queue>
#include <mutex>

struct Greet {
    std::string message;
    bool is_end;
};

class GreeterStream {

public:
    GreeterStream(std::string);
    ~GreeterStream();
    void run();
    std::shared_ptr<Greet> readGreet();
    static void sleep_ms(unsigned int secs);

private:
    std::string name;
    std::thread *greet_thread = nullptr;
    std::queue<std::shared_ptr<Greet>> queue;
    std::mutex queue_mutex;

    void startGreet();
    void writeGreet(int);
};



#endif //GRPC_HELLOWORLD_MULTITHREAD_GREETER_STREAM_H
