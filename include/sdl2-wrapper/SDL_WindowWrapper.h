#pragma once
#include <base/Wrapper.h>
#include <memory>
#include <sdl2-wrapper/SDL_Initializer.h>
#include <sdl2-wrapper/SDL_RendererWrapper.h>

namespace video
{
	/// @brief 创建渲染器时用来选择渲染器的类型
	enum class RendererFlags
	{
		/// @brief 不进行设置。
		/// @note 这个是我自己添加的，官方的 SDL_RendererFlags 里是没有的，
		/// 但是官方的函数是使用 uint32_t 接收 SDL_RendererFlags 类型变量的，并且允许
		/// 传入 0 或者 SDL_RendererFlags 中的一个或多个值按位相或。所以我在这里添加一个
		/// 扩展的 unset
		Unset = 0,

		/// @brief The renderer is a software fallback
		Software = ::SDL_RendererFlags::SDL_RENDERER_SOFTWARE,

		/// @brief The renderer uses hardware acceleration
		Accelerated = ::SDL_RendererFlags::SDL_RENDERER_ACCELERATED,

		/// @brief Present is synchronized with the refresh rate
		Presentvsync = ::SDL_RendererFlags::SDL_RENDERER_PRESENTVSYNC,

		/// @brief The renderer supports rendering to texture
		RendererTargetTexture = ::SDL_RendererFlags::SDL_RENDERER_TARGETTEXTURE,
	};

	/// @brief SDL 窗口包装器
	class SDL_WindowWrapper :
		public base::Wrapper<SDL_Window>
	{
	private:
		SDL_Window *_wrapped_obj = nullptr;

	public:
		SDL_WindowWrapper(char const *title,
						  int x, int y,
						  int w, int h,
						  SDL_WindowFlags flags);

		~SDL_WindowWrapper();

		int UpdateWindowSurface();
		SDL_Surface *GetWindowSurface();

		/// @brief 创建一个渲染器
		/// @param index
		/// @param flags
		/// @return
		std::shared_ptr<video::SDL_RendererWrapper>
		CreateRenderer(int index, RendererFlags flags = RendererFlags::Unset);

		/// @brief 设置窗口的位置
		/// @param x
		/// @param y
		void SetWindowPosition(int x, int y);

		/// @brief 获取屏幕尺寸
		/// @return
		static SDL_Rect GetScreenSize();

		SDL_Window *WrappedObj() const override
		{
			return _wrapped_obj;
		}
	};
} // namespace video
