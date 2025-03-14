#pragma once
#include <iostream>
#include <sdl2-wrapper/SDL_Initializer.h>

namespace video
{
    /// @brief 包装原生的 SDL_Event
    class SDL_EventResult final
    {
    public:
        /// @brief 获取事件的操作是否成功。如果失败了，_event 字段无效。
        bool _success = false;

        /// @brief _success 为 true 时此字段才有效。获取到的事件被放到这里。
        SDL_Event _event{};
    };

    /// @brief SDL 事件获取器
    /// @note 本类是窗口事件循环的基础，一个窗口一定要有一个事件循环，否则一旦发生窗口事件，
    /// 例如拖动窗口等，程序就会卡死。
    class SDL_EventGetter final
    {
    public:
        SDL_EventGetter();

        /// @brief 查询一次，看是否有事件发生。
        /// @return
        SDL_EventResult PollEvent();

        /// @brief 阻塞，等待事件。
        /// 此操作无法取消，且会无限时长阻塞。除非发生错误，否则不会没拿到事件就返回。
        /// @return
        SDL_EventResult WaitEvent();

        /// @brief 轮询一次事件并检查是否是 SDL_EventType::SDL_QUIT 事件
        /// @return 成功轮询到事件，并且是 SDL_EventType::SDL_QUIT 事件则返回 true，否则返回 false。
        bool PollQuitEvent();

        /// @brief 等待 SDL_EventType::SDL_QUIT 事件的发生。发生后本函数返回，否则永远不会返回。
        /// @note 可以用来阻塞主线程，防止主线程退出后后台线程被杀死。
        /// @note 经过测试，如果是控制台项目，没有创建 SDL 窗口的，按下 Ctrl + C 后，也会收到
        /// SDL_EventType::SDL_QUIT 事件。但是调试模式下，这么做会导致 visual studio 报错。
        /// @note 此操作无法取消，且会无限时长阻塞。
        void WaitQuitEvent();
    };
} // namespace video
