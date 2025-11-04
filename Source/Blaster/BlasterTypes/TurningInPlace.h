#pragma once

/**
 * その場での回転状態を表す列挙型
 * キャラクターが静止時にカメラの向きに合わせて回転する方向を定義
 */
UENUM(BlueprintType)
enum class ETurningInPlace : uint8
{
	/// <summary>
	/// 左に回転中
	/// </summary>
	ETIP_Left UMETA(DisplayName = "Turning Left"),

	/// <summary>
	/// 右に回転中
	/// </summary>
	ETIP_Right UMETA(DisplayName = "Turning Right"),

	/// <summary>
	/// 回転していない
	/// </summary>
	ETIP_NotTurning UMETA(DisplayName = "Not Turning"),

	/// <summary>
	/// 列挙型の最大値（内部使用）
	/// </summary>
	ETIP_MAX UMETA(DisplayName = "DefaultMAX")
};