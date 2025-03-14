#include "Timer.h"
#include <iostream>

video::Timer::Timer()
{
	SDL_Initializer::Initialize();
}

video::Timer::~Timer()
{
	Stop();
}

void video::Timer::StopAsync()
{
	_callback_should_stop = true;
}

void video::Timer::Stop()
{
	_callback_should_stop = true;
	_callback_has_stopped.Wait();
}

uint32_t video::Timer::static_callback(uint32_t interval, void *param)
{
	Timer *self = static_cast<Timer *>(param);
	if (self->_callback_should_stop)
	{
		std::cout << "Timer 停止" << std::endl;
		self->_callback_has_stopped.SetResult();
		return 0;
	}

	uint32_t ret = self->_callback(interval);
	if (ret == 0)
	{
		// _callback 返回值为 0，说明用户想停止定时器。
		std::cout << "Timer 停止" << std::endl;
		self->_callback_has_stopped.SetResult();
		return 0;
	}

	return ret;
}

void video::Timer::Start(std::chrono::milliseconds interval)
{
	if (!_callback_has_stopped.IsCompleted())
	{
		/* _callback_has_stopped 没完成，说明当前有一个回调任务没被停下来，
		 * 即有一个定时器正在运行。
		 */
		std::cout << "请先停止当前定时任务再启动新的" << std::endl;
		return;
	}

	if (!_callback)
	{
		throw std::runtime_error("Timer 的回调函数不能为空");
	}

	// 到这里，条件满足了，可以开启定时器了。
	_callback_should_stop = false;
	_callback_has_stopped.Reset();

	SDL_AddTimer(interval.count(), static_callback, this);
}
