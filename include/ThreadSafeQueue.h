#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <cstddef>

template <typename T>
class ThreadSafeQueue {
private:
    std::queue<T> queue_;
    mutable std::mutex mutex_;
    std::condition_variable condVar_;
    std::condition_variable pushCondVar_;
    std::size_t maxCapacity_;
    bool closed_;

public:
    ThreadSafeQueue(std::size_t maxCapacity) : maxCapacity_(maxCapacity), closed_(false) {}

    bool push(T item) {
        std::unique_lock<std::mutex> lock(mutex_);
        pushCondVar_.wait(lock, [this]() { return queue_.size() < maxCapacity_ || closed_; });
        if (closed_) {
            return false;
        }
        queue_.push(std::move(item));
        condVar_.notify_one();
        return true;
    }

    bool waitAndPop(T& outItem) {
        std::unique_lock<std::mutex> lock(mutex_);
        condVar_.wait(lock, [this]() { return !queue_.empty() || closed_; });
        
        if (queue_.empty() && closed_) {
            return false;
        }
        
        outItem = std::move(queue_.front());
        queue_.pop();
        pushCondVar_.notify_one();
        return true;
    }

    bool tryPop(T& outItem) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            return false;
        }
        outItem = std::move(queue_.front());
        queue_.pop();
        pushCondVar_.notify_one();
        return true;
    }

    void close() {
        std::lock_guard<std::mutex> lock(mutex_);
        closed_ = true;
        condVar_.notify_all();
        pushCondVar_.notify_all();
    }

    bool isClosed() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return closed_;
    }

    std::size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }
};
