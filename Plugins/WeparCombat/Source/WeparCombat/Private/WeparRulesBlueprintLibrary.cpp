#include "WeparRulesBlueprintLibrary.h"
#include "WeparRules.h"

FWeparInfusePreview UWeparRulesBlueprintLibrary::PreviewInfuse(
    float CasterHP, float TargetHP, float TargetMaxHP, float HealMultiplier)
{
    const WeparRules::FInfuseOutcome R =
        WeparRules::CalculateInfuse(CasterHP, TargetHP, TargetMaxHP, HealMultiplier);
    FWeparInfusePreview Output;
    Output.bValid = R.Valid;
    Output.Sacrificed = R.Sacrificed;
    Output.ActualHealing = R.Healed;
    Output.CasterHealthAfter = R.CasterAfter;
    Output.TargetHealthAfter = R.TargetAfter;
    return Output;
}

bool UWeparRulesBlueprintLibrary::CanTarget(
    bool bCasterAlive, bool bTargetAlive, bool bSameActor,
    int32 CasterTeam, int32 TargetTeam, bool bWantAlly,
    float DistanceCm, float MaxRangeCm, bool bHasLineOfSight)
{
    return WeparRules::IsValidTarget(bCasterAlive, bTargetAlive, bSameActor,
                                    CasterTeam, TargetTeam, bWantAlly,
                                    DistanceCm, MaxRangeCm, bHasLineOfSight);
}
