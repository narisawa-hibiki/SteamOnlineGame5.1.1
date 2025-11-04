// Fill out your copyright notice in the Description page of Project Settings.

#include "OverheadWidget.h"
#include "Components/TextBlock.h"

void UOverheadWidget::SetDisplayText(FString TextToDisplay)
{
	if (DisplayText)
	{
		// 指定された文字列をテキストブロックに設定
		DisplayText->SetText(FText::FromString(TextToDisplay));
	}
}

void UOverheadWidget::ShowPlayerNetRole(APawn* InPawn)
{
	// ポーンのリモートロールを取得
	ENetRole RemoteRole = InPawn->GetRemoteRole();
	FString Role;
	// ネットワークロールに応じて表示文字列を決定
	switch (RemoteRole)
	{
	case ENetRole::ROLE_Authority:
		// サーバー権限を持つ（このマシンがサーバー）
		Role = FString("Authority");
		break;
	case ENetRole::ROLE_AutonomousProxy:
		// 自律プロキシ（ローカル制御のクライアント）
		Role = FString("Autonomous Proxy");
		break;
	case ENetRole::ROLE_SimulatedProxy:
		// シミュレートプロキシ（リモート制御のクライアント）
		Role = FString("Simulated Proxy");
		break;
	case ENetRole::ROLE_None:
		// ロールなし
		Role = FString("None");
		break;
	}
	// ロール情報を含む文字列を作成して表示
	FString RemoteRoleString = FString::Printf(TEXT("Remote Role: %s"), *Role);
	SetDisplayText(RemoteRoleString);
}

// 以前のレベル削除時のクリーンアップ処理（現在は使用されていない）
/*void UOverheadWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	RemoveFromParent();
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}*/

void UOverheadWidget::NativeDestruct()
{
	// ウィジェットを親から削除（クリーンアップ）
	RemoveFromParent();
	Super::NativeDestruct();
}
