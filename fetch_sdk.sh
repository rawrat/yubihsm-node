#!/usr/bin/env bash

if [[ "$OSTYPE" == "darwin"* ]]; then
  curl "https://developers.yubico.com/YubiHSM2/Releases/yubihsm2-sdk-2019-03-darwin-amd64.tar.gz" | tar xfz -
  install_name_tool -id "$PWD/yubihsm2-sdk/lib/libyubihsm.2.dylib" yubihsm2-sdk/lib/libyubihsm.2.dylib
fi