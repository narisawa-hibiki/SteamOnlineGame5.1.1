// Fill out your copyright notice in the Description page of Project Settings.

#include "Flag.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Blaster/Character/BlasterCharacter.h"

AFlag::AFlag()
{
	// フラグメッシュを作成してルートコンポーネントに設定
	FlagMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlagMesh"));
	SetRootComponent(FlagMesh);
	// エリア球体と拾得ウィジェットをフラグメッシュにアタッチ
	GetAreaSphere()->SetupAttachment(FlagMesh);
	GetPickupWidget()->SetupAttachment(FlagMesh);
	// フラグメッシュのコリジョンを無効化
	FlagMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	FlagMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AFlag::Dropped()
{
	// フラグ状態をドロップに設定
	SetWeaponState(EWeaponState::EWS_Dropped);
	// フラグをデタッチ（ワールド座標を維持）
	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	FlagMesh->DetachFromComponent(DetachRules);
	// オーナー情報をクリア
	SetOwner(nullptr);
	BlasterOwnerCharacter = nullptr;
	BlasterOwnerController = nullptr;
}

void AFlag::ResetFlag()
{
	// フラグを持っているキャラクターを取得
	ABlasterCharacter* FlagBearer = Cast<ABlasterCharacter>(GetOwner());
	if (FlagBearer)
	{
		// フラグ保持状態を解除
		FlagBearer->SetHoldingTheFlag(false);
		FlagBearer->SetOverlappingWeapon(nullptr);
		FlagBearer->UnCrouch();
	}

	// サーバーのみ実行
	if (!HasAuthority()) return;

	// フラグをデタッチ
	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	FlagMesh->DetachFromComponent(DetachRules);
	// 初期状態に戻す
	SetWeaponState(EWeaponState::EWS_Initial);
	// エリア球体のコリジョンを有効化（再度拾得可能に）
	GetAreaSphere()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetAreaSphere()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	// オーナー情報をクリア
	SetOwner(nullptr);
	BlasterOwnerCharacter = nullptr;
	BlasterOwnerController = nullptr;

	// フラグを初期位置に戻す
	SetActorTransform(InitialTransform);
}

void AFlag::OnEquipped()
{
	// 拾得ウィジェットを非表示
	ShowPickupWidget(false);
	// エリア球体のコリジョンを無効化
	GetAreaSphere()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// フラグメッシュの物理シミュレーションを無効化
	FlagMesh->SetSimulatePhysics(false);
	FlagMesh->SetEnableGravity(false);
	// コリジョンをクエリのみに設定
	FlagMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	FlagMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	// カスタムデプスを無効化
	EnableCustomDepth(false);
}

void AFlag::OnDropped()
{
	// サーバーのみエリア球体のコリジョンを設定
	if (HasAuthority())
	{
		GetAreaSphere()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	// 物理シミュレーションを有効化（フラグが地面に落ちる）
	FlagMesh->SetSimulatePhysics(true);
	FlagMesh->SetEnableGravity(true);
	FlagMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	FlagMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	FlagMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	FlagMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	// カスタムデプスを有効化（青色アウトライン）
	FlagMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_BLUE);
	FlagMesh->MarkRenderStateDirty();
	EnableCustomDepth(true);
}

void AFlag::BeginPlay()
{
	Super::BeginPlay();
	// 初期位置とトランスフォームを保存（リセット時に使用）
	InitialTransform = GetActorTransform();
}
