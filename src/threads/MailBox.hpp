#pragma once

#include <mutex>
#include <stack>

namespace Quakman::Threads
{
    template <typename UpwardMessagesType, typename DownwardMessagesType>
    class MailBox
    {
    public: 
        std::mutex mutex;
        std::stack<UpwardMessagesType> upwardMessages;
        std::stack<DownwardMessagesType> downwardMessages;
    };
}