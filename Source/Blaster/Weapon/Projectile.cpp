// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/Blaster.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	// コリジョンボックスを作成してルートコンポーネントに設定
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);
	// コリジョン設定
	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	// 可視性チャンネル、ワールド静的オブジェクト、スケルタルメッシュにのみ反応
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_SkeletalMesh, ECollisionResponse::ECR_Block);
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	// トレーサーパーティクルをスポーン（旧システム）
	if (Tracer)
	{
		TracerComponent = UGameplayStatics::SpawnEmitterAttached(
			Tracer,
			CollisionBox,
			FName(),
			GetActorLocation(),
			GetActorRotation(),
			EAttachLocation::KeepWorldPosition
		);
	}

	// サーバーのみヒットイベントをバインド
	if (HasAuthority())
	{
		CollisionBox->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	}
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 基底クラスでは即座に破棄（派生クラスでオーバーライドして独自の処理を追加）
	Destroy();
}

void AProjectile::SpawnTrailSystem()
{
	// トレイルエフェクトをスポーン（Niagaraシステム）
	if (TrailSystem)
	{
		TrailSystemComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			TrailSystem,
			GetRootComponent(),
			FName(),
			GetActorLocation(),
			GetActorRotation(),
			EAttachLocation::KeepWorldPosition,
			false
		);
	}
}

void AProjectile::ExplodeDamage()
{
	// 発射したポーンを取得
	APawn* FiringPawn = GetInstigator();
	if (FiringPawn && HasAuthority())
	{
		AController* FiringController = FiringPawn->GetController();
		if (FiringController)
		{
			// 範囲ダメージを距離減衰付きで適用
			UGameplayStatics::ApplyRadialDamageWithFalloff(
				this, // ワールドコンテキストオブジェクト
				Damage, // 基本ダメージ（内側半径での最大ダメージ）
				10.f, // 最小ダメージ（外側半径での最小ダメージ）
				GetActorLocation(), // 爆発中心
				DamageInnerRadius, // 内側半径（最大ダメージ範囲）
				DamageOuterRadius, // 外側半径（ダメージが適用される最大範囲）
				1.f, // ダメージ減衰係数
				UDamageType::StaticClass(), // ダメージタイプクラス
				TArray<AActor*>(), // 無視するアクター（空）
				this, // ダメージを与えたアクター
				FiringController // 攻撃者のコントローラー
			);
		}
	}
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::StartDestroyTimer()
{
	// 一定時間後にプロジェクタイルを破棄するタイマーを設定
	GetWorldTimerManager().SetTimer(
		DestroyTimer,
		this,
		&AProjectile::DestroyTimerFinished,
		DestroyTime
	);
}

void AProjectile::DestroyTimerFinished()
{
	// タイマー完了時にプロジェクタイルを破棄
	Destroy();
}

void AProjectile::Destroyed()
{
	Super::Destroyed();

	// 衝突パーティクルを再生
	if (ImpactParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, GetActorTransform());
	}
	// 衝突サウンドを再生
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}
}

