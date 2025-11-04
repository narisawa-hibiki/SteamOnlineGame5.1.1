#pragma once

/// <summary>
/// トレース（ライントレース）の最大距離
/// 武器の射程距離として使用
/// </summary>
#define TRACE_LENGTH 80000.f

/// <summary>
/// カスタムデプス：紫色
/// アウトライン表示用のステンシル値
/// </summary>
#define CUSTOM_DEPTH_PURPLE 250

/// <summary>
/// カスタムデプス：青色
/// アウトライン表示用のステンシル値（青チーム用）
/// </summary>
#define CUSTOM_DEPTH_BLUE 251

/// <summary>
/// カスタムデプス：タン色
/// アウトライン表示用のステンシル値（赤チーム用）
/// </summary>
#define CUSTOM_DEPTH_TAN 252

/**
 * 武器の種類を表す列挙型
 * ゲーム内で使用可能な全ての武器タイプを定義
 */
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	/// <summary>
	/// アサルトライフル（自動小銃）
	/// </summary>
	EWT_AssaultRifle UMETA(DisplayName = "Assault Rifle"),

	/// <summary>
	/// ロケットランチャー（爆発物投射武器）
	/// </summary>
	EWT_RocketLauncher UMETA(DisplayName = "Rocket Launcher"),

	/// <summary>
	/// ピストル（拳銃）
	/// </summary>
	EWT_Pistol UMETA(DisplayName = "Pistol"),

	/// <summary>
	/// サブマシンガン（短機関銃）
	/// </summary>
	EWT_SubmachineGun UMETA(DisplayName = "Submachine Gun"),

	/// <summary>
	/// ショットガン（散弾銃）
	/// </summary>
	EWT_Shotgun UMETA(DisplayName = "Shotgun"),

	/// <summary>
	/// スナイパーライフル（狙撃銃）
	/// </summary>
	EWT_SniperRifle UMETA(DisplayName = "Sniper Rifle"),

	/// <summary>
	/// グレネードランチャー（擲弾発射器）
	/// </summary>
	EWT_GrenadeLauncher UMETA(DisplayName = "Grenade Launcher"),

	/// <summary>
	/// フラグ（旗）
	/// CTFモード用
	/// </summary>
	EWT_Flag UMETA(DisplayName = "Flag"),

	/// <summary>
	/// 列挙型の最大値（内部使用）
	/// </summary>
	EWT_MAX UMETA(DisplayName = "DefaultMAX")
};