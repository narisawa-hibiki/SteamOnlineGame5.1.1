		// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Blaster/HUD/BlasterHUD.h"
#include "Blaster/Weapon/WeaponTypes.h"
#include "Blaster/BlasterTypes/CombatState.h"
#include "CombatComponent.generated.h"

/**
 * 戦闘コンポーネント
 * 武器の装備、射撃、リロード、グレネード投擲などの戦闘関連機能を管理
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	friend class ABlasterCharacter;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/// <summary>
	/// 武器を装備
	/// </summary>
	/// <param name="WeaponToEquip">装備する武器</param>
	void EquipWeapon(class AWeapon* WeaponToEquip);

	/// <summary>
	/// プライマリ武器とセカンダリ武器を入れ替え
	/// </summary>
	void SwapWeapons();

	/// <summary>
	/// リロードを開始
	/// </summary>
	void Reload();

	/// <summary>
	/// リロードアニメーション完了時に呼ばれる
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void FinishReloading();

	/// <summary>
	/// 武器交換アニメーション完了時に呼ばれる
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void FinishSwap();

	/// <summary>
	/// 武器交換時の武器アタッチ処理完了時に呼ばれる
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void FinishSwapAttachWeapons();

	/// <summary>
	/// 射撃ボタンの押下状態を設定
	/// </summary>
	/// <param name="bPressed">ボタンが押されているか</param>
	void FireButtonPressed(bool bPressed);

	/// <summary>
	/// ショットガンのシェル単位リロード時に呼ばれる
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void ShotgunShellReload();

	/// <summary>
	/// ショットガンのリロードを強制終了
	/// </summary>
	void JumpToShotgunEnd();

	/// <summary>
	/// グレネード投擲アニメーション完了時に呼ばれる
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void ThrowGrenadeFinished();

	/// <summary>
	/// グレネードを発射
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void LaunchGrenade();

	/// <summary>
	/// サーバーでグレネードを発射
	/// </summary>
	/// <param name="Target">グレネードの目標地点</param>
	UFUNCTION(Server, Reliable)
	void ServerLaunchGrenade(const FVector_NetQuantize& Target);

	/// <summary>
	/// 弾薬を拾得
	/// </summary>
	/// <param name="WeaponType">武器タイプ</param>
	/// <param name="AmmoAmount">弾薬数</param>
	void PickupAmmo(EWeaponType WeaponType, int32 AmmoAmount);

	/// <summary>
	/// ローカルでリロード中かのフラグ
	/// </summary>
	bool bLocallyReloading = false;

protected:
	virtual void BeginPlay() override;

	/// <summary>
	/// エイム状態を設定
	/// </summary>
	/// <param name="bIsAiming">エイム中か</param>
	void SetAiming(bool bIsAiming);

	/// <summary>
	/// サーバーでエイム状態を設定
	/// </summary>
	/// <param name="bIsAiming">エイム中か</param>
	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bIsAiming);

	/// <summary>
	/// 装備中の武器がレプリケートされた時に呼ばれる
	/// </summary>
	UFUNCTION()
	void OnRep_EquippedWeapon();

	/// <summary>
	/// セカンダリ武器がレプリケートされた時に呼ばれる
	/// </summary>
	UFUNCTION()
	void OnRep_SecondaryWeapon();

	/// <summary>
	/// 射撃を実行
	/// </summary>
	void Fire();

	/// <summary>
	/// プロジェクタイル武器の射撃
	/// </summary>
	void FireProjectileWeapon();

	/// <summary>
	/// ヒットスキャン武器の射撃
	/// </summary>
	void FireHitScanWeapon();

	/// <summary>
	/// ショットガンの射撃
	/// </summary>
	void FireShotgun();

	/// <summary>
	/// ローカルクライアントで射撃エフェクトを再生
	/// </summary>
	/// <param name="TraceHitTarget">ヒット位置</param>
	void LocalFire(const FVector_NetQuantize& TraceHitTarget);

	/// <summary>
	/// ローカルクライアントでショットガン射撃エフェクトを再生
	/// </summary>
	/// <param name="TraceHitTargets">各ペレットのヒット位置</param>
	void ShotgunLocalFire(const TArray<FVector_NetQuantize>& TraceHitTargets);

	/// <summary>
	/// サーバーで射撃を実行
	/// </summary>
	/// <param name="TraceHitTarget">ヒット位置</param>
	/// <param name="FireDelay">射撃遅延</param>
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFire(const FVector_NetQuantize& TraceHitTarget, float FireDelay);

	/// <summary>
	/// 全クライアントで射撃エフェクトを再生
	/// </summary>
	/// <param name="TraceHitTarget">ヒット位置</param>
	UFUNCTION(NetMulticast, Reliable)
	void MulticastFire(const FVector_NetQuantize& TraceHitTarget);

	/// <summary>
	/// サーバーでショットガン射撃を実行
	/// </summary>
	/// <param name="TraceHitTargets">各ペレットのヒット位置</param>
	/// <param name="FireDelay">射撃遅延</param>
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerShotgunFire(const TArray<FVector_NetQuantize>& TraceHitTargets, float FireDelay);

	/// <summary>
	/// 全クライアントでショットガン射撃エフェクトを再生
	/// </summary>
	/// <param name="TraceHitTargets">各ペレットのヒット位置</param>
	UFUNCTION(NetMulticast, Reliable)
	void MulticastShotgunFire(const TArray<FVector_NetQuantize>& TraceHitTargets);

	/// <summary>
	/// クロスヘア下のトレースを実行
	/// </summary>
	/// <param name="TraceHitResult">トレース結果</param>
	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	/// <summary>
	/// HUDのクロスヘアを更新
	/// </summary>
	/// <param name="DeltaTime">デルタ時間</param>
	void SetHUDCrosshairs(float DeltaTime);

	/// <summary>
	/// サーバーでリロードを実行
	/// </summary>
	UFUNCTION(Server, Reliable)
	void ServerReload();

	/// <summary>
	/// リロード処理を実行
	/// </summary>
	void HandleReload();

	/// <summary>
	/// リロードする弾薬数を計算
	/// </summary>
	/// <returns>リロードする弾薬数</returns>
	int32 AmountToReload();

	/// <summary>
	/// グレネードを投擲
	/// </summary>
	void ThrowGrenade();

	/// <summary>
	/// サーバーでグレネードを投擲
	/// </summary>
	UFUNCTION(Server, Reliable)
	void ServerThrowGrenade();

	/// <summary>
	/// グレネードのクラス
	/// </summary>
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> GrenadeClass;

	/// <summary>
	/// 装備中の武器をドロップ
	/// </summary>
	void DropEquippedWeapon();

	/// <summary>
	/// アクターを右手にアタッチ
	/// </summary>
	/// <param name="ActorToAttach">アタッチするアクター</param>
	void AttachActorToRightHand(AActor* ActorToAttach);

	/// <summary>
	/// アクターを左手にアタッチ
	/// </summary>
	/// <param name="ActorToAttach">アタッチするアクター</param>
	void AttachActorToLeftHand(AActor* ActorToAttach);

	/// <summary>
	/// フラグを左手にアタッチ
	/// </summary>
	/// <param name="Flag">アタッチするフラグ</param>
	void AttachFlagToLeftHand(AWeapon* Flag);

	/// <summary>
	/// アクターをバックパックにアタッチ
	/// </summary>
	/// <param name="ActorToAttach">アタッチするアクター</param>
	void AttachActorToBackpack(AActor* ActorToAttach);

	/// <summary>
	/// 所持弾薬数を更新
	/// </summary>
	void UpdateCarriedAmmo();

	/// <summary>
	/// 武器装備時の音を再生
	/// </summary>
	/// <param name="WeaponToEquip">装備する武器</param>
	void PlayEquipWeaponSound(AWeapon* WeaponToEquip);

	/// <summary>
	/// 弾薬が空の武器をリロード
	/// </summary>
	void ReloadEmptyWeapon();

	/// <summary>
	/// アタッチされたグレネードの表示/非表示を切り替え
	/// </summary>
	/// <param name="bShowGrenade">グレネードを表示するか</param>
	void ShowAttachedGrenade(bool bShowGrenade);

	/// <summary>
	/// プライマリ武器を装備
	/// </summary>
	/// <param name="WeaponToEquip">装備する武器</param>
	void EquipPrimaryWeapon(AWeapon* WeaponToEquip);

	/// <summary>
	/// セカンダリ武器を装備
	/// </summary>
	/// <param name="WeaponToEquip">装備する武器</param>
	void EquipSecondaryWeapon(AWeapon* WeaponToEquip);

private:
	/// <summary>
	/// このコンポーネントを所有するキャラクター
	/// </summary>
	UPROPERTY()
	class ABlasterCharacter* Character;

	/// <summary>
	/// プレイヤーコントローラー
	/// </summary>
	UPROPERTY()
	class ABlasterPlayerController* Controller;

	/// <summary>
	/// HUD
	/// </summary>
	UPROPERTY()
	class ABlasterHUD* HUD;

	/// <summary>
	/// 現在装備中の武器
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	AWeapon* EquippedWeapon;

	/// <summary>
	/// セカンダリ武器
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_SecondaryWeapon)
	AWeapon* SecondaryWeapon;

	/// <summary>
	/// エイム中かのフラグ
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_Aiming)
	bool bAiming = false;

	/// <summary>
	/// エイムボタンが押されているか
	/// </summary>
	bool bAimButtonPressed = false;

	/// <summary>
	/// エイム状態がレプリケートされた時に呼ばれる
	/// </summary>
	UFUNCTION()
	void OnRep_Aiming();

	/// <summary>
	/// 通常時の歩行速度
	/// </summary>
	UPROPERTY(EditAnywhere)
	float BaseWalkSpeed;

	/// <summary>
	/// エイム時の歩行速度
	/// </summary>
	UPROPERTY(EditAnywhere)
	float AimWalkSpeed;

	/// <summary>
	/// 射撃ボタンが押されているか
	/// </summary>
	bool bFireButtonPressed;

	/// <summary>
	/// クロスヘアの速度による拡散係数
	/// </summary>
	float CrosshairVelocityFactor;

	/// <summary>
	/// クロスヘアの空中時の拡散係数
	/// </summary>
	float CrosshairInAirFactor;

	/// <summary>
	/// クロスヘアのエイム時の縮小係数
	/// </summary>
	float CrosshairAimFactor;

	/// <summary>
	/// クロスヘアの射撃時の拡散係数
	/// </summary>
	float CrosshairShootingFactor;

	/// <summary>
	/// クロスヘア下のヒット位置
	/// </summary>
	FVector HitTarget;

	/// <summary>
	/// HUDクロスヘアパッケージ
	/// </summary>
	FHUDPackage HUDPackage;

	/// <summary>
	/// エイムしていない時のFOV（BeginPlayでカメラのベースFOVに設定）
	/// </summary>
	float DefaultFOV;

	/// <summary>
	/// エイム時のFOV
	/// </summary>
	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomedFOV = 30.f;

	/// <summary>
	/// 現在のFOV
	/// </summary>
	float CurrentFOV;

	/// <summary>
	/// ズーム時の補間速度
	/// </summary>
	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomInterpSpeed = 20.f;

	/// <summary>
	/// FOVを補間
	/// </summary>
	/// <param name="DeltaTime">デルタ時間</param>
	void InterpFOV(float DeltaTime);

	/// <summary>
	/// 射撃タイマーハンドル
	/// </summary>
	FTimerHandle FireTimer;

	/// <summary>
	/// 射撃可能か
	/// </summary>
	bool bCanFire = true;

	/// <summary>
	/// 射撃タイマーを開始
	/// </summary>
	void StartFireTimer();

	/// <summary>
	/// 射撃タイマー完了時に呼ばれる
	/// </summary>
	void FireTimerFinished();

	/// <summary>
	/// 射撃可能かをチェック
	/// </summary>
	/// <returns>射撃可能ならtrue</returns>
	bool CanFire();

	/// <summary>
	/// 現在装備中の武器の所持弾薬数
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_CarriedAmmo)
	int32 CarriedAmmo;

	/// <summary>
	/// 所持弾薬がレプリケートされた時に呼ばれる
	/// </summary>
	UFUNCTION()
	void OnRep_CarriedAmmo();

	/// <summary>
	/// 武器タイプごとの所持弾薬マップ
	/// </summary>
	TMap<EWeaponType, int32> CarriedAmmoMap;

	/// <summary>
	/// 最大所持弾薬数
	/// </summary>
	UPROPERTY(EditAnywhere)
	int32 MaxCarriedAmmo = 500;

	/// <summary>
	/// アサルトライフルの初期弾薬数
	/// </summary>
	UPROPERTY(EditAnywhere)
	int32 StartingARAmmo = 30;

	/// <summary>
	/// ロケットランチャーの初期弾薬数
	/// </summary>
	UPROPERTY(EditAnywhere)
	int32 StartingRocketAmmo = 0;

	/// <summary>
	/// ピストルの初期弾薬数
	/// </summary>
	UPROPERTY(EditAnywhere)
	int32 StartingPistolAmmo = 0;

	/// <summary>
	/// SMGの初期弾薬数
	/// </summary>
	UPROPERTY(EditAnywhere)
	int32 StartingSMGAmmo = 0;

	/// <summary>
	/// ショットガンの初期弾薬数
	/// </summary>
	UPROPERTY(EditAnywhere)
	int32 StartingShotgunAmmo = 0;

	/// <summary>
	/// スナイパーライフルの初期弾薬数
	/// </summary>
	UPROPERTY(EditAnywhere)
	int32 StartingSniperAmmo = 0;

	/// <summary>
	/// グレネードランチャーの初期弾薬数
	/// </summary>
	UPROPERTY(EditAnywhere)
	int32 StartingGrenadeLauncherAmmo = 0;

	/// <summary>
	/// 所持弾薬マップを初期化
	/// </summary>
	void InitializeCarriedAmmo();

	/// <summary>
	/// 現在の戦闘状態
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_CombatState)
	ECombatState CombatState = ECombatState::ECS_Unoccupied;

	/// <summary>
	/// 戦闘状態がレプリケートされた時に呼ばれる
	/// </summary>
	UFUNCTION()
	void OnRep_CombatState();

	/// <summary>
	/// 弾薬関連の値を更新
	/// </summary>
	void UpdateAmmoValues();

	/// <summary>
	/// ショットガンの弾薬関連の値を更新
	/// </summary>
	void UpdateShotgunAmmoValues();

	/// <summary>
	/// 所持グレネード数
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_Grenades)
	int32 Grenades = 4;

	/// <summary>
	/// グレネード数がレプリケートされた時に呼ばれる
	/// </summary>
	UFUNCTION()
	void OnRep_Grenades();

	/// <summary>
	/// 最大グレネード数
	/// </summary>
	UPROPERTY(EditAnywhere)
	int32 MaxGrenades = 4;

	/// <summary>
	/// HUDのグレネード数を更新
	/// </summary>
	void UpdateHUDGrenades();

	/// <summary>
	/// フラグを持っているか
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_HoldingTheFlag)
	bool bHoldingTheFlag = false;

	/// <summary>
	/// フラグ保持状態がレプリケートされた時に呼ばれる
	/// </summary>
	UFUNCTION()
	void OnRep_HoldingTheFlag();

	/// <summary>
	/// 保持しているフラグ
	/// </summary>
	UPROPERTY()
	AWeapon* TheFlag;

public:	
	/// <summary>
	/// 所持グレネード数を取得
	/// </summary>
	/// <returns>所持グレネード数</returns>
	FORCEINLINE int32 GetGrenades() const { return Grenades; }

	/// <summary>
	/// 武器を交換すべきかチェック
	/// </summary>
	/// <returns>交換すべきならtrue</returns>
	bool ShouldSwapWeapons();
};
