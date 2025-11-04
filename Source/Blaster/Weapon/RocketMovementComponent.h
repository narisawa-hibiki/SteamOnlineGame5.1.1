// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "RocketMovementComponent.generated.h"

/**
 * ロケット移動コンポーネント
 * プロジェクタイル移動コンポーネントの派生クラスで、ロケット特有の挙動を実装
 * ブロッキングヒット時の処理をカスタマイズし、ロケットの爆発挙動を制御
 */
UCLASS()
class BLASTER_API URocketMovementComponent : public UProjectileMovementComponent
{
	GENERATED_BODY()
protected:
	/// <summary>
	/// ブロッキングヒットの処理方法を決定
	/// ロケットが壁や地面などに衝突した時の挙動を制御
	/// </summary>
	/// <param name="Hit">ヒット結果</param>
	/// <param name="TimeTick">時間刻み</param>
	/// <param name="MoveDelta">移動デルタ</param>
	/// <param name="SubTickTimeRemaining">残りのサブティック時間</param>
	/// <returns>ブロッキングヒットの処理結果</returns>
	virtual EHandleBlockingHitResult HandleBlockingHit(const FHitResult& Hit, float TimeTick, const FVector& MoveDelta, float& SubTickTimeRemaining) override;
	
	/// <summary>
	/// 衝突時の処理を実行
	/// ロケットの爆発エフェクトやダメージ処理をトリガー
	/// </summary>
	/// <param name="Hit">ヒット結果</param>
	/// <param name="TimeSlice">時間スライス</param>
	/// <param name="MoveDelta">移動デルタ</param>
	virtual void HandleImpact(const FHitResult& Hit, float TimeSlice = 0.f, const FVector& MoveDelta = FVector::ZeroVector) override;
};
