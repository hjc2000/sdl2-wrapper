#include "sdl2-wrapper/AudioFramePlayer.h"
#include <ffmpeg-wrapper/input-format/InputFormat.h>
#include <ffmpeg-wrapper/wrapper/AVCodecContextWrapper.h>
#include <ffmpeg-wrapper/wrapper/AVStreamWrapper.h>

video::AudioFramePlayer::AudioFramePlayer(IAudioStreamInfoCollection const &infos)
{
	_time_base = infos.TimeBase();
	_device = std::shared_ptr<SDL_DefaultAudioDevice>{new SDL_DefaultAudioDevice{}};
	std::cout << _device->ToString() << std::endl;

	_device->_audio_callback = [&](uint8_t *stream, int len)
	{
		AudioCallbackHandler(stream, len);
	};

	_swr_pipe = std::shared_ptr<SwrPipe>{new SwrPipe{*_device}};
	_swr_pipe->ConsumerList().Add(_frame_queue);
}

video::AudioFramePlayer::~AudioFramePlayer()
{
	Dispose();
	std::cout << "~AudioFramePlayer()" << std::endl;
}

void video::AudioFramePlayer::Dispose()
{
	if (_disposed)
	{
		return;
	}

	_disposed = true;

	_frame_queue->Dispose();
	_device->Dispose();
}

void video::AudioFramePlayer::AudioCallbackHandler(uint8_t *stream, int len)
{
	if (_should_pause || _disposed)
	{
		return;
	}

	// 尝试从重采样器中读取帧
	bool result = _frame_queue->ReadData(_swr_out_frame);
	if (!result)
	{
		// 发生了未知错误，或者发生了 eof，即播放器已经被冲洗了。

		/* 冲洗播放器的方法是调用本播放器对象的 send_frame 方法时传入空指针，
		 * 这将冲洗重采样器。
		 */
		Pause(true);
		std::cout << "发生了错误或到达文件尾，AudioFramePlayer 停止播放" << std::endl;
		return;
	}

	// 读取成功
	_swr_out_frame.CopyAudioDataToBuffer(stream, len);
	_time_interpolator.SyncTime(_swr_out_frame.PtsToMilliseconds().count());
}

int64_t video::AudioFramePlayer::RefTime()
{
	return _time_interpolator.InterpolatedTimeInMilliseconds();
}

void video::AudioFramePlayer::Pause(bool pause)
{
	/* 这里没什么要加锁的，设置标志位而已。而且标志位还用了原子量。_device->Pause
	 * 也是线程安全的。
	 */
	if (pause)
	{
		// 暂停播放
		_should_pause = true;
		_device->Pause(true);
		_time_interpolator.Pause();
	}
	else
	{
		// 启动播放（第一次启动或暂停后再次启动）
		_should_pause = false;
		_device->Pause(false);
	}
}

void video::AudioFramePlayer::SendData(AVFrameWrapper &frame)
{
	if (_disposed)
	{
		throw std::runtime_error{"此对象已释放，不能再使用"};
	}

	try
	{
		_swr_pipe->SendData(frame);
	}
	catch (std::exception &e)
	{
		std::cout << CODE_POS_STR << e.what() << std::endl;
	}
}

void video::AudioFramePlayer::Flush()
{
	_swr_pipe->Flush();
}
