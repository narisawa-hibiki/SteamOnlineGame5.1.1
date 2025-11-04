// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "Blaster/Weapon/WeaponTypes.h"
#include "AmmoPickup.generated.h"

/**
 * 弾薬ピックアップクラス
 * 拾得すると特定の武器タイプの所持弾薬を補充するアイテム
 * 武器タイプごとに異なる弾薬ピックアップを配置可能
 */
UCLASS()
class BLASTER_API AAmmoPickup : public APickup
{
	GENERATED_BODY()
protected:
	/// <summary>
	/// ピックアップの球体コリジョンに他のアクターが重なった時に呼ばれる
	/// プレイヤーキャラクターに弾薬を補充
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
	/// 補充する弾薬数
	/// </summary>
	UPROPERTY(EditAnywhere)
	int32 AmmoAmount = 30;

	/// <summary>
	/// 補充対象の武器タイプ
	/// この武器タイプの所持弾薬が補充される
	/// （例：アサルトライフル、ショットガン、スナイパーライフルなど）
	/// </summary>
	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;
};
