// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Blaster/BlasterTypes/TurningInPlace.h"
#include "Blaster/Interfaces/InteractWithCrosshairsInterface.h"
#include "Components/TimelineComponent.h"
#include "Blaster/BlasterTypes/CombatState.h"
#include "Blaster/BlasterTypes/Team.h"
#include "BlasterCharacter.generated.h"

/// <summary>
/// プレイヤーがゲームから退出した時に呼ばれるデリゲート
/// </summary>
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLeftGame);

/**
 * ブラスターキャラクタークラス
 * マルチプレイヤーFPSゲームのプレイヤーキャラクターを表現
 * 武器の装備、射撃、ダメージ処理、エリミネーション、チームシステムなどを管理
 */
UCLASS()
class BLASTER_API ABlasterCharacter : public ACharacter, public IInteractWithCrosshairsInterface
{
	GENERATED_BODY()

public:
	ABlasterCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;

	/// <summary>
	/// 射撃アニメーションモンタージュを再生
	/// </summary>
	/// <param name="bAiming">エイム中か</param>
	void PlayFireMontage(bool bAiming);

	/// <summary>
	/// リロードアニメーションモンタージュを再生
	/// </summary>
	void PlayReloadMontage();

	/// <summary>
	/// エリミネーションアニメーションモンタージュを再生
	/// </summary>
	void PlayElimMontage();

	/// <summary>
	/// グレネード投擲アニメーションモンタージュを再生
	/// </summary>
	void PlayThrowGrenadeMontage();

	/// <summary>
	/// 武器交換アニメーションモンタージュを再生
	/// </summary>
	void PlaySwapMontage();

	virtual void OnRep_ReplicatedMovement() override;

	/// <summary>
	/// キャラクターをエリミネート
	/// </summary>
	/// <param name="bPlayerLeftGame">プレイヤーがゲームから退出したか</param>
	void Elim(bool bPlayerLeftGame);

	/// <summary>
	/// 全クライアントでエリミネーションエフェクトを再生
	/// </summary>
	/// <param name="bPlayerLeftGame">プレイヤーがゲームから退出したか</param>
	UFUNCTION(NetMulticast, Reliable)
	void MulticastElim(bool bPlayerLeftGame);

	virtual void Destroyed() override;

	/// <summary>
	/// ゲームプレイを無効化するフラグ（エリミネート時など）
	/// </summary>
	UPROPERTY(Replicated)
	bool bDisableGameplay = false;

	/// <summary>
	/// スナイパースコープウィジェットの表示/非表示を切り替え（ブループリント実装）
	/// </summary>
	/// <param name="bShowScope">スコープを表示するか</param>
	UFUNCTION(BlueprintImplementableEvent)
	void ShowSniperScopeWidget(bool bShowScope);

	/// <summary>
	/// HUDの体力表示を更新
	/// </summary>
	void UpdateHUDHealth();

	/// <summary>
	/// HUDのシールド表示を更新
	/// </summary>
	void UpdateHUDShield();

	/// <summary>
	/// HUDの弾薬表示を更新
	/// </summary>
	void UpdateHUDAmmo();

	/// <summary>
	/// デフォルト武器をスポーン
	/// </summary>
	void SpawDefaultWeapon();

	/// <summary>
	/// サーバーサイド巻き戻し用のヒットボックスマップ
	/// ボーン名をキーとして各部位のヒットボックスを管理
	/// </summary>
	UPROPERTY()
	TMap<FName, class UBoxComponent*> HitCollisionBoxes;

	/// <summary>
	/// 武器交換が完了したか
	/// </summary>
	bool bFinishedSwapping = false;

	/// <summary>
	/// サーバーにゲーム退出を通知
	/// </summary>
	UFUNCTION(Server, Reliable)
	void ServerLeaveGame();

	/// <summary>
	/// ゲーム退出時のデリゲート
	/// </summary>
	FOnLeftGame OnLeftGame;

	/// <summary>
	/// 全クライアントでリード獲得エフェクトを表示
	/// </summary>
	UFUNCTION(NetMulticast, Reliable)
	void MulticastGainedTheLead();

