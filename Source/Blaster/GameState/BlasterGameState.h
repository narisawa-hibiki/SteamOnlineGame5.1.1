// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BlasterGameState.generated.h"

/**
 * ブラスターゲームステートクラス
 * ゲーム全体の状態を管理し、全クライアントで共有される情報を保持
 * トップスコアプレイヤー、チームスコア、チーム所属プレイヤーなどを管理
 * ネットワーク全体でレプリケートされ、全プレイヤーが同じゲーム状態を共有
 */
UCLASS()
class BLASTER_API ABlasterGameState : public AGameState
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/// <summary>
	/// トップスコアを更新
	/// プレイヤーがスコアを獲得した時に呼ばれ、最高スコアとトップスコアプレイヤーを更新
	/// 複数のプレイヤーが同点トップの場合、全員がTopScoringPlayers配列に追加される
	/// </summary>
	/// <param name="ScoringPlayer">スコアを獲得したプレイヤー</param>
	void UpdateTopScore(class ABlasterPlayerState* ScoringPlayer);

	/// <summary>
	/// トップスコアのプレイヤー配列
	/// 最高スコアを持つプレイヤー（複数の場合は全員）を格納
	/// マッチ終了時の勝者判定やリーダークラウンエフェクトの表示に使用
	/// </summary>
	UPROPERTY(Replicated)
	TArray<ABlasterPlayerState*> TopScoringPlayers;

	/** 
	* チーム関連
	*/

	/// <summary>
	/// 赤チームのスコアを加算
	/// 赤チームがキルを獲得した時に呼ばれる
	/// </summary>
	void RedTeamScores();

	/// <summary>
	/// 青チームのスコアを加算
	/// 青チームがキルを獲得した時に呼ばれる
	/// </summary>
	void BlueTeamScores();

	/// <summary>
	/// 赤チームに所属するプレイヤーの配列
	/// </summary>
	TArray<ABlasterPlayerState*> RedTeam;

	/// <summary>
	/// 青チームに所属するプレイヤーの配列
	/// </summary>
	TArray<ABlasterPlayerState*> BlueTeam;

	/// <summary>
	/// 赤チームの合計スコア
	/// チームモードで赤チームの得点を管理
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_RedTeamScore)
	float RedTeamScore = 0.f;

	/// <summary>
	/// 青チームの合計スコア
	/// チームモードで青チームの得点を管理
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_BlueTeamScore)
	float BlueTeamScore = 0.f;

	/// <summary>
	/// 赤チームスコアがレプリケートされた時に呼ばれる
	/// HUDの赤チームスコア表示を更新
	/// </summary>
	UFUNCTION()
	void OnRep_RedTeamScore();

	/// <summary>
	/// 青チームスコアがレプリケートされた時に呼ばれる
	/// HUDの青チームスコア表示を更新
	/// </summary>
	UFUNCTION()
	void OnRep_BlueTeamScore();

private:
	/// <summary>
	/// 現在のトップスコア
	/// UpdateTopScoreでプレイヤーのスコアと比較し、最高スコアを追跡
	/// </summary>
	float TopScore = 0.f;
};
