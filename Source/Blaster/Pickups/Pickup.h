// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

/**
 * ピックアップアイテム基底クラス
 * マップ上に配置される拾得可能なアイテムの共通機能を提供
 * 回転アニメーション、エフェクト、サウンド、拾得判定などを管理
 * ヘルス、シールド、弾薬、スピード、ジャンプなどのピックアップの基底クラス
 */
UCLASS()
class BLASTER_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	APickup();
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

protected:
	virtual void BeginPlay() override;

	/// <summary>
	/// ピックアップの球体コリジョンに他のアクターが重なった時に呼ばれる
	/// プレイヤーキャラクターがピックアップを拾得した時の処理
	/// 派生クラスでオーバーライドして固有の効果を実装
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

	/// <summary>
	/// ピックアップの回転速度（度/秒）
	/// Y軸を中心にこの速度で回転し、プレイヤーの注意を引く
	/// </summary>
	UPROPERTY(EditAnywhere)
	float BaseTurnRate = 45.f;

private:
	/// <summary>
	/// 拾得判定用の球体コリジョン
	/// プレイヤーがこの範囲に入るとピックアップを拾得
	/// </summary>
	UPROPERTY(EditAnywhere)
	class USphereComponent* OverlapSphere;

	/// <summary>
	/// ピックアップ拾得時のサウンド
	/// </summary>
	UPROPERTY(EditAnywhere)
	class USoundCue* PickupSound;

	/// <summary>
	/// ピックアップのメッシュ
	/// アイテムの見た目を表示
	/// </summary>
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PickupMesh;

	/// <summary>
	/// ピックアップエフェクトコンポーネント（Niagara）
	/// アイテムの周囲に表示されるパーティクルエフェクト
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* PickupEffectComponent;

	/// <summary>
	/// ピックアップエフェクトシステム（Niagara）
	/// スポーン時に使用されるエフェクト
	/// </summary>
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* PickupEffect;

	/// <summary>
	/// オーバーラップバインドタイマーハンドル
	/// スポーン直後の意図しない拾得を防ぐための遅延タイマー
	/// </summary>
	FTimerHandle BindOverlapTimer;

	/// <summary>
	/// オーバーラップイベントをバインドするまでの遅延時間（秒）
	/// スポーン直後にプレイヤーがいても即座に拾得されないようにする
	/// </summary>
	float BindOverlapTime = 0.25f;

	/// <summary>
	/// バインドタイマー完了時に呼ばれる
	/// オーバーラップイベントをバインドして拾得可能にする
	/// </summary>
	void BindOverlapTimerFinished();

public:	

};
