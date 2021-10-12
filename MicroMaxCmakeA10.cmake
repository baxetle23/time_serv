# the name of the target operating system
SET(CMAKE_SYSTEM_NAME      Linux)
SET(CMAKE_SYSTEM_PROCESSOR arm)

# which compilers to use for C and C++ and ASM-ATT
SET(CMAKE_C_COMPILER       /home/aponyatov/Desktop/cross/crostool_4_9_1/bin/arm-cortex_a9-linux-gnueabi-gcc )
SET(CMAKE_CXX_COMPILER     /home/aponyatov/Desktop/cross/crostool_4_9_1/bin/arm-cortex_a9-linux-gnueabi-g++ )
SET(CMAKE_ASM-ATT_COMPILER /home/aponyatov/Desktop/cross/crostool_4_9_1/bin/arm-cortex_a9-linux-gnueabi-as  )


# here is the target environment located
#SET(CMAKE_FIND_ROOT_PATH  /usr/i686-pc-mingw32
#                          /usr/i686-pc-mingw32/sys-root/i686-pc-mingw32
#    )

# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search
# programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)