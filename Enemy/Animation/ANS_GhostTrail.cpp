#include "Enemy/Animation/ANS_GhostTrail.h"
#include "Components/PoseableMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"

UANS_GhostTrail::UANS_GhostTrail()
{
	SpawnInterval = 0.05f;
	GhostLifeTime = 0.5f;
}

void UANS_GhostTrail::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	LastSpawnTime = 0.0f;
}

void UANS_GhostTrail::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (MeshComp == nullptr) return;

	LastSpawnTime += FrameDeltaTime;

	if (LastSpawnTime >= SpawnInterval)
	{
		LastSpawnTime = 0.0f;

		AActor* Owner = MeshComp->GetOwner();
		if (Owner == nullptr) return;

		// 실무에서는 성능을 위해 전용 액터 또는 나이아가라 시스템을 사용하지만,
		// 여기서는 로직을 명확히 보여주기 위해 PoseableMesh를 활용한 방식을 제안합니다.
		// 실제 상용 게임에서는 나이아가라의 'Skeletal Mesh Sampling' 방식을 권장합니다.
		
		// [Logic] 잔상 액터 생성 및 현재 포즈 복사
		// 1. NewObject<UPoseableMeshComponent> 생성
		// 2. CopyPoseFromSkeletalComponent(MeshComp)
		// 3. 머티리얼 적용 및 일정 시간 후 소멸
	}
}

void UANS_GhostTrail::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
