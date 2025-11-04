// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "ProjectileRocket.generated.h"

/**
 * プロジェクタイルロケットクラス
 * ロケットランチャーから発射されるロケット弾を表現
 * 飛行音のループ再生とカスタム移動コンポーネントを使用
 */
UCLASS()
class BLASTER_API AProjectileRocket : public AProjectile
{
	GENERATED_BODY()
public:
	AProjectileRocket();
	virtual void Destroyed() override;

protected:
	/// <summary>
	/// 衝突時の処理
	/// ロケットが何かに当たった時の爆発処理を実行
	/// </summary>
	/// <param name="HitComp">ヒットしたコンポーネント</param>
	/// <param name="OtherActor">衝突相手のアクター</param>
	/// <param name="OtherComp">衝突相手のコンポーネント</param>
	/// <param name="NormalImpulse">法線方向の衝撃</param>
	/// <param name="Hit">ヒット結果</param>
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	
	virtual void BeginPlay() override;

	/// <summary>
	/// ロケット飛行中のループサウンド
	/// </summary>
	UPROPERTY(EditAnywhere)
	USoundCue* ProjectileLoop;

	/// <summary>
	/// ループサウンドのオーディオコンポーネント
	/// 再生中のサウンドを制御するために使用
	/// </summary>
	UPROPERTY()
	UAudioComponent* ProjectileLoopComponent;

	/// <summary>
	/// ループサウンドの減衰設定
	/// 距離による音量の減衰を制御
	/// </summary>
	UPROPERTY(EditAnywhere)
	USoundAttenuation* LoopingSoundAttenuation;

	/// <summary>
	/// ロケット専用の移動コンポーネント
	/// 壁に当たっても停止せず、コリジョンボックスで爆発判定を行う特殊な挙動を実装
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	class URocketMovementComponent* RocketMovementComponent;

private:
};