	/// <summary>
	/// 全クライアントでリード喪失エフェクトを表示
	/// </summary>
	UFUNCTION(NetMulticast, Reliable)
	void MulticastLostTheLead();

	/// <summary>
	/// チームカラーを設定
	/// </summary>
	/// <param name="Team">チーム種別</param>
	void SetTeamColor(ETeam Team);

protected:
	virtual void BeginPlay() override;

	/// <summary>
	/// 前後移動入力処理
	/// </summary>
	/// <param name="Value">入力値</param>
	void MoveForward(float Value);

	/// <summary>
	/// 左右移動入力処理
	/// </summary>
	/// <param name="Value">入力値</param>
	void MoveRight(float Value);

	/// <summary>
	/// 左右視点回転入力処理
	/// </summary>
	/// <param name="Value">入力値</param>
	void Turn(float Value);

	/// <summary>
	/// 上下視点回転入力処理
	/// </summary>
	/// <param name="Value">入力値</param>
	void LookUp(float Value);

	/// <summary>
	/// 装備ボタン押下時の処理
	/// </summary>
	void EquipButtonPressed();

	/// <summary>
	/// しゃがみボタン押下時の処理
	/// </summary>
	void CrouchButtonPressed();

	/// <summary>
	/// リロードボタン押下時の処理
	/// </summary>
	void ReloadButtonPressed();

	/// <summary>
	/// エイムボタン押下時の処理
	/// </summary>
	void AimButtonPressed();

	/// <summary>
	/// エイムボタン解放時の処理
	/// </summary>
	void AimButtonReleased();

	/// <summary>
	/// エイムオフセットを計算（上半身の回転を調整）
	/// </summary>
	/// <param name="DeltaTime">デルタ時間</param>
	void AimOffset(float DeltaTime);

	/// <summary>
	/// エイムピッチ（上下方向）を計算
	/// </summary>
	void CalculateAO_Pitch();

	/// <summary>
	/// シミュレートされたプロキシの回転処理
	/// </summary>
	void SimProxiesTurn();

	virtual void Jump() override;

	/// <summary>
	/// 射撃ボタン押下時の処理
	/// </summary>
	void FireButtonPressed();

	/// <summary>
	/// 射撃ボタン解放時の処理
	/// </summary>
	void FireButtonReleased();

	/// <summary>
	/// ヒットリアクションモンタージュを再生
	/// </summary>
	void PlayHitReactMontage();

	/// <summary>
	/// グレネードボタン押下時の処理
	/// </summary>
	void GrenadeButtonPressed();

	/// <summary>
	/// 武器をドロップまたは破棄
	/// </summary>
	/// <param name="Weapon">対象の武器</param>
	void DropOrDestroyWeapon(AWeapon* Weapon);

	/// <summary>
	/// 全ての武器をドロップまたは破棄
	/// </summary>
	void DropOrDestroyWeapons();

	/// <summary>
	/// スポーン地点を設定
	/// </summary>
	void SetSpawnPoint();

	/// <summary>
	/// プレイヤーステート初期化完了時の処理
	/// </summary>
	void OnPlayerStateInitialized();

