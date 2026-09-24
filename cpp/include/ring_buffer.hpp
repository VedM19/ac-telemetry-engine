#pragma once

#include <array>
#include <atomic>
#include <cstddef>


template <typename T, std::size_t Capacity>
class SPSCQueue
{
    static_assert(Capacity > 0, "SPSCQueue capacity must be greater than zero.");
    static_assert((Capacity & (Capacity - 1)) == 0,
                  "SPSCQueue capacity must be a power of two.");

public:
    SPSCQueue() = default;

    bool push(const T& item)
    {
        (void)item;
        // TODO: Implement this step
        return false;
    }

   
    bool pop(T& item)
    {
        (void)item;
        // TODO: Implement this step
        return false;
    }

    
    bool empty() const
    {
        // TODO: Implement this step
        return true;
    }

   
    bool full() const
    {
        // TODO: Implement this step
        return false;
    }

    
    std::size_t size() const
    {
        // TODO: Implement this step
        return 0;
    }

private:
    
    static constexpr std::size_t kCacheLineSize = 64;
    
    static constexpr std::size_t kIndexMask = Capacity - 1;
    
    struct alignas(kCacheLineSize) PaddedAtomicIndex
    {
        std::atomic<std::size_t> value{0};

        std::array<char, kCacheLineSize - sizeof(std::atomic<std::size_t>)> padding{};
    };

    PaddedAtomicIndex head_{};

   
    PaddedAtomicIndex tail_{};

    
    std::array<T, Capacity> storage_{};
};

