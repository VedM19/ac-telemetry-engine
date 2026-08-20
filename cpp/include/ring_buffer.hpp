#pragma once

#include <array>
#include <atomic>
#include <cstddef>

/**
 * Header-only learning scaffold for a Single-Producer Single-Consumer ring
 * buffer.
 *
 * This file intentionally does not implement the atomic synchronization,
 * full/empty detection, or index wrapping math. The goal is to leave the
 * interesting parts visible for study while giving you a compilable shape to
 * fill in.
 */
template <typename T, std::size_t Capacity>
class SPSCQueue
{
    static_assert(Capacity > 0, "SPSCQueue capacity must be greater than zero.");
    static_assert((Capacity & (Capacity - 1)) == 0,
                  "SPSCQueue capacity must be a power of two.");

public:
    /**
     * Construct an empty SPSC queue.
     *
     * Synchronization notes:
     * - Construction happens before the producer and consumer threads are
     *   started, so no inter-thread synchronization should be necessary here.
     * - The head and tail indices should begin at the same value so the queue
     *   represents an empty range.
     *
     * Memory-ordering topics to evaluate:
     * - Whether ordinary construction of std::atomic<std::size_t> is enough
     *   before publishing the queue to other threads.
     * - Why memory_order_relaxed may be enough for later single-owner index
     *   updates, but not necessarily enough when observing the other thread's
     *   index.
     */
    SPSCQueue() = default;

    /**
     * Push one item into the queue from the producer thread.
     *
     * Synchronization notes:
     * - Only the producer thread should call push().
     * - The producer owns advancing head_, but must observe tail_ to avoid
     *   overwriting unread consumer data.
     * - The item should become visible in storage_ before the producer
     *   publishes the new head_ value.
     *
     * Memory-ordering topics to evaluate:
     * - memory_order_relaxed for reading the producer-owned head_.
     * - memory_order_acquire when reading the consumer-owned tail_, if the
     *   producer needs to observe the consumer's released tail_ update.
     * - memory_order_release when publishing the new head_ so the consumer
     *   sees the stored item after observing that head_ value.
     *
     * Power-of-two indexing:
     * - Because Capacity is a power of two, a monotonically increasing index can
     *   be mapped into storage_ with a mask such as index & (Capacity - 1).
     * - This is the bitwise equivalent of modulo for powers of two, and avoids
     *   the cost and semantics of the % operator in the hot path.
     *
     * Full-condition topic:
     * - Decide how to compare head_ and tail_ so push() returns false when the
     *   next write would collide with unread data.
     */
    bool push(const T& item)
    {
        (void)item;
        // TODO: Implement this step
        return false;
    }

    /**
     * Pop one item from the queue on the consumer thread.
     *
     * Synchronization notes:
     * - Only the consumer thread should call pop().
     * - The consumer owns advancing tail_, but must observe head_ to avoid
     *   reading unwritten producer data.
     * - The consumer should read storage_ only after it has observed a producer
     *   head_ value that proves an item is available.
     *
     * Memory-ordering topics to evaluate:
     * - memory_order_relaxed for reading the consumer-owned tail_.
     * - memory_order_acquire when reading the producer-owned head_, so the item
     *   written before the producer's release-store is visible.
     * - memory_order_release when publishing the new tail_ so the producer can
     *   safely reuse that slot after observing the updated tail_.
     *
     * Power-of-two indexing:
     * - Use the same Capacity - 1 mask to translate the logical read index into
     *   a physical slot.
     * - Keeping logical indices monotonic can make size and full/empty
     *   reasoning clearer than wrapping the stored counters themselves.
     *
     * Empty-condition topic:
     * - Decide how to compare head_ and tail_ so pop() returns false when there
     *   is no committed producer data to read.
     */
    bool pop(T& item)
    {
        (void)item;
        // TODO: Implement this step
        return false;
    }

    /**
     * Report whether the queue is empty.
     *
     * Synchronization notes:
     * - This observes both producer-owned and consumer-owned state.
     * - In a strict SPSC design, decide whether empty() is only advisory or is
     *   meant to be called by either thread with meaningful synchronization.
     *
     * Memory-ordering topics to evaluate:
     * - memory_order_acquire when observing the producer's head_ from the
     *   consumer side.
     * - memory_order_relaxed for owner-local reads where no cross-thread data
     *   visibility is required.
     * - Whether an externally called observer needs stronger guarantees or
     *   should be documented as approximate under concurrent mutation.
     *
     * Empty-condition topic:
     * - Decide which relationship between head_ and tail_ represents no
     *   readable elements.
     */
    bool empty() const
    {
        // TODO: Implement this step
        return true;
    }

    /**
     * Report whether the queue is full.
     *
     * Synchronization notes:
     * - This must account for producer progress and consumer progress without a
     *   mutex.
     * - The producer usually needs this check before writing storage_, while an
     *   external observer may only receive an advisory answer.
     *
     * Memory-ordering topics to evaluate:
     * - memory_order_acquire when observing the consumer's tail_ from the
     *   producer side.
     * - memory_order_relaxed for producer-owned head_ reads.
     * - Whether full() should share helper logic with push() after you have
     *   decided the exact invariant.
     *
     * Full-condition topic:
     * - Decide whether the queue stores all Capacity elements by using
     *   monotonic counters, or reserves one slot to distinguish full from empty.
     */
    bool full() const
    {
        // TODO: Implement this step
        return false;
    }