	/// <summary>
	/// ダメージ受信時のコールバック
	/// </summary>
	/// <param name="DamagedActor">ダメージを受けたアクター</param>
	/// <param name="Damage">ダメージ量</param>
	/// <param name="DamageType">ダメージタイプ</param>
	/// <param name="InstigatorController">攻撃者のコントローラー</param>
	/// <param name="DamageCauser">ダメージを与えたアクター</param>
	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);

	/// <summary>
	/// 関連クラスの取得とHUD初期化をポーリング
	/// </summary>
	void PollInit();

	/// <summary>
	/// その場での回転処理
	/// </summary>
	/// <param name="DeltaTime">デルタ時間</param>
	void RotateInPlace(float DeltaTime);

	// サーバーサイド巻き戻し用のヒットボックス

	/// <summary>
	/// 頭部のヒットボックス
	/// </summary>
	UPROPERTY(EditAnywhere)
	class UBoxComponent* head;

	/// <summary>
	/// 骨盤のヒットボックス
	/// </summary>
	UPROPERTY(EditAnywhere)
	UBoxComponent* pelvis;

	/// <summary>
	/// 背骨中部のヒットボックス
	/// </summary>
	UPROPERTY(EditAnywhere)
	UBoxComponent* spine_02;

	/// <summary>
	/// 背骨上部のヒットボックス
	/// </summary>
	UPROPERTY(EditAnywhere)
	UBoxComponent* spine_03;

	/// <summary>
	/// 左上腕のヒットボックス
	/// </summary>
	UPROPERTY(EditAnywhere)
	UBoxComponent* upperarm_l;

	/// <summary>
	/// 右上腕のヒットボックス
	/// </summary>
	UPROPERTY(EditAnywhere)
	UBoxComponent* upperarm_r;

	/// <summary>
	/// 左前腕のヒットボックス
	/// </summary>
	UPROPERTY(EditAnywhere)
	UBoxComponent* lowerarm_l;

	/// <summary>
	/// 右前腕のヒットボックス
	/// </summary>
	UPROPERTY(EditAnywhere)
	UBoxComponent* lowerarm_r;

	/// <summary>
	/// 左手のヒットボックス
	/// </summary>
	UPROPERTY(EditAnywhere)
	UBoxComponent* hand_l;

	/// <summary>
	/// 右手のヒットボックス
	/// </summary>
	UPROPERTY(EditAnywhere)
	UBoxComponent* hand_r;

	/// <summary>
	/// バックパックのヒットボックス
	/// </summary>
	UPROPERTY(EditAnywhere)
	UBoxComponent* backpack;

	/// <summary>
	/// ブランケットのヒットボックス
	/// </summary>
	UPROPERTY(EditAnywhere)
	UBoxComponent* blanket;

	/// <summary>
	/// 左太もものヒットボックス
	/// </summary>
	UPROPERTY(EditAnywhere)
	UBoxComponent* thigh_l;

	/// <summary>
	/// 右太ももののヒットボックス
	/// </summary>
	UPROPERTY(EditAnywhere)
	UBoxComponent* thigh_r;

	/// <summary>
	/// 左ふくらはぎのヒットボックス
	/// </summary>
	UPROPERTY(EditAnywhere)
	UBoxComponent* calf_l;

	/// <summary>
	/// 右ふくらはぎのヒットボックス
	/// </summary>
	UPROPERTY(EditAnywhere)
	UBoxComponent* calf_r;

	/// <summary>
	/// 左足のヒットボックス
	/// </summary>
	UPROPERTY(EditAnywhere)
	UBoxComponent* foot_l;

	/// <summary>
	/// 右足のヒットボックス
	/// </summary>
	UPROPERTY(EditAnywhere)
	UBoxComponent* foot_r;

