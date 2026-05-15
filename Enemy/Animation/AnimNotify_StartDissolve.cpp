#include "Enemy/Animation/AnimNotify_StartDissolve.h"
#include "Enemy/EnemyBase.h"

void UAnimNotify_StartDissolve::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    Super::Notify(MeshComp, Animation);

    if (MeshComp == nullptr)
    {
        return;
    }

    // [Fact] Mesh의 Owner를 EnemyBase로 캐스팅하여 소멸 연출 함수를 호출합니다.
    AEnemyBase* Enemy = Cast<AEnemyBase>(MeshComp->GetOwner());
    if (IsValid(Enemy))
    {
        //Enemy->OnStartDissolve();
    }
}