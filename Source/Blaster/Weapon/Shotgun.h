// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitScanWeapon.h"
#include "Shotgun.generated.h"

/**
 * ショットガンクラス
 * ヒットスキャン武器の派生クラスで、複数のペレット（散弾）を同時に発射
 * 1回の射撃で複数のヒット判定を行い、近距離で高いダメージを与える
 */
UCLASS()
class BLASTER_API AShotgun : public AHitScanWeapon
{
	GENERATED_BODY()
public:
	/// <summary>
	/// ショットガンを発射
	/// 複数のペレットのヒット位置に対してダメージ処理を実行
	/// </summary>
	/// <param name="HitTargets">各ペレットのヒット位置の配列</param>
	virtual void FireShotgun(const TArray<FVector_NetQuantize>& HitTargets);

	/// <summary>
	/// スキャッター（散乱）を適用したショットガンのトレース終点を計算
	/// 複数のペレットの軌道を計算して配列に格納
	/// </summary>
	/// <param name="HitTarget">元のヒット目標位置</param>
	/// <param name="HitTargets">計算された各ペレットのヒット位置を格納する配列（出力）</param>
	void ShotgunTraceEndWithScatter(const FVector& HitTarget, TArray<FVector_NetQuantize>& HitTargets);

private:
	/// <summary>
	/// 1回の射撃で発射されるペレット（散弾）の数
	/// ショットガンの威力と範囲に影響する
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	uint32 NumberOfPellets = 10;
};
