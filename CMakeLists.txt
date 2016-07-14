#
# @author   : huang li long <huanglilongwk@outlook.com>
# @time     : 2016/07/14
# @brief    : read and write data through serial port 
# 

# cmake version 3.2 or later
cmake_minimum_required(VERSION 3.2)

# project name
project(SerialPort)

# include header file directory 
include_directories(${PROJECT_SOURCE_DIR}
                    ${PROJECT_SOURCE_DIR}/serial_port)

# add serial port library 
add_subdirectory(serial_port)

# add executable
add_executable(${PROJECT_NAME} main.cpp)

# link to library 
target_link_libraries(${PROJECT_NAME} serial_port pthread)
