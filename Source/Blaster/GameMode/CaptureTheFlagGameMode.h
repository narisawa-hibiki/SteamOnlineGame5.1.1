// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TeamsGameMode.h"
#include "CaptureTheFlagGameMode.generated.h"

/**
 * キャプチャー・ザ・フラッグゲームモードクラス
 * 赤チームと青チームがそれぞれ相手チームの旗を自陣に持ち帰るCTFモードを実装
 * チームゲームモードを継承し、旗の奪取と得点システムを追加
 * 旗を敵陣から奪い、自陣のフラッグゾーンに持ち帰ることで得点を獲得
 */
UCLASS()
class BLASTER_API ACaptureTheFlagGameMode : public ATeamsGameMode
{
	GENERATED_BODY()
public:
	/// <summary>
	/// プレイヤーがエリミネートされた時の処理
	/// CTFモードでは旗を持っているプレイヤーがエリミネートされた場合、旗をドロップ
	/// チームスコアは加算せず、旗の奪取と持ち帰りのみで得点
	/// </summary>
	/// <param name="ElimmedCharacter">エリミネートされたキャラクター</param>
	/// <param name="VictimController">犠牲者のプレイヤーコントローラー</param>
	/// <param name="AttackerController">攻撃者のプレイヤーコントローラー</param>
	virtual void PlayerEliminated(class ABlasterCharacter* ElimmedCharacter, class ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController) override;

	/// <summary>
	/// 旗が奪取された時の処理
	/// プレイヤーが敵の旗を自陣のフラッグゾーンに持ち帰った時に呼ばれる
	/// 該当チームのスコアを加算し、旗を元の位置にリセット
	/// </summary>
	/// <param name="Flag">奪取された旗</param>
	/// <param name="Zone">旗が持ち込まれたフラッグゾーン</param>
	void FlagCaptured(class AFlag* Flag, class AFlagZone* Zone);
};
