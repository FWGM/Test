#include "Enemy/Animation/EnemyAnimInstance.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();
    Owner = Cast<AEnemyBase>(TryGetPawnOwner());
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (Owner == nullptr)
    {
        Owner = Cast<AEnemyBase>(TryGetPawnOwner());
    }

    if (Owner)
    {
        // [Fact] 물리 엔진의 실제 속도를 변수에 반영
        MoveSpeed = Owner->GetVelocity().Size();

        // [Fact] C++에서 관리하는 상태값을 애니메이션 변수로 동기화
        CurrentState = Owner->GetCurrentState();
    }
}
