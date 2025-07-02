add_library(${ProjectName} STATIC)
target_import_src(${ProjectName})
target_import_base(${ProjectName} PUBLIC)
target_import_sdl2(${ProjectName} PUBLIC)
target_import_ffmpeg_wrapper(${ProjectName} PUBLIC)


# 添加测试程序
if(platform STREQUAL "msys")
	set(exe_name "test")
	add_executable(${exe_name} ${CMAKE_CURRENT_SOURCE_DIR}/exe/main.cpp)

	# 将资源目录用预定义传给 C++ 程序
	target_compile_definitions(${exe_name} PRIVATE Predefine_ResourceDir="D:/video/视频开发测试")

	target_link_libraries(${exe_name} ${ProjectName})
	target_install(${exe_name})
endif()
