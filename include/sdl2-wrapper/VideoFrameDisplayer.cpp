#include "VideoFrameDisplayer.h"

using namespace video;

VideoFrameDisplayer::VideoFrameDisplayer(int x,
                                         int y,
                                         int width,
                                         int height,
                                         AVPixelFormat pix_format,
                                         std::string window_title,
                                         SDL_WindowFlags flags)
{
    _window = std::shared_ptr<SDL_WindowWrapper>{
        new SDL_WindowWrapper{
            window_title.c_str(),
            x,
            y,
            width,
            height,
            flags,
        },
    };

    _renderer = _window->CreateRenderer(-1);

    _texture = _renderer->CreateTexture(pix_format,
                                        SDL_TextureAccess::SDL_TEXTUREACCESS_STREAMING,
                                        width,
                                        height);
}

void VideoFrameDisplayer::SendData(AVFrameWrapper &frame)
{
    // 更新纹理
    if (_texture->UpdateYUVTexture(nullptr, frame) < 0)
    {
        std::cout << SDL_GetError() << std::endl;
        return;
    }

    _renderer->Clear();
    if (_renderer->RenderTexture(_texture))
    {
        std::cout << SDL_GetError() << std::endl;
        return;
    }

    _renderer->Present();
}

void video::VideoFrameDisplayer::Flush()
{
}
