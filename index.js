var addon = require('bindings')('addon.node')

class Yubi {
  constructor(config) {
    this.ctx = addon.get_context(config)
    // this.keymap = 
  }
  
  
}