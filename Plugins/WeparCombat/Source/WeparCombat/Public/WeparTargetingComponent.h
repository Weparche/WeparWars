#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeparTargetingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeparTargetChanged, AActor*, NewTarget);

// Attach to an owned Lyra Pawn. Server validates a requested target.
// Team / skill / LoS validation also happens at ability resolution.
UCLASS(ClassGroup=(Wepar), meta=(BlueprintSpawnableComponent))
class WEPARCOMBAT_API UWeparTargetingComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UWeparTargetingComponent();

    UPROPERTY(BlueprintAssignable, Category="Wepar|Targeting")
    FWeparTargetChanged OnTargetChanged;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Wepar|Targeting",
              meta=(ClampMin="100.0"))
    float MaxSelectionRangeCm = 3000.0f;

    UFUNCTION(BlueprintCallable, Category="Wepar|Targeting")
    void RequestTarget(AActor* NewTarget);

    UFUNCTION(BlueprintPure, Category="Wepar|Targeting")
    AActor* GetSelectedTarget() const { return SelectedTarget; }

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    UPROPERTY(ReplicatedUsing=OnRep_SelectedTarget)
    TObjectPtr<AActor> SelectedTarget;

    bool IsCandidateValid(AActor* Candidate) const;
    void SetTargetInternal(AActor* NewTarget);

    UFUNCTION()
    void OnRep_SelectedTarget();

    UFUNCTION(Server, Reliable)
    void ServerRequestTarget(AActor* NewTarget);

    // Confirm even if server target was unchanged (avoids stale client prediction).
    UFUNCTION(Client, Reliable)
    void ClientConfirmTarget(AActor* AuthoritativeTarget);
};
