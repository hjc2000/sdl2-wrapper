#pragma once
#include <base/IDisposable.h>
#include <base/pipe/IConsumer.h>
#include <base/pipe/PipeBlockingQueue.h>
#include <base/pipe/Pump.h>
#include <base/task/CancellationTokenSource.h>
#include <ffmpeg-wrapper/ErrorCode.h>
#include <ffmpeg-wrapper/pipe/ThreadDecoderPipe.h>
#include <sdl2-wrapper/VideoFramePlayer.h>

namespace video
{
	class VideoPacketPlayer :
		public base::IDisposable,
		public base::IConsumer<AVPacketWrapper>
	{
	private:
		std::atomic_bool _disposed = false;
		std::shared_ptr<base::PipeBlockingQueue<AVPacketWrapper>> _packet_queue;
		base::CancellationTokenSource _cancel_pump_source;
		std::shared_ptr<base::Pump<AVPacketWrapper>> _packet_pump;
		std::shared_ptr<IDecoderPipe> _decoder_pipe;
		std::shared_ptr<VideoFramePlayer> _player;

		/// @brief 解码线程创建后会立刻等待此信号，当时机成熟，解码线程可以开始执行了，
		/// 就将此信号设置为已完成。
		base::TaskCompletionSignal _decoding_thread_can_start{false};

		/// @brief 解码线程退出后设为已完成。
		base::TaskCompletionSignal _thread_has_exited{true};

		/// @brief 用于解码的线程函数
		void DecodingThreadFunc();

	public:
		/// @brief 视频包播放器。
		/// @param x 窗口的横坐标
		/// @param y 窗口的纵坐标
		/// @param stream 想要播放的流。必须是视频流。
		VideoPacketPlayer(int x, int y, AVStreamWrapper const &stream);
		~VideoPacketPlayer();
		void Dispose() override;

		/// @brief 向播放器送入包。可以送入空指针，用来冲洗播放器。
		/// 当内部的包队列满时，此函数会被阻塞。
		/// @param packet 要送入播放器的包。送入空指针表示冲洗播放器。
		void SendData(AVPacketWrapper &packet) override;

		void Flush() override
		{
			_packet_queue->Flush();
		}

		void Pause(bool pause);

		std::shared_ptr<IRefTimer> RefTimer();

		/// @brief 设置参考时钟。
		/// @note 传入非空指针则开启同步。本视频帧播放器会同步到此参考时钟。
		/// @note 传入空指针可以关闭同步。
		/// @note 可以随时传入空指针来关闭音视频同步。
		/// @param value
		void SetRefTimer(std::shared_ptr<IRefTimer> value);
	};
} // namespace video
