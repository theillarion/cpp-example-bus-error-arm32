set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(TARGET_ARCHITECTURE armhf) # This variable is required for the debian installation package
set(GCC_PREFIX "arm-linux-gnueabihf")

if(CMAKE_HOST_LINUX)
  if(DEFINED ENV{TOOLCHAIN})
    set(TOOLCHAIN $ENV{TOOLCHAIN}) # CI/CD
  else()
    set(TOOLCHAIN "$ENV{HOME}/toolchain/gcc-linaro-6.3.1-2017.05-x86_64_${GCC_PREFIX}")
  endif()
  if(DEFINED ENV{SYSROOT}) # CI/CD
    set(SYSROOT $ENV{SYSROOT})
  else()
    set(SYSROOT "${TOOLCHAIN}/../sysroot")
  endif()
  set(GCC_POSTFIX "")
elseif(CMAKE_HOST_WIN32)
  set(TOOLCHAIN "C:/SysGCC/beaglebone")
  set(SYSROOT "${TOOLCHAIN}/${GCC_PREFIX}/sysroot")
  set(GCC_POSTFIX ".exe")
else()
  message(FATAL_ERROR "Host type is not supported")
endif()


set(CMAKE_SYSROOT ${SYSROOT})
set(CMAKE_SYSROOT_LINK ${SYSROOT})
set(CMAKE_SYSROOT_COMPILE ${SYSROOT})

set(CMAKE_FIND_ROOT_PATH ${SYSROOT})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(CMAKE_C_COMPILER ${TOOLCHAIN}/bin/${GCC_PREFIX}-gcc${GCC_POSTFIX})
set(CMAKE_CXX_COMPILER ${TOOLCHAIN}/bin/${GCC_PREFIX}-g++${GCC_POSTFIX})
set(CMAKE_AR ${TOOLCHAIN}/bin/${GCC_PREFIX}-ar${GCC_POSTFIX})
set(CMAKE_STRIP ${TOOLCHAIN}/bin/${GCC_PREFIX}-strip${GCC_POSTFIX})
set(CMAKE_OBJCOPY ${TOOLCHAIN}/bin/${GCC_PREFIX}-objcopy${GCC_POSTFIX})

# Compiler hardware flags
# Flag `-fasynchronous-unwind-tables`: generates additional information about stack unwinding in dwarf2 format.
set(CMAKE_C_FLAGS "-march=armv7-a -marm -mfpu=neon -mfloat-abi=hard -fasynchronous-unwind-tables --sysroot=${SYSROOT}")
set(CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS})
add_compile_definitions(ARM)

# Additional common flags
# DEBUG without optimisation
# Flag `-rdynamic`: adds information about symbols to the binary file
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -ffunction-sections -Wall -Wextra -O0 -rdynamic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -ffunction-sections -Wall -Wextra -Weffc++ -O0 -rdynamic")

# RELEASE with optimisation
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -ffunction-sections -Wall -Wextra -O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -ffunction-sections -Wall -Wextra -Weffc++ -O2 -DNDEBUG -rdynamic -ggdb")


set(CMAKE_EXE_LINKER_FLAGS "--sysroot=${SYSROOT}")

set(CMAKE_LIBRARY_PATH  ${SYSROOT}/usr/lib
                        ${SYSROOT}/lib
                        ${SYSROOT}/lib/arm-linux-gnueabihf)

include_directories(${TOOLCHAIN}/usr/include/
                    ${TOOLCHAIN}/usr/include/arm-linux-gnueabihf/)