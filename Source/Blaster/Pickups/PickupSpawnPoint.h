// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupSpawnPoint.generated.h"

/**
 * ピックアップスポーン地点クラス
 * マップ上でピックアップアイテムを定期的にスポーンする地点を管理
 * ピックアップが拾われると、ランダムな時間後に新しいピックアップをスポーン
 */
UCLASS()
class BLASTER_API APickupSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	APickupSpawnPoint();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	/// <summary>
	/// スポーン可能なピックアップのクラス配列
	/// ここから1つがランダムに選ばれてスポーンされる
	/// </summary>
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class APickup>> PickupClasses;

	/// <summary>
	/// 現在スポーンされているピックアップ
	/// 拾われた時の通知を受け取るために保持
	/// </summary>
	UPROPERTY()
	APickup* SpawnedPickup;

	/// <summary>
	/// ピックアップをスポーン
	/// PickupClassesからランダムに1つ選んでスポーン
	/// </summary>
	void SpawnPickup();

	/// <summary>
	/// スポーンタイマー完了時に呼ばれる
	/// 新しいピックアップをスポーン
	/// </summary>
	void SpawnPickupTimerFinished();

	/// <summary>
	/// ピックアップが破棄された時に呼ばれる
	/// 次のスポーンまでのタイマーを開始
	/// </summary>
	/// <param name="DestroyedActor">破棄されたピックアップ</param>
	UFUNCTION()
	void StartSpawnPickupTimer(AActor* DestroyedActor);

private:
	/// <summary>
	/// ピックアップスポーンタイマーハンドル
	/// </summary>
	FTimerHandle SpawnPickupTimer;

	/// <summary>
	/// ピックアップスポーンまでの最小時間（秒）
	/// </summary>
	UPROPERTY(EditAnywhere)
	float SpawnPickupTimeMin;

	/// <summary>
	/// ピックアップスポーンまでの最大時間（秒）
	/// </summary>
	UPROPERTY(EditAnywhere)
	float SpawnPickupTimeMax;

public:	

};
