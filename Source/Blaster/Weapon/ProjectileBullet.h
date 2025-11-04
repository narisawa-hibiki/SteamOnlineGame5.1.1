// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "ProjectileBullet.generated.h"

/**
 * プロジェクタイル弾丸クラス
 * 通常の銃弾を表現するプロジェクタイル
 * ヒットスキャン武器とは異なり、物理演算による弾道を持つ
 * エディタでのプロパティ変更に対応
 */
UCLASS()
class BLASTER_API AProjectileBullet : public AProjectile
{
	GENERATED_BODY()
public:
	AProjectileBullet();

#if WITH_EDITOR
	/// <summary>
	/// エディタでプロパティが変更された時に呼ばれる
	/// InitialSpeedなどのプロパティ変更時に移動コンポーネントを更新
	/// </summary>
	/// <param name="Event">プロパティ変更イベント</param>
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& Event) override;
#endif

protected:
	/// <summary>
	/// 衝突時の処理
	/// 弾丸が何かに当たった時の処理を実行
	/// </summary>
	/// <param name="HitComp">ヒットしたコンポーネント</param>
	/// <param name="OtherActor">衝突相手のアクター</param>
	/// <param name="OtherComp">衝突相手のコンポーネント</param>
	/// <param name="NormalImpulse">法線方向の衝撃</param>
	/// <param name="Hit">ヒット結果</param>
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	
	virtual void BeginPlay() override;
};
