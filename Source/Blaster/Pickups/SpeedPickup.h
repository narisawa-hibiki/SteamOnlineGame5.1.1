// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "SpeedPickup.generated.h"

/**
 * スピードピックアップクラス
 * 拾得すると一定時間プレイヤーの移動速度を上昇させるアイテム
 * 通常移動速度としゃがみ移動速度の両方にバフ効果を適用
 */
UCLASS()
class BLASTER_API ASpeedPickup : public APickup
{
	GENERATED_BODY()
protected:
	/// <summary>
	/// ピックアップの球体コリジョンに他のアクターが重なった時に呼ばれる
	/// プレイヤーキャラクターにスピードバフを適用
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
	/// バフ適用時の通常移動速度
	/// </summary>
	UPROPERTY(EditAnywhere)
	float BaseSpeedBuff = 1600.f;

	/// <summary>
	/// バフ適用時のしゃがみ移動速度
	/// </summary>
	UPROPERTY(EditAnywhere)
	float CrouchSpeedBuff = 850.f;

	/// <summary>
	/// スピードバフの持続時間（秒）
	/// </summary>
	UPROPERTY(EditAnywhere)
	float SpeedBuffTime = 30.f;
};
