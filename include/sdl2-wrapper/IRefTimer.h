#pragma once
#include <sdl2-wrapper/pch.h>

namespace video
{
	///
	/// @brief 参考时钟
	///
	///
	class IRefTimer
	{
	public:
		virtual ~IRefTimer() = default;

		///
		/// @brief 获取当前的参考时间。单位毫秒。
		///
		/// @return int64_t
		///
		virtual int64_t RefTime() = 0;
	};
} // namespace video
