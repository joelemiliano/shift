// Copyright 2008 Dolphin Emulator Project / 2017 Citra Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "core/core_timing_util.h"

#include <cinttypes>
#include <limits>
#include "common/logging/log.h"
#include "common/uint128.h"
#include "core/hardware_properties.h"

namespace Core::Timing {

constexpr u64 MAX_VALUE_TO_MULTIPLY = std::numeric_limits<s64>::max() / Hardware::BASE_CLOCK_RATE;

s64 msToCycles(std::chrono::milliseconds ms) {
    if (static_cast<u64>(ms.count() / 1000) > MAX_VALUE_TO_MULTIPLY) {
        LOG_ERROR(Core_Timing, "Integer overflow, use max value");
        return std::numeric_limits<s64>::max();
    }
    if (static_cast<u64>(ms.count()) > MAX_VALUE_TO_MULTIPLY) {
        LOG_DEBUG(Core_Timing, "Time very big, do rounding");
        return static_cast<s64>(Hardware::BASE_CLOCK_RATE * static_cast<u64>(ms.count() / 1000));
    }
    return static_cast<s64>((Hardware::BASE_CLOCK_RATE * static_cast<u64>(ms.count())) / 1000);
}

s64 usToCycles(std::chrono::microseconds us) {
    if (static_cast<u64>(us.count() / 1000000) > MAX_VALUE_TO_MULTIPLY) {
        LOG_ERROR(Core_Timing, "Integer overflow, use max value");
        return std::numeric_limits<s64>::max();
    }
    if (static_cast<u64>(us.count()) > MAX_VALUE_TO_MULTIPLY) {
        LOG_DEBUG(Core_Timing, "Time very big, do rounding");
        return static_cast<s64>(Hardware::BASE_CLOCK_RATE * static_cast<u64>(us.count() / 1000000));
    }
    return static_cast<s64>((Hardware::BASE_CLOCK_RATE * static_cast<u64>(us.count())) / 1000000);
}

s64 nsToCycles(std::chrono::nanoseconds ns) {
    const u128 temp =
        Common::Multiply64Into128(static_cast<u64>(ns.count()), Hardware::BASE_CLOCK_RATE);
    return static_cast<s64>(Common::Divide128On32(temp, static_cast<u32>(1000000000)).first);
}

u64 msToClockCycles(std::chrono::milliseconds ms) {
    const auto count = static_cast<u64>(ms.count());
    const u128 temp = Common::Multiply64Into128(count, Hardware::CNTFREQ);
    return Common::Divide128On32(temp, 1000).first;
}

u64 usToClockCycles(std::chrono::microseconds us) {
    const auto count = static_cast<u64>(us.count());
    const u128 temp = Common::Multiply64Into128(count, Hardware::CNTFREQ);
    return Common::Divide128On32(temp, 1000000).first;
}

u64 nsToClockCycles(std::chrono::nanoseconds ns) {
    const auto count = static_cast<u64>(ns.count());
    const u128 temp = Common::Multiply64Into128(count, Hardware::CNTFREQ);
    return Common::Divide128On32(temp, 1000000000).first;
}

u64 CpuCyclesToClockCycles(u64 ticks) {
    const u128 temp = Common::Multiply64Into128(ticks, Hardware::CNTFREQ);
    return Common::Divide128On32(temp, static_cast<u32>(Hardware::BASE_CLOCK_RATE)).first;
}

std::chrono::milliseconds CyclesToMs(s64 cycles) {
    const u128 temp = Common::Multiply64Into128(static_cast<u64>(cycles), 1000);
    const u64 ms = Common::Divide128On32(temp, static_cast<u32>(Hardware::BASE_CLOCK_RATE)).first;
    return std::chrono::milliseconds(ms);
}

std::chrono::nanoseconds CyclesToNs(s64 cycles) {
    const u128 temp = Common::Multiply64Into128(static_cast<u64>(cycles), 1000000000);
    const u64 ns = Common::Divide128On32(temp, static_cast<u32>(Hardware::BASE_CLOCK_RATE)).first;
    return std::chrono::nanoseconds(ns);
}

std::chrono::microseconds CyclesToUs(s64 cycles) {
    const u128 temp = Common::Multiply64Into128(static_cast<u64>(cycles), 1000000);
    const u64 us = Common::Divide128On32(temp, static_cast<u32>(Hardware::BASE_CLOCK_RATE)).first;
    return std::chrono::microseconds(us);
}

} // namespace Core::Timing
