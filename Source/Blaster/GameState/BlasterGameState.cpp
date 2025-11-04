// Fill out your copyright notice in the Description page of Project Settings.

#include "BlasterGameState.h"
#include "Net/UnrealNetwork.h"
#include "Blaster/PlayerState/BlasterPlayerState.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"

void ABlasterGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// トップスコアプレイヤー配列とチームスコアをレプリケート
	DOREPLIFETIME(ABlasterGameState, TopScoringPlayers);
	DOREPLIFETIME(ABlasterGameState, RedTeamScore);
	DOREPLIFETIME(ABlasterGameState, BlueTeamScore);
}

void ABlasterGameState::UpdateTopScore(class ABlasterPlayerState* ScoringPlayer)
{
	// トップスコアプレイヤーがまだいない場合、最初のプレイヤーを追加
	if (TopScoringPlayers.Num() == 0)
	{
		TopScoringPlayers.Add(ScoringPlayer);
		TopScore = ScoringPlayer->GetScore();
	}
	// プレイヤーのスコアが現在のトップスコアと同じ場合、同点トップとして追加
	else if (ScoringPlayer->GetScore() == TopScore)
	{
		TopScoringPlayers.AddUnique(ScoringPlayer);
	}
	// プレイヤーのスコアが現在のトップスコアを上回った場合
	else if (ScoringPlayer->GetScore() > TopScore)
	{
		// 既存のトップスコアプレイヤーをクリア
		TopScoringPlayers.Empty();
		// 新しいトップスコアプレイヤーを追加
		TopScoringPlayers.AddUnique(ScoringPlayer);
		// トップスコアを更新
		TopScore = ScoringPlayer->GetScore();
	}
}

void ABlasterGameState::RedTeamScores()
{
	// 赤チームのスコアを1増やす
	++RedTeamScore;
	// 最初のプレイヤーコントローラーを取得
	ABlasterPlayerController* BPlayer = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayer)
	{
		// HUDの赤チームスコアを更新
		BPlayer->SetHUDRedTeamScore(RedTeamScore);
	}
}

void ABlasterGameState::BlueTeamScores()
{
	// 青チームのスコアを1増やす
	++BlueTeamScore;
	// 最初のプレイヤーコントローラーを取得
	ABlasterPlayerController* BPlayer = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayer)
	{
		// HUDの青チームスコアを更新
		BPlayer->SetHUDBlueTeamScore(BlueTeamScore);
	}
}

void ABlasterGameState::OnRep_RedTeamScore()
{
	// 赤チームスコアがレプリケートされた時、最初のプレイヤーコントローラーを取得
	ABlasterPlayerController* BPlayer = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayer)
	{
		// HUDの赤チームスコアを更新
		BPlayer->SetHUDRedTeamScore(RedTeamScore);
	}
}

void ABlasterGameState::OnRep_BlueTeamScore()
{
	// 青チームスコアがレプリケートされた時、最初のプレイヤーコントローラーを取得
	ABlasterPlayerController* BPlayer = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayer)
	{
		// HUDの青チームスコアを更新
		BPlayer->SetHUDBlueTeamScore(BlueTeamScore);
	}
}
