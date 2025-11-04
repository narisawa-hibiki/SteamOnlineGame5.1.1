// Fill out your copyright notice in the Description page of Project Settings.

#include "RocketMovementComponent.h"

URocketMovementComponent::EHandleBlockingHitResult URocketMovementComponent::HandleBlockingHit(const FHitResult& Hit, float TimeTick, const FVector& MoveDelta, float& SubTickTimeRemaining)
{
	// 基底クラスの処理を実行
	Super::HandleBlockingHit(Hit, TimeTick, MoveDelta, SubTickTimeRemaining);
	// 常に次のサブステップに進む（ロケットは衝突しても停止しない）
	return EHandleBlockingHitResult::AdvanceNextSubstep;
}

void URocketMovementComponent::HandleImpact(const FHitResult& Hit, float TimeSlice, const FVector& MoveDelta)
{
	// ロケットは停止しない
	// CollisionBoxが衝突を検出した時のみ爆発する
	// この関数は意図的に空実装
}