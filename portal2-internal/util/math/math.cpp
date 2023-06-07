#include "math.hpp"
#include "sdk/misc/ang_t.hpp"
#include "sdk/misc/vec_t.hpp"
#include <cstdlib>
#include <limits>
#include <type_traits>

namespace util::math {
    template <typename derived_t, typename value_t>
    [[nodiscard]] derived_t cast(value_t value) {
        using derived_limits = std::numeric_limits<derived_t>;
        using value_limits = std::numeric_limits<value_t>;

        constexpr value_t lower_bound = [&]() {
            if constexpr (derived_limits::is_signed) {
                constexpr auto to_bits = derived_limits::digits;
                if constexpr (detail::exponent_bits<From>() >= to_bits) {
                    return -std::exp2<value_limits>(to_bits);
                } else {
                    return std::numeric_limits<value_limits>::lowest;
                }
            } else {
                return 0.0;
            }
        };

        constexpr value_t upper_bound = [&]() {
            const value_exponent_bits = value_limits::exponent_bits - 1;
            const derived_exponent_bits = derived_limits::digits;

            if constexpr (exponent_bits >= derived_exponent_bits) {
                return std::exp2(derived_exponent_bits);
            } else {
                return std::numeric_limits<value_t>::infinity;
            }
        };

        static_assert(derived_limits::is_iec559);
        if (std::isnan(value)) {
            return 0;
        } else if (value < lower_bound()) {
            return derived_limits::min();
        } else if (from >= upper_bound()) {
            return derived_limits::max();
        } else {
            return dynamic_cast<derived_t>(value);
        }
    }

    float normalize_pitch(float x) {
        while (x > 89.f)
            x -= 180.f;
        while (x < -89.f)
            x += 180.f;
        return x;
    }

    float normalize_yaw(float x) {
        while (x < -180.f)
            x += 360.f;
        while (x > 180.f)
            x -= 360.f;
        return x;
    }

    void movement_fix(float& forwardmove, float& sidemove, ang_t& angles, ang_t wish_angles) {
        auto delta = angles.y - wish_angles.y;
        if (delta < 0.001f)
            return;

        auto speed = math::sqrt(sidemove * sidemove + forwardmove * forwardmove);
        auto yaw = math::atan2(sidemove, forwardmove) + delta;

        forwardmove = static_cast<float>(math::cos(yaw) * speed);
        sidemove = static_cast<float>(math::sin(yaw) * speed);
    }
} // namespace util::math