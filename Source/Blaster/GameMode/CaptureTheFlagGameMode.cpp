// Fill out your copyright notice in the Description page of Project Settings.

#include "CaptureTheFlagGameMode.h"
#include "Blaster/Weapon/Flag.h"
#include "Blaster/CaptureTheFlag/FlagZone.h"
#include "Blaster/GameState/BlasterGameState.h"

void ACaptureTheFlagGameMode::PlayerEliminated(class ABlasterCharacter* ElimmedCharacter, class ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController)
{
	// BlasterGameModeの基底実装を呼ぶ（TeamsGameModeのスコア加算をスキップ）
	// CTFモードではキルによるスコア加算を行わず、旗の奪取のみで得点
	ABlasterGameMode::PlayerEliminated(ElimmedCharacter, VictimController, AttackerController);
}

void ACaptureTheFlagGameMode::FlagCaptured(AFlag* Flag, AFlagZone* Zone)
{
	// 有効な奪取かチェック（敵チームの旗を自陣のゾーンに持ち込んだか）
	bool bValidCapture = Flag->GetTeam() != Zone->Team;
	// ゲームステートを取得
	ABlasterGameState* BGameState = Cast<ABlasterGameState>(GameState);
	if (BGameState)
	{
		// 青チームのゾーンに旗が持ち込まれた場合、青チームのスコアを加算
		if (Zone->Team == ETeam::ET_BlueTeam)
		{
			BGameState->BlueTeamScores();
		}
		// 赤チームのゾーンに旗が持ち込まれた場合、赤チームのスコアを加算
		if (Zone->Team == ETeam::ET_RedTeam)
		{
			BGameState->RedTeamScores();
		}
	}
}
