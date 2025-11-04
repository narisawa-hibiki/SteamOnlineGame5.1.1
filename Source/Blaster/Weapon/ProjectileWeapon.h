// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "ProjectileWeapon.generated.h"

/**
 * プロジェクタイル武器クラス
 * 物理演算による弾道を持つ発射体を発射する武器の基底クラス
 * ロケットランチャー、グレネードランチャーなどが該当
 */
UCLASS()
class BLASTER_API AProjectileWeapon : public AWeapon
{
	GENERATED_BODY()
public:
	/// <summary>
	/// プロジェクタイルを発射
	/// マズル位置からヒット目標に向けてプロジェクタイルをスポーン
	/// </summary>
	/// <param name="HitTarget">ヒット目標位置</param>
	virtual void Fire(const FVector& HitTarget) override;
	
private:
	/// <summary>
	/// スポーンするプロジェクタイルのクラス
	/// 通常のプロジェクタイル（サーバーサイド巻き戻しなし）
	/// </summary>
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> ProjectileClass;

	/// <summary>
	/// サーバーサイド巻き戻し用のプロジェクタイルクラス
	/// ラグ補償が有効な場合に使用される
	/// </summary>
	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> ServerSideRewindProjectileClass;
};
