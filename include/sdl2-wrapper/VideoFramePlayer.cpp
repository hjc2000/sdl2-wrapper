#include "VideoFramePlayer.h"

video::VideoFramePlayer::VideoFramePlayer(int x,
										  int y,
										  IVideoStreamInfoCollection const &infos,
										  std::string window_title,
										  SDL_WindowFlags flags)
{
	_video_stream_infos = infos;

	_displayer = std::shared_ptr<VideoFrameDisplayer>{
		new VideoFrameDisplayer{
			x,
			y,
			_video_stream_infos.Width(),
			_video_stream_infos.Height(),
			_video_stream_infos.PixelFormat(),
			window_title,
			flags,
		},
	};

	_timer._callback = [&](uint32_t interval_in_milliseconds) -> uint32_t
	{
		return SDL_TimerCallbackHandler(interval_in_milliseconds);
	};
}

video::VideoFramePlayer::~VideoFramePlayer()
{
	Dispose();
	std::cout << "~VideoFramePlayer()" << std::endl;
}

void video::VideoFramePlayer::Dispose()
{
	if (_disposed)
	{
		return;
	}

	_disposed = true;

	_frame_queue.Dispose();
	_timer.Stop();
}

std::shared_ptr<video::IRefTimer> video::VideoFramePlayer::RefTimer()
{
	return _ref_timer;
}

uint32_t video::VideoFramePlayer::SDL_TimerCallbackHandler(uint32_t interval_in_milliseconds)
{
	AVFrameWrapper frame;
	try
	{
		frame = _frame_queue.Dequeue();
	}
	catch (std::runtime_error const &e)
	{
		std::cout << CODE_POS_STR << "_frame_queue.Dequeue() 抛出异常。停止 SDL 定时器" << std::endl;
		return 0;
	}

	_displayer->SendData(frame);

	frame.SetTimeBase(_video_stream_infos.TimeBase());
	int64_t video_time = frame.PtsToMilliseconds().count();
	if (!_ref_timer)
	{
		// 不进行音视频同步
		return interval_in_milliseconds;
	}

	// 需要进行音视频同步
	int64_t ref_time = RefTime();
	int64_t e_t = video_time - ref_time;
	std::cout << e_t << std::endl;

	int64_t next_interval = static_cast<int64_t>(_video_stream_infos.FrameInterval() * 1000 + e_t);
	if (next_interval <= 0)
	{
		/*
		 * 如果对 SDL 返回负数，因为 SDL 按无符号处理，所以会导致相当长的定时时间。
		 * 如果对 SDL 返回 0 会导致停止定时器。
		 * 所以返回 1，这是允许的最小值了。
		 */
		next_interval = 1;
	}

	return static_cast<uint32_t>(next_interval);
}

void video::VideoFramePlayer::Pause(bool pause)
{
	/* 这里使用的是原子量，且 _timer 的方法是线程安全的，_video_frame_infos 又不会在另一个线程中
	 * 被修改 frame_interval_in_milliseconds 属性，所以不用竞争 _not_private_methods_lock。
	 */
	if (pause)
	{
		// 暂停播放
		_timer.StopAsync();
		return;
	}

	// 开始播放
	int64_t interval = static_cast<int64_t>(_video_stream_infos.FrameInterval() * 1000);
	std::cout << "开始播放，帧间隔为：" << interval << std::endl;
	_timer.Start(std::chrono::milliseconds{interval});
}

void video::VideoFramePlayer::SendData(AVFrameWrapper &frame)
{
	_frame_queue.Enqueue(frame);
	return;
}

void video::VideoFramePlayer::Flush()
{
	_frame_queue.Flush();
}

void video::VideoFramePlayer::SetRefTimer(std::shared_ptr<IRefTimer> value)
{
	_ref_timer = value;
}

int64_t video::VideoFramePlayer::RefTime()
{
	return _ref_timer->RefTime();
}
