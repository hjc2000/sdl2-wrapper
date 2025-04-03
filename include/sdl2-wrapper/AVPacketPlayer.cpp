#include <base/file/IFileStream.h>
#include <base/string/define.h>
#include <ffmpeg-wrapper/input-format/InputFormat.h>
#include <ffmpeg-wrapper/wrapper/AVIOContextWrapper.h>
#include <sdl2-wrapper/AVPacketPlayer.h>

video::AVPacketPlayer::AVPacketPlayer(int x,
									  int y,
									  AVStreamWrapper const &video_stream,
									  AVStreamWrapper const &audio_stream)
{
	_audio_packet_player = std::shared_ptr<AudioPacketPlayer>{new AudioPacketPlayer{audio_stream}};
	_video_packet_player = std::shared_ptr<VideoPacketPlayer>{new VideoPacketPlayer{x, y, video_stream}};
	_video_packet_player->SetRefTimer(_audio_packet_player);
	_video_stream_index = video_stream.Index();
	_audio_stream_index = audio_stream.Index();
}

video::AVPacketPlayer::~AVPacketPlayer()
{
	Dispose();
}

void video::AVPacketPlayer::Dispose()
{
	if (_disposed)
	{
		return;
	}

	_disposed = true;

	_audio_packet_player->Dispose();
	_video_packet_player->Dispose();
}

void video::AVPacketPlayer::Pause(bool pause)
{
	_audio_packet_player->Pause(pause);
	_video_packet_player->Pause(pause);
}

void video::AVPacketPlayer::SendData(AVPacketWrapper &packet)
{
	if (packet.StreamIndex() == _video_stream_index)
	{
		_video_packet_player->SendData(packet);
	}
	else if (packet.StreamIndex() == _audio_stream_index)
	{
		_audio_packet_player->SendData(packet);
	}
}

void video::test_AVPacketPlayer()
{
	auto fs = base::file::OpenExisting("idol.mp4");

	std::shared_ptr<AVIOContextWrapper> io_context{new AVIOContextWrapper{
		video::AVIOContextWrapper_IsWrite{false},
		fs,
	}};

	std::shared_ptr<InputFormat> in_fmt_ctx{new InputFormat{io_context}};
	in_fmt_ctx->DumpFormat();

	AVStreamWrapper best_audio_stream = in_fmt_ctx->FindBestStream(AVMediaType::AVMEDIA_TYPE_AUDIO);
	AVStreamWrapper best_video_stream = in_fmt_ctx->FindBestStream(AVMediaType::AVMEDIA_TYPE_VIDEO);

	std::shared_ptr<AVPacketPlayer> player{
		new AVPacketPlayer{
			0,
			70,
			best_video_stream,
			best_audio_stream,
		},
	};

	AVPacketWrapper packet;
	base::CancellationTokenSource cancellation_token_source;
	auto cancellation_token = cancellation_token_source.Token();
	base::TaskCompletionSignal thread_has_exited{false};
	base::Pump<AVPacketWrapper> packet_pump{in_fmt_ctx};
	packet_pump.ConsumerList().Add(player);

	std::thread{
		[&]()
		{
			// 将包从封装泵送到播放器。
			try
			{
				packet_pump.PumpDataToConsumers(cancellation_token);
			}
			catch (std::exception &e)
			{
				std::cout << CODE_POS_STR << e.what() << std::endl;
			}

			thread_has_exited.SetResult();
		}}
		.detach();

	// 开始播放
	player->Pause(false);

	// 等待退出
	SDL_EventGetter event_getter;
	event_getter.WaitQuitEvent();
	cancellation_token_source.Cancel();
	player->Dispose();
	thread_has_exited.Wait();
}

void video::AVPacketPlayer::Flush()
{
	_video_packet_player->Flush();
	_audio_packet_player->Flush();
}
