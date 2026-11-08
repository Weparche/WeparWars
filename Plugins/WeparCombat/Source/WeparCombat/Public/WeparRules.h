#pragma once

// Portable, testable C++17 combat math. No Unreal dependencies.
#include <algorithm>
#include <cmath>

namespace WeparRules
{
    constexpr float InfuseCastSeconds = 0.25f;
    constexpr float DefaultRangeCm = 3000.0f;
    constexpr float DefaultInfuseMultiplier = 2.0f; // Prototype balance, NOT original GW1 formula.

    struct FInfuseOutcome
    {
        bool Valid = false;
        float Sacrificed = 0.0f;
        float Healed = 0.0f;
        float CasterAfter = 0.0f;
        float TargetAfter = 0.0f;
    };

    inline bool IsFiniteNonNegative(float Value)
    {
        return std::isfinite(Value) && Value >= 0.0f;
    }

    // Must be called *after* validation of team, target, cast, LoS and range.
    // Health sacrifice is based on caster HP at cast resolution, not cast start.
    inline FInfuseOutcome CalculateInfuse(float CasterHP, float TargetHP,
                                          float TargetMaxHP,
                                          float Multiplier = DefaultInfuseMultiplier)
    {
        FInfuseOutcome R;
        if (!IsFiniteNonNegative(CasterHP) || !IsFiniteNonNegative(TargetHP) ||
            !IsFiniteNonNegative(TargetMaxHP) || !IsFiniteNonNegative(Multiplier) ||
            CasterHP <= 0.0f || TargetMaxHP <= 0.0f ||
            TargetHP > TargetMaxHP)
        {
            return R;
        }

        R.Valid = true;
        R.Sacrificed = CasterHP * 0.5f;
        R.CasterAfter = CasterHP - R.Sacrificed;
        R.TargetAfter = std::min(TargetMaxHP, TargetHP + R.Sacrificed * Multiplier);
        R.Healed = R.TargetAfter - TargetHP;
        return R;
    }

    inline float ApplyDamage(float CurrentHP, float Damage)
    {
        if (!IsFiniteNonNegative(CurrentHP) || !IsFiniteNonNegative(Damage))
        {
            return CurrentHP;
        }
        return std::max(0.0f, CurrentHP - Damage);
    }

    inline bool IsValidTarget(bool CasterAlive, bool TargetAlive, bool SameActor,
                              int CasterTeam, int TargetTeam, bool WantAlly,
                              float DistanceCm, float MaxRangeCm, bool HasLineOfSight)
    {
        if (!CasterAlive || !TargetAlive || SameActor || !HasLineOfSight ||
            CasterTeam < 0 || TargetTeam < 0 ||
            !IsFiniteNonNegative(DistanceCm) ||
            !IsFiniteNonNegative(MaxRangeCm) || DistanceCm > MaxRangeCm)
        {
            return false;
        }
        return (CasterTeam == TargetTeam) == WantAlly;
    }

    // A cast resolves iff it has not been interrupted and the authority tick
    // has reached the cast deadline. Do not use client clock for authority.
    inline bool CanResolveCast(double ServerNow, double CastStart,
                               double DurationSeconds, bool Interrupted)
    {
        return std::isfinite(ServerNow) && std::isfinite(CastStart) &&
               std::isfinite(DurationSeconds) && DurationSeconds >= 0.0 &&
               !Interrupted && ServerNow >= CastStart + DurationSeconds;
    }
}
