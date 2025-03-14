#pragma once
#include <cstdint>

namespace video
{
	/// @brief 参考时钟
	class IRefTimer
	{
	public:
		virtual ~IRefTimer() = default;

		/// @brief 获取当前的参考时间。单位毫秒。
		/// @return
		virtual int64_t RefTime() = 0;
	};
} // namespace video
