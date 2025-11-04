// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReturnToMainMenu.generated.h"

/**
 * メインメニューに戻るウィジェットクラス
 * ゲーム中にメインメニューに戻るためのUIを提供
 * セッションの破棄とメインメニューレベルへの遷移を管理
 */
UCLASS()
class BLASTER_API UReturnToMainMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	/// <summary>
	/// メニューをセットアップ
	/// ウィジェットを画面に表示し、入力モードをUI用に設定
	/// </summary>
	void MenuSetup();

	/// <summary>
	/// メニューを破棄
	/// ウィジェットを画面から削除し、入力モードをゲーム用に戻す
	/// </summary>
	void MenuTearDown();

protected:
	/// <summary>
	/// ウィジェット初期化時に呼ばれる
	/// ボタンのバインドなど初期設定を行う
	/// </summary>
	virtual bool Initialize() override;

	/// <summary>
	/// セッション破棄完了時のコールバック
	/// セッションの破棄が成功/失敗した後に呼ばれる
	/// </summary>
	/// <param name="bWasSuccessful">セッション破棄が成功したか</param>
	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);

	/// <summary>
	/// プレイヤーがゲームから退出した時のコールバック
	/// メインメニューレベルへの遷移を実行
	/// </summary>
	UFUNCTION()
	void OnPlayerLeftGame();

private:
	/// <summary>
	/// メインメニューに戻るボタン
	/// ブループリントウィジェットから自動的にバインド
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	class UButton* ReturnButton;

	/// <summary>
	/// 戻るボタンがクリックされた時のコールバック
	/// セッション破棄処理を開始
	/// </summary>
	UFUNCTION()
	void ReturnButtonClicked();

	/// <summary>
	/// マルチプレイヤーセッションサブシステム
	/// セッションの破棄とメインメニューへの遷移を管理
	/// </summary>
	UPROPERTY()
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	/// <summary>
	/// プレイヤーコントローラー
	/// 入力モードの切り替えに使用
	/// </summary>
	UPROPERTY()
	class APlayerController* PlayerController;
};
