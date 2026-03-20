cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_PROJECT_NAME bf_k6_firmware)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

enable_language(C CXX ASM)

set(VERSION_NUMBER 0.1.0)

set(EXE_NAME ${CMAKE_PROJECT_NAME}-${VERSION_NUMBER})
message("Binary name: " ${EXE_NAME})

add_executable(${EXE_NAME})

add_subdirectory(lib)
add_subdirectory(src)

target_link_libraries(${EXE_NAME} app)
target_compile_definitions(${EXE_NAME} PRIVATE $<$<CONFIG:Debug>:DEBUG>)
target_compile_definitions(${EXE_NAME} PRIVATE VERSION_NUMBER="${VERSION_NUMBER}")
target_link_options(${EXE_NAME} PRIVATE -Wl,-Map=${EXE_NAME}.map)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Generate .bin file
add_custom_command(
    TARGET ${EXE_NAME}
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary ${EXE_NAME}.elf ${EXE_NAME}.bin
    COMMENT "Generate .bin file"
)

# 
add_custom_command(
    TARGET ${EXE_NAME}
    POST_BUILD
    COMMAND arm-none-eabi-objdump -D ${EXE_NAME}.elf > ${EXE_NAME}.asm
    COMMENT "Disassemble"
)
