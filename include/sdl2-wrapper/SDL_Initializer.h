#pragma once
#include <base/string/define.h>
#include <base/Wrapper.h>
#include <cstdint>
#include <exception>
#include <functional>
#include <iostream>
#include <mutex>
#include <string>

extern "C"
{
#include <SDL.h>
}

namespace video
{
    /// @brief 选择初始化哪些模块
    enum class SDL_InitOption
    {
        Timer = SDL_INIT_TIMER,
        Audio = SDL_INIT_AUDIO,
        Video = SDL_INIT_VIDEO,
        Joystick = SDL_INIT_JOYSTICK,
        Haptic = SDL_INIT_HAPTIC,
        GameController = SDL_INIT_GAMECONTROLLER,
        Event = SDL_INIT_EVENTS,
        Sensor = SDL_INIT_SENSOR,
        Noparachute = SDL_INIT_NOPARACHUTE,
        Everything = SDL_INIT_EVERYTHING,
    };

    /// @brief SDL 初始化器。
    /// @note 调用任何 SDL 的函数之前，应该至少调用过一次本类的 Initialize 方法。
    class SDL_Initializer
    {
    private:
        SDL_Initializer() = delete;

    public:
        /// <summary>
        ///		初始化 SDL。此函数幂等且线程安全。
        ///		通过本函数初始化，在进程退出后会自动执行 SDL_Quit 取消所有初始化。
        /// </summary>
        /// <param name="option"></param>
        /// <exception cref="Exception 失败会抛出异常"></exception>
        static void Initialize(SDL_InitOption option = SDL_InitOption::Everything);
    };
} // namespace video
