#pragma once
#include "base.hpp"
#include "sdk/misc/ang_t.hpp"
#include "sdk/misc/vec_t.hpp"


namespace util::math {
    template <typename derived_t, typename value_t>
    derived_t cast(value_t value);
    float normalize_pitch(float x);
    float normalize_yaw(float x);
    void movement_fix(float& forwardmove, float& sidemove, ang_t& angles, ang_t wish_angles);
} // namespace util::math
