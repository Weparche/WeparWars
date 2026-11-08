#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeparCastStateComponent.generated.h"

USTRUCT(BlueprintType)
struct FWeparCastSnapshot
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category="Wepar")
    bool bActive = false;

    UPROPERTY(BlueprintReadOnly, Category="Wepar")
    FName AbilityId = NAME_None;

    // Server world time, not local actor time.
    UPROPERTY(BlueprintReadOnly, Category="Wepar")
    float StartServerTime = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category="Wepar")
    float Duration = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category="Wepar")
    int32 Sequence = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeparCastChanged,
                                             FWeparCastSnapshot, State);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeparCastResolved,
                                            FName, AbilityId,
                                            bool, bInterrupted);

// Replicated cast-bar state. Only ability logic running on server can start,
// finish or interrupt a cast. It does NOT apply health/damage effects itself.
UCLASS(ClassGroup=(Wepar), meta=(BlueprintSpawnableComponent))
class WEPARCOMBAT_API UWeparCastStateComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UWeparCastStateComponent();

    UPROPERTY(BlueprintAssignable, Category="Wepar|Cast")
    FWeparCastChanged OnCastChanged;

    UPROPERTY(BlueprintAssignable, Category="Wepar|Cast")
    FWeparCastResolved OnCastResolved;

    UFUNCTION(BlueprintPure, Category="Wepar|Cast")
    FWeparCastSnapshot GetCastState() const { return CastState; }

    UFUNCTION(BlueprintPure, Category="Wepar|Cast")
    float GetRemainingSeconds() const;

    // Must be invoked by server-authoritative GAS ability; never by arbitrary client RPC.
    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Wepar|Cast")
    bool BeginCast(FName AbilityId, float DurationSeconds);

    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Wepar|Cast")
    bool InterruptCast();

    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Wepar|Cast")
    void CancelCast();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    UPROPERTY(ReplicatedUsing=OnRep_CastState)
    FWeparCastSnapshot CastState;

    FTimerHandle FinishTimerHandle;

    void EndCast(bool bInterrupted);
    void TimerFinished();
    float GetServerTime() const;

    UFUNCTION()
    void OnRep_CastState();
};
