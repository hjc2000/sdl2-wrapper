#include "sdl2-wrapper/SDL_EventGetter.h"
#include <iostream>

video::SDL_EventGetter::SDL_EventGetter()
{
	SDL_Initializer::Initialize();
}

video::SDL_EventResult video::SDL_EventGetter::PollEvent()
{
	SDL_EventResult result{};
	result._success = SDL_PollEvent(&result._event);
	return result;
}

video::SDL_EventResult video::SDL_EventGetter::WaitEvent()
{
	SDL_EventResult result{};
	result._success = SDL_WaitEvent(&result._event);
	if (!result._success)
	{
		std::cout << CODE_POS_STR << SDL_GetError() << std::endl;
	}

	return result;
}

bool video::SDL_EventGetter::PollQuitEvent()
{
	SDL_EventResult result = PollEvent();
	if (result._success && result._event.type == SDL_EventType::SDL_QUIT)
	{
		return true;
	}

	return false;
}

void video::SDL_EventGetter::WaitQuitEvent()
{
	while (true)
	{
		SDL_EventResult result = WaitEvent();
		if (result._success && result._event.type == SDL_EventType::SDL_QUIT)
		{
			break;
		}
	}
}
