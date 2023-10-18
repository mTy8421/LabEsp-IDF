# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/opt/esp/idf/components/bootloader/subproject"
  "/data/HttpGet/build/bootloader"
  "/data/HttpGet/build/bootloader-prefix"
  "/data/HttpGet/build/bootloader-prefix/tmp"
  "/data/HttpGet/build/bootloader-prefix/src/bootloader-stamp"
  "/data/HttpGet/build/bootloader-prefix/src"
  "/data/HttpGet/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/data/HttpGet/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/data/HttpGet/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
