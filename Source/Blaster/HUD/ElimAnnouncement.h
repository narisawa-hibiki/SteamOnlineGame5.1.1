// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ElimAnnouncement.generated.h"

/**
 * エリミネーション通知ウィジェットクラス
 * プレイヤーがエリミネートされた時の通知を画面に表示
 * 攻撃者と犠牲者の名前を含むキルフィードを提供
 * 画面右側に一時的に表示され、時間経過で自動的に消える
 */
UCLASS()
class BLASTER_API UElimAnnouncement : public UUserWidget
{
	GENERATED_BODY()
public:
	/// <summary>
	/// エリミネーション通知テキストを設定
	/// 攻撃者と犠牲者の名前を使って「AttackerName eliminated VictimName」形式のテキストを表示
	/// </summary>
	/// <param name="AttackerName">攻撃者のプレイヤー名</param>
	/// <param name="VictimName">犠牲者のプレイヤー名</param>
	void SetElimAnnouncementText(FString AttackerName, FString VictimName);

	/// <summary>
	/// 通知ボックス
	/// ブループリントウィジェットから自動的にバインド
	/// テキストやアイコンを水平に配置するコンテナ
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* AnnouncementBox;

	/// <summary>
	/// 通知テキスト
	/// ブループリントウィジェットから自動的にバインド
	/// エリミネーション情報を表示するテキストブロック
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AnnouncementText;
};
