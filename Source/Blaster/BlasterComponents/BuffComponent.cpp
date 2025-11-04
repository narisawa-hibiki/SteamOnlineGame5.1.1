// Fill out your copyright notice in the Description page of Project Settings.

#include "BuffComponent.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UBuffComponent::UBuffComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBuffComponent::Heal(float HealAmount, float HealingTime)
{
	bHealing = true;
	// 1秒あたりの回復量を計算
	HealingRate = HealAmount / HealingTime;
	// 回復量を累積（複数の回復アイテムを同時に使用可能）
	AmountToHeal += HealAmount;
}

void UBuffComponent::ReplenishShield(float ShieldAmount, float ReplenishTime)
{
	bReplenishingShield = true;
	// 1秒あたりのシールド回復量を計算
	ShieldReplenishRate = ShieldAmount / ReplenishTime;
	// 回復量を累積（複数のシールドアイテムを同時に使用可能）
	ShieldReplenishAmount += ShieldAmount;
}

void UBuffComponent::HealRampUp(float DeltaTime)
{
	// 回復中でない、キャラクターが無効、またはエリミネート済みの場合は処理しない
	if (!bHealing || Character == nullptr || Character->IsElimmed()) return;

	// このフレームで回復する量を計算
	const float HealThisFrame = HealingRate * DeltaTime;
	// 体力を更新（最小0、最大MaxHealth）
	Character->SetHealth(FMath::Clamp(Character->GetHealth() + HealThisFrame, 0.f, Character->GetMaxHealth()));
	Character->UpdateHUDHealth();
	// 残りの回復量を減算
	AmountToHeal -= HealThisFrame;

	// 回復が完了したか、体力が満タンになったら回復終了
	if (AmountToHeal <= 0.f || Character->GetHealth() >= Character->GetMaxHealth())
	{
		bHealing = false;
		AmountToHeal = 0.f;
	}
}

void UBuffComponent::ShieldRampUp(float DeltaTime)
{
	// シールド回復中でない、キャラクターが無効、またはエリミネート済みの場合は処理しない
	if (!bReplenishingShield || Character == nullptr || Character->IsElimmed()) return;

	// このフレームで回復するシールド量を計算
	const float ReplenishThisFrame = ShieldReplenishRate * DeltaTime;
	// シールドを更新（最小0、最大MaxShield）
	Character->SetShield(FMath::Clamp(Character->GetShield() + ReplenishThisFrame, 0.f, Character->GetMaxShield()));
	Character->UpdateHUDShield();
	// 残りの回復量を減算
	ShieldReplenishAmount -= ReplenishThisFrame;

	// 回復が完了したか、シールドが満タンになったら回復終了
	if (ShieldReplenishAmount <= 0.f || Character->GetShield() >= Character->GetMaxShield())
	{
		bReplenishingShield = false;
		ShieldReplenishAmount = 0.f;
	}
}

void UBuffComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBuffComponent::SetInitialSpeeds(float BaseSpeed, float CrouchSpeed)
{
	InitialBaseSpeed = BaseSpeed;
	InitialCrouchSpeed = CrouchSpeed;
}

void UBuffComponent::SetInitialJumpVelocity(float Velocity)
{
	InitialJumpVelocity = Velocity;
}

void UBuffComponent::BuffSpeed(float BuffBaseSpeed, float BuffCrouchSpeed, float BuffTime)
{
	if (Character == nullptr) return;

	// バフ終了時に速度をリセットするタイマーを設定
	Character->GetWorldTimerManager().SetTimer(
		SpeedBuffTimer,
		this,
		&UBuffComponent::ResetSpeeds,
		BuffTime
	);

	// 移動速度を更新
	if (Character->GetCharacterMovement())
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = BuffBaseSpeed;
		Character->GetCharacterMovement()->MaxWalkSpeedCrouched = BuffCrouchSpeed;
	}
	// 全クライアントで速度バフを適用
	MulticastSpeedBuff(BuffBaseSpeed, BuffCrouchSpeed);
}

void UBuffComponent::ResetSpeeds()
{
	if (Character == nullptr || Character->GetCharacterMovement() == nullptr) return;

	// 移動速度を初期値に戻す
	Character->GetCharacterMovement()->MaxWalkSpeed = InitialBaseSpeed;
	Character->GetCharacterMovement()->MaxWalkSpeedCrouched = InitialCrouchSpeed;
	// 全クライアントで速度をリセット
	MulticastSpeedBuff(InitialBaseSpeed, InitialCrouchSpeed);
}

void UBuffComponent::MulticastSpeedBuff_Implementation(float BaseSpeed, float CrouchSpeed)
{
	if (Character && Character->GetCharacterMovement())
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = BaseSpeed;
		Character->GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;
	}
}

void UBuffComponent::BuffJump(float BuffJumpVelocity, float BuffTime)
{
	if (Character == nullptr) return;

	// バフ終了時にジャンプ力をリセットするタイマーを設定
	Character->GetWorldTimerManager().SetTimer(
		JumpBuffTimer,
		this,
		&UBuffComponent::ResetJump,
		BuffTime
	);

	// ジャンプ力を更新
	if (Character->GetCharacterMovement())
	{
		Character->GetCharacterMovement()->JumpZVelocity = BuffJumpVelocity;
	}
	// 全クライアントでジャンプバフを適用
	MulticastJumpBuff(BuffJumpVelocity);
}

void UBuffComponent::MulticastJumpBuff_Implementation(float JumpVelocity)
{
	if (Character && Character->GetCharacterMovement())
	{
		Character->GetCharacterMovement()->JumpZVelocity = JumpVelocity;
	}
}

void UBuffComponent::ResetJump()
{
	if (Character->GetCharacterMovement())
	{
		// ジャンプ力を初期値に戻す
		Character->GetCharacterMovement()->JumpZVelocity = InitialJumpVelocity;
	}
	// 全クライアントでジャンプ力をリセット
	MulticastJumpBuff(InitialJumpVelocity);
}

void UBuffComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 毎フレーム体力とシールドの回復処理を実行
	HealRampUp(DeltaTime);
	ShieldRampUp(DeltaTime);
}

