// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponTypes.h"
#include "Blaster/BlasterTypes/Team.h"
#include "Weapon.generated.h"

/**
 * 武器の状態を表す列挙型
 * 武器のライフサイクル（初期状態、装備中、ドロップなど）を管理
 */
UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	/// <summary>
	/// 初期状態（スポーン直後）
	/// </summary>
	EWS_Initial UMETA(DisplayName = "Initial State"),

	/// <summary>
	/// プライマリ武器として装備中
	/// </summary>
	EWS_Equipped UMETA(DisplayName = "Equipped"),

	/// <summary>
	/// セカンダリ武器として装備中
	/// </summary>
	EWS_EquippedSecondary UMETA(DisplayName = "Equipped Secondary"),

	/// <summary>
	/// ドロップされた状態
	/// </summary>
	EWS_Dropped UMETA(DisplayName = "Dropped"),

	/// <summary>
	/// 列挙型の最大値（内部使用）
	/// </summary>
	EWS_MAX UMETA(DisplayName = "DefaultMAX")
};

/**
 * 射撃タイプを表す列挙型
 * 武器の弾道計算方法を定義
 */
UENUM(BlueprintType)
enum class EFireType : uint8
{
	/// <summary>
	/// ヒットスキャン武器（即着弾）
	/// ライントレースで即座にヒット判定
	/// </summary>
	EFT_HitScan UMETA(DisplayName = "Hit Scan Weapon"),

	/// <summary>
	/// プロジェクタイル武器（弾道あり）
	/// 物理演算で弾丸の軌道を計算
	/// </summary>
	EFT_Projectile UMETA(DisplayName = "Projectile Weapon"),

	/// <summary>
	/// ショットガン（複数ペレット）
	/// 複数のヒットスキャンを同時実行
	/// </summary>
	EFT_Shotgun UMETA(DisplayName = "Shotgun Weapon"),

	/// <summary>
	/// 列挙型の最大値（内部使用）
	/// </summary>
	EFT_MAX UMETA(DisplayName = "DefaultMAX")
};

/**
 * 武器アクタークラス
 * ゲーム内の全ての武器の基底クラス
 * 射撃、リロード、拾得、ドロップなどの機能を提供
 */
UCLASS()
class BLASTER_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnRep_Owner() override;

	/// <summary>
	/// HUDの弾薬表示を更新
	/// </summary>
	void SetHUDAmmo();

	/// <summary>
	/// 拾得ウィジェットの表示/非表示を切り替え
	/// </summary>
	/// <param name="bShowWidget">表示するか</param>
	void ShowPickupWidget(bool bShowWidget);

	/// <summary>
	/// 武器を発射
	/// </summary>
	/// <param name="HitTarget">ヒット目標位置</param>
	virtual void Fire(const FVector& HitTarget);

	/// <summary>
	/// 武器をドロップ
	/// </summary>
	virtual void Dropped();

	/// <summary>
	/// 弾薬を追加
	/// </summary>
	/// <param name="AmmoToAdd">追加する弾薬数</param>
	void AddAmmo(int32 AmmoToAdd);

	/// <summary>
	/// スキャッター（弾の散乱）を適用したトレース終点を計算
	/// </summary>
	/// <param name="HitTarget">元のヒット目標位置</param>
	/// <returns>散乱を適用した終点</returns>
	FVector TraceEndWithScatter(const FVector& HitTarget);

	/// <summary>
	/// クロスヘア中央のテクスチャ
	/// </summary>
	UPROPERTY(EditAnywhere, Category = Crosshairs)
	class UTexture2D* CrosshairsCenter;

	/// <summary>
	/// クロスヘア左側のテクスチャ
	/// </summary>
	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsLeft;

	/// <summary>
	/// クロスヘア右側のテクスチャ
	/// </summary>
	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsRight;

	/// <summary>
	/// クロスヘア上側のテクスチャ
	/// </summary>
	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsTop;

	/// <summary>
	/// クロスヘア下側のテクスチャ
	/// </summary>
	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsBottom;

	/// <summary>
	/// エイム時のズームFOV
	/// </summary>
	UPROPERTY(EditAnywhere)
	float ZoomedFOV = 30.f;

	/// <summary>
	/// ズーム時の補間速度
	/// </summary>
	UPROPERTY(EditAnywhere)
	float ZoomInterpSpeed = 20.f;

	/// <summary>
	/// 射撃間隔（秒）
	/// </summary>
	UPROPERTY(EditAnywhere, Category = Combat)
	float FireDelay = .15f;

	/// <summary>
	/// フルオート武器か
	/// </summary>
	UPROPERTY(EditAnywhere, Category = Combat)
	bool bAutomatic = true;

	/// <summary>
	/// 装備時のサウンド
	/// </summary>
	UPROPERTY(EditAnywhere)
	class USoundCue* EquipSound;

	/// <summary>
	/// カスタムデプス（アウトライン表示）の有効/無効を切り替え
	/// </summary>
	/// <param name="bEnable">有効にするか</param>
	void EnableCustomDepth(bool bEnable);

	/// <summary>
	/// ドロップ時に武器を破棄するか
	/// </summary>
	bool bDestroyWeapon = false;

	/// <summary>
	/// 射撃タイプ
	/// </summary>
	UPROPERTY(EditAnywhere)
	EFireType FireType;

	/// <summary>
	/// スキャッター（弾の散乱）を使用するか
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	bool bUseScatter = false;

