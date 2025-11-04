#pragma once

/**
 * 戦闘状態を表す列挙型
 * キャラクターの現在の戦闘アクション状態を定義し、他のアクションとの競合を防ぐ
 */
UENUM(BlueprintType)
enum class ECombatState : uint8
{
	/// <summary>
	/// 待機中（アクション実行可能）
	/// </summary>
	ECS_Unoccupied UMETA(DisplayName = "Unoccupied"),

	/// <summary>
	/// リロード中
	/// </summary>
	ECS_Reloading UMETA(DisplayName = "Reloading"),

	/// <summary>
	/// グレネード投擲中
	/// </summary>
	ECS_ThrowingGrenade UMETA(DisplayName = "Throwing Grenade"),

	/// <summary>
	/// 武器交換中
	/// </summary>
	ECS_SwappingWeapons UMETA(DisplayName = "Swapping Weapons"),

	/// <summary>
	/// 列挙型の最大値（内部使用）
	/// </summary>
	ECS_MAX UMETA(DisplayName = "DefaultMAX")
};