#pragma once
#include<napi.h>

#include <functional>
#include <unordered_map>
#include <variant>
#include "types.hpp"
using namespace Napi;

typedef Promise::Deferred Deferred;

class PromiseWorker: public AsyncWorker {
    private:
        static Value noop( CallbackInfo const & info ) {
            return info.Env().Undefined();
        }
        static Reference< Function > fake_callback;
        static Reference< Function > const & get_fake_callback( Napi::Env const & env ) {
            if ( fake_callback.IsEmpty() ) {
                fake_callback = Reference< Function >::New( Function::New( env, noop ), 1 );
            }
            return fake_callback;
        }
        Deferred deferred;
    public:
        PromiseWorker( Deferred const & d ): AsyncWorker( get_fake_callback( d.Env() ).Value() ), deferred( d ) {}
        virtual void Resolve( Deferred const & deferred ) = 0; 
        void OnOK() {
          HandleScope scope(Env());
          Resolve( deferred );
        }
        void OnError( Error const & error ) {
          printf("OnError called\n");
          deferred.Reject( error.Value() );
        }
};

Reference< Function > PromiseWorker::fake_callback;

class EchoWorker : public PromiseWorker {
    public:
        EchoWorker(Deferred const & d, const MyTask &fun, std::function<void()> finished_callback)
        : PromiseWorker(d), fun(fun), finished_callback(finished_callback) {}

        ~EchoWorker() {}

    // This code will be executed on the worker thread
    void Execute() {
        try {
          this->result = this->fun();
        } catch(const std::exception& e) {
          SetError(e.what());
        } catch(...) {
          SetError("Unknown Error");
        }
    }
    
    void Resolve(Deferred const &deferred) {
      if (const auto returnStr (std::get_if<std::string>(&this->result)); returnStr) {
        const auto obj = String::New(Env(), *returnStr);
        deferred.Resolve(obj);
      } else if (const auto bytesPtr (std::get_if<bytes>(&this->result)); bytesPtr) {
        /**
          * Buffer::New does not copy the data. We need to make sure the 
          * data stays around for the lifetime of the Javascript Buffer.
          * The buffer behind bytesPtr is stack allocated, so we need to make a 
          * copy on the heap.
          */
        uint8_t *retained_buffer = (uint8_t *)strndup((const char *)bytesPtr->data(), bytesPtr->size());
        const auto buffer = Buffer<uint8_t>::New(Env(), retained_buffer, bytesPtr->size(), [](auto env, auto buffer) {
          /** 
            * This callback will be called when the Javascript Buffer gets 
            * garbage collected. We need to free our retained Buffer.
            */
          free(buffer);
        });
        deferred.Resolve(buffer);
      } else if(const auto mymap (std::get_if<MyMap>(&this->result)); mymap) {
        auto obj = Object::New(Env());
        for ( const auto &[key, value]: *mymap) {
          // obj.Set()
          const auto key_string = std::get_if<std::string>(&key);
          // std::cout << "key: " << key << " value: " << value;
          if (const auto intPtr (std::get_if<int>(&value)); intPtr) {
            obj.Set(*key_string, *intPtr);        
          } else if (const auto stringPtr (std::get_if<std::string>(&value)); stringPtr) {
            obj.Set(*key_string, *stringPtr);
          }
        }
        deferred.Resolve(obj);
      }
      this->finished_callback();
    }
    
    void OnError( Error const & error ) {
      PromiseWorker::OnError(error);
      this->finished_callback();
    }

    private:
        MyTask fun;
        MyReturnType result;
        std::function<void()> finished_callback;
};
