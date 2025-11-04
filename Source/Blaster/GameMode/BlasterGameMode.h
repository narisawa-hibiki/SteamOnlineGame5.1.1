// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BlasterGameMode.generated.h"

/**
 * マッチ状態の名前空間
 * カスタムマッチ状態を定義
 */
namespace MatchState
{
	/// <summary>
	/// クールダウン状態
	/// マッチ時間が終了し、勝者を表示してクールダウンタイマーを開始
	/// </summary>
	extern BLASTER_API const FName Cooldown;
}

/**
 * ブラスターゲームモードクラス
 * デスマッチモードの基底クラスとして機能
 * マッチの進行管理（ウォームアップ、プレイ、クールダウン）を担当
 * プレイヤーのエリミネート、リスポーン、ダメージ計算などのゲームロジックを管理
 */
UCLASS()
class BLASTER_API ABlasterGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ABlasterGameMode();
	virtual void Tick(float DeltaTime) override;

	/// <summary>
	/// プレイヤーがエリミネートされた時の処理
	/// スコアを更新し、リスポーンタイマーを設定
	/// </summary>
	/// <param name="ElimmedCharacter">エリミネートされたキャラクター</param>
	/// <param name="VictimController">犠牲者のプレイヤーコントローラー</param>
	/// <param name="AttackerController">攻撃者のプレイヤーコントローラー</param>
	virtual void PlayerEliminated(class ABlasterCharacter* ElimmedCharacter, class ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController);

	/// <summary>
	/// リスポーンをリクエスト
	/// エリミネートされたプレイヤーを再スポーン
	/// </summary>
	/// <param name="ElimmedCharacter">エリミネートされたキャラクター</param>
	/// <param name="ElimmedController">エリミネートされたプレイヤーのコントローラー</param>
	virtual void RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController);

	/// <summary>
	/// プレイヤーがゲームから退出した時の処理
	/// スコアを保存し、プレイヤーをクリーンアップ
	/// </summary>
	/// <param name="PlayerLeaving">退出するプレイヤーのプレイヤーステート</param>
	void PlayerLeftGame(class ABlasterPlayerState* PlayerLeaving);

	/// <summary>
	/// ダメージを計算
	/// チームモードではフレンドリーファイアを無効化するなど、ゲームモードに応じたダメージ調整
	/// </summary>
	/// <param name="Attacker">攻撃者のコントローラー</param>
	/// <param name="Victim">犠牲者のコントローラー</param>
	/// <param name="BaseDamage">基本ダメージ量</param>
	/// <returns>実際に適用されるダメージ</returns>
	virtual float CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage);
	
	/// <summary>
	/// ゲームを再起動
	/// マッチ終了後に新しいマッチを開始
	/// </summary>
	virtual void RestartGame() override;
	
	/// <summary>
	/// ウォームアップ時間（秒）
	/// マッチ開始前の準備時間
	/// </summary>
	UPROPERTY(EditDefaultsOnly)
	float WarmupTime = 10.f;

	/// <summary>
	/// マッチ時間（秒）
	/// ゲームプレイの制限時間
	/// </summary>
	UPROPERTY(EditDefaultsOnly)
	float MatchTime = 120.f;

	/// <summary>
	/// クールダウン時間（秒）
	/// マッチ終了後、勝者を表示して次のマッチに移るまでの時間
	/// </summary>
	UPROPERTY(EditDefaultsOnly)
	float CooldownTime = 10.f;

	/// <summary>
	/// レベル開始時刻
	/// マッチタイマーの基準時刻として使用
	/// </summary>
	float LevelStartingTime = 0.f;

	/// <summary>
	/// チームマッチか
	/// trueの場合はチームモード、falseの場合はデスマッチモード
	/// </summary>
	bool bTeamsMatch = false;

protected:
	virtual void BeginPlay() override;

	/// <summary>
	/// マッチ状態が変更された時に呼ばれる
	/// ウォームアップ、プレイ、クールダウンなどの状態遷移を処理
	/// </summary>
	virtual void OnMatchStateSet() override;

private:
	/// <summary>
	/// カウントダウン時間
	/// 現在のマッチ状態の残り時間
	/// </summary>
	float CountdownTime = 0.f;

public:
	/// <summary>
	/// カウントダウン時間を取得
	/// </summary>
	/// <returns>現在のカウントダウン時間</returns>
	FORCEINLINE float GetCountdownTime() const { return CountdownTime; }
};
