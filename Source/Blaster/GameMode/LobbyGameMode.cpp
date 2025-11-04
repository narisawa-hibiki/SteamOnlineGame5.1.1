// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "MultiplayerSessionsSubsystem.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	// 基底クラスのログイン処理を実行
	Super::PostLogin(NewPlayer);

	// 現在のプレイヤー数を取得
	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();

	// ゲームインスタンスを取得
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		// マルチプレイヤーセッションサブシステムを取得
		UMultiplayerSessionsSubsystem* Subsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		check(Subsystem);

		// プレイヤー数が設定された人数に達した場合
		if (NumberOfPlayers == Subsystem->DesiredNumPublicConnections)
		{
			UWorld* World = GetWorld();
			if (World)
			{
				// シームレストラベルを有効化（プレイヤーを保持したままマップ遷移）
				bUseSeamlessTravel = true;

				// マッチタイプに応じて適切なマップに遷移
				FString MatchType = Subsystem->DesiredMatchType;
				if (MatchType == "FreeForAll")
				{
					// デスマッチモード用のマップに遷移
					World->ServerTravel(FString("/Game/Maps/BlasterMap?listen"));
				}
				else if (MatchType == "Teams")
				{
					// チームデスマッチモード用のマップに遷移
					World->ServerTravel(FString("/Game/Maps/Teams?listen"));
				}
				else if (MatchType == "CaptureTheFlag")
				{
					// キャプチャー・ザ・フラッグモード用のマップに遷移
					World->ServerTravel(FString("/Game/Maps/CaptureTheFlag?listen"));
				}
			}
		}
	}
}