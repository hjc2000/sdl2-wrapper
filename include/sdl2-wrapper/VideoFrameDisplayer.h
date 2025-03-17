#pragma once
#include <base/pipe/IConsumer.h>
#include <ffmpeg-wrapper/info-collection/IVideoStreamInfoCollection.h>
#include <ffmpeg-wrapper/wrapper/AVFrameWrapper.h>
#include <sdl2-wrapper/pch.h>
#include <sdl2-wrapper/SDL_EventGetter.h>
#include <sdl2-wrapper/SDL_RendererWrapper.h>
#include <sdl2-wrapper/SDL_TextureWrapper.h>
#include <sdl2-wrapper/SDL_WindowWrapper.h>

namespace video
{
	/// @brief 视频帧显示器
	class VideoFrameDisplayer :
		public base::IConsumer<AVFrameWrapper>
	{
	private:
		std::shared_ptr<SDL_WindowWrapper> _window;
		std::shared_ptr<SDL_RendererWrapper> _renderer;
		std::shared_ptr<SDL_TextureWrapper> _texture;

	public:
		VideoFrameDisplayer(int x,
							int y,
							int width,
							int height,
							AVPixelFormat pix_format,
							std::string window_title,
							SDL_WindowFlags flags);

		/// @brief 显示视频帧。
		/// @param frame 要显示的帧。送入空指针会直接返回，不会有任何影响。
		/// 注意，一定要送入视频帧，不能送入音频帧。
		void SendData(AVFrameWrapper &frame) override;

		void Flush() override;
	};
} // namespace video
