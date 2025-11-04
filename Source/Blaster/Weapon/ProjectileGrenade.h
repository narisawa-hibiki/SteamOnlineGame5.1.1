// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "ProjectileGrenade.generated.h"

/**
 * プロジェクタイルグレネードクラス
 * グレネードランチャーから発射されるグレネード弾を表現
 * 地面や壁に跳ね返り、時間経過で爆発する
 */
UCLASS()
class BLASTER_API AProjectileGrenade : public AProjectile
{
	GENERATED_BODY()
public:
	AProjectileGrenade();
	virtual void Destroyed() override;

protected:
	virtual void BeginPlay() override;

	/// <summary>
	/// グレネードが跳ね返った時のコールバック
	/// 跳ねる度にサウンドを再生
	/// </summary>
	/// <param name="ImpactResult">衝突結果</param>
	/// <param name="ImpactVelocity">衝突時の速度</param>
	UFUNCTION()
	void OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity);

private:
	/// <summary>
	/// グレネードが地面や壁に跳ね返った時のサウンド
	/// </summary>
	UPROPERTY(EditAnywhere)
	USoundCue* BounceSound;
};
