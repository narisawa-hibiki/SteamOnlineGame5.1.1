// Fill out your copyright notice in the Description page of Project Settings.

#include "FlagZone.h"
#include "Components/SphereComponent.h"
#include "Blaster/Weapon/Flag.h"
#include "Blaster/GameMode/CaptureTheFlagGameMode.h"
#include "Blaster/Character/BlasterCharacter.h"

AFlagZone::AFlagZone()
{
	PrimaryActorTick.bCanEverTick = false;

	// ゾーンの範囲を定義する球体コリジョンを作成
	ZoneSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ZoneSphere"));
	SetRootComponent(ZoneSphere);
}

void AFlagZone::BeginPlay()
{
	Super::BeginPlay();
	
	// 球体コリジョンへのオーバーラップイベントをバインド
	ZoneSphere->OnComponentBeginOverlap.AddDynamic(this, &AFlagZone::OnSphereOverlap);
}

void AFlagZone::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 重なったアクターがフラッグかチェック
	AFlag* OverlappingFlag = Cast<AFlag>(OtherActor);
	// フラッグが存在し、かつ敵チームのフラッグ（このゾーンとは異なるチーム）の場合
	if (OverlappingFlag && OverlappingFlag->GetTeam() != Team)
	{
		// ゲームモードを取得
		ACaptureTheFlagGameMode* GameMode = GetWorld()->GetAuthGameMode<ACaptureTheFlagGameMode>();
		if (GameMode)
		{
			// フラッグキャプチャーをゲームモードに通知
			GameMode->FlagCaptured(OverlappingFlag, this);
		}
		// フラッグを元の位置にリセット
		OverlappingFlag->ResetFlag();
	}
}
