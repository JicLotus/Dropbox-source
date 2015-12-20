cmake_minimum_required (VERSION 2.6)
project (Servidor)
# The version number.
set (Servidor_VERSION_MAJOR 1)
set (Servidor_VERSION_MINOR 0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAG} -std=c++11 -lrocksdb -pthread -Wl,--no-as-needed -lrt -lsnappy -lgflags -lz -lbz2 -lgtest -lgtest_main")

include_directories("${PROJECT_BINARY_DIR}")
include_directories(${PROJECT_SOURCE_DIR}/lib)
include_directories(${PROJECT_SOURCE_DIR}/src)

add_library(libRocksDb STATIC IMPORTED)
set_property(TARGET libRocksDb PROPERTY IMPORTED_LOCATION ../lib/librocksdb.a)

add_library(libBz2 STATIC IMPORTED)
set_property(TARGET libBz2 PROPERTY IMPORTED_LOCATION ../lib/libbz2.a)

add_library(libSnappy STATIC IMPORTED)
set_property(TARGET libSnappy PROPERTY IMPORTED_LOCATION ../lib/libsnappy.a)

add_library(libBz STATIC IMPORTED)
set_property(TARGET libBz PROPERTY IMPORTED_LOCATION ../lib/libz.a)

add_library(libJson STATIC IMPORTED)
set_property(TARGET libJson PROPERTY IMPORTED_LOCATION ../lib/libjson_linux-gcc-4.8_libmt.a)

# add the executable
add_executable(Servidor ./src/Servidor.cpp ./src/mongoose/MongooseHandler.cpp ./src/mongoose/mongoose.c ./src/mongoose/UserHandler.cpp ./src/rocksDb/RocksdbHandler.cpp ./src/entities/User.cpp ./src/entities/FileContent.cpp ./src/entities/Token.cpp ./src/rocksDb/RocksDb.cpp ./src/log/Log.cpp ./src/mongoose/FileHandler.cpp ./src/entities/File.cpp)

# add the executable tests
add_executable(Test ./testing/AllTests.cpp ./testing/TestFileHandler.cpp ./testing/TestUserHandler.cpp ./testing/TestFiles.cpp ./testing/TestFileContent.cpp ./testing/TestToken.cpp ./testing/TestUsers.cpp ./src/mongoose/MongooseHandler.cpp ./src/mongoose/mongoose.c ./src/mongoose/UserHandler.cpp ./src/rocksDb/RocksdbHandler.cpp ./src/entities/User.cpp ./src/entities/FileContent.cpp ./src/entities/Token.cpp ./src/rocksDb/RocksDb.cpp ./src/log/Log.cpp ./src/mongoose/FileHandler.cpp ./src/entities/File.cpp)

target_link_libraries(Servidor libRocksDb libBz2 libSnappy libBz libJson)

target_link_libraries(Test libRocksDb libBz2 libSnappy libBz libJson)
