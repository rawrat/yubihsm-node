'use strict'

const { Session } = require('bindings')('addon')

const usb_connection = "yhusb://"
const connector_connection = "http://127.0.0.1:12345"
const { performance } = require('perf_hooks')
async function main() {
  const config = {
    url: usb_connection,
    password: "password",
    // authkey: 39106,
    authkey: 1,
  }
  
  const ecdh_key_id = 22096
  const session = new Session(config)
  await session.open()
  try {
    // const keys = await session.getKeys()
    const a = performance.now()

    // const keys = await session.ecdh(ecdh_key_id, "EOS6UqHJB1A58qPS4yUbV1ZNkhETFo6F7bAuhrDXvpEq95Jq94LdC")
    const keys = await session.getKeys()
    const b = performance.now()
    
    console.log("Keys: ", keys)
    console.log(`ECDH took ${b-a} milliseconds`)
  } catch(e) {
    console.log("Error: ", e.message)
  }
  // session.close()
}
main()

async function test() {
  const config = {
    url: usb_connection,
    password: "password",
    authkey: 1,
  }
  const session = new Session(config)
  const response = await session.test("ohai")
  console.log("Response: ", response)
}

// test()

/* 
 * Test if destructor is getting called
 */
function forceGC() {
  if (global.gc) {
    global.gc();
  } else {
    console.warn('No GC hook! Start your program as `node --expose-gc ./addon.js`.');
  }
}
// forceGC();
// setTimeout(forceGC, 1000);