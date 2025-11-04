// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlasterGameMode.h"
#include "TeamsGameMode.generated.h"

/**
 * チームゲームモードクラス
 * 赤チームと青チームに分かれて戦うチームデスマッチモードを実装
 * プレイヤーを自動的に2つのチームに振り分け、チーム内でのダメージを無効化
 * チームスコアを管理し、より高いスコアのチームが勝利
 */
UCLASS()
class BLASTER_API ATeamsGameMode : public ABlasterGameMode
{
	GENERATED_BODY()
public:
	ATeamsGameMode();

	/// <summary>
	/// プレイヤーログイン時の処理
	/// 新しいプレイヤーを赤チームまたは青チームに自動的に振り分ける
	/// チーム人数のバランスを取るため、人数が少ないチームに優先的に配属
	/// </summary>
	/// <param name="NewPlayer">ログインしたプレイヤーコントローラー</param>
	virtual void PostLogin(APlayerController* NewPlayer) override;

	/// <summary>
	/// プレイヤーログアウト時の処理
	/// 退出したプレイヤーをチーム配列から削除
	/// </summary>
	/// <param name="Exiting">ログアウトするコントローラー</param>
	virtual void Logout(AController* Exiting) override;

	/// <summary>
	/// ダメージを計算
	/// 同じチーム内でのダメージ（フレンドリーファイア）を無効化
	/// 敵チームへの攻撃のみ有効なダメージを返す
	/// </summary>
	/// <param name="Attacker">攻撃者のコントローラー</param>
	/// <param name="Victim">犠牲者のコントローラー</param>
	/// <param name="BaseDamage">基本ダメージ量</param>
	/// <returns>実際に適用されるダメージ（同じチームの場合は0）</returns>
	virtual float CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage) override;

	/// <summary>
	/// プレイヤーがエリミネートされた時の処理
	/// 攻撃者のチームにスコアを加算し、チームスコアを更新
	/// </summary>
	/// <param name="ElimmedCharacter">エリミネートされたキャラクター</param>
	/// <param name="VictimController">犠牲者のプレイヤーコントローラー</param>
	/// <param name="AttackerController">攻撃者のプレイヤーコントローラー</param>
	virtual void PlayerEliminated(class ABlasterCharacter* ElimmedCharacter, class ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController) override;

protected:
	/// <summary>
	/// マッチ開始時の処理
	/// プレイヤーをチームに振り分け、チームスコア表示を初期化
	/// </summary>
	virtual void HandleMatchHasStarted() override;
};
