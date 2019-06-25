#include <yubihsm.h>
#include "helpers.hpp"
#include "public_key.hpp"
#include "micro-ecc/uECC.h"
#include <unordered_map>
#include <variant>
#include "taskqueue.hpp"

class Session : public Napi::ObjectWrap<Session> {
  public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    Session(const Napi::CallbackInfo &info);
    ~Session();
    

  private:
    TaskQueue *queue;
    static Napi::FunctionReference constructor;
    yh_session *session = nullptr;
    yh_connector *connector = nullptr;
    std::string url;
    std::string password;
    uint16_t authkey;
    uint16_t domain;

    // JS-exposed functions
    Napi::Value open(const Napi::CallbackInfo &info);
    Napi::Value close(const Napi::CallbackInfo &info);
    Napi::Value getPublicKey(const Napi::CallbackInfo& info);
    Napi::Value getKeys(const Napi::CallbackInfo& info);
    Napi::Value genKey(const Napi::CallbackInfo& info);
    Napi::Value addAuthKey(const Napi::CallbackInfo& info);
    Napi::Value ecdh(const Napi::CallbackInfo& info);
    Napi::Value ecdh2(const Napi::CallbackInfo& info);

    Napi::Value test(const Napi::CallbackInfo& info);

    
    // internal functions
    void close_session();

};
