#include "sdl2-wrapper/SDL_Initializer.h"
#include <cstdint>
#include <iostream>
#include <mutex>

void video::SDL_Initializer::Initialize(SDL_InitOption option)
{
	static std::mutex lock;
	static bool initialzed = false;

	std::lock_guard l(lock);
	if (initialzed)
	{
		// 已经被初始化过了，直接返回
		return;
	}

	int result = SDL_Init((uint32_t)option);
	if (result)
	{
		// 发生了错误
		auto error = SDL_GetError();
		std::cout << error << std::endl;
		throw std::runtime_error(error);
	}

	initialzed = true;
}
