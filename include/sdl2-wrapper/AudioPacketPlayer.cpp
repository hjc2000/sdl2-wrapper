#include "AudioPacketPlayer.h"
#include "ffmpeg-wrapper/pipe/ThreadDecoderPipe.h"
#include <thread>

void video::AudioPacketPlayer::DecodingThreadFunc()
{
	_decoding_thread_can_start.Wait();
	_packet_pump->PumpDataToConsumers(_cancel_pump_source.Token());
}

video::AudioPacketPlayer::AudioPacketPlayer(AVStreamWrapper const &stream)
{
	/* #region 安装管道 */

	// 根据音频流构造音频帧播放器
	_player = std::shared_ptr<AudioFramePlayer>{new AudioFramePlayer{stream}};

	// 根据音频流创建解码器
	_decoder_pipe = std::unique_ptr<ThreadDecoderPipe>{new ThreadDecoderPipe{stream}};
	_decoder_pipe->ConsumerList().Add(_player);

	_packet_queue = std::shared_ptr<base::PipeBlockingQueue<AVPacketWrapper>>{new base::PipeBlockingQueue<AVPacketWrapper>{}};

	// 将包从队列送到管道解码器的泵
	_packet_pump = std::shared_ptr<base::Pump<AVPacketWrapper>>{new base::Pump<AVPacketWrapper>{_packet_queue}};

	_packet_pump->ConsumerList().Add(_decoder_pipe);

	/* #endregion */

	// 解码线程
	std::thread(
		[&]()
		{
			try
			{
				DecodingThreadFunc();
			}
			catch (std::exception &e)
			{
				std::cout << CODE_POS_STR << e.what() << std::endl;
			}

			_decoding_thread_has_exited.SetResult();
		})
		.detach();
}

video::AudioPacketPlayer::~AudioPacketPlayer()
{
	Dispose();
	std::cout << "~AudioPacketPlayer()" << std::endl;
}

void video::AudioPacketPlayer::Dispose()
{
	if (_disposed)
	{
		return;
	}

	_disposed = true;

	_decoding_thread_can_start.Dispose();
	_cancel_pump_source.Cancel();
	_decoder_pipe->Dispose();
	_player->Dispose();
	_packet_queue->Dispose();
	_decoding_thread_has_exited.Wait();
}

int64_t video::AudioPacketPlayer::RefTime()
{
	return _player->RefTime();
}

void video::AudioPacketPlayer::Pause(bool pause)
{
	if (pause)
	{
		/* 暂停播放
		 * 暂停帧播放器。
		 * 解码线程在帧播放器的缓冲区达到上限后会阻塞，所以只要暂停帧播放器，让帧播放器
		 * 不要消费，就可以达到暂停播放的目的。
		 */
		_player->Pause(true);
	}
	else
	{
		// 开始播放
		_decoding_thread_has_exited.Reset();
		_decoding_thread_can_start.SetResult();
		_player->Pause(false);
	}
}

void video::AudioPacketPlayer::SendData(AVPacketWrapper &packet)
{
	_packet_queue->SendData(packet);
}