private:
	/// <summary>
	/// カメラブーム（スプリングアーム）
	/// </summary>
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraBoom;

	/// <summary>
	/// フォローカメラ
	/// </summary>
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FollowCamera;

	/// <summary>
	/// 頭上に表示されるウィジェット
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* OverheadWidget;

	/// <summary>
	/// 現在重なっている武器
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	class AWeapon* OverlappingWeapon;

	/// <summary>
	/// 重なっている武器がレプリケートされた時に呼ばれる
	/// </summary>
	/// <param name="LastWeapon">前回重なっていた武器</param>
	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	/// <summary>
	/// 戦闘コンポーネント
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCombatComponent* Combat;

	/// <summary>
	/// バフコンポーネント
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	class UBuffComponent* Buff;

	/// <summary>
	/// ラグ補償コンポーネント
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	class ULagCompensationComponent* LagCompensation;

	/// <summary>
	/// サーバーで装備ボタン押下処理を実行
	/// </summary>
	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();

	/// <summary>
	/// エイムオフセットのヨー（左右回転）
	/// </summary>
	float AO_Yaw;

	/// <summary>
	/// 補間されたエイムオフセットのヨー
	/// </summary>
	float InterpAO_Yaw;

	/// <summary>
	/// エイムオフセットのピッチ（上下回転）
	/// </summary>
	float AO_Pitch;

	/// <summary>
	/// エイム開始時の回転
	/// </summary>
	FRotator StartingAimRotation;

	/// <summary>
	/// その場での回転状態
	/// </summary>
	ETurningInPlace TurningInPlace;

	/// <summary>
	/// その場で回転
	/// </summary>
	/// <param name="DeltaTime">デルタ時間</param>
	void TurnInPlace(float DeltaTime);

	/// <summary>
	/// 射撃武器アニメーションモンタージュ
	/// </summary>
	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* FireWeaponMontage;

	/// <summary>
	/// リロードアニメーションモンタージュ
	/// </summary>
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* ReloadMontage;

	/// <summary>
	/// ヒットリアクションアニメーションモンタージュ
	/// </summary>
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* HitReactMontage;

	/// <summary>
	/// エリミネーションアニメーションモンタージュ
	/// </summary>
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* ElimMontage;

	/// <summary>
	/// グレネード投擲アニメーションモンタージュ
	/// </summary>
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* ThrowGrenadeMontage;

	/// <summary>
	/// 武器交換アニメーションモンタージュ
	/// </summary>
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* SwapMontage;

	/// <summary>
	/// キャラクターが近い場合にカメラを非表示
	/// </summary>
	void HideCameraIfCharacterClose();

	/// <summary>
	/// カメラ非表示の距離閾値
	/// </summary>
	UPROPERTY(EditAnywhere)
	float CameraThreshold = 200.f;

	/// <summary>
	/// ルートボーンを回転させるか
	/// </summary>
	bool bRotateRootBone;

	/// <summary>
	/// 回転の閾値
	/// </summary>
	float TurnThreshold = 0.5f;

	/// <summary>
	/// 前フレームのプロキシ回転
	/// </summary>
	FRotator ProxyRotationLastFrame;

	/// <summary>
	/// 現在のプロキシ回転
	/// </summary>
	FRotator ProxyRotation;

	/// <summary>
	/// プロキシのヨー
	/// </summary>
	float ProxyYaw;

	/// <summary>
	/// 最後の移動レプリケーションからの経過時間
	/// </summary>
	float TimeSinceLastMovementReplication;

	/// <summary>
	/// 現在の速度を計算
	/// </summary>
	/// <returns>速度</returns>
	float CalculateSpeed();

	/// <summary>
	/// 最大体力
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxHealth = 100.f;

	/// <summary>
	/// 現在の体力
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Player Stats")
	float Health = 100.f;

	/// <summary>
	/// 体力がレプリケートされた時に呼ばれる
	/// </summary>
	/// <param name="LastHealth">前回の体力</param>
	UFUNCTION()
	void OnRep_Health(float LastHealth);

	/// <summary>
	/// 最大シールド
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxShield = 100.f;

	/// <summary>
	/// 現在のシールド
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_Shield, EditAnywhere, Category = "Player Stats")
	float Shield = 0.f;

	/// <summary>
	/// シールドがレプリケートされた時に呼ばれる
	/// </summary>
	/// <param name="LastShield">前回のシールド</param>
	UFUNCTION()
	void OnRep_Shield(float LastShield);

	/// <summary>
	/// プレイヤーコントローラー
	/// </summary>
	UPROPERTY()
	class ABlasterPlayerController* BlasterPlayerController;

	/// <summary>
	/// エリミネート済みか
	/// </summary>
	bool bElimmed = false;

	/// <summary>
	/// エリミネーションタイマーハンドル
	/// </summary>
	FTimerHandle ElimTimer;

	/// <summary>
	/// エリミネーション後のリスポーン遅延時間
	/// </summary>
	UPROPERTY(EditDefaultsOnly)
	float ElimDelay = 3.f;

	/// <summary>
	/// エリミネーションタイマー完了時の処理
	/// </summary>
	void ElimTimerFinished();

	/// <summary>
	/// ゲームから退出したか
	/// </summary>
	bool bLeftGame = false;

	/// <summary>
	/// ディゾルブエフェクト用のタイムライン
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* DissolveTimeline;

	/// <summary>
	/// ディゾルブトラック
	/// </summary>
	FOnTimelineFloat DissolveTrack;

	/// <summary>
	/// ディゾルブカーブ
	/// </summary>
	UPROPERTY(EditAnywhere)
	UCurveFloat* DissolveCurve;

	/// <summary>
	/// ディゾルブマテリアルを更新
	/// </summary>
	/// <param name="DissolveValue">ディゾルブ値</param>
	UFUNCTION()
	void UpdateDissolveMaterial(float DissolveValue);

	/// <summary>
	/// ディゾルブを開始
	/// </summary>
	void StartDissolve();

	/// <summary>
	/// 動的ディゾルブマテリアルインスタンス（実行時に変更可能）
	/// </summary>
	UPROPERTY(VisibleAnywhere, Category = Elim)
	UMaterialInstanceDynamic* DynamicDissolveMaterialInstance;

	/// <summary>
	/// ディゾルブマテリアルインスタンス（ブループリントで設定）
	/// </summary>
	UPROPERTY(VisibleAnywhere, Category = Elim)
	UMaterialInstance* DissolveMaterialInstance;

	/// <summary>
	/// 赤チーム用ディゾルブマテリアル
	/// </summary>
	UPROPERTY(EditAnywhere, Category = Elim)
	UMaterialInstance* RedDissolveMatInst;

	/// <summary>
	/// 赤チーム用マテリアル
	/// </summary>
	UPROPERTY(EditAnywhere, Category = Elim)
	UMaterialInstance* RedMaterial;

	/// <summary>
	/// 青チーム用ディゾルブマテリアル
	/// </summary>
	UPROPERTY(EditAnywhere, Category = Elim)
	UMaterialInstance* BlueDissolveMatInst;

	/// <summary>
	/// 青チーム用マテリアル
	/// </summary>
	UPROPERTY(EditAnywhere, Category = Elim)
	UMaterialInstance* BlueMaterial;

	/// <summary>
	/// オリジナルマテリアル
	/// </summary>
	UPROPERTY(EditAnywhere, Category = Elim)
	UMaterialInstance* OriginalMaterial;

	/// <summary>
	/// エリミネーションボットのパーティクルエフェクト
	/// </summary>
	UPROPERTY(EditAnywhere)
	UParticleSystem* ElimBotEffect;

	/// <summary>
	/// エリミネーションボットのパーティクルコンポーネント
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* ElimBotComponent;

	/// <summary>
	/// エリミネーションボットのサウンド
	/// </summary>
	UPROPERTY(EditAnywhere)
	class USoundCue* ElimBotSound;

	/// <summary>
	/// プレイヤーステート
	/// </summary>
	UPROPERTY()
	class ABlasterPlayerState* BlasterPlayerState;

	/// <summary>
	/// リード時の王冠エフェクト（Niagara）
	/// </summary>
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* CrownSystem;

	/// <summary>
	/// 王冠エフェクトのコンポーネント
	/// </summary>
	UPROPERTY()
	class UNiagaraComponent* CrownComponent;

	/// <summary>
	/// アタッチされたグレネードのメッシュ
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* AttachedGrenade;

	/// <summary>
	/// デフォルト武器のクラス
	/// </summary>
	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> DefaultWeaponClass;

	/// <summary>
	/// ゲームモード
	/// </summary>
	UPROPERTY()
	class ABlasterGameMode* BlasterGameMode;