protected:
	virtual void BeginPlay() override;

	/// <summary>
	/// 武器状態が設定された時に呼ばれる
	/// </summary>
	virtual void OnWeaponStateSet();

	/// <summary>
	/// 武器が装備された時に呼ばれる
	/// </summary>
	virtual void OnEquipped();

	/// <summary>
	/// 武器がドロップされた時に呼ばれる
	/// </summary>
	virtual void OnDropped();

	/// <summary>
	/// セカンダリ武器として装備された時に呼ばれる
	/// </summary>
	virtual void OnEquippedSecondary();

	/// <summary>
	/// エリア球体に他のアクターが重なった時のコールバック
	/// </summary>
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
	/// エリア球体から他のアクターが離れた時のコールバック
	/// </summary>
	UFUNCTION()
	void OnSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	/// <summary>
	/// スキャッター球体の中心までの距離
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	float DistanceToSphere = 800.f;

	/// <summary>
	/// スキャッター球体の半径
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	float SphereRadius = 75.f;

	/// <summary>
	/// 基本ダメージ
	/// </summary>
	UPROPERTY(EditAnywhere)
	float Damage = 20.f;

	/// <summary>
	/// ヘッドショットダメージ
	/// </summary>
	UPROPERTY(EditAnywhere)
	float HeadShotDamage = 40.f;

	/// <summary>
	/// サーバーサイド巻き戻しを使用するか
	/// </summary>
	UPROPERTY(Replicated, EditAnywhere)
	bool bUseServerSideRewind = false;

	/// <summary>
	/// 武器を所有するブラスターキャラクター
	/// </summary>
	UPROPERTY()
	class ABlasterCharacter* BlasterOwnerCharacter;

	/// <summary>
	/// 武器を所有するプレイヤーコントローラー
	/// </summary>
	UPROPERTY()
	class ABlasterPlayerController* BlasterOwnerController;

	/// <summary>
	/// Pingが高すぎる時のコールバック
	/// </summary>
	/// <param name="bPingTooHigh">Pingが高すぎるか</param>
	UFUNCTION()
	void OnPingTooHigh(bool bPingTooHigh);

