// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "HealthPickup.generated.h"

/**
 * ヘルスピックアップクラス
 * 拾得すると時間をかけてプレイヤーの体力を回復するアイテム
 * 即座に全回復するのではなく、徐々に回復することでゲームバランスを調整
 */
UCLASS()
class BLASTER_API AHealthPickup : public APickup
{
	GENERATED_BODY()
public:
	AHealthPickup();

protected:
	/// <summary>
	/// ピックアップの球体コリジョンに他のアクターが重なった時に呼ばれる
	/// プレイヤーキャラクターに体力回復効果を適用
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
	/// 回復する体力の総量
	/// </summary>
	UPROPERTY(EditAnywhere)
	float HealAmount = 100.f;

	/// <summary>
	/// 体力回復にかかる時間（秒）
	/// この時間をかけて徐々に体力が回復する
	/// </summary>
	UPROPERTY(EditAnywhere)
	float HealingTime = 5.f;
};
