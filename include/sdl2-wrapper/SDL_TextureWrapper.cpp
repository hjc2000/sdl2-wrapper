#include "sdl2-wrapper/SDL_TextureWrapper.h"

video::SDL_TextureWrapper::SDL_TextureWrapper(::SDL_Texture *texture)
{
	_wrapped_obj = texture;
}

video::SDL_TextureWrapper::~SDL_TextureWrapper()
{
	std::cout << "SDL_TextureWrapper 析构函数" << std::endl;
	SDL_DestroyTexture(_wrapped_obj);
}

int video::SDL_TextureWrapper::UpdateYUVTexture(SDL_Rect const *rect,
												Uint8 const *Yplane, int Ysize,
												Uint8 const *Uplane, int Usize,
												Uint8 const *Vplane, int Vsize)
{
	return ::SDL_UpdateYUVTexture(_wrapped_obj, rect,
								  Yplane, Ysize,
								  Uplane, Usize,
								  Vplane, Vsize);
}

int video::SDL_TextureWrapper::UpdateYUVTexture(SDL_Rect const *rect, video::AVFrameWrapper &frame)
{
	return UpdateYUVTexture(rect,
							frame->data[0], frame->linesize[0],
							frame->data[1], frame->linesize[1],
							frame->data[2], frame->linesize[2]);
}
