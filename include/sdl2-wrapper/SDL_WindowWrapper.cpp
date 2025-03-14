#include "sdl2-wrapper/SDL_WindowWrapper.h"
#include <iostream>

video::SDL_WindowWrapper::SDL_WindowWrapper(char const *title,
                                            int x, int y,
                                            int w, int h,
                                            SDL_WindowFlags flags)
{
    SDL_Initializer::Initialize();
    _wrapped_obj = SDL_CreateWindow(title, x, y, w, h, flags);
    if (!_wrapped_obj)
    {
        throw std::runtime_error(SDL_GetError());
    }
}

video::SDL_WindowWrapper::~SDL_WindowWrapper()
{
    std::cout << "~SDL_WindowWrapper()" << std::endl;
    SDL_DestroyWindow(_wrapped_obj);
}

int video::SDL_WindowWrapper::UpdateWindowSurface()
{
    return SDL_UpdateWindowSurface(_wrapped_obj);
}

SDL_Surface *video::SDL_WindowWrapper::GetWindowSurface()
{
    return SDL_GetWindowSurface(_wrapped_obj);
}

std::shared_ptr<video::SDL_RendererWrapper>
video::SDL_WindowWrapper::CreateRenderer(int index, RendererFlags flags)
{
    SDL_Renderer *renderer = SDL_CreateRenderer(_wrapped_obj,
                                                index,
                                                static_cast<uint32_t>(flags));

    return std::shared_ptr<video::SDL_RendererWrapper>{
        new video::SDL_RendererWrapper{
            renderer,
        },
    };
}

void video::SDL_WindowWrapper::SetWindowPosition(int x, int y)
{
    SDL_SetWindowPosition(_wrapped_obj, x, y);
}

SDL_Rect video::SDL_WindowWrapper::GetScreenSize()
{
    SDL_Rect screenRect;
    int ret = SDL_GetDisplayBounds(0, &screenRect);
    if (ret < 0)
    {
        throw std::runtime_error{SDL_GetError()};
    }

    return screenRect;
}
