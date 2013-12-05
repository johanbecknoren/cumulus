# cumulusGL #

if(GLEW_FOUND)
  include_directories(${GLEW_INCLUDE_DIR})
  list(APPEND PLATFORM_LIBS ${GLEW_LIBRARY})
endif()

file(GLOB HEADERS *.h*)
file(GLOB SRCS *.cpp)

include_directories(
  ${PROJECT_SOURCE_DIR}/core
)

f_create_glfw_executable(cumulusGL ${PLATFORM_LIBS} ${SRCS} ${HEADERS})