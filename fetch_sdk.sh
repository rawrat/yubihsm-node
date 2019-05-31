#!/bin/sh -e
export npm_config_python=python2.7

if [[ "$OSTYPE" == "linux-gnu" ]]; then
  echo "Linux not implemented yet"
  exit 1
  curl "https://developers.yubico.com/YubiHSM2/Releases/yubihsm2-sdk-2019-03-ubuntu1804-amd64.tar.gz" | tar xfz -
elif [[ "$OSTYPE" == "darwin"* ]]; then
  curl "https://developers.yubico.com/YubiHSM2/Releases/yubihsm2-sdk-2019-03-darwin-amd64.tar.gz" | tar xfz -
  install_name_tool -id "$PWD/yubihsm2-sdk/lib/libyubihsm.2.dylib" yubihsm2-sdk/lib/libyubihsm.2.dylib
fi