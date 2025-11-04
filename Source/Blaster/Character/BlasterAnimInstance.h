// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Blaster/BlasterTypes/TurningInPlace.h"
#include "BlasterAnimInstance.generated.h"

/**
 * ブラスターアニメーションインスタンスクラス
 * キャラクターの状態に基づいてアニメーションを更新
 * 移動、武器装備、エイム、その場回転などのアニメーション制御を管理
 */
UCLASS()
class BLASTER_API UBlasterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:
	/// <summary>
	/// このアニメーションインスタンスを所有するブラスターキャラクター
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	class ABlasterCharacter* BlasterCharacter;

	/// <summary>
	/// キャラクターの現在の移動速度
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float Speed;

	/// <summary>
	/// キャラクターが空中にいるか（ジャンプ/落下中）
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	/// <summary>
	/// キャラクターが加速中か
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;

	/// <summary>
	/// 武器を装備しているか
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bWeaponEquipped;

	/// <summary>
	/// 現在装備中の武器
	/// </summary>
	class AWeapon* EquippedWeapon;

	/// <summary>
	/// しゃがんでいるか
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsCrouched;

	/// <summary>
	/// エイム中か
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bAiming;

	/// <summary>
	/// 移動方向とキャラクターの向きの差（ヨー方向）
	/// ストレイフアニメーションに使用
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float YawOffset;

	/// <summary>
	/// キャラクターの傾き（移動時の体の傾斜）
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float Lean;

	/// <summary>
	/// 前フレームのキャラクター回転
	/// </summary>
	FRotator CharacterRotationLastFrame;

	/// <summary>
	/// 現在のキャラクター回転
	/// </summary>
	FRotator CharacterRotation;

	/// <summary>
	/// 前フレームからの回転差分
	/// </summary>
	FRotator DeltaRotation;

	/// <summary>
	/// エイムオフセットのヨー（左右回転）
	/// 上半身の左右回転に使用
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float AO_Yaw;

	/// <summary>
	/// エイムオフセットのピッチ（上下回転）
	/// 上半身の上下回転に使用
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float AO_Pitch;

	/// <summary>
	/// 左手のトランスフォーム
	/// 武器の左手IKに使用
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	FTransform LeftHandTransform;

	/// <summary>
	/// その場での回転状態
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	ETurningInPlace TurningInPlace;

	/// <summary>
	/// 右手の回転
	/// 武器のエイム方向に合わせるために使用
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	FRotator RightHandRotation;

	/// <summary>
	/// ローカルで制御されているか
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bLocallyControlled;

	/// <summary>
	/// ルートボーンを回転させるか
	/// その場回転時に使用
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bRotateRootBone;

	/// <summary>
	/// エリミネート済みか
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bElimmed;

	/// <summary>
	/// FABRIKを使用するか
	/// 左手の武器IKに使用
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bUseFABRIK;

	/// <summary>
	/// エイムオフセットを使用するか
	/// 上半身の回転アニメーションに使用
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bUseAimOffsets;

	/// <summary>
	/// 右手をトランスフォームするか
	/// エイム時の右手の位置調整に使用
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bTransformRightHand;

	/// <summary>
	/// フラグを持っているか
	/// CTFモード用
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bHoldingTheFlag;
};
