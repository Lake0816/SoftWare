INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/public)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/com/inc)

SET(SRC_LISTS main.c)

LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})

ADD_EXECUTABLE(${CMAKE_PROJECT_NAME} ${SRC_LISTS})

if (CONFIG_COMMON_LINUX_SEM)
#   Below methord is both OK
    TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} softlinuxsem)
#    TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} softcrc)
#    TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} -lsoftcrc)
endif (CONFIG_COMMON_LINUX_SEM)

