// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BlasterPlayerController.generated.h"

/// <summary>
/// Pingが高すぎる時に呼ばれるデリゲート
/// サーバーサイド巻き戻しの有効/無効を切り替えるために使用
/// </summary>
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHighPingDelegate, bool, bPingTooHigh);

/**
 * ブラスタープレイヤーコントローラークラス
 * HUD更新、サーバー時刻同期、Ping監視、マッチ状態管理を担当
 * クライアントとサーバー間の時刻差を補正し、ネットワーク遅延を監視
 */
UCLASS()
class BLASTER_API ABlasterPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	/// <summary>
	/// HUDの体力表示を更新
	/// </summary>
	/// <param name="Health">現在の体力</param>
	/// <param name="MaxHealth">最大体力</param>
	void SetHUDHealth(float Health, float MaxHealth);

	/// <summary>
	/// HUDのシールド表示を更新
	/// </summary>
	/// <param name="Shield">現在のシールド</param>
	/// <param name="MaxShield">最大シールド</param>
	void SetHUDShield(float Shield, float MaxShield);

	/// <summary>
	/// HUDのスコア表示を更新
	/// </summary>
	/// <param name="Score">現在のスコア</param>
	void SetHUDScore(float Score);

	/// <summary>
	/// HUDの敗北数表示を更新
	/// </summary>
	/// <param name="Defeats">敗北数</param>
	void SetHUDDefeats(int32 Defeats);

	/// <summary>
	/// HUDの武器弾薬表示を更新
	/// </summary>
	/// <param name="Ammo">マガジン内の弾薬数</param>
	void SetHUDWeaponAmmo(int32 Ammo);

	/// <summary>
	/// HUDの所持弾薬表示を更新
	/// </summary>
	/// <param name="Ammo">所持弾薬数</param>
	void SetHUDCarriedAmmo(int32 Ammo);

	/// <summary>
	/// HUDのマッチカウントダウン表示を更新
	/// </summary>
	/// <param name="CountdownTime">残り時間（秒）</param>
	void SetHUDMatchCountdown(float CountdownTime);

	/// <summary>
	/// HUDのアナウンスメントカウントダウン表示を更新
	/// </summary>
	/// <param name="CountdownTime">残り時間（秒）</param>
	void SetHUDAnnouncementCountdown(float CountdownTime);

	/// <summary>
	/// HUDのグレネード数表示を更新
	/// </summary>
	/// <param name="Grenades">所持グレネード数</param>
	void SetHUDGrenades(int32 Grenades);

	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/// <summary>
	/// チームスコア表示を非表示
	/// </summary>
	void HideTeamScores();

	/// <summary>
	/// チームスコア表示を初期化
	/// </summary>
	void InitTeamScores();

	/// <summary>
	/// HUDの赤チームスコアを更新
	/// </summary>
	/// <param name="RedScore">赤チームのスコア</param>
	void SetHUDRedTeamScore(int32 RedScore);

	/// <summary>
	/// HUDの青チームスコアを更新
	/// </summary>
	/// <param name="BlueScore">青チームのスコア</param>
	void SetHUDBlueTeamScore(int32 BlueScore);

	/// <summary>
	/// サーバー時刻を取得（サーバーワールドクロックと同期）
	/// </summary>
	/// <returns>同期されたサーバー時刻</returns>
	virtual float GetServerTime();

	/// <summary>
	/// プレイヤー接続完了時に呼ばれる
	/// できるだけ早くサーバークロックと同期
	/// </summary>
	virtual void ReceivedPlayer() override;

	/// <summary>
	/// マッチ状態が変更された時に呼ばれる
	/// </summary>
	/// <param name="State">新しいマッチ状態</param>
	/// <param name="bTeamsMatch">チームマッチか</param>
	void OnMatchStateSet(FName State, bool bTeamsMatch = false);

	/// <summary>
	/// マッチ開始処理
	/// </summary>
	/// <param name="bTeamsMatch">チームマッチか</param>
	void HandleMatchHasStarted(bool bTeamsMatch = false);

	/// <summary>
	/// クールダウン処理
	/// </summary>
	void HandleCooldown();

	/// <summary>
	/// サーバーへの片道通信時間
	/// ラグ補償の計算に使用
	/// </summary>
	float SingleTripTime = 0.f;

	/// <summary>
	/// Pingが高すぎる時のデリゲート
	/// </summary>
	FHighPingDelegate HighPingDelegate;

	/// <summary>
	/// エリミネーション通知をブロードキャスト
	/// </summary>
	/// <param name="Attacker">攻撃者のプレイヤーステート</param>
	/// <param name="Victim">犠牲者のプレイヤーステート</param>
	void BroadcastElim(APlayerState* Attacker, APlayerState* Victim);

