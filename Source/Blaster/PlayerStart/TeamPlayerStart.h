// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Blaster/BlasterTypes/Team.h"
#include "TeamPlayerStart.generated.h"

/**
 * チームプレイヤースタートクラス
 * チーム専用のスポーン地点を定義
 * チームゲームモードで各チームのプレイヤーが初期スポーンする位置を設定
 */
UCLASS()
class BLASTER_API ATeamPlayerStart : public APlayerStart
{
	GENERATED_BODY()
public:
	/// <summary>
	/// このスポーン地点が所属するチーム
	/// 該当チームのプレイヤーのみがこの地点からスポーンする
	/// </summary>
	UPROPERTY(EditAnywhere)
	ETeam Team;
};
