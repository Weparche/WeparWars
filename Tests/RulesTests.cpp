#include "../Plugins/WeparCombat/Source/WeparCombat/Public/WeparRules.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>

static bool Near(float A, float B) { return std::fabs(A - B) < 0.0001f; }

int main()
{
    using namespace WeparRules;

    // Infuse 100 -> 50 caster HP; 2x sacrifice -> 100 heal, capped at max.
    auto R = CalculateInfuse(100.0f, 20.0f, 200.0f);
    assert(R.Valid && Near(R.Sacrificed, 50.0f));
    assert(Near(R.CasterAfter, 50.0f));
    assert(Near(R.TargetAfter, 120.0f) && Near(R.Healed, 100.0f));

    R = CalculateInfuse(100.0f, 190.0f, 200.0f);
    assert(R.Valid && Near(R.TargetAfter, 200.0f) && Near(R.Healed, 10.0f));
    assert(Near(R.CasterAfter, 50.0f)); // Cost unchanged by overheal.

    assert(!CalculateInfuse(0.0f, 20.0f, 200.0f).Valid);
    assert(!CalculateInfuse(100.0f, 201.0f, 200.0f).Valid);
    assert(!CalculateInfuse(100.0f, 20.0f, -1.0f).Valid);
    assert(!CalculateInfuse(100.0f, 20.0f, 200.0f, -2.0f).Valid);
    assert(!CalculateInfuse(std::numeric_limits<float>::quiet_NaN(), 1, 100).Valid);

    assert(Near(ApplyDamage(100.0f, 40.0f), 60.0f));
    assert(Near(ApplyDamage(20.0f, 200.0f), 0.0f));
    assert(Near(ApplyDamage(20.0f, -10.0f), 20.0f));

    assert(IsValidTarget(true, true, false, 0, 0, true, 2000, 3000, true));
    assert(!IsValidTarget(true, true, false, 0, 1, true, 2000, 3000, true));
    assert(IsValidTarget(true, true, false, 0, 1, false, 3000, 3000, true));
    assert(!IsValidTarget(true, true, true, 0, 0, true, 1, 3000, true));
    assert(!IsValidTarget(true, true, false, 0, 0, true, 3001, 3000, true));
    assert(!IsValidTarget(true, true, false, 0, 0, true, 100, 3000, false));
    assert(!IsValidTarget(true, false, false, 0, 0, true, 100, 3000, true));
    assert(!IsValidTarget(true, true, false, -1, 0, false, 100, 3000, true));

    assert(!CanResolveCast(0.249, 0.0, 0.25, false));
    assert(CanResolveCast(0.25, 0.0, 0.25, false));
    assert(!CanResolveCast(0.3, 0.0, 0.25, true));
    assert(!CanResolveCast(0.3, 0.0, -1.0, false));

    std::cout << "WeparWars portable combat rules: PASS\n";
    return 0;
}
