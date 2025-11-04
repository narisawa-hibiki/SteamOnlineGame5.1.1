// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterOverlay.generated.h"

/**
 * キャラクターオーバーレイウィジェットクラス
 * ゲーム中にプレイヤーに表示されるメインHUD要素を管理
 * 体力、シールド、弾薬、スコア、チームスコア、マッチ時間などの情報を表示
 * ブループリントウィジェットから各UI要素を自動的にバインド
 */
UCLASS()
class BLASTER_API UCharacterOverlay : public UUserWidget
{
	GENERATED_BODY()
public:
	/// <summary>
	/// 体力バー
	/// ブループリントウィジェットから自動的にバインド
	/// プレイヤーの現在の体力を視覚的に表示
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

	/// <summary>
	/// 体力テキスト
	/// ブループリントウィジェットから自動的にバインド
	/// 体力を数値で表示（例：「75/100」）
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HealthText;

	/// <summary>
	/// シールドバー
	/// ブループリントウィジェットから自動的にバインド
	/// プレイヤーの現在のシールドを視覚的に表示
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ShieldBar;

	/// <summary>
	/// シールドテキスト
	/// ブループリントウィジェットから自動的にバインド
	/// シールドを数値で表示（例：「50/100」）
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ShieldText;

	/// <summary>
	/// スコア表示
	/// ブループリントウィジェットから自動的にバインド
	/// プレイヤーの現在のスコアを表示（デスマッチモード用）
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreAmount;

	/// <summary>
	/// 赤チームスコア表示
	/// ブループリントウィジェットから自動的にバインド
	/// チームモードで赤チームの合計スコアを表示
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	UTextBlock* RedTeamScore;

	/// <summary>
	/// 青チームスコア表示
	/// ブループリントウィジェットから自動的にバインド
	/// チームモードで青チームの合計スコアを表示
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	UTextBlock* BlueTeamScore;

	/// <summary>
	/// スコア区切りテキスト
	/// ブループリントウィジェットから自動的にバインド
	/// 赤チームと青チームのスコアの間に表示される区切り文字（例：「|」）
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreSpacerText;

	/// <summary>
	/// 敗北数表示
	/// ブループリントウィジェットから自動的にバインド
	/// プレイヤーのデス数を表示
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DefeatsAmount;

	/// <summary>
	/// 武器弾薬数表示
	/// ブループリントウィジェットから自動的にバインド
	/// 現在装備中の武器のマガジン内の弾薬数を表示
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	UTextBlock* WeaponAmmoAmount;

	/// <summary>
	/// 所持弾薬数表示
	/// ブループリントウィジェットから自動的にバインド
	/// リロード用の予備弾薬数を表示
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CarriedAmmoAmount;

	/// <summary>
	/// マッチカウントダウンテキスト
	/// ブループリントウィジェットから自動的にバインド
	/// マッチの残り時間を表示（例：「05:30」）
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MatchCountdownText;

	/// <summary>
	/// グレネード数表示
	/// ブループリントウィジェットから自動的にバインド
	/// プレイヤーが所持しているグレネードの数を表示
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	UTextBlock* GrenadesText;

	/// <summary>
	/// 高Ping警告アイコン
	/// ブループリントウィジェットから自動的にバインド
	/// ネットワーク遅延が高い時に表示される警告アイコン
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	class UImage* HighPingImage;

	/// <summary>
	/// 高Ping警告アニメーション
	/// ブループリントウィジェットから自動的にバインド
	/// Pingが高い時に再生される警告アニメーション（点滅など）
	/// Transientフラグにより、シリアライズ時に保存されない
	/// </summary>
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* HighPingAnimation;
};
