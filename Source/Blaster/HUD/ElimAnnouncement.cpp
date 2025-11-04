// Fill out your copyright notice in the Description page of Project Settings.

#include "ElimAnnouncement.h"
#include "Components/TextBlock.h"

void UElimAnnouncement::SetElimAnnouncementText(FString AttackerName, FString VictimName)
{
	// エリミネーション通知テキストを作成（「攻撃者名 elimmed 犠牲者名!」形式）
	FString ElimAnnouncementText = FString::Printf(TEXT("%s elimmed %s!"), *AttackerName, *VictimName);
	if (AnnouncementText)
	{
		// 作成したテキストをテキストブロックに設定
		AnnouncementText->SetText(FText::FromString(ElimAnnouncementText));
	}
}