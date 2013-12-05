macro(f_create_glfw_executable project libs src headers)
  add_executable(${project} ${src} ${headers})
  target_link_libraries(${project} ${libs} GL X11 m Xxf86vm Xrandr pthread Xi)
endmacro()