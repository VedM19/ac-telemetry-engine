#include "ring_buffer.hpp"

#include <gtest/gtest.h>

#include <cstddef>
#include <thread>

namespace
{


TEST(SPSCQueue, ProducerConsumerSkeleton)
{
    SPSCQueue<int, 1024> queue;
    constexpr std::size_t item_count = 1024;

    std::thread producer([&queue]() {
        /**
         * Producer-side TODO:
         * - Generate a known monotonic sequence.
         * - Call queue.push(value) from this thread only.
         * - Decide whether to busy-spin, yield, or back off when push() reports
         *   that the queue is full.
         */
    });

    std::thread consumer([&queue]() {
        /**
         * Consumer-side TODO:
         * - Call queue.pop(value) from this thread only.
         * - Verify that received values match the producer's sequence.
         * - Decide how the consumer knows when the test is complete without
         *   adding unsafe shared state.
         */
    });

    producer.join();
    consumer.join();

    /**
     * Verification TODO:
     * - Replace this placeholder with order, count, and end-state assertions
     *   after push(), pop(), empty(), full(), and size() are implemented.
     */
    (void)queue;
    (void)item_count;
    SUCCEED();
}

} // namespace

