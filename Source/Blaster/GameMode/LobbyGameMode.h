// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * ロビーゲームモードクラス
 * マルチプレイヤーマッチ開始前の待機ロビーを管理
 * プレイヤーがログインするとカウントし、一定数に達したらメインゲームマップに遷移
 * マッチメイキングの待機エリアとして機能
 */
UCLASS()
class BLASTER_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	/// <summary>
	/// プレイヤーログイン時の処理
	/// 接続したプレイヤー数をカウントし、規定人数に達したらゲームマップに遷移
	/// マルチプレイヤーセッションの開始トリガーとして機能
	/// </summary>
	/// <param name="NewPlayer">ログインしたプレイヤーコントローラー</param>
	virtual void PostLogin(APlayerController* NewPlayer) override;
};
