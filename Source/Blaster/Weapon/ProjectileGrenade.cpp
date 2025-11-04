// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileGrenade.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

AProjectileGrenade::AProjectileGrenade()
{
	// グレネードメッシュを作成してルートにアタッチ
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Grenade Mesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// プロジェクタイル移動コンポーネントを作成
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->SetIsReplicated(true);
	// グレネードは地面や壁で跳ね返る
	ProjectileMovementComponent->bShouldBounce = true;
}

void AProjectileGrenade::BeginPlay()
{
	// Projectileの基底クラスではなく、AActorのBeginPlayを呼ぶ
	// これによりProjectileのOnHitバインドをスキップ（グレネードは衝突しても即爆発しない）
	AActor::BeginPlay();

	// トレイルエフェクトをスポーン
	SpawnTrailSystem();
	// 爆発までのタイマーを開始（時間経過で自動的に爆発）
	StartDestroyTimer();

	// 跳ね返りイベントをバインド
	ProjectileMovementComponent->OnProjectileBounce.AddDynamic(this, &AProjectileGrenade::OnBounce);
}

void AProjectileGrenade::OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	// 跳ね返るたびにサウンドを再生
	if (BounceSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			BounceSound,
			GetActorLocation()
		);
	}
}

void AProjectileGrenade::Destroyed()
{
	// グレネードが破棄される時（タイマー終了時）に爆発ダメージを適用
	ExplodeDamage();
	Super::Destroyed();
}