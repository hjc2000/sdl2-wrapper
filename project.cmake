add_library(${ProjectName} STATIC)

# 将资源目录用预定义传给 C++ 程序
target_compile_definitions(${ProjectName} PRIVATE Predefine_ResourceDir="${CMAKE_CURRENT_SOURCE_DIR}/资源")
target_import_src(${ProjectName})
target_import_base(${ProjectName} PUBLIC)
target_import_sdl2(${ProjectName} PUBLIC)
target_import_ffmpeg_wrapper(${ProjectName} PUBLIC)
