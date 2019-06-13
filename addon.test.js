'use strict'
const { Session } = require('bindings')('addon')
const eosjs_ecc = require('eosjs-ecc-priveos')
const usb_connection = "yhusb://"

/* All YubiHSM2's come with a default authkey 1 with password "password" */
const config = {
  url: usb_connection,
  password: "password",
  authkey: 1,
}
const ecdh_auth_key_password = "ecdh_auth_key_password"
let ecdh_auth_key_id
let ecdh_key_id
let ecdh_public_key
let session
test('addAuthKey', async () => {
  session = new Session(config)
  await session.open()
  const res = await session.addAuthKey("ecdh auth", ecdh_auth_key_password)
  ecdh_auth_key_id = res.key_id
  
  expect(typeof ecdh_auth_key_id).toBe("number")  
})

test('genKey', async () => {
  console.log("genKey.config: ", config)
  const res = await session.genKey("ecdh key", ecdh_auth_key_password)
  ecdh_key_id = res.key_id
  ecdh_public_key = res.public_key
  
  expect(typeof ecdh_key_id).toBe("number")
  expect(eosjs_ecc.isValidPublic(ecdh_public_key)).toBeTruthy()
  
  /* We're using this new auth key for the upcoming tests */
  config.authkey = ecdh_auth_key_id
  config.password = ecdh_auth_key_password
  await session.close()
})

test('getPublicKey', async () => {
  session = new Session(config)
  await session.open()
  const pkey = await session.getPublicKey(ecdh_key_id)
  expect(pkey).toBe(ecdh_public_key)
})

test('getKeys', async () => {
  const res = await session.getKeys()
  let found = false
  for(const [pkey, key_id] of Object.entries(res)) {
    if(key_id === ecdh_key_id && pkey === ecdh_public_key) {
      found = true
      break
    }
  }
  expect(found).toBeTruthy()
})


test('ecdh', async () => {
  const shared_secret = await session.ecdh(ecdh_key_id, "EOS6UqHJB1A58qPS4yUbV1ZNkhETFo6F7bAuhrDXvpEq95Jq94LdC")
  
  expect(shared_secret.length).toBe(32)
  expect(shared_secret instanceof Buffer).toBeTruthy()
})

test('test parallel execution', async () => {
  const pk = "EOS6UqHJB1A58qPS4yUbV1ZNkhETFo6F7bAuhrDXvpEq95Jq94LdC"
  
  /* If the C++ task queue is not implemented correctly, this will crash */
  const shared_secrets = await Promise.all([
    session.ecdh(ecdh_key_id, pk),
    session.ecdh(ecdh_key_id, pk),
    session.ecdh(ecdh_key_id, pk),
    session.ecdh(ecdh_key_id, pk),
    session.ecdh(ecdh_key_id, pk),
    session.ecdh(ecdh_key_id, pk),
    session.ecdh(ecdh_key_id, pk),
  ])
  
  /* All results should be identical */
  const first = shared_secrets[0]
  for (const s of shared_secrets) {
    expect(s).toEqual(first)
  }
  
  /* This is the final test, close session */
  await session.close()
})

