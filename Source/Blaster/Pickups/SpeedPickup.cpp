// Fill out your copyright notice in the Description page of Project Settings.

#include "SpeedPickup.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/BlasterComponents/BuffComponent.h"

void ASpeedPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
			// スピードバフを適用（通常移動速度、しゃがみ移動速度、持続時間）
			Buff->BuffSpeed(BaseSpeedBuff, CrouchSpeedBuff, SpeedBuffTime);
		}
	}

	// ピックアップアイテムを破棄
	Destroy();
}