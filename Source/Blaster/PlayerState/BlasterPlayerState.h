// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Blaster/BlasterTypes/Team.h"
#include "BlasterPlayerState.generated.h"

/**
 * ブラスタープレイヤーステートクラス
 * プレイヤーのスコア、敗北数、チーム所属などの状態を管理
 * ネットワーク全体でレプリケートされ、全クライアントで同期される
 */
UCLASS()
class BLASTER_API ABlasterPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	/// <summary>
	/// スコアがレプリケートされた時に呼ばれる
	/// HUDのスコア表示を更新
	/// </summary>
	virtual void OnRep_Score() override;

	/// <summary>
	/// 敗北数がレプリケートされた時に呼ばれる
	/// HUDの敗北数表示を更新
	/// </summary>
	UFUNCTION()
	virtual void OnRep_Defeats();

	/// <summary>
	/// スコアに加算
	/// キル成功時などに呼ばれる
	/// </summary>
	/// <param name="ScoreAmount">加算するスコア</param>
	void AddToScore(float ScoreAmount);

	/// <summary>
	/// 敗北数に加算
	/// エリミネートされた時に呼ばれる
	/// </summary>
	/// <param name="DefeatsAmount">加算する敗北数</param>
	void AddToDefeats(int32 DefeatsAmount);

private:
	/// <summary>
	/// このプレイヤーステートに関連付けられたキャラクター
	/// </summary>
	UPROPERTY()
	class ABlasterCharacter* Character;

	/// <summary>
	/// このプレイヤーステートに関連付けられたプレイヤーコントローラー
	/// </summary>
	UPROPERTY()
	class ABlasterPlayerController* Controller;

	/// <summary>
	/// プレイヤーの敗北数（デス数）
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_Defeats)
	int32 Defeats;

	/// <summary>
	/// プレイヤーの所属チーム
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_Team)
	ETeam Team = ETeam::ET_NoTeam;

	/// <summary>
	/// チームがレプリケートされた時に呼ばれる
	/// キャラクターのチームカラーを更新
	/// </summary>
	UFUNCTION()
	void OnRep_Team();

public:
	/// <summary>
	/// 所属チームを取得
	/// </summary>
	/// <returns>所属チーム</returns>
	FORCEINLINE ETeam GetTeam() const { return Team; }

	/// <summary>
	/// チームを設定
	/// </summary>
	/// <param name="TeamToSet">設定するチーム</param>
	void SetTeam(ETeam TeamToSet);
};
