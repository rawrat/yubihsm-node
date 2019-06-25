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

