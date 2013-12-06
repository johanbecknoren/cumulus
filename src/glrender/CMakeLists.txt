# cumulusGL #

if(GLEW_FOUND)
  include_directories(${GLEW_INCLUDE_DIR})
  list(APPEND PLATFORM_LIBS ${GLEW_LIBRARY} ${GLFW_LIBRARY})
endif()

file(GLOB HEADERS *.h*)
file(GLOB SRCS *.cpp)

include_directories(
  ${PROJECT_SOURCE_DIR}/core
)

f_create_glfw_executable(cumulusGL
  ${SRCS}
  ${HEADERS})

target_link_libraries(cumulusGL
  ${PLATFORM_LIBS} cumulusCore)