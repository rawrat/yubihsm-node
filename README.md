# YubiHSM2-Node

## What is it?
This is a node package that let's you interface with a YubiHMS2 Hardware Security Module with nodejs. It's implemented as a native add-on in C++.

## Installation
The node build process for addons only works with python2. If the standard python in your system is python3, please use a virtualenv or similar to make sure the `python` command refers to python2 before running the installation.

### Linux
Download and install the YubiHSM2 SDK for your flavor of Linux from the [releases page](https://developers.yubico.com/YubiHSM2/Releases/).

Once the SDK is successfully installed in the system, you should be able to build the project by:
```
npm install
```

### Mac OS X
On Mac OS X, the SDK will be downloaded automatically, so it's sufficient to use the command:
```
npm install
```

## Usage
Please see `addon.test.js` for usage examples.

## Limitations
It only exposes a subset of features to javascript. It is currently hard-coded to use the secp256k1 elliptic curve and the EOSIO public key format. It could easily be extended to work with different elliptic curves and different public key formats.
