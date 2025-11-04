// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "ShieldPickup.generated.h"

/**
 * シールドピックアップクラス
 * 拾得すると時間をかけてプレイヤーのシールドを回復するアイテム
 * 即座に全回復するのではなく、徐々に回復することでゲームバランスを調整
 */
UCLASS()
class BLASTER_API AShieldPickup : public APickup
{
	GENERATED_BODY()
protected:
	/// <summary>
	/// ピックアップの球体コリジョンに他のアクターが重なった時に呼ばれる
	/// プレイヤーキャラクターにシールド回復効果を適用
	/// </summary>
	/// <param name="OverlappedComponent">重なられたコンポーネント</param>
	/// <param name="OtherActor">重なったアクター</param>
	/// <param name="OtherComp">重なった相手のコンポーネント</param>
	/// <param name="OtherBodyIndex">ボディインデックス</param>
	/// <param name="bFromSweep">スイープによる重なりか</param>
	/// <param name="SweepResult">スイープの結果</param>
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

private:
	/// <summary>
	/// 回復するシールドの総量
	/// </summary>
	UPROPERTY(EditAnywhere)
	float ShieldReplenishAmount = 100.f;

	/// <summary>
	/// シールド回復にかかる時間（秒）
	/// この時間をかけて徐々にシールドが回復する
	/// </summary>
	UPROPERTY(EditAnywhere)
	float ShieldReplenishTime = 5.f;
};