public:
	/// <summary>
	/// 重なっている武器を設定
	/// </summary>
	/// <param name="Weapon">武器</param>
	void SetOverlappingWeapon(AWeapon* Weapon);

	/// <summary>
	/// 武器を装備しているか
	/// </summary>
	/// <returns>装備している場合true</returns>
	bool IsWeaponEquipped();

	/// <summary>
	/// エイム中か
	/// </summary>
	/// <returns>エイム中の場合true</returns>
	bool IsAiming();

	/// <summary>
	/// エイムオフセットのヨーを取得
	/// </summary>
	/// <returns>エイムオフセットのヨー</returns>
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }

	/// <summary>
	/// エイムオフセットのピッチを取得
	/// </summary>
	/// <returns>エイムオフセットのピッチ</returns>
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }

	/// <summary>
	/// 装備中の武器を取得
	/// </summary>
	/// <returns>装備中の武器</returns>
	AWeapon* GetEquippedWeapon();

	/// <summary>
	/// その場での回転状態を取得
	/// </summary>
	/// <returns>回転状態</returns>
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }

	/// <summary>
	/// ヒット目標位置を取得
	/// </summary>
	/// <returns>ヒット目標位置</returns>
	FVector GetHitTarget() const;

	/// <summary>
	/// フォローカメラを取得
	/// </summary>
	/// <returns>フォローカメラ</returns>
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/// <summary>
	/// ルートボーンを回転させるべきか
	/// </summary>
	/// <returns>回転させる場合true</returns>
	FORCEINLINE bool ShouldRotateRootBone() const { return bRotateRootBone; }

	/// <summary>
	/// エリミネート済みか
	/// </summary>
	/// <returns>エリミネート済みの場合true</returns>
	FORCEINLINE bool IsElimmed() const { return bElimmed; }

	/// <summary>
	/// 現在の体力を取得
	/// </summary>
	/// <returns>体力</returns>
	FORCEINLINE float GetHealth() const { return Health; }

	/// <summary>
	/// 体力を設定
	/// </summary>
	/// <param name="Amount">体力</param>
	FORCEINLINE void SetHealth(float Amount) { Health = Amount; }

	/// <summary>
	/// 最大体力を取得
	/// </summary>
	/// <returns>最大体力</returns>
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	/// <summary>
	/// 現在のシールドを取得
	/// </summary>
	/// <returns>シールド</returns>
	FORCEINLINE float GetShield() const { return Shield; }

	/// <summary>
	/// シールドを設定
	/// </summary>
	/// <param name="Amount">シールド</param>
	FORCEINLINE void SetShield(float Amount) { Shield = Amount; }

	/// <summary>
	/// 最大シールドを取得
	/// </summary>
	/// <returns>最大シールド</returns>
	FORCEINLINE float GetMaxShield() const { return MaxShield; }

	/// <summary>
	/// 戦闘状態を取得
	/// </summary>
	/// <returns>戦闘状態</returns>
	ECombatState GetCombatState() const;

	/// <summary>
	/// 戦闘コンポーネントを取得
	/// </summary>
	/// <returns>戦闘コンポーネント</returns>
	FORCEINLINE UCombatComponent* GetCombat() const { return Combat; }

	/// <summary>
	/// ゲームプレイが無効化されているか
	/// </summary>
	/// <returns>無効化されている場合true</returns>
	FORCEINLINE bool GetDisableGameplay() const { return bDisableGameplay; }

	/// <summary>
	/// リロードモンタージュを取得
	/// </summary>
	/// <returns>リロードモンタージュ</returns>
	FORCEINLINE UAnimMontage* GetReloadMontage() const { return ReloadMontage; }

	/// <summary>
	/// アタッチされたグレネードを取得
	/// </summary>
	/// <returns>グレネードメッシュ</returns>
	FORCEINLINE UStaticMeshComponent* GetAttachedGrenade() const { return AttachedGrenade; }

	/// <summary>
	/// バフコンポーネントを取得
	/// </summary>
	/// <returns>バフコンポーネント</returns>
	FORCEINLINE UBuffComponent* GetBuff() const { return Buff; }

	/// <summary>
	/// ローカルでリロード中か
	/// </summary>
	/// <returns>リロード中の場合true</returns>
	bool IsLocallyReloading();

	/// <summary>
	/// ラグ補償コンポーネントを取得
	/// </summary>
	/// <returns>ラグ補償コンポーネント</returns>
	FORCEINLINE ULagCompensationComponent* GetLagCompensation() const { return LagCompensation; }

	/// <summary>
	/// フラグを持っているか
	/// </summary>
	/// <returns>持っている場合true</returns>
	bool IsHoldingTheFlag() const;

	/// <summary>
	/// 所属チームを取得
	/// </summary>
	/// <returns>チーム</returns>
	ETeam GetTeam();

	/// <summary>
	/// フラグ保持状態を設定
	/// </summary>
	/// <param name="bHolding">保持しているか</param>
	void SetHoldingTheFlag(bool bHolding);
};
