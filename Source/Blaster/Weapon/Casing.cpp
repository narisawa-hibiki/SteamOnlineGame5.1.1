// Fill out your copyright notice in the Description page of Project Settings.

#include "Casing.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

ACasing::ACasing()
{
	PrimaryActorTick.bCanEverTick = false;

	// 薬莢メッシュを作成してルートコンポーネントに設定
	CasingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CasingMesh"));
	SetRootComponent(CasingMesh);
	// カメラとのコリジョンを無視
	CasingMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	// 物理シミュレーションを有効化（薬莢が飛び出して落ちる）
	CasingMesh->SetSimulatePhysics(true);
	CasingMesh->SetEnableGravity(true);
	// 衝突イベントを有効化（地面に落ちた時のサウンド再生のため）
	CasingMesh->SetNotifyRigidBodyCollision(true);
	// デフォルトの排出衝撃力を設定
	ShellEjectionImpulse = 10.f;
}

void ACasing::BeginPlay()
{
	Super::BeginPlay();
	
	// 衝突イベントをバインド
	CasingMesh->OnComponentHit.AddDynamic(this, &ACasing::OnHit);
	// 薬莢を前方に飛ばす（排出シミュレーション）
	CasingMesh->AddImpulse(GetActorForwardVector() * ShellEjectionImpulse);
}

void ACasing::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 薬莢が地面に落ちた時のサウンドを再生
	if (ShellSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ShellSound, GetActorLocation());
	}
	// サウンド再生後に薬莢を破棄（パフォーマンス最適化）
	Destroy();
}

