// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blaster/BlasterTypes/Team.h"
#include "FlagZone.generated.h"

/**
 * フラッグゾーンアクター
 * キャプチャー・ザ・フラッグモードにおいて、敵チームのフラッグを持ち帰る目標地点を定義
 * プレイヤーがこのゾーンに敵フラッグを持ち込むことでポイントを獲得
 */
UCLASS()
class BLASTER_API AFlagZone : public AActor
{
	GENERATED_BODY()
	
public:	
	AFlagZone();

	/// <summary>
	/// このゾーンが所属するチーム
	/// 対応するチームのプレイヤーのみがこのゾーンに敵フラッグを持ち込める
	/// </summary>
	UPROPERTY(EditAnywhere)
	ETeam Team;

protected:
	virtual void BeginPlay() override;

	/// <summary>
	/// ゾーンの球体コリジョンに他のアクターが重なった時に呼ばれる
	/// フラッグを持ったプレイヤーが正しいゾーンに入ったかを判定
	/// </summary>
	/// <param name="OverlappedComponent">重なられたコンポーネント</param>
	/// <param name="OtherActor">重なったアクター</param>
	/// <param name="OtherComp">重なった相手のコンポーネント</param>
	/// <param name="OtherBodyIndex">ボディインデックス</param>
	/// <param name="bFromSweep">スイープによる重なりか</param>
	/// <param name="SweepResult">スイープの結果</param>
	UFUNCTION()
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

private:
	/// <summary>
	/// ゾーンの範囲を定義する球体コリジョン
	/// この範囲内にフラッグを持ったプレイヤーが入るとキャプチャー判定が発生
	/// </summary>
	UPROPERTY(EditAnywhere)
	class USphereComponent* ZoneSphere;

public:	

};
