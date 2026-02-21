add_library(${ProjectName} OBJECT)
target_import_src(${ProjectName})
target_import_base(${ProjectName} PUBLIC)
target_import_sdl2(${ProjectName} PUBLIC)
target_import_ffmpeg_wrapper(${ProjectName} PUBLIC)


# 添加测试程序
if(("${platform}" STREQUAL "msys") OR
   ("${platform}" STREQUAL "msys-clang"))
	set(test_exe_target_name "${ProjectName}-test")
	add_executable(${test_exe_target_name})
	target_import_test(${test_exe_target_name})

	# 将资源目录用预定义传给 C++ 程序
	target_compile_definitions(${test_exe_target_name} PUBLIC Predefine_ResourceDir="D:/video/视频开发测试")

	target_link_libraries(${test_exe_target_name} PUBLIC ${ProjectName})
endif()
