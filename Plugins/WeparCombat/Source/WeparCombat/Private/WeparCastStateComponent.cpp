#include "WeparCastStateComponent.h"
#include "Engine/World.h"
#include "GameFramework/GameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"

UWeparCastStateComponent::UWeparCastStateComponent()
{
    SetIsReplicatedByDefault(true);
}

float UWeparCastStateComponent::GetServerTime() const
{
    const UWorld* World = GetWorld();
    if (!World)
    {
        return 0.0f;
    }
    const AGameStateBase* GS = World->GetGameState();
    return GS ? GS->GetServerWorldTimeSeconds() : World->GetTimeSeconds();
}

float UWeparCastStateComponent::GetRemainingSeconds() const
{
    if (!CastState.bActive)
    {
        return 0.0f;
    }
    return FMath::Max(0.0f, (CastState.StartServerTime + CastState.Duration)
                                - GetServerTime());
}

bool UWeparCastStateComponent::BeginCast(FName AbilityId, float DurationSeconds)
{
    const AActor* OwnerActor = GetOwner();
    if (!OwnerActor || !OwnerActor->HasAuthority() || CastState.bActive ||
        AbilityId.IsNone() || !FMath::IsFinite(DurationSeconds) || DurationSeconds <= 0.0f)
    {
        return false;
    }

    ++CastState.Sequence;
    CastState.bActive = true;
    CastState.AbilityId = AbilityId;
    CastState.StartServerTime = GetServerTime();
    CastState.Duration = DurationSeconds;
    OnCastChanged.Broadcast(CastState);
    GetOwner()->ForceNetUpdate();

    GetWorld()->GetTimerManager().SetTimer(
        FinishTimerHandle, this, &UWeparCastStateComponent::TimerFinished,
        DurationSeconds, false);
    return true;
}

void UWeparCastStateComponent::TimerFinished()
{
    EndCast(false);
}

bool UWeparCastStateComponent::InterruptCast()
{
    if (!GetOwner() || !GetOwner()->HasAuthority() || !CastState.bActive)
    {
        return false;
    }
    EndCast(true);
    return true;
}

void UWeparCastStateComponent::CancelCast()
{
    if (GetOwner() && GetOwner()->HasAuthority() && CastState.bActive)
    {
        EndCast(true);
    }
}

void UWeparCastStateComponent::EndCast(bool bInterrupted)
{
    GetWorld()->GetTimerManager().ClearTimer(FinishTimerHandle);
    const FName FinishedAbilityId = CastState.AbilityId;
    CastState.bActive = false;
    OnCastChanged.Broadcast(CastState);
    GetOwner()->ForceNetUpdate();
    OnCastResolved.Broadcast(FinishedAbilityId, bInterrupted);
}

void UWeparCastStateComponent::OnRep_CastState()
{
    OnCastChanged.Broadcast(CastState);
}

void UWeparCastStateComponent::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UWeparCastStateComponent, CastState);
}
