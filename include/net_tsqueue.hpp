#pragma once

#include "net_common.hpp"

namespace olc
{
    namespace net
    {
        template<typename T>
        class tsqueue
        {
        public:
            tsqueue() = default;
            tsqueue(const tsqueue<T> &) = delete;
            virtual ~tsqueue() { clear(); }

            const T &front()
            {
                std::scoped_lock lock(mxQueue);
                return deqQueue.front();
            }

            const T &back()
            {
                std::scoped_lock lock(mxQueue);
                return deqQueue.back();
            }

            void push_back(const T &item)
            {
                std::scoped_lock lock(mxQueue);
                deqQueue.emplace_back(std::move(item));

                std::unique_lock<std::mutex> ul(muxBlocking);
				cvBlocking.notify_one();
            }

            void push_front(const T &item)
            {
                std::scoped_lock lock(mxQueue);
                deqQueue.push_front(std::move(item));

                std::unique_lock<std::mutex> ul(muxBlocking);
				cvBlocking.notify_one();
            }

            bool empty()
            {
                std::scoped_lock lock(mxQueue);
                return deqQueue.empty();
            }

            size_t count()
            {
                std::scoped_lock lock(mxQueue);
                return deqQueue.size();
            }

            void clear()
            {
                std::scoped_lock lock(mxQueue);
                deqQueue.clear();
            }

            T pop_front()
            {
                std::scoped_lock lock(mxQueue);
                auto t = std::move(deqQueue.front());
                deqQueue.pop_front();
                return t;
            }

            T pop_back()
            {
                std::scoped_lock lock(mxQueue);
                auto t = std::move(deqQueue.back());
                deqQueue.pop_back();
                return t;
            }
            void wait()
            {
                while(empty())
                {
                    std::unique_lock<std::mutex> ul(muxBlocking);
					cvBlocking.wait(ul);
                }
            }
        protected:
            std::mutex mxQueue;
            std::deque<T> deqQueue;
            
            std::condition_variable cvBlocking;
			std::mutex muxBlocking;
        };

    }


}