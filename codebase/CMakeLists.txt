add_executable(gsp MACOSX_BUNDLE gsp.cpp Node.h dag.h)
target_link_libraries(gsp fssimplewindow)
target_link_libraries(gsp ysclass)
target_link_libraries(gsp ysbitmapfont)
