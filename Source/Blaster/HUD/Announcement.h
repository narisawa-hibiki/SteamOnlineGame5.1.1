// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Announcement.generated.h"

/**
 * アナウンスメントウィジェットクラス
 * マッチ開始前のウォームアップ時とマッチ終了後のクールダウン時に表示される情報パネル
 * ウォームアップ時のカウントダウン、マッチ開始/終了のアナウンス、勝者情報などを表示
 * 画面中央に大きく表示され、プレイヤーに重要な情報を伝える
 */
UCLASS()
class BLASTER_API UAnnouncement : public UUserWidget
{
	GENERATED_BODY()
public:
	/// <summary>
	/// ウォームアップ時間表示
	/// ブループリントウィジェットから自動的にバインド
	/// マッチ開始までのカウントダウンを表示（例：「00:30」）
	/// クールダウン時は次のマッチ開始までの時間を表示
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* WarmupTime;

	/// <summary>
	/// アナウンスメントテキスト
	/// ブループリントウィジェットから自動的にバインド
	/// 「Waiting for players...」「New Match Starts In:」などのメインメッセージを表示
	/// プレイヤーに現在の状況を伝える大きなテキスト
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	UTextBlock* AnnouncementText;

	/// <summary>
	/// 情報テキスト
	/// ブループリントウィジェットから自動的にバインド
	/// マッチ終了時の勝者情報や追加情報を表示
	/// （例：「Winner: PlayerName」「Red Team Wins」「Players tied for the win」など）
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InfoText;
};