protected:
	virtual void BeginPlay() override;

	/// <summary>
	/// HUDの時刻表示を更新
	/// </summary>
	void SetHUDTime();

	/// <summary>
	/// HUDとゲームモードの初期化をポーリング
	/// </summary>
	void PollInit();

	virtual void SetupInputComponent() override;

	/// <summary>
	/// サーバーに現在のサーバー時刻をリクエスト
	/// クライアントがリクエストを送った時刻を渡す
	/// </summary>
	/// <param name="TimeOfClientRequest">クライアントのリクエスト時刻</param>
	UFUNCTION(Server, Reliable)
	void ServerRequestServerTime(float TimeOfClientRequest);

	/// <summary>
	/// サーバーの現在時刻をクライアントに報告
	/// ServerRequestServerTimeへの応答
	/// </summary>
	/// <param name="TimeOfClientRequest">クライアントのリクエスト時刻</param>
	/// <param name="TimeServerReceivedClientRequest">サーバーがリクエストを受信した時刻</param>
	UFUNCTION(Client, Reliable)
	void ClientReportServerTime(float TimeOfClientRequest, float TimeServerReceivedClientRequest);

	/// <summary>
	/// クライアントとサーバーの時刻差
	/// </summary>
	float ClientServerDelta = 0.f;

	/// <summary>
	/// 時刻同期の頻度（秒）
	/// </summary>
	UPROPERTY(EditAnywhere, Category = Time)
	float TimeSyncFrequency = 5.f;

	/// <summary>
	/// 時刻同期の経過時間
	/// </summary>
	float TimeSyncRunningTime = 0.f;

	/// <summary>
	/// 時刻同期をチェック
	/// </summary>
	/// <param name="DeltaTime">デルタ時間</param>
	void CheckTimeSync(float DeltaTime);

	/// <summary>
	/// サーバーにマッチ状態の確認をリクエスト
	/// 途中参加時に使用
	/// </summary>
	UFUNCTION(Server, Reliable)
	void ServerCheckMatchState();

	/// <summary>
	/// 途中参加時にマッチ情報をクライアントに通知
	/// </summary>
	/// <param name="StateOfMatch">現在のマッチ状態</param>
	/// <param name="Warmup">ウォームアップ時間</param>
	/// <param name="Match">マッチ時間</param>
	/// <param name="Cooldown">クールダウン時間</param>
	/// <param name="StartingTime">開始時刻</param>
	UFUNCTION(Client, Reliable)
	void ClientJoinMidgame(FName StateOfMatch, float Warmup, float Match, float Cooldown, float StartingTime);

	/// <summary>
	/// 高Ping警告を表示
	/// </summary>
	void HighPingWarning();

	/// <summary>
	/// 高Ping警告を停止
	/// </summary>
	void StopHighPingWarning();

	/// <summary>
	/// Pingをチェック
	/// </summary>
	/// <param name="DeltaTime">デルタ時間</param>
	void CheckPing(float DeltaTime);

	/// <summary>
	/// メインメニューに戻るウィジェットを表示
	/// </summary>
	void ShowReturnToMainMenu();

	/// <summary>
	/// クライアントにエリミネーション通知を送信
	/// </summary>
	/// <param name="Attacker">攻撃者のプレイヤーステート</param>
	/// <param name="Victim">犠牲者のプレイヤーステート</param>
	UFUNCTION(Client, Reliable)
	void ClientElimAnnouncement(APlayerState* Attacker, APlayerState* Victim);

	/// <summary>
	/// チームスコアを表示するか
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_ShowTeamScores)
	bool bShowTeamScores = false;

	/// <summary>
	/// チームスコア表示フラグがレプリケートされた時に呼ばれる
	/// </summary>
	UFUNCTION()
	void OnRep_ShowTeamScores();

	/// <summary>
	/// プレイヤー情報テキストを取得
	/// </summary>
	/// <param name="Players">プレイヤーステートの配列</param>
	/// <returns>情報テキスト</returns>
	FString GetInfoText(const TArray<class ABlasterPlayerState*>& Players);

	/// <summary>
	/// チーム情報テキストを取得
	/// </summary>
	/// <param name="BlasterGameState">ゲームステート</param>
	/// <returns>チーム情報テキスト</returns>
	FString GetTeamsInfoText(class ABlasterGameState* BlasterGameState);

