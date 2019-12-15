#include "multithread_greeter_stream.h"
#include <stdio.h>
#include <sys/select.h>

using namespace std;

GreeterStream::GreeterStream(string _name):name(_name) {}

GreeterStream::~GreeterStream() {
    this->greet_thread->join();
    delete this->greet_thread;
}

void GreeterStream::run() {
    cout << "MultiThreadStreamServer start thread for greet!" << endl;
    greet_thread = new thread(&GreeterStream::startGreet, this);
}

void GreeterStream::startGreet() {
    for(int i = 1; i <= 10; i++) {
        writeGreet(i);
        sleep_ms(1000);
    }
}

void GreeterStream::writeGreet(int no) {
    std::lock_guard<std::mutex> lock(this->queue_mutex);
    shared_ptr<Greet> greet = make_shared<Greet>();
    greet->message = "Hello," + this->name + ". No." + to_string(no);
    if(no == 10){
        greet->is_end = true;
    }
    this->queue.push(greet);
}

std::shared_ptr<Greet> GreeterStream::readGreet() {
    std::lock_guard<std::mutex> lock(this->queue_mutex);
    if (this->queue.size() == 0) {
        return NULL;
    }
    std::shared_ptr<Greet> greet = this->queue.front();
    this->queue.pop();
    return greet;
}

void GreeterStream::sleep_ms(unsigned int secs) {
    struct timeval tval;
    tval.tv_sec=secs/1000;
    tval.tv_usec=(secs*1000)%1000000;
    select(0,NULL,NULL,NULL,&tval);
}