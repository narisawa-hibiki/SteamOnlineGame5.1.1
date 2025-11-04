// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverheadWidget.generated.h"

/**
 * オーバーヘッドウィジェットクラス
 * プレイヤーキャラクターの頭上に表示される情報ウィジェット
 * 主にネットワークロール（サーバー、クライアント、シミュレート）の表示に使用
 * デバッグや開発時にプレイヤーの接続状態を視覚的に確認するために利用
 */
UCLASS()
class BLASTER_API UOverheadWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	/// <summary>
	/// 表示テキスト
	/// ブループリントウィジェットから自動的にバインド
	/// プレイヤーのネットワークロールや名前などの情報を表示
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DisplayText;

	/// <summary>
	/// 表示テキストを設定
	/// 指定された文字列をDisplayTextに表示
	/// </summary>
	/// <param name="TextToDisplay">表示する文字列</param>
	void SetDisplayText(FString TextToDisplay);

	/// <summary>
	/// プレイヤーのネットワークロールを表示
	/// ポーンのネットワークロール（Authority、AutonomousProxy、SimulatedProxy）を
	/// 頭上のテキストとして表示し、デバッグに役立てる
	/// </summary>
	/// <param name="InPawn">表示対象のポーン</param>
	UFUNCTION(BlueprintCallable)
	void ShowPlayerNetRole(APawn* InPawn);

protected:
	/// <summary>
	/// ウィジェットが破棄される時に呼ばれる
	/// クリーンアップ処理を実行
	/// </summary>
	virtual void NativeDestruct() override;
};
