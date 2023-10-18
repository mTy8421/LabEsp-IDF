# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/opt/esp/idf/components/bootloader/subproject"
  "/data/lab09END/build/bootloader"
  "/data/lab09END/build/bootloader-prefix"
  "/data/lab09END/build/bootloader-prefix/tmp"
  "/data/lab09END/build/bootloader-prefix/src/bootloader-stamp"
  "/data/lab09END/build/bootloader-prefix/src"
  "/data/lab09END/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/data/lab09END/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/data/lab09END/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
