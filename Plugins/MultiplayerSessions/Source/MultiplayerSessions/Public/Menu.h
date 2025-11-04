// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Menu.generated.h"

/**
 * マルチプレイヤーセッションのメニューUI用ウィジェットクラス
 * ホストの作成、セッションの検索・参加機能
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	/// <summary>
	/// メニューのセットアップを行う
	/// </summary>
	/// <param name="NumberOfPublicConnections">公開接続の最大数（デフォルト: 4）</param>
	/// <param name="TypeOfMatch">マッチのタイプ（デフォルト: "FreeForAll"）</param>
	/// <param name="LobbyPath">ロビーマップへのパス（デフォルト: "/Game/ThirdPersonCPP/Maps/Lobby"）</param>
	UFUNCTION(BlueprintCallable)
	void MenuSetup(int32 NumberOfPublicConnections = 4, FString TypeOfMatch = FString(TEXT("FreeForAll")), FString LobbyPath = FString(TEXT("/Game/ThirdPersonCPP/Maps/Lobby")));

protected:
	/// <summary>
	/// ウィジェットの初期化処理
	/// ボタンのバインドなど初期設定を実行
	/// </summary>
	virtual bool Initialize() override;

	/// <summary>
	/// ウィジェットの破棄処理
	/// メニューのクリーンアップを実行
	/// </summary>
	virtual void NativeDestruct() override;

	/// <summary>
	/// セッション作成完了時のコールバック
	/// </summary>
	/// <param name="bWasSuccessful">セッション作成が成功したか</param>
	UFUNCTION()
	void OnCreateSession(bool bWasSuccessful);

	/// <summary>
	/// セッション検索完了時のコールバック
	/// </summary>
	/// <param name="SessionResults">検索されたセッションの配列</param>
	/// <param name="bWasSuccessful">セッション検索が成功したか</param>
	void OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);

	/// <summary>
	/// セッション参加完了時のコールバック
	/// </summary>
	/// <param name="Result">参加結果のタイプ</param>
	void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);

	/// <summary>
	/// セッション破棄完了時のコールバック
	/// </summary>
	/// <param name="bWasSuccessful">セッション破棄が成功したか</param>
	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);

	/// <summary>
	/// セッション開始完了時のコールバック
	/// </summary>
	/// <param name="bWasSuccessful">セッション開始が成功したか</param>
	UFUNCTION()
	void OnStartSession(bool bWasSuccessful);

private:
	/// <summary>
	/// ホストボタン
	/// セッションをホストするためのボタン
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	/// <summary>
	/// 参加ボタン
	/// セッションに参加するためのボタン
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	/// <summary>
	/// ホストボタンクリック時の処理
	/// セッションの作成を開始
	/// </summary>
	UFUNCTION()
	void HostButtonClicked();

	/// <summary>
	/// 参加ボタンクリック時の処理
	/// セッションの検索を開始
	/// </summary>
	UFUNCTION()
	void JoinButtonClicked();

	/// <summary>
	/// メニューの終了処理
	/// メニューを画面から削除し、入力モードを元に戻す
	/// </summary>
	void MenuTearDown();

	/// <summary>
	/// マルチプレイヤーセッションサブシステムへの参照
	/// すべてのオンラインセッション機能を処理するサブシステム
	/// </summary>
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;
	
	/// <summary>
	/// 公開接続数
	/// セッションに参加可能な最大プレイヤー数
	/// </summary>
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 NumPublicConnections{4};

	/// <summary>
	/// マッチタイプ
	/// セッションのゲームモード（例: "FreeForAll", "TeamDeathMatch" など）
	/// </summary>
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString MatchType{TEXT("FreeForAll")};

	/// <summary>
	/// ロビーへのパス
	/// セッション作成後に移動するロビーマップのパス
	/// </summary>
	FString PathToLobby{TEXT("")};
};
