#pragma once
#include <deque>
#include "types.hpp"
#include "async.hpp"
#include <fmt/core.h>

class TaskQueue {
  public:
    TaskQueue(){}
    ~TaskQueue() {}
    
   /**
     * Returns a promise immediately (non-blocking). Takes a lambda as a second 
     * argument.
     * Resolves the promise with the result of the lambda once it's finished. 
     * The lambda is being executed in a separate thread.
     * The lambda must return either a std::string, bytes or a MyMap.
     * MyMap is a std::unordered_map<MyAny, MyAny> (see typedef above).
     * 
     * This TaskQueue makes sure all added tasks are executed sequentially.
     * Sequential execution is needed because the YubiHSM does not support 
     * multithreading/multitasking.
     */
    Napi::Promise add(const Napi::Env &env, const MyTask &task) {
      auto deferred = Napi::Promise::Deferred::New(env);      
      const auto finished_callback = [&]() {this->finished();};
      this->queue.push_back([=]() {
        /* The EchoWorker destructs itself once the task is finished. No need to manually release memory */
        EchoWorker* wk = new EchoWorker(deferred, task, finished_callback);
        wk->Queue();
      });
      this->consume();

      return deferred.Promise();
    }
    
    void consume() {
      if(this->running) {
        return;
      }
      if(this->queue.empty()) {
        return;
      }
      const auto x = this->queue.front();
      this->queue.pop_front();
      this->running = true;
      x();
    }
    
    void finished() {
      this->running = false;
      this->consume();
    }
    
  private:
    std::deque<std::function<void()>> queue;
    bool running = false;
};