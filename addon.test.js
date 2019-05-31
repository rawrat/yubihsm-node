'use strict'
const { Session } = require('bindings')('addon')
const eosjs_ecc = require('eosjs-ecc-priveos')
const usb_connection = "yhusb://"
const config = {
  url: usb_connection,
  password: "password",
  authkey: 1,
}
const ecdh_auth_key_password = "ecdh_auth_key_password"
let ecdh_auth_key_id
let ecdh_key_id
let ecdh_public_key

test('addAuthKey', async () => {
  const session = new Session(config)
  const res = await session.addAuthKey("ecdh auth", ecdh_auth_key_password)
  ecdh_auth_key_id = res.key_id
  
  expect(typeof ecdh_auth_key_id).toBe("number")

  
  session.close()
})

test('genKey', async () => {
  console.log("genKey.config: ", config)
  const session = new Session(config)
  const res = await session.genKey("ecdh key", ecdh_auth_key_password)
  ecdh_key_id = res.key_id
  ecdh_public_key = res.public_key
  
  expect(typeof ecdh_key_id).toBe("number")
  expect(eosjs_ecc.isValidPublic(ecdh_public_key)).toBeTruthy()
  
  /* We're using this new auth key for the upcoming tests */
  config.authkey = ecdh_auth_key_id
  config.password = ecdh_auth_key_password
  session.close()
})

test('getPublicKey', async () => {
  const session = new Session(config)
  const pkey = await session.getPublicKey(ecdh_key_id)
  expect(pkey).toBe(ecdh_public_key)
  session.close()
})

test('getKeys', async () => {
  const session = new Session(config)
  const res = await session.getKeys()
  let found = false
  for(const [pkey, key_id] of Object.entries(res)) {
    if(key_id === ecdh_key_id && pkey === ecdh_public_key) {
      found = true
      break
    }
  }
  expect(found).toBeTruthy()
  session.close()
})


test('ecdh', async () => {
  const session = new Session(config)
  const shared_secret = await session.ecdh(ecdh_key_id, "EOS6UqHJB1A58qPS4yUbV1ZNkhETFo6F7bAuhrDXvpEq95Jq94LdC")
  
  expect(shared_secret.length).toBe(32)
  expect(shared_secret instanceof Buffer).toBeTruthy()
  session.close()
})
