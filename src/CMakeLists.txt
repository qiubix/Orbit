set(CMAKE_INCLUDE_CURRENT_DIR ON)

link_directories(${CMAKE_SOURCE_DIR}/lib/GLUT)
set(GLUT_LIBS GLU32)

file(GLOB PROJECT_SRC_LIST *.cpp)
add_executable(${PROJECT_NAME} ${PROJECT_SRC_LIST}) 
target_link_libraries(${PROJECT_NAME} ${GLUT_LIBS})
