// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "HitScanWeapon.generated.h"

/**
 * ヒットスキャン武器クラス
 * 即着弾型の武器（アサルトライフル、ピストル、スナイパーライフルなど）の基底クラス
 * ライントレースを使用して即座にヒット判定を行い、弾道の物理演算は行わない
 */
UCLASS()
class BLASTER_API AHitScanWeapon : public AWeapon
{
	GENERATED_BODY()
public:
	/// <summary>
	/// ヒットスキャン武器を発射
	/// ライントレースでヒット判定を即座に実行
	/// </summary>
	/// <param name="HitTarget">ヒット目標位置</param>
	virtual void Fire(const FVector& HitTarget) override;

protected:
	/// <summary>
	/// 武器のトレースヒット判定を実行
	/// トレース開始位置からヒット目標までのライントレースを行う
	/// </summary>
	/// <param name="TraceStart">トレース開始位置</param>
	/// <param name="HitTarget">ヒット目標位置</param>
	/// <param name="OutHit">ヒット結果（出力）</param>
	void WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit);

	/// <summary>
	/// 衝突時のパーティクルエフェクト
	/// 弾丸が壁やキャラクターに当たった時の火花や血しぶき
	/// </summary>
	UPROPERTY(EditAnywhere)
	class UParticleSystem* ImpactParticles;

	/// <summary>
	/// 衝突時のサウンド
	/// 弾丸が何かに当たった時の音
	/// </summary>
	UPROPERTY(EditAnywhere)
	USoundCue* HitSound;

private:
	/// <summary>
	/// ビームパーティクル
	/// 銃口から着弾点までの弾道の軌跡を表示
	/// </summary>
	UPROPERTY(EditAnywhere)
	UParticleSystem* BeamParticles;

	/// <summary>
	/// マズルフラッシュパーティクル
	/// 銃口の発砲時の閃光エフェクト
	/// </summary>
	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;

	/// <summary>
	/// 発砲サウンド
	/// 武器を発射した時の音
	/// </summary>
	UPROPERTY(EditAnywhere)
	USoundCue* FireSound;
};
