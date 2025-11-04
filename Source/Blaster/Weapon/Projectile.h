// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

/**
 * プロジェクタイル（発射体）基底クラス
 * 弾丸、ロケット、グレネードなど全ての発射体の共通機能を提供
 * 衝突判定、ダメージ処理、エフェクト再生、サーバーサイド巻き戻し対応
 */
UCLASS()
class BLASTER_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

	/// <summary>
	/// サーバーサイド巻き戻しを使用するか
	/// ラグ補償のために過去の位置でヒット判定を行う
	/// </summary>
	bool bUseServerSideRewind = false;

	/// <summary>
	/// トレース開始位置
	/// サーバーサイド巻き戻し用
	/// </summary>
	FVector_NetQuantize TraceStart;

	/// <summary>
	/// 初速度ベクトル
	/// サーバーサイド巻き戻し用
	/// </summary>
	FVector_NetQuantize100 InitialVelocity;

	/// <summary>
	/// プロジェクタイルの初速度
	/// 弾道の速さを決定
	/// </summary>
	UPROPERTY(EditAnywhere)
	float InitialSpeed = 15000;

	/// <summary>
	/// 基本ダメージ
	/// グレネードとロケットのみ設定（爆発ダメージ）
	/// </summary>
	UPROPERTY(EditAnywhere)
	float Damage = 20.f;

	/// <summary>
	/// ヘッドショットダメージ
	/// グレネードとロケットでは使用しない
	/// </summary>
	UPROPERTY(EditAnywhere)
	float HeadShotDamage = 40.f;

protected:
	virtual void BeginPlay() override;

	/// <summary>
	/// 破棄タイマーを開始
	/// 一定時間後にプロジェクタイルを自動破棄
	/// </summary>
	void StartDestroyTimer();

	/// <summary>
	/// 破棄タイマー完了時の処理
	/// </summary>
	void DestroyTimerFinished();

	/// <summary>
	/// トレイルシステムをスポーン
	/// 弾道の軌跡エフェクトを生成
	/// </summary>
	void SpawnTrailSystem();

	/// <summary>
	/// 爆発ダメージを適用
	/// 範囲内の全アクターにダメージを与える
	/// </summary>
	void ExplodeDamage();

	/// <summary>
	/// 衝突時のコールバック
	/// ヒット処理、エフェクト、サウンドの再生
	/// </summary>
	/// <param name="HitComp">ヒットしたコンポーネント</param>
	/// <param name="OtherActor">衝突相手のアクター</param>
	/// <param name="OtherComp">衝突相手のコンポーネント</param>
	/// <param name="NormalImpulse">法線方向の衝撃</param>
	/// <param name="Hit">ヒット結果</param>
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/// <summary>
	/// 衝突時のパーティクルエフェクト
	/// </summary>
	UPROPERTY(EditAnywhere)
	class UParticleSystem* ImpactParticles;

	/// <summary>
	/// 衝突時のサウンド
	/// </summary>
	UPROPERTY(EditAnywhere)
	class USoundCue* ImpactSound;

	/// <summary>
	/// 衝突判定用のボックスコリジョン
	/// </summary>
	UPROPERTY(EditAnywhere)
	class UBoxComponent* CollisionBox;

	/// <summary>
	/// トレイルエフェクトシステム（Niagara）
	/// </summary>
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* TrailSystem;

	/// <summary>
	/// スポーンされたトレイルコンポーネント
	/// </summary>
	UPROPERTY()
	class UNiagaraComponent* TrailSystemComponent;

	/// <summary>
	/// プロジェクタイル移動コンポーネント
	/// 弾道の物理演算を処理
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;

	/// <summary>
	/// プロジェクタイルのメッシュ
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ProjectileMesh;

	/// <summary>
	/// 爆発ダメージの内側半径
	/// この範囲内は最大ダメージ
	/// </summary>
	UPROPERTY(EditAnywhere)
	float DamageInnerRadius = 200.f;

	/// <summary>
	/// 爆発ダメージの外側半径
	/// この範囲まで距離減衰ダメージ
	/// </summary>
	UPROPERTY(EditAnywhere)
	float DamageOuterRadius = 500.f;

private:
	/// <summary>
	/// トレーサーパーティクルシステム（旧システム）
	/// </summary>
	UPROPERTY(EditAnywhere)
	UParticleSystem* Tracer;

	/// <summary>
	/// スポーンされたトレーサーコンポーネント
	/// </summary>
	UPROPERTY()
	class UParticleSystemComponent* TracerComponent;

	/// <summary>
	/// 破棄タイマーハンドル
	/// </summary>
	FTimerHandle DestroyTimer;

	/// <summary>
	/// 自動破棄までの時間（秒）
	/// </summary>
	UPROPERTY(EditAnywhere)
	float DestroyTime = 3.f;

public:	
};
