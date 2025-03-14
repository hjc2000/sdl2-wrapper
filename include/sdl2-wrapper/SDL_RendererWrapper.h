#pragma once
#include <memory>
#include <sdl2-wrapper/SDL_Initializer.h>
#include <sdl2-wrapper/SDL_TextureWrapper.h>

namespace video
{
	/// @brief SDL 渲染器包装类
	class SDL_RendererWrapper :
		public base::Wrapper<SDL_Renderer>
	{
	private:
		SDL_Renderer *_wrapped_obj = nullptr;

	public:
		SDL_RendererWrapper(SDL_Renderer *renderer);
		SDL_RendererWrapper(SDL_RendererWrapper const &o);
		~SDL_RendererWrapper();

		SDL_RendererWrapper &operator=(SDL_RendererWrapper const &o);

		SDL_Renderer *WrappedObj() const override
		{
			return _wrapped_obj;
		}

		/// @brief 设置用来绘制的颜色
		/// @param r 红色通道数值
		/// @param g 绿色通道数值
		/// @param b 蓝色通道数值
		/// @param a 透明通道数值
		/// @return 成功返回 0，失败返回错误代码
		int SetDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

		/// @brief 在缓冲区中绘制矩形
		/// @param rect
		/// @return 成功返回 0，失败返回错误代码
		int DrawRect(SDL_Rect const *rect);

		/// @brief 绘制一个实心的矩形。矩形内部也会被充满画笔颜色
		/// @param rect
		/// @return 成功返回 0，失败返回错误代码
		int DrawFillRect(SDL_Rect const *rect);

		/// @brief 设置混合模式。
		/// 透明通道需要设置混合模式为 SDL_BlendMode::SDL_BLENDMODE_BLEND 才有用
		/// @param mode 混合模式
		/// @return 成功返回 0，失败返回错误代码。
		int SetBlendMode(SDL_BlendMode mode);

		/// @brief 清除画布
		/// 会用当前画笔的颜色涂满画布。画笔颜色通过 SetDrawColor 方法设置。
		/// @return 成功返回 0，失败返回错误代码。
		int Clear();

		/// @brief 将纹理复制到本渲染器进行渲染。
		/// @param texture 要渲染的纹理。
		/// @param srcrect 选择 texture 中的一块矩形区域作为输入，传入空指针表示将整个 texture 作为输入
		/// @param dstrect 选择渲染目标的一块矩形区域作为输出，传入空指针表示将整个渲染目标作为输出
		/// @return 成功返回 0，失败返回错误代码。
		int RenderTexture(std::shared_ptr<SDL_TextureWrapper> texture,
						  SDL_Rect const *srcrect,
						  SDL_Rect const *dstrect);

		/// @brief 将整个纹理复制到本渲染器的整个矩形中进行渲染。如果 texture
		/// 的大小与本渲染器的矩形大小不匹配，会缩放 texture 以适应。
		/// @param texture 要渲染的纹理
		/// @return 成功返回 0，失败返回错误代码
		int RenderTexture(std::shared_ptr<SDL_TextureWrapper> texture);

		/// @brief 创建纹理
		/// @param format
		/// @param access
		/// @param w
		/// @param h
		/// @return
		std::shared_ptr<SDL_TextureWrapper> CreateTexture(SDL_PixelFormatEnum format,
														  SDL_TextureAccess access,
														  int w, int h);

		/// @brief 创建纹理
		/// @param format
		/// @param access
		/// @param w
		/// @param h
		/// @return
		std::shared_ptr<SDL_TextureWrapper> CreateTexture(AVPixelFormat format,
														  SDL_TextureAccess access,
														  int w, int h);

		/// @brief 要调用这个函数才能让渲染器将缓冲区中的 RGBA 渲染到屏幕中，不然
		/// 再怎么编辑缓冲区，数据都只是停留在缓冲区而已，不会呈现到屏幕上。
		void Present();
	};
} // namespace video
