#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WeparRulesBlueprintLibrary.generated.h"

USTRUCT(BlueprintType)
struct FWeparInfusePreview
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category="Wepar")
    bool bValid = false;

    UPROPERTY(BlueprintReadOnly, Category="Wepar")
    float Sacrificed = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category="Wepar")
    float ActualHealing = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category="Wepar")
    float CasterHealthAfter = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category="Wepar")
    float TargetHealthAfter = 0.0f;
};

UCLASS()
class WEPARCOMBAT_API UWeparRulesBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    // Preview only. GAS effects must be applied by the *server* after revalidating.
    UFUNCTION(BlueprintPure, Category="Wepar|Combat")
    static FWeparInfusePreview PreviewInfuse(float CasterHP, float TargetHP,
                                           float TargetMaxHP, float HealMultiplier = 2.0f);

    UFUNCTION(BlueprintPure, Category="Wepar|Combat")
    static bool CanTarget(bool bCasterAlive, bool bTargetAlive, bool bSameActor,
                          int32 CasterTeam, int32 TargetTeam, bool bWantAlly,
                          float DistanceCm, float MaxRangeCm, bool bHasLineOfSight);
};
