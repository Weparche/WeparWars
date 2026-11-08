#include "WeparTargetingComponent.h"
#include "GameFramework/Pawn.h"
#include "Net/UnrealNetwork.h"

UWeparTargetingComponent::UWeparTargetingComponent()
{
    SetIsReplicatedByDefault(true);
}

bool UWeparTargetingComponent::IsCandidateValid(AActor* Candidate) const
{
    const AActor* OwnerActor = GetOwner();
    if (!OwnerActor)
    {
        return false;
    }
    if (!Candidate)
    {
        return true; // Clear selection.
    }
    return Candidate != OwnerActor &&
           IsValid(Candidate) &&
           Candidate->IsA<APawn>() &&
           !Candidate->IsActorBeingDestroyed() &&
           FVector::DistSquared(OwnerActor->GetActorLocation(),
                                Candidate->GetActorLocation()) <=
                 FMath::Square(MaxSelectionRangeCm);
}

void UWeparTargetingComponent::SetTargetInternal(AActor* NewTarget)
{
    if (SelectedTarget != NewTarget)
    {
        SelectedTarget = NewTarget;
        OnTargetChanged.Broadcast(SelectedTarget);
        if (AActor* OwnerActor = GetOwner())
        {
            if (OwnerActor->HasAuthority())
            {
                OwnerActor->ForceNetUpdate();
            }
        }
    }
}

void UWeparTargetingComponent::RequestTarget(AActor* NewTarget)
{
    if (!IsCandidateValid(NewTarget))
    {
        return;
    }

    if (GetOwner() && GetOwner()->HasAuthority())
    {
        SetTargetInternal(NewTarget);
    }
    else
    {
        SetTargetInternal(NewTarget); // Only cosmetic local prediction.
        ServerRequestTarget(NewTarget);
    }
}

void UWeparTargetingComponent::ServerRequestTarget_Implementation(AActor* NewTarget)
{
    if (IsCandidateValid(NewTarget))
    {
        SetTargetInternal(NewTarget);
    }
    ClientConfirmTarget(SelectedTarget);
}

void UWeparTargetingComponent::ClientConfirmTarget_Implementation(AActor* AuthoritativeTarget)
{
    SetTargetInternal(AuthoritativeTarget);
}

void UWeparTargetingComponent::OnRep_SelectedTarget()
{
    OnTargetChanged.Broadcast(SelectedTarget);
}

void UWeparTargetingComponent::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME_CONDITION(UWeparTargetingComponent, SelectedTarget, COND_OwnerOnly);
}