private:
	/// <summary>
	/// 武器のスケルタルメッシュ
	/// </summary>
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USkeletalMeshComponent* WeaponMesh;

	/// <summary>
	/// 拾得判定用の球体コリジョン
	/// </summary>
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class USphereComponent* AreaSphere;

	/// <summary>
	/// 武器の現在の状態
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere, Category = "Weapon Properties")
	EWeaponState WeaponState;

	/// <summary>
	/// 武器状態がレプリケートされた時に呼ばれる
	/// </summary>
	UFUNCTION()
	void OnRep_WeaponState();

	/// <summary>
	/// 拾得ウィジェット
	/// </summary>
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class UWidgetComponent* PickupWidget;

	/// <summary>
	/// 射撃アニメーション
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	class UAnimationAsset* FireAnimation;

	/// <summary>
	/// 薬莢のクラス
	/// </summary>
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACasing> CasingClass;

	/// <summary>
	/// 現在のマガジン内の弾薬数
	/// </summary>
	UPROPERTY(EditAnywhere)
	int32 Ammo;

	/// <summary>
	/// クライアントに弾薬数を更新
	/// </summary>
	/// <param name="ServerAmmo">サーバー側の弾薬数</param>
	UFUNCTION(Client, Reliable)
	void ClientUpdateAmmo(int32 ServerAmmo);

	/// <summary>
	/// クライアントに弾薬を追加
	/// </summary>
	/// <param name="AmmoToAdd">追加する弾薬数</param>
	UFUNCTION(Client, Reliable)
	void ClientAddAmmo(int32 AmmoToAdd);

	/// <summary>
	/// 1発消費
	/// </summary>
	void SpendRound();

	/// <summary>
	/// マガジン容量
	/// </summary>
	UPROPERTY(EditAnywhere)
	int32 MagCapacity;

	/// <summary>
	/// 未処理のサーバー弾薬リクエスト数
	/// SpendRoundで増加、ClientUpdateAmmoで減少
	/// </summary>
	int32 Sequence = 0;

	/// <summary>
	/// 武器タイプ
	/// </summary>
	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;

	/// <summary>
	/// 所属チーム
	/// </summary>
	UPROPERTY(EditAnywhere)
	ETeam Team;

public:	
	/// <summary>
	/// 武器状態を設定
	/// </summary>
	/// <param name="State">設定する状態</param>
	void SetWeaponState(EWeaponState State);

	/// <summary>
	/// エリア球体を取得
	/// </summary>
	FORCEINLINE USphereComponent* GetAreaSphere() const { return AreaSphere; }

	/// <summary>
	/// 武器メッシュを取得
	/// </summary>
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }

	/// <summary>
	/// 拾得ウィジェットを取得
	/// </summary>
	FORCEINLINE UWidgetComponent* GetPickupWidget() const { return PickupWidget; }

	/// <summary>
	/// ズームFOVを取得
	/// </summary>
	FORCEINLINE float GetZoomedFOV() const { return ZoomedFOV; }

	/// <summary>
	/// ズーム補間速度を取得
	/// </summary>
	FORCEINLINE float GetZoomInterpSpeed() const { return ZoomInterpSpeed; }

	/// <summary>
	/// 弾薬が空か
	/// </summary>
	bool IsEmpty();

	/// <summary>
	/// 弾薬が満タンか
	/// </summary>
	bool IsFull();

	/// <summary>
	/// 武器タイプを取得
	/// </summary>
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }

	/// <summary>
	/// 現在の弾薬数を取得
	/// </summary>
	FORCEINLINE int32 GetAmmo() const { return Ammo; }

	/// <summary>
	/// マガジン容量を取得
	/// </summary>
	FORCEINLINE int32 GetMagCapacity() const { return MagCapacity; }

	/// <summary>
	/// 基本ダメージを取得
	/// </summary>
	FORCEINLINE float GetDamage() const { return Damage; }

	/// <summary>
	/// ヘッドショットダメージを取得
	/// </summary>
	FORCEINLINE float GetHeadShotDamage() const { return HeadShotDamage; }

	/// <summary>
	/// 所属チームを取得
	/// </summary>
	FORCEINLINE ETeam GetTeam() const { return Team; }
};
