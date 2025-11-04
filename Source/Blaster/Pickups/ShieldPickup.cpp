// Fill out your copyright notice in the Description page of Project Settings.

#include "ShieldPickup.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/BlasterComponents/BuffComponent.h"

void AShieldPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 基底クラスのオーバーラップ処理を実行（エフェクト、サウンド再生など）
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	// 重なったアクターがブラスターキャラクターかチェック
	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor);
	if (BlasterCharacter)
	{
		// バフコンポーネントを取得
		UBuffComponent* Buff = BlasterCharacter->GetBuff();
		if (Buff)
		{
			// シールドを時間をかけて回復（回復量、回復時間）
			Buff->ReplenishShield(ShieldReplenishAmount, ShieldReplenishTime);
		}
	}

	// ピックアップアイテムを破棄
	Destroy();
}