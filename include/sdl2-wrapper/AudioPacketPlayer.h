#pragma once
#include <base/container/HysteresisBlockingQueue.h>
#include <base/pipe/IConsumer.h>
#include <base/pipe/PipeBlockingQueue.h>
#include <base/pipe/Pump.h>
#include <base/task/CancellationTokenSource.h>
#include <base/task/TaskCompletionSignal.h>
#include <ffmpeg-wrapper/pipe/ThreadDecoderPipe.h>
#include <ffmpeg-wrapper/wrapper/AVCodecContextWrapper.h>
#include <ffmpeg-wrapper/wrapper/AVStreamWrapper.h>
#include <sdl2-wrapper/AudioFramePlayer.h>

namespace video
{
	/// @brief 在回调中向播放器送入音频包，播放器会解码，然后播放。
	class AudioPacketPlayer :
		public base::IDisposable,
		public IRefTimer,
		public base::IConsumer<AVPacketWrapper>
	{
	private:
		std::atomic_bool _disposed = false;
		std::shared_ptr<AudioFramePlayer> _player;
		std::shared_ptr<IDecoderPipe> _decoder_pipe;
		std::shared_ptr<base::PipeBlockingQueue<AVPacketWrapper>> _packet_queue;
		base::CancellationTokenSource _cancel_pump_source;
		std::shared_ptr<base::Pump<AVPacketWrapper>> _packet_pump;

		/// @brief 此任务完成，说明解码线程函数 DecodingThreadFunc 已经结束了，且不会再执行了，
		/// 已经彻底退出了。初始时线程没有启动，所以为已完成。
		base::TaskCompletionSignal _decoding_thread_has_exited{true};

		/// @brief 线程启动后会立刻等待此信号。时机成熟后将此信号设置为完成以让线程函数真正开始执行。
		base::TaskCompletionSignal _decoding_thread_can_start{false};

		/// @brief 用于解码的线程函数。
		void DecodingThreadFunc();

	public:
		/// @brief 通过音频流初始化播放器。包括创建内部解码器等操作。
		/// @param stream
		AudioPacketPlayer(AVStreamWrapper const &stream);
		~AudioPacketPlayer();
		void Dispose() override;

		int64_t RefTime() override;

		/// @brief 暂停或开始播放。此方法会启动解码线程。
		/// @param pause 传入 false 开始播放，传入 true 暂停。
		void Pause(bool pause);

		/// @brief 向播放器送入包。
		/// @note 当内部的包队列满时，此函数会被阻塞。
		/// @param packet 要送入播放器的包。送入空指针表示冲洗播放器。
		void SendData(AVPacketWrapper &packet) override;

		void Flush() override
		{
			_packet_queue->Flush();
		}
	};
} // namespace video
