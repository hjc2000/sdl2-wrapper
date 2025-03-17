#pragma once
#include <base/container/HysteresisBlockingQueue.h>
#include <base/container/SafeQueue.h>
#include <base/IDisposable.h>
#include <base/pipe/IConsumer.h>
#include <ffmpeg-wrapper/ErrorCode.h>
#include <ffmpeg-wrapper/info-collection/VideoStreamInfoCollection.h>
#include <ffmpeg-wrapper/wrapper/AVCodecContextWrapper.h>
#include <ffmpeg-wrapper/wrapper/AVStreamWrapper.h>
#include <sdl2-wrapper/IRefTimer.h>
#include <sdl2-wrapper/Timer.h>
#include <sdl2-wrapper/VideoFrameDisplayer.h>

namespace video
{
	class VideoFramePlayer :
		public base::IConsumer<AVFrameWrapper>,
		public base::IDisposable
	{
	private:
		std::atomic_bool _disposed = false;
		Timer _timer;
		std::shared_ptr<VideoFrameDisplayer> _displayer;
		VideoStreamInfoCollection _video_stream_infos{};
		base::HysteresisBlockingQueue<AVFrameWrapper> _frame_queue{10};
		std::shared_ptr<IRefTimer> _ref_timer;

		/// @brief Timer 回调处理函数，需要在这里向显示器送入帧。
		/// @param interval_in_milliseconds
		/// @return
		uint32_t SDL_TimerCallbackHandler(uint32_t interval_in_milliseconds);

	public:
		VideoFramePlayer(int x,
						 int y,
						 IVideoStreamInfoCollection const &infos,
						 std::string window_title,
						 SDL_WindowFlags flags);

		~VideoFramePlayer();
		void Dispose() override;

		/// @brief 暂停播放。
		/// @param pause 本方法不会阻塞，可以在回调函数中使用。
		void Pause(bool pause);

		/// @brief 将帧送入队列。送入空指针冲洗队列。
		/// 播放器内部队列满时本方法会阻塞，直到消费到小于阈值才会取消阻塞。
		/// @param frame 要被送入队列的帧
		void SendData(AVFrameWrapper &frame) override;

		void Flush() override;

#pragma region 参考时钟
		std::shared_ptr<IRefTimer> RefTimer();

		/// @brief 设置参考时钟。
		/// @note 传入非空指针则开启同步。本视频帧播放器会同步到此参考时钟。
		/// @note 传入空指针可以关闭同步。
		/// @note 可以随时传入空指针来关闭音视频同步。
		/// @param value
		void SetRefTimer(std::shared_ptr<IRefTimer> value);

		/// @brief 获取当前的参考时间。单位：毫秒。
		/// @return
		int64_t RefTime();
#pragma endregion
	};
} // namespace video
