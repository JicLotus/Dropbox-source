#!/bin/bash

apt-get install -y wget
mkdir temp_install && \
cd temp_install && \
wget https://github.com/facebook/rocksdb/archive/v3.13.1.zip && \
unzip v3.13.1.zip && \
cd rocksdb-3.13.1 && \
make static_lib && \
sudo cp librocksdb.a /usr/lib && \
sudo cp -r include/rocksdb/ /usr/local/include/
