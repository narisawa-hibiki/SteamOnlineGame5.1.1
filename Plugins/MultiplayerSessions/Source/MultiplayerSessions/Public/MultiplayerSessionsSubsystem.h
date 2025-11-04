// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "MultiplayerSessionsSubsystem.generated.h"

/// <summary>
/// セッション作成完了時に呼ばれるカスタムデリゲート
/// </summary>
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnCreateSessionComplete, bool, bWasSuccessful);

/// <summary>
/// セッション検索完了時に呼ばれるカスタムデリゲート
/// </summary>
DECLARE_MULTICAST_DELEGATE_TwoParams(FMultiplayerOnFindSessionsComplete, const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);

/// <summary>
/// セッション参加完了時に呼ばれるカスタムデリゲート
/// </summary>
DECLARE_MULTICAST_DELEGATE_OneParam(FMultiplayerOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type Result);

/// <summary>
/// セッション破棄完了時に呼ばれるカスタムデリゲート
/// </summary>
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnDestroySessionComplete, bool, bWasSuccessful);

/// <summary>
/// セッション開始完了時に呼ばれるカスタムデリゲート
/// </summary>
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnStartSessionComplete, bool, bWasSuccessful);

/**
 * マルチプレイヤーセッションを管理するサブシステム
 * オンラインセッションの作成、検索、参加、破棄、開始の機能
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMultiplayerSessionsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UMultiplayerSessionsSubsystem();

	/// <summary>
	/// セッションを作成
	/// </summary>
	/// <param name="NumPublicConnections">公開接続数</param>
	/// <param name="MatchType">マッチタイプ</param>
	void CreateSession(int32 NumPublicConnections, FString MatchType);

	/// <summary>
	/// セッションを検索
	/// </summary>
	/// <param name="MaxSearchResults">検索結果の最大数</param>
	void FindSessions(int32 MaxSearchResults);

	/// <summary>
	/// セッションに参加
	/// </summary>
	/// <param name="SessionResult">参加するセッションの検索結果</param>
	void JoinSession(const FOnlineSessionSearchResult& SessionResult);

	/// <summary>
	/// セッションを破棄
	/// </summary>
	void DestroySession();

	/// <summary>
	/// セッションを開始
	/// </summary>
	void StartSession();

	/// <summary>
	/// セッション作成完了時のカスタムデリゲート
	/// Menuクラスがコールバックをバインド
	/// </summary>
	FMultiplayerOnCreateSessionComplete MultiplayerOnCreateSessionComplete;

	/// <summary>
	/// セッション検索完了時のカスタムデリゲート
	/// Menuクラスがコールバックをバインド
	/// </summary>
	FMultiplayerOnFindSessionsComplete MultiplayerOnFindSessionsComplete;

	/// <summary>
	/// セッション参加完了時のカスタムデリゲート
	/// Menuクラスがコールバックをバインド
	/// </summary>
	FMultiplayerOnJoinSessionComplete MultiplayerOnJoinSessionComplete;

	/// <summary>
	/// セッション破棄完了時のカスタムデリゲート
	/// Menuクラスがコールバックをバインド
	/// </summary>
	FMultiplayerOnDestroySessionComplete MultiplayerOnDestroySessionComplete;

	/// <summary>
	/// セッション開始完了時のカスタムデリゲート
	/// Menuクラスがコールバックをバインド
	/// </summary>
	FMultiplayerOnStartSessionComplete MultiplayerOnStartSessionComplete;

	/// <summary>
	/// 希望する公開接続数
	/// </summary>
	int32 DesiredNumPublicConnections{};

	/// <summary>
	/// 希望するマッチタイプ
	/// </summary>
	FString DesiredMatchType{};

protected:
	/// <summary>
	/// セッション作成完了時の内部コールバック
	/// </summary>
	/// <param name="SessionName">セッション名</param>
	/// <param name="bWasSuccessful">作成が成功したか</param>
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	/// <summary>
	/// セッション検索完了時の内部コールバック
	/// </summary>
	/// <param name="bWasSuccessful">検索が成功したか</param>
	void OnFindSessionsComplete(bool bWasSuccessful);

	/// <summary>
	/// セッション参加完了時の内部コールバック
	/// </summary>
	/// <param name="SessionName">セッション名</param>
	/// <param name="Result">参加結果</param>
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	/// <summary>
	/// セッション破棄完了時の内部コールバック
	/// </summary>
	/// <param name="SessionName">セッション名</param>
	/// <param name="bWasSuccessful">破棄が成功したか</param>
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	/// <summary>
	/// セッション開始完了時の内部コールバック
	/// </summary>
	/// <param name="SessionName">セッション名</param>
	/// <param name="bWasSuccessful">開始が成功したか</param>
	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);

	/// <summary>
	/// セッションインターフェースが有効かチェック
	/// </summary>
	/// <returns>有効な場合true</returns>
	bool IsValidSessionInterface();

private:
	/// <summary>
	/// オンラインセッションインターフェースへのポインタ
	/// </summary>
	IOnlineSessionPtr SessionInterface;

	/// <summary>
	/// 最後に使用したセッション設定
	/// </summary>
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;

	/// <summary>
	/// 最後に使用したセッション検索設定
	/// </summary>
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;

	/// <summary>
	/// セッション作成完了デリゲート
	/// </summary>
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;

	/// <summary>
	/// セッション作成完了デリゲートハンドル
	/// </summary>
	FDelegateHandle CreateSessionCompleteDelegateHandle;

	/// <summary>
	/// セッション検索完了デリゲート
	/// </summary>
	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;

	/// <summary>
	/// セッション検索完了デリゲートハンドル
	/// </summary>
	FDelegateHandle FindSessionsCompleteDelegateHandle;

	/// <summary>
	/// セッション参加完了デリゲート
	/// </summary>
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;

	/// <summary>
	/// セッション参加完了デリゲートハンドル
	/// </summary>
	FDelegateHandle JoinSessionCompleteDelegateHandle;

	/// <summary>
	/// セッション破棄完了デリゲート
	/// </summary>
	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;

	/// <summary>
	/// セッション破棄完了デリゲートハンドル
	/// </summary>
	FDelegateHandle DestroySessionCompleteDelegateHandle;

	/// <summary>
	/// セッション開始完了デリゲート
	/// </summary>
	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;

	/// <summary>
	/// セッション開始完了デリゲートハンドル
	/// </summary>
	FDelegateHandle StartSessionCompleteDelegateHandle;

	/// <summary>
	/// セッション破棄後に新しいセッションを作成するかのフラグ
	/// </summary>
	bool bCreateSessionOnDestroy{ false };

	/// <summary>
	/// 最後に使用した公開接続数
	/// </summary>
	int32 LastNumPublicConnections;

	/// <summary>
	/// 最後に使用したマッチタイプ
	/// </summary>
	FString LastMatchType;
};
