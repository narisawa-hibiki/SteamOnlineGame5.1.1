// Fill out your copyright notice in the Description page of Project Settings.

#include "BlasterGameMode.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Blaster/PlayerState/BlasterPlayerState.h"
#include "Blaster/GameState/BlasterGameState.h"
#include "GameFramework/GameSession.h"

namespace MatchState
{
	// カスタムマッチ状態：クールダウン
	const FName Cooldown = FName("Cooldown");
}

ABlasterGameMode::ABlasterGameMode()
{
	// 遅延スタートを有効化（ウォームアップ期間を設ける）
	bDelayedStart = true;
}

void ABlasterGameMode::BeginPlay()
{
	Super::BeginPlay();

	// レベル開始時刻を記録（マッチタイマーの基準）
	LevelStartingTime = GetWorld()->GetTimeSeconds();
}

void ABlasterGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 待機状態（ウォームアップ中）
	if (MatchState == MatchState::WaitingToStart)
	{
		// ウォームアップの残り時間を計算
		CountdownTime = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			// ウォームアップ終了、マッチを開始
			StartMatch();
		}
	}
	// マッチ進行中
	else if (MatchState == MatchState::InProgress)
	{
		// マッチの残り時間を計算
		CountdownTime = WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			// マッチ時間終了、クールダウンに移行
			SetMatchState(MatchState::Cooldown);
		}
	}
	// クールダウン状態
	else if (MatchState == MatchState::Cooldown)
	{
		// クールダウンの残り時間を計算
		CountdownTime = CooldownTime + WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			// クールダウン終了、ゲームを再起動
			RestartGame();
		}
	}
}

void ABlasterGameMode::RestartGame()
{
	// ゲームセッションがゲーム再起動を許可しているかチェック
	if (GameSession && GameSession->CanRestartGame())
	{
		UWorld* World = GetWorld();
		if (World)
		{
			// シームレストラベルを有効化してロビーマップに戻る
			bUseSeamlessTravel = true;
			World->ServerTravel(FString("/Game/Maps/Lobby?listen"));
		}
	}
}

void ABlasterGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	// 全プレイヤーコントローラーにマッチ状態変更を通知
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ABlasterPlayerController* BlasterPlayer = Cast<ABlasterPlayerController>(*It);
		if (BlasterPlayer)
		{
			BlasterPlayer->OnMatchStateSet(MatchState, bTeamsMatch);
		}
	}
}

float ABlasterGameMode::CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage)
{
	// 基底クラスでは基本ダメージをそのまま返す
	// 派生クラス（TeamsGameMode）でフレンドリーファイアを無効化
	return BaseDamage;
}

void ABlasterGameMode::PlayerEliminated(class ABlasterCharacter* ElimmedCharacter, class ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController)
{
	// 攻撃者または犠牲者のコントローラーやステートが無効な場合は処理しない
	if (AttackerController == nullptr || AttackerController->PlayerState == nullptr) return;
	if (VictimController == nullptr || VictimController->PlayerState == nullptr) return;
	
	// プレイヤーステートを取得
	ABlasterPlayerState* AttackerPlayerState = AttackerController ? Cast<ABlasterPlayerState>(AttackerController->PlayerState) : nullptr;
	ABlasterPlayerState* VictimPlayerState = VictimController ? Cast<ABlasterPlayerState>(VictimController->PlayerState) : nullptr;

	// ゲームステートを取得
	ABlasterGameState* BlasterGameState = GetGameState<ABlasterGameState>();

	// 攻撃者が犠牲者と異なる場合（自殺でない場合）
	if (AttackerPlayerState && AttackerPlayerState != VictimPlayerState && BlasterGameState)
	{
		// 現在のリーダーを記録
		TArray<ABlasterPlayerState*> PlayersCurrentlyInTheLead;
		for (auto LeadPlayer : BlasterGameState->TopScoringPlayers)
		{
			PlayersCurrentlyInTheLead.Add(LeadPlayer);
		}

		// 攻撃者のスコアを加算
		AttackerPlayerState->AddToScore(1.f);
		// トップスコアを更新
		BlasterGameState->UpdateTopScore(AttackerPlayerState);
		
		// 攻撃者が新たにリーダーになった場合、リーダークラウンエフェクトを表示
		if (BlasterGameState->TopScoringPlayers.Contains(AttackerPlayerState))
		{
			ABlasterCharacter* Leader = Cast<ABlasterCharacter>(AttackerPlayerState->GetPawn());
			if (Leader)
			{
				Leader->MulticastGainedTheLead();
			}
		}

		// 以前のリーダーがリーダーでなくなった場合、リーダークラウンエフェクトを削除
		for (int32 i = 0; i < PlayersCurrentlyInTheLead.Num(); i++)
		{
			if (!BlasterGameState->TopScoringPlayers.Contains(PlayersCurrentlyInTheLead[i]))
			{
				ABlasterCharacter* Loser = Cast<ABlasterCharacter>(PlayersCurrentlyInTheLead[i]->GetPawn());
				if (Loser)
				{
					Loser->MulticastLostTheLead();
				}
			}
		}
	}
	
	// 犠牲者の敗北数を加算
	if (VictimPlayerState)
	{
		VictimPlayerState->AddToDefeats(1);
	}

	// エリミネートされたキャラクターのエリミネート処理を実行
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Elim(false);
	}

	// 全プレイヤーにエリミネーション通知をブロードキャスト
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ABlasterPlayerController* BlasterPlayer = Cast<ABlasterPlayerController>(*It);
		if (BlasterPlayer && AttackerPlayerState && VictimPlayerState)
		{
			BlasterPlayer->BroadcastElim(AttackerPlayerState, VictimPlayerState);
		}
	}
}

void ABlasterGameMode::RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController)
{
	// エリミネートされたキャラクターをリセットして破棄
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Reset();
		ElimmedCharacter->Destroy();
	}
	
	// エリミネートされたプレイヤーをランダムなプレイヤースタート地点でリスポーン
	if (ElimmedController)
	{
		// 全てのプレイヤースタート地点を取得
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
		// ランダムに1つ選択
		int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);
		// 選択されたプレイヤースタート地点でプレイヤーを再スポーン
		RestartPlayerAtPlayerStart(ElimmedController, PlayerStarts[Selection]);
	}
}

void ABlasterGameMode::PlayerLeftGame(ABlasterPlayerState* PlayerLeaving)
{
	if (PlayerLeaving == nullptr) return;
	
	// ゲームステートを取得
	ABlasterGameState* BlasterGameState = GetGameState<ABlasterGameState>();
	// 退出するプレイヤーがトップスコアプレイヤーの場合、配列から削除
	if (BlasterGameState && BlasterGameState->TopScoringPlayers.Contains(PlayerLeaving))
	{
		BlasterGameState->TopScoringPlayers.Remove(PlayerLeaving);
	}
	
	// 退出するプレイヤーのキャラクターをエリミネート（退出フラグを立てる）
	ABlasterCharacter* CharacterLeaving = Cast<ABlasterCharacter>(PlayerLeaving->GetPawn());
	if (CharacterLeaving)
	{
		CharacterLeaving->Elim(true);
	}
}