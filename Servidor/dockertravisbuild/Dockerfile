FROM  stackbrew/ubuntu:14.04
RUN apt-get update -y

RUN apt-get install -y cmake-curses-gui
RUN apt-get install -y git
RUN apt-get install -y build-essential checkinstall

RUN apt-get install -y zlib1g-dev
RUN apt-get install -y libbz2-dev
RUN apt-get install -y libsnappy-dev
RUN apt-get install -y libgflags-dev
RUN apt-get install -y wget
RUN apt-get install -y npm
RUN apt-get install -y scons
RUN apt-get install -y zip

# Install cmake
RUN wget http://www.cmake.org/files/v3.3/cmake-3.3.2.tar.gz
RUN tar xf cmake-3.3.2.tar.gz
RUN cd cmake-3.3.2 && ./configure && make && make install


# Install mongoose
RUN npm install -y mongoose

# Install rocksdb
RUN git clone https://github.com/gflags/gflags.git gflags && cd gflags && mkdir build && cd build && cmake .. && make && make install
#RUN git clone https://github.com/facebook/rocksdb/tree/v3.13.1 rocksdb && cd rocksdb && make && make install


# Install jsoncpp
RUN wget http://downloads.sourceforge.net/project/jsoncpp/jsoncpp/0.5.0/jsoncpp-src-0.5.0.tar.gz
RUN tar -xvzf jsoncpp-src-0.5.0.tar.gz
RUN cd jsoncpp-src-0.5.0 && scons platform=linux-gcc && cd libs/linux-gcc-4.8 && cp libjson_linux-gcc-4.8_libmt.a /usr/local/lib/

# Install gtest
RUN wget https://github.com/google/googletest/archive/release-1.7.0.zip && unzip release-1.7.0.zip
RUN cd googletest-release-1.7.0 && ls && mkdir mybuild && cd mybuild  && cmake -DBUILD_SHARED_LIBS=ON -Dgtest_build_samples=ON -G"Unix Makefiles" .. && make && cp -r ../include/gtest/ /usr/local/include/ && cp lib*.so /usr/local/lib/
RUN sudo bash -c 'echo /usr/local/lib >> /etc/ld.so.conf ' && sudo ldconfig

# Download repository
RUN git clone https://github.com/pabloalbanese/taller2.git && cd taller2/Servidor && mkdir lib 

RUN cd taller2/ && sh script_librerias.sh
RUN cd ../

RUN cp /usr/lib/librocksdb.a taller2/Servidor/lib/ 
RUN cp /usr/lib/x86_64-linux-gnu/libbz2.a taller2/Servidor/lib/ 
RUN cp /usr/lib/x86_64-linux-gnu/libz.a taller2/Servidor/lib/
RUN cp /usr/lib/libsnappy.a taller2/Servidor/lib/
RUN cp /usr/local/lib/libjson_linux-gcc-4.8_libmt.a taller2/Servidor/lib/

RUN cd taller2/Servidor && sh CompileServer.sh
