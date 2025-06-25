#pragma once
#include "base/IDisposable.h"
#include "base/pipe/IConsumer.h"
#include "sdl2-wrapper/AudioPacketPlayer.h"
#include "sdl2-wrapper/VideoPacketPlayer.h"

namespace video
{
	///
	/// @brief 同时播放一路流的音频包和一路流的视频包。
	///
	///
	class AVPacketPlayer final :
		public base::IDisposable,
		public base::IConsumer<AVPacketWrapper>
	{
	private:
		std::atomic_bool _disposed = false;
		std::shared_ptr<VideoPacketPlayer> _video_packet_player;
		std::shared_ptr<AudioPacketPlayer> _audio_packet_player;
		int _video_stream_index = 0;
		int _audio_stream_index = 0;

	public:
		///
		/// @brief 新建播放器。
		///
		/// @param x 窗口横坐标。
		/// @param y 窗口纵坐标。
		/// @param video_stream 要播放的视频流
		/// @param audio_stream 要播放的音频流
		///
		AVPacketPlayer(int x, int y,
					   AVStreamWrapper const &video_stream,
					   AVStreamWrapper const &audio_stream);

		~AVPacketPlayer();
		void Dispose() override;

		void Pause(bool pause);
		void SendData(AVPacketWrapper &packet) override;

		void Flush() override;
	};

	void test_AVPacketPlayer();
} // namespace video
