// Fill out your copyright notice in the Description page of Project Settings.

#include "AmmoPickup.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/BlasterComponents/CombatComponent.h"

void AAmmoPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 基底クラスのオーバーラップ処理を実行（エフェクト、サウンド再生など）
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	// 重なったアクターがブラスターキャラクターかチェック
	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor);
	if (BlasterCharacter)
	{
		// 戦闘コンポーネントを取得
		UCombatComponent* Combat = BlasterCharacter->GetCombat();
		if (Combat)
		{
			// 指定された武器タイプの弾薬を補充（武器タイプ、弾薬数）
			Combat->PickupAmmo(WeaponType, AmmoAmount);
		}
	}
	// ピックアップアイテムを破棄
	Destroy();
}