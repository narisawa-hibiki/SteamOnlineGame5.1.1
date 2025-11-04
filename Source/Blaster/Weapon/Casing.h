// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Casing.generated.h"

/**
 * 薬莢クラス
 * 武器発射時に排出される薬莢を表現
 * 物理シミュレーションで飛び出し、地面に落ちた時にサウンドを再生
 */
UCLASS()
class BLASTER_API ACasing : public AActor
{
	GENERATED_BODY()
	
public:	
	ACasing();

protected:
	virtual void BeginPlay() override;

	/// <summary>
	/// 薬莢が何かに衝突した時のコールバック
	/// 地面に落ちた時にサウンドを再生
	/// </summary>
	/// <param name="HitComp">ヒットしたコンポーネント</param>
	/// <param name="OtherActor">衝突相手のアクター</param>
	/// <param name="OtherComp">衝突相手のコンポーネント</param>
	/// <param name="NormalImpulse">法線方向の衝撃</param>
	/// <param name="Hit">ヒット結果</param>
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	/// <summary>
	/// 薬莢のメッシュ
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CasingMesh;

	/// <summary>
	/// 薬莢排出時の衝撃力
	/// 薬莢が飛び出す勢いを決定
	/// </summary>
	UPROPERTY(EditAnywhere)
	float ShellEjectionImpulse;

	/// <summary>
	/// 薬莢が地面に落ちた時のサウンド
	/// </summary>
	UPROPERTY(EditAnywhere)
	class USoundCue* ShellSound;
};