private:
	/// <summary>
	/// ブラスターHUD
	/// </summary>
	UPROPERTY()
	class ABlasterHUD* BlasterHUD;

	/// <summary>
	/// ブラスターゲームモード
	/// </summary>
	UPROPERTY()
	class ABlasterGameMode* BlasterGameMode;

	/// <summary>
	/// メインメニューに戻るウィジェットのクラス
	/// </summary>
	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<class UUserWidget> ReturnToMainMenuWidget;

	/// <summary>
	/// メインメニューに戻るウィジェットのインスタンス
	/// </summary>
	UPROPERTY()
	class UReturnToMainMenu* ReturnToMainMenu;

	/// <summary>
	/// メインメニューウィジェットが開いているか
	/// </summary>
	bool bReturnToMainMenuOpen = false;

	/// <summary>
	/// レベル開始時刻
	/// </summary>
	float LevelStartingTime = 0.f;

	/// <summary>
	/// マッチ時間
	/// </summary>
	float MatchTime = 0.f;

	/// <summary>
	/// ウォームアップ時間
	/// </summary>
	float WarmupTime = 0.f;

	/// <summary>
	/// クールダウン時間
	/// </summary>
	float CooldownTime = 0.f;

	/// <summary>
	/// カウントダウンの整数値
	/// </summary>
	uint32 CountdownInt = 0;

	/// <summary>
	/// 現在のマッチ状態
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
	FName MatchState;

	/// <summary>
	/// マッチ状態がレプリケートされた時に呼ばれる
	/// </summary>
	UFUNCTION()
	void OnRep_MatchState();

	/// <summary>
	/// キャラクターオーバーレイウィジェット
	/// </summary>
	UPROPERTY()
	class UCharacterOverlay* CharacterOverlay;

	// HUD表示値のキャッシュと初期化フラグ

	/// <summary>
	/// HUD表示用の体力値
	/// </summary>
	float HUDHealth;

	/// <summary>
	/// 体力が初期化されたか
	/// </summary>
	bool bInitializeHealth = false;

	/// <summary>
	/// HUD表示用の最大体力値
	/// </summary>
	float HUDMaxHealth;

	/// <summary>
	/// HUD表示用のスコア値
	/// </summary>
	float HUDScore;

	/// <summary>
	/// スコアが初期化されたか
	/// </summary>
	bool bInitializeScore = false;

	/// <summary>
	/// HUD表示用の敗北数
	/// </summary>
	int32 HUDDefeats;

	/// <summary>
	/// 敗北数が初期化されたか
	/// </summary>
	bool bInitializeDefeats = false;

	/// <summary>
	/// HUD表示用のグレネード数
	/// </summary>
	int32 HUDGrenades;

	/// <summary>
	/// グレネード数が初期化されたか
	/// </summary>
	bool bInitializeGrenades = false;

	/// <summary>
	/// HUD表示用のシールド値
	/// </summary>
	float HUDShield;

	/// <summary>
	/// シールドが初期化されたか
	/// </summary>
	bool bInitializeShield = false;

	/// <summary>
	/// HUD表示用の最大シールド値
	/// </summary>
	float HUDMaxShield;

	/// <summary>
	/// HUD表示用の所持弾薬数
	/// </summary>
	float HUDCarriedAmmo;

	/// <summary>
	/// 所持弾薬数が初期化されたか
	/// </summary>
	bool bInitializeCarriedAmmo = false;

	/// <summary>
	/// HUD表示用の武器弾薬数
	/// </summary>
	float HUDWeaponAmmo;

	/// <summary>
	/// 武器弾薬数が初期化されたか
	/// </summary>
	bool bInitializeWeaponAmmo = false;

	/// <summary>
	/// 高Pingの経過時間
	/// </summary>
	float HighPingRunningTime = 0.f;

	/// <summary>
	/// 高Ping警告の表示時間（秒）
	/// </summary>
	UPROPERTY(EditAnywhere)
	float HighPingDuration = 5.f;

	/// <summary>
	/// Pingアニメーションの経過時間
	/// </summary>
	float PingAnimationRunningTime = 0.f;

	/// <summary>
	/// Pingチェックの頻度（秒）
	/// </summary>
	UPROPERTY(EditAnywhere)
	float CheckPingFrequency = 20.f;

	/// <summary>
	/// サーバーにPing状態を報告
	/// </summary>
	/// <param name="bHighPing">Pingが高いか</param>
	UFUNCTION(Server, Reliable)
	void ServerReportPingStatus(bool bHighPing);

	/// <summary>
	/// 高Pingの閾値（ミリ秒）
	/// </summary>
	UPROPERTY(EditAnywhere)
	float HighPingThreshold = 50.f;
};
