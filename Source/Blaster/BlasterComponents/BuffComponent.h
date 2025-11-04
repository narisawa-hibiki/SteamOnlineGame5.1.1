// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuffComponent.generated.h"

/**
 * バフコンポーネント
 * ヒール、シールド回復、速度上昇、ジャンプ力上昇などの一時的な強化効果を管理
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API UBuffComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBuffComponent();
	friend class ABlasterCharacter;

	/// <summary>
	/// 時間をかけて体力を回復
	/// </summary>
	/// <param name="HealAmount">回復する体力の総量</param>
	/// <param name="HealingTime">回復にかかる時間</param>
	void Heal(float HealAmount, float HealingTime);

	/// <summary>
	/// 時間をかけてシールドを回復
	/// </summary>
	/// <param name="ShieldAmount">回復するシールドの総量</param>
	/// <param name="ReplenishTime">回復にかかる時間</param>
	void ReplenishShield(float ShieldAmount, float ReplenishTime);

	/// <summary>
	/// 一時的に移動速度を上昇
	/// </summary>
	/// <param name="BuffBaseSpeed">バフ適用時の通常移動速度</param>
	/// <param name="BuffCrouchSpeed">バフ適用時のしゃがみ移動速度</param>
	/// <param name="BuffTime">バフの持続時間</param>
	void BuffSpeed(float BuffBaseSpeed, float BuffCrouchSpeed, float BuffTime);

	/// <summary>
	/// 一時的にジャンプ力を上昇
	/// </summary>
	/// <param name="BuffJumpVelocity">バフ適用時のジャンプ速度</param>
	/// <param name="BuffTime">バフの持続時間</param>
	void BuffJump(float BuffJumpVelocity, float BuffTime);

	/// <summary>
	/// 初期移動速度を設定
	/// </summary>
	/// <param name="BaseSpeed">通常移動速度</param>
	/// <param name="CrouchSpeed">しゃがみ移動速度</param>
	void SetInitialSpeeds(float BaseSpeed, float CrouchSpeed);

	/// <summary>
	/// 初期ジャンプ速度を設定
	/// </summary>
	/// <param name="Velocity">ジャンプ速度</param>
	void SetInitialJumpVelocity(float Velocity);

protected:
	virtual void BeginPlay() override;

	/// <summary>
	/// 時間経過による体力回復処理
	/// </summary>
	/// <param name="DeltaTime">デルタ時間</param>
	void HealRampUp(float DeltaTime);

	/// <summary>
	/// 時間経過によるシールド回復処理
	/// </summary>
	/// <param name="DeltaTime">デルタ時間</param>
	void ShieldRampUp(float DeltaTime);

private:
	/// <summary>
	/// このコンポーネントを所有するキャラクター
	/// </summary>
	UPROPERTY()
	class ABlasterCharacter* Character;

	/// <summary>
	/// 現在回復中か
	/// </summary>
	bool bHealing = false;

	/// <summary>
	/// 1秒あたりの回復量
	/// </summary>
	float HealingRate = 0.f;

	/// <summary>
	/// 残りの回復量
	/// </summary>
	float AmountToHeal = 0.f;

	/// <summary>
	/// 現在シールド回復中か
	/// </summary>
	bool bReplenishingShield = false;

	/// <summary>
	/// 1秒あたりのシールド回復量
	/// </summary>
	float ShieldReplenishRate = 0.f;

	/// <summary>
	/// 残りのシールド回復量
	/// </summary>
	float ShieldReplenishAmount = 0.f;

	/// <summary>
	/// 速度バフのタイマーハンドル
	/// </summary>
	FTimerHandle SpeedBuffTimer;

	/// <summary>
	/// 移動速度を初期値に戻す
	/// </summary>
	void ResetSpeeds();

	/// <summary>
	/// 初期通常移動速度
	/// </summary>
	float InitialBaseSpeed;

	/// <summary>
	/// 初期しゃがみ移動速度
	/// </summary>
	float InitialCrouchSpeed;

	/// <summary>
	/// 全クライアントで速度バフを適用
	/// </summary>
	/// <param name="BaseSpeed">通常移動速度</param>
	/// <param name="CrouchSpeed">しゃがみ移動速度</param>
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSpeedBuff(float BaseSpeed, float CrouchSpeed);

	/// <summary>
	/// ジャンプバフのタイマーハンドル
	/// </summary>
	FTimerHandle JumpBuffTimer;

	/// <summary>
	/// ジャンプ速度を初期値に戻す
	/// </summary>
	void ResetJump();

	/// <summary>
	/// 初期ジャンプ速度
	/// </summary>
	float InitialJumpVelocity;

	/// <summary>
	/// 全クライアントでジャンプバフを適用
	/// </summary>
	/// <param name="JumpVelocity">ジャンプ速度</param>
	UFUNCTION(NetMulticast, Reliable)
	void MulticastJumpBuff(float JumpVelocity);

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
