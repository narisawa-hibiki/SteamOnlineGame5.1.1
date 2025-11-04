// Fill out your copyright notice in the Description page of Project Settings.

#include "TeamsGameMode.h"
#include "Blaster/GameState/BlasterGameState.h"
#include "Blaster/PlayerState/BlasterPlayerState.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "Kismet/GameplayStatics.h"

ATeamsGameMode::ATeamsGameMode()
{
	// チームマッチフラグを有効化
	bTeamsMatch = true;
}

void ATeamsGameMode::PostLogin(APlayerController* NewPlayer)
{
	// 基底クラスのログイン処理を実行
	Super::PostLogin(NewPlayer);

	// ゲームステートを取得
	ABlasterGameState* BGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));
	if (BGameState)
	{
		// プレイヤーステートを取得
		ABlasterPlayerState* BPState = NewPlayer->GetPlayerState<ABlasterPlayerState>();
		// プレイヤーがまだチームに所属していない場合
		if (BPState && BPState->GetTeam() == ETeam::ET_NoTeam)
		{
			// 青チームの人数が赤チーム以上の場合、赤チームに配属
			if (BGameState->BlueTeam.Num() >= BGameState->RedTeam.Num())
			{
				BGameState->RedTeam.AddUnique(BPState);
				BPState->SetTeam(ETeam::ET_RedTeam);
			}
			// それ以外は青チームに配属（チームバランスを維持）
			else
			{
				BGameState->BlueTeam.AddUnique(BPState);
				BPState->SetTeam(ETeam::ET_BlueTeam);
			}
		}
	}
}

void ATeamsGameMode::Logout(AController* Exiting)
{
	// ゲームステートとプレイヤーステートを取得
	ABlasterGameState* BGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));
	ABlasterPlayerState* BPState = Exiting->GetPlayerState<ABlasterPlayerState>();
	if (BGameState && BPState)
	{
		// 赤チームに所属している場合は赤チームから削除
		if (BGameState->RedTeam.Contains(BPState))
		{
			BGameState->RedTeam.Remove(BPState);
		}
		// 青チームに所属している場合は青チームから削除
		if (BGameState->BlueTeam.Contains(BPState))
		{
			BGameState->BlueTeam.Remove(BPState);
		}
	}
}

void ATeamsGameMode::HandleMatchHasStarted()
{
	// 基底クラスのマッチ開始処理を実行
	Super::HandleMatchHasStarted();

	// ゲームステートを取得
	ABlasterGameState* BGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));
	if (BGameState)
	{
		// 全プレイヤーをループしてチームに振り分け
		for (auto PState : BGameState->PlayerArray)
		{
			ABlasterPlayerState* BPState = Cast<ABlasterPlayerState>(PState.Get());
			// プレイヤーがまだチームに所属していない場合
			if (BPState && BPState->GetTeam() == ETeam::ET_NoTeam)
			{
				// 青チームの人数が赤チーム以上の場合、赤チームに配属
				if (BGameState->BlueTeam.Num() >= BGameState->RedTeam.Num())
				{
					BGameState->RedTeam.AddUnique(BPState);
					BPState->SetTeam(ETeam::ET_RedTeam);
				}
				// それ以外は青チームに配属（チームバランスを維持）
				else
				{
					BGameState->BlueTeam.AddUnique(BPState);
					BPState->SetTeam(ETeam::ET_BlueTeam);
				}
			}
		}
	}
}

float ATeamsGameMode::CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage)
{
	// 攻撃者と犠牲者のプレイヤーステートを取得
	ABlasterPlayerState* AttackerPState = Attacker->GetPlayerState<ABlasterPlayerState>();
	ABlasterPlayerState* VictimPState = Victim->GetPlayerState<ABlasterPlayerState>();
	// どちらかが取得できない場合は基本ダメージを返す
	if (AttackerPState == nullptr || VictimPState == nullptr) return BaseDamage;
	// 自分自身へのダメージは有効（自爆など）
	if (VictimPState == AttackerPState)
	{
		return BaseDamage;
	}
	// 同じチームへの攻撃（フレンドリーファイア）は無効化
	if (AttackerPState->GetTeam() == VictimPState->GetTeam())
	{
		return 0.f;
	}
	// 異なるチームへの攻撃は基本ダメージを適用
	return BaseDamage;
}

void ATeamsGameMode::PlayerEliminated(ABlasterCharacter* ElimmedCharacter, ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController)
{
	// 基底クラスのエリミネート処理を実行
	Super::PlayerEliminated(ElimmedCharacter, VictimController, AttackerController);

	// ゲームステートと攻撃者のプレイヤーステートを取得
	ABlasterGameState* BGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));
	ABlasterPlayerState* AttackerPlayerState = AttackerController ? Cast<ABlasterPlayerState>(AttackerController->PlayerState) : nullptr;
	if (BGameState && AttackerPlayerState)
	{
		// 攻撃者が青チームの場合、青チームのスコアを加算
		if (AttackerPlayerState->GetTeam() == ETeam::ET_BlueTeam)
		{
			BGameState->BlueTeamScores();
		}
		// 攻撃者が赤チームの場合、赤チームのスコアを加算
		if (AttackerPlayerState->GetTeam() == ETeam::ET_RedTeam)
		{
			BGameState->RedTeamScores();
		}
	}
}