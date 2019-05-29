#include "helpers.hpp"

std::string get_public_key(yh_session *session, uint16_t key_id) {
  yh_rc yrc = YHR_GENERIC_ERROR;
  auto pk = abieos::public_key{};
  uint8_t key[65];
  size_t public_key_len = sizeof(key);
  yrc =
    yh_util_get_public_key(session, key_id, key, &public_key_len, NULL);
  
  if(yrc != YHR_SUCCESS) {
    throw "Couldn't get public key";
  }
  
  int valid = uECC_valid_public_key(key, uECC_secp256k1());
  // printf("Key is valid: %d\n", valid);
  if(!valid) {
    throw "This is not a valid secp256k1 key";
  }
  uECC_compress(key, pk.data.data(), uECC_secp256k1());  
  return public_key_to_string(pk);
}

yh_rc establish_session(yh_session **session) {
  yh_connector *connector = NULL;
  yh_rc yrc = YHR_GENERIC_ERROR;
  
  uint16_t authkey = 1;
  
  const uint8_t password[] = "password";

  yrc = yh_init();
  if(yrc != YHR_SUCCESS) {
    return yrc;
  }

  yrc = yh_init_connector(YH_USB_URL_SCHEME, &connector);
  if(yrc != YHR_SUCCESS) {
    return yrc;
  }

  yrc = yh_connect(connector, 0);
  if(yrc != YHR_SUCCESS) {
    return yrc;
  }

  yrc = yh_create_session_derived(connector, authkey, password,
                                  sizeof(password), false, session);
  if(yrc != YHR_SUCCESS) {
    return yrc;
  }

  yrc = yh_authenticate_session(*session);
  if(yrc != YHR_SUCCESS) {
    return yrc;
  }
  
  return YHR_SUCCESS;
}