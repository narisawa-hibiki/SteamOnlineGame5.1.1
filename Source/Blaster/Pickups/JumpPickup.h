// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "JumpPickup.generated.h"

/**
 * ジャンプピックアップクラス
 * 拾得すると一定時間プレイヤーのジャンプ力を上昇させるアイテム
 * より高い場所に到達できるようになり、戦術的な優位性を提供
 */
UCLASS()
class BLASTER_API AJumpPickup : public APickup
{
	GENERATED_BODY()
protected:
	/// <summary>
	/// ピックアップの球体コリジョンに他のアクターが重なった時に呼ばれる
	/// プレイヤーキャラクターにジャンプバフを適用
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
	/// バフ適用時のジャンプZ方向速度
	/// 値が大きいほど高くジャンプできる
	/// </summary>
	UPROPERTY(EditAnywhere)
	float JumpZVelocityBuff = 4000.f;

	/// <summary>
	/// ジャンプバフの持続時間（秒）
	/// </summary>
	UPROPERTY(EditAnywhere)
	float JumpBuffTime = 30.f;
};