    /**
     * Return the number of items currently believed to be in the queue.
     *
     * Synchronization notes:
     * - A lock-free size() can be inherently transient because head_ and tail_
     *   may change while size() is reading them.
     * - Decide whether this method is for diagnostics only, or whether it must
     *   participate in correctness-sensitive control flow.
     *
     * Memory-ordering topics to evaluate:
     * - memory_order_acquire for observing the other thread's published index.
     * - memory_order_relaxed for owner-local or approximate diagnostic reads.
     * - Whether taking both snapshots with acquire ordering improves the
     *   meaning of the result enough to justify the cost.
     *
     * Power-of-two indexing:
     * - If head_ and tail_ are monotonic counters, size can be reasoned about as
     *   a distance between logical indices while slot selection still uses a
     *   Capacity - 1 mask.
     */
    std::size_t size() const
    {
        // TODO: Implement this step
        return 0;
    }

private:
    /**
     * Cache-line size used to isolate hot producer and consumer state.
     *
     * False-sharing notes:
     * - Many mainstream CPUs use 64-byte cache lines.
     * - If head_ and tail_ sit on the same cache line, the producer's writes to
     *   head_ can invalidate the consumer's cache line for tail_, and vice
     *   versa, even though they are logically separate variables.
     * - Aligning each frequently written atomic to its own cache line reduces
     *   coherence traffic in a high-frequency telemetry path.
     */
    static constexpr std::size_t kCacheLineSize = 64;

    /**
     * Mask value to use when translating logical indices into storage slots.
     *
     * Power-of-two indexing:
     * - Capacity is statically verified to be a power of two.
     * - After you implement indexing, a logical index can map to an array slot
     *   with index & kIndexMask instead of index % Capacity.
     * - The mask is valid because powers of two have a binary form with exactly
     *   one bit set, so Capacity - 1 has all lower slot-selection bits set.
     */
    static constexpr std::size_t kIndexMask = Capacity - 1;

    /**
     * One cache-line-aligned atomic index wrapper.
     *
     * Synchronization notes:
     * - The wrapped atomic should be updated by one owning thread and observed
     *   by the other thread.
     * - The memory order used for each load or store should be selected at the
     *   call site based on whether that operation publishes or consumes data.
     *
     * False-sharing notes:
     * - alignas(64) asks the compiler to place each instance on a separate
     *   cache-line boundary.
     * - The padding member reserves enough space so adjacent queue members are
     *   less likely to occupy the same cache line.
     */
    struct alignas(kCacheLineSize) PaddedAtomicIndex
    {
        /**
         * Atomic logical index.
         *
         * Synchronization notes:
         * - head_.value is the producer-published write position.
         * - tail_.value is the consumer-published read position.
         * - Evaluate relaxed, acquire, and release orderings separately for
         *   owner-local snapshots, cross-thread observation, and publication.
         */
        std::atomic<std::size_t> value{0};

        /**
         * Explicit padding to keep each atomic index isolated.
         *
         * False-sharing notes:
         * - The padding fills the rest of the cache line after the atomic value.
         * - This prevents another hot member from being placed immediately next
         *   to the atomic inside the same cache line.
         */
        std::array<char, kCacheLineSize - sizeof(std::atomic<std::size_t>)> padding{};
    };

    /**
     * Producer-owned logical write index.
     *
     * Synchronization notes:
     * - push() should advance this after writing an item into storage_.
     * - The consumer should acquire-load this when it needs to learn whether a
     *   written item is available.
     * - The producer can often read its own head_ with memory_order_relaxed.
     *
     * False-sharing notes:
     * - This member is cache-line aligned and padded because the producer
     *   updates it frequently.
     */
    PaddedAtomicIndex head_{};

    /**
     * Consumer-owned logical read index.
     *
     * Synchronization notes:
     * - pop() should advance this after reading an item from storage_.
     * - The producer should acquire-load this when it needs to learn whether a
     *   slot has become reusable.
     * - The consumer can often read its own tail_ with memory_order_relaxed.
     *
     * False-sharing notes:
     * - This member is cache-line aligned and padded because the consumer
     *   updates it frequently.
     */
    PaddedAtomicIndex tail_{};

    /**
     * Fixed-size storage for queued items.
     *
     * Synchronization notes:
     * - The producer writes a slot before release-storing a new head_ value.
     * - The consumer acquire-loads head_ before reading a slot that producer
     *   has made available.
     * - Because this is SPSC, no two producers write the same slot and no two
     *   consumers read the same slot, assuming the index protocol is correct.
     *
     * Power-of-two indexing:
     * - Once implemented, logical indices should select slots with the
     *   Capacity - 1 mask described above.
     */
    std::array<T, Capacity> storage_{};
};

