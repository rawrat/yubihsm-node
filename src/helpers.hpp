#include <napi.h>
#include <yubihsm.h>
#include "public_key.hpp"
#include "micro-ecc/uECC.h"
#include <fmt/core.h>


#define THROW(env, ...)           \
  Napi::Error::New(env, fmt::format(__VA_ARGS__)).ThrowAsJavaScriptException();
  
#define THROW_ASYNC(...) \
  throw std::runtime_error(fmt::format(__VA_ARGS__));

std::string get_public_key(yh_session *session, uint16_t key_id);
yh_rc establish_session(yh_session **session);
