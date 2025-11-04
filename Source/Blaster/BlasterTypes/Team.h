#pragma once

/**
 * チーム種別を表す列挙型
 * マルチプレイヤーゲームにおけるプレイヤーの所属チームを定義
 */
UENUM(BlueprintType)
enum class ETeam : uint8
{
	/// <summary>
	/// 赤チーム
	/// </summary>
	ET_RedTeam UMETA(DisplayName = "RedTeam"),

	/// <summary>
	/// 青チーム
	/// </summary>
	ET_BlueTeam UMETA(DisplayName = "BlueTeam"),

	/// <summary>
	/// チーム未所属（デスマッチモードなど）
	/// </summary>
	ET_NoTeam UMETA(DisplayName = "NoTeam"),

	/// <summary>
	/// 列挙型の最大値（内部使用）
	/// </summary>
	ET_MAX UMETA(DisplayName = "DefaultMAX")
};