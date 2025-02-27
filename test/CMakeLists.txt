# Copyright 2021 Larthia, University of Pisa. All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.

cmake_minimum_required(VERSION 3.0)

project (nethuns-test)

include(../Nethuns.cmake)

#
# Compiler options...
#


set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -g -O2 -march=native -fomit-frame-pointer -Wall -Wextra -Wshadow")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2 -std=c++17 -march=native -fomit-frame-pointer -Wall -Wextra -Wshadow")

find_library(LIBPCAP_LIBRARY NAMES libpcap.a PATHS "/usr/local/opt/licap/lib" NO_DEFAULT_PATH NO_SYSTEM_ENVIRONMENT_PATH)
find_path(LIBPCAP_INCLUDE_DIR NAMES pcap/pcap.h PATHS "/usr/local/opt/libpcap/include/" NO_DEFAULT_PATH_ENVIRONMENT_PATH)

include_directories(/usr/local/lib . ../src)
link_directories(/usr/local/lib)

add_executable(nethuns-version      src/version.c)
add_executable(nethuns-test         src/test.c)
add_executable(nethuns-send         src/send.cpp)
add_executable(nethuns-meter        src/meter.cpp)
add_executable(nethuns-filter       src/filter.cpp)
add_executable(nethuns-file-pcap    src/file-pcap.cpp)
add_executable(nethuns-file-pcap-mt src/file-pcap-mt.cpp)
add_executable(nethuns-forward      src/forward.cpp)
add_executable(nethuns-forward-mt   src/forward-mt.cpp)
add_executable(nethuns-meter-mt     src/meter-mt.cpp)

add_executable(pure-pcap-meter      src/pure-meter-pcap.cpp)
add_executable(pure-pcap-meter-mt   src/pure-meter-pcap-mt.cpp)
add_executable(pure-pcap-forward    src/pure-forward-pcap.cpp)
add_executable(pure-pcap-forward-mt src/pure-forward-pcap-mt.cpp)
add_executable(pcap-version         src/pcap-version.c)

target_link_libraries(nethuns-file-pcap-mt  ${NETHUNS_LIBS} -pthread)

target_link_libraries(nethuns-version       ${NETHUNS_LIBS} -pthread)
target_link_libraries(nethuns-test       	${NETHUNS_LIBS} -pthread)
target_link_libraries(nethuns-meter-mt      ${NETHUNS_LIBS} -pthread)
target_link_libraries(nethuns-send       	${NETHUNS_LIBS} -pthread)
target_link_libraries(nethuns-forward    	${NETHUNS_LIBS} -pthread)
target_link_libraries(nethuns-forward-mt 	${NETHUNS_LIBS} -pthread)
target_link_libraries(nethuns-file-pcap  	${NETHUNS_LIBS} -pthread)
target_link_libraries(nethuns-meter         ${NETHUNS_LIBS} -pthread)
target_link_libraries(nethuns-filter        ${NETHUNS_LIBS} -pthread)

target_link_libraries(pure-pcap-forward      -pthread ${LIBPCAP_LIBRARY})
target_link_libraries(pure-pcap-forward-mt   -pthread ${LIBPCAP_LIBRARY})
target_link_libraries(pure-pcap-meter        -pthread ${LIBPCAP_LIBRARY})
target_link_libraries(pure-pcap-meter-mt     -pthread ${LIBPCAP_LIBRARY})
target_link_libraries(pcap-version           ${LIBPCAP_LIBRARY})
