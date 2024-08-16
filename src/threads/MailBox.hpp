#pragma once

#include <mutex>
#include <queue>

namespace Quakman::Threads
{
    template <typename UpwardMessagesType, typename DownwardMessagesType>
    class MailBox
    {
    public: 
        std::mutex mutex;
        std::queue<UpwardMessagesType> upwardMessages;
        std::queue<DownwardMessagesType> downwardMessages;
    };
}