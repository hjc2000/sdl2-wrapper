#include "SDL_RendererWrapper.h"
#include <iostream>

video::SDL_RendererWrapper::SDL_RendererWrapper(SDL_Renderer *renderer)
{
    _wrapped_obj = renderer;
}

video::SDL_RendererWrapper::SDL_RendererWrapper(SDL_RendererWrapper const &o)
{
    *this = o;
}

video::SDL_RendererWrapper::~SDL_RendererWrapper()
{
    std::cout << "~SDL_RendererWrapper()" << std::endl;
    SDL_DestroyRenderer(_wrapped_obj);
}

video::SDL_RendererWrapper &video::SDL_RendererWrapper::operator=(SDL_RendererWrapper const &o)
{
    _wrapped_obj = o._wrapped_obj;
    return *this;
}

int video::SDL_RendererWrapper::SetDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return SDL_SetRenderDrawColor(_wrapped_obj, r, g, b, a);
}

int video::SDL_RendererWrapper::DrawRect(SDL_Rect const *rect)
{
    return SDL_RenderDrawRect(_wrapped_obj, rect);
}

int video::SDL_RendererWrapper::DrawFillRect(SDL_Rect const *rect)
{
    return SDL_RenderFillRect(_wrapped_obj, rect);
}

int video::SDL_RendererWrapper::SetBlendMode(SDL_BlendMode mode)
{
    return SDL_SetRenderDrawBlendMode(_wrapped_obj, mode);
}

int video::SDL_RendererWrapper::Clear()
{
    return SDL_RenderClear(_wrapped_obj);
}

int video::SDL_RendererWrapper::RenderTexture(std::shared_ptr<SDL_TextureWrapper> texture,
                                              SDL_Rect const *srcrect,
                                              SDL_Rect const *dstrect)
{
    return SDL_RenderCopy(_wrapped_obj, *texture, srcrect, dstrect);
}

int video::SDL_RendererWrapper::RenderTexture(std::shared_ptr<SDL_TextureWrapper> texture)
{
    return SDL_RenderCopy(_wrapped_obj,
                          *texture,
                          nullptr,
                          nullptr);
}

std::shared_ptr<video::SDL_TextureWrapper>
video::SDL_RendererWrapper::CreateTexture(SDL_PixelFormatEnum format,
                                          SDL_TextureAccess access,
                                          int w, int h)
{
    SDL_Texture *tex = SDL_CreateTexture(_wrapped_obj, format, access, w, h);
    if (tex == nullptr)
    {
        auto error = SDL_GetError();
        std::cout << error << std::endl;
        throw std::runtime_error(error);
    }

    return std::shared_ptr<SDL_TextureWrapper>{new SDL_TextureWrapper{tex}};
}

std::shared_ptr<video::SDL_TextureWrapper>
video::SDL_RendererWrapper::CreateTexture(AVPixelFormat format,
                                          SDL_TextureAccess access,
                                          int w, int h)
{
    SDL_PixelFormatEnum sdl_pixel_format;
    switch (format)
    {
    case AVPixelFormat::AV_PIX_FMT_YUV420P:
        {
            sdl_pixel_format = SDL_PixelFormatEnum::SDL_PIXELFORMAT_IYUV;
            break;
        }
    case AVPixelFormat::AV_PIX_FMT_YUV422P:
        {
            sdl_pixel_format = SDL_PixelFormatEnum::SDL_PIXELFORMAT_YUY2;
            break;
        }
    case AVPixelFormat::AV_PIX_FMT_RGB24:
        {
            sdl_pixel_format = SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGB888;
            break;
        }
    case AVPixelFormat::AV_PIX_FMT_YUV420P10LE:
        {
            throw std::runtime_error{CODE_POS_STR + std::string("暂不支持")};
        }
    default:
        {
            throw std::runtime_error{CODE_POS_STR + std::string("暂不支持")};
        }
    }

    return CreateTexture(sdl_pixel_format, access, w, h);
}

void video::SDL_RendererWrapper::Present()
{
    SDL_RenderPresent(_wrapped_obj);
}
