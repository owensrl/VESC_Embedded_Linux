cmake_minimum_required(VERSION 2.8.3)
project(vesc_bbb_uart)

find_package(catkin REQUIRED COMPONENTS)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES ${PROJECT_NAME}
)

###########
## Build ##
###########

include_directories(include/${PROJECT_NAME})

add_library(${PROJECT_NAME}
  src/${PROJECT_NAME}/bldc.cpp
  src/${PROJECT_NAME}/bldc_interface.cpp
  src/${PROJECT_NAME}/bldc_interface_uart.cpp
  src/${PROJECT_NAME}/buffer.cpp
  src/${PROJECT_NAME}/comm_uart.cpp
  src/${PROJECT_NAME}/crc.cpp
  src/${PROJECT_NAME}/packet.cpp
)

#############
## Install ##
#############

install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
  FILES_MATCHING PATTERN "*.h"
)
