// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LagCompensationComponent.generated.h"

/// <summary>
/// ヒットボックスの位置、回転、サイズ情報を保持する構造体
/// </summary>
USTRUCT(BlueprintType)
struct FBoxInformation
{
	GENERATED_BODY()

	/// <summary>
	/// コンストラクタ
	/// </summary>
	FBoxInformation()
		: Location(0.0f, 0.0f, 0.0f)
		, Rotation(0.0f, 0.0f, 0.0f)
		, BoxExtent(0.0f, 0.0f, 0.0f)
	{
	}

	/// <summary>
	/// ヒットボックスのワールド座標
	/// </summary>
	UPROPERTY()
	FVector Location;

	/// <summary>
	/// ヒットボックスの回転
	/// </summary>
	UPROPERTY()
	FRotator Rotation;

	/// <summary>
	/// ヒットボックスの半分のサイズ
	/// </summary>
	UPROPERTY()
	FVector BoxExtent;
};

/// <summary>
/// 特定の時刻におけるキャラクターの全ヒットボックス情報を保持するフレームパッケージ
/// ラグ補償のために過去のフレーム情報を記録
/// </summary>
USTRUCT(BlueprintType)
struct FFramePackage
{
	GENERATED_BODY()

	/// <summary>
	/// コンストラクタ
	/// </summary>
	FFramePackage()
		: Time(0.0f)
		, Character(nullptr)
	{
	}

	/// <summary>
	/// このフレームが記録された時刻
	/// </summary>
	UPROPERTY()
	float Time;

	/// <summary>
	/// 各ヒットボックスの情報をボーン名でマッピング
	/// </summary>
	UPROPERTY()
	TMap<FName, FBoxInformation> HitBoxInfo;

	/// <summary>
	/// このフレームパッケージが属するキャラクター
	/// </summary>
	UPROPERTY()
	ABlasterCharacter* Character;
};

/// <summary>
/// サーバーサイド巻き戻しの結果を保持する構造体
/// ヒット確認とヘッドショット判定を含む
/// </summary>
USTRUCT(BlueprintType)
struct FServerSideRewindResult
{
	GENERATED_BODY()

	/// <summary>
	/// ヒットが確認されたか
	/// </summary>
	UPROPERTY()
	bool bHitConfirmed;

	/// <summary>
	/// ヘッドショットだったか
	/// </summary>
	UPROPERTY()
	bool bHeadShot;
};

/// <summary>
/// ショットガン用のサーバーサイド巻き戻し結果を保持する構造体
/// 複数のキャラクターに対するヘッドショットとボディショットの数を記録
/// </summary>
USTRUCT(BlueprintType)
struct FShotgunServerSideRewindResult
{
	GENERATED_BODY()

	/// <summary>
	/// コンストラクタ
	/// </summary>
	FShotgunServerSideRewindResult()
	{
	}

	/// <summary>
	/// 各キャラクターに対するヘッドショットの数
	/// </summary>
	UPROPERTY()
	TMap<ABlasterCharacter*, uint32> HeadShots;

	/// <summary>
	/// 各キャラクターに対するボディショットの数
	/// </summary>
	UPROPERTY()
	TMap<ABlasterCharacter*, uint32> BodyShots;
};

/**
 * ラグ補償コンポーネント
 * サーバーサイド巻き戻しを使用してネットワークラグを補償
 * 過去のフレーム情報を保存し、クライアントのヒット時刻に基づいて検証
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API ULagCompensationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	ULagCompensationComponent();
	friend class ABlasterCharacter;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	/// <summary>
	/// フレームパッケージの内容をデバッグ表示
	/// </summary>
	/// <param name="Package">表示するフレームパッケージ</param>
	/// <param name="Color">表示する色</param>
	void ShowFramePackage(const FFramePackage& Package, const FColor& Color);

	/// <summary>
	/// ヒットスキャン武器用のサーバーサイド巻き戻し
	/// 指定された時刻のヒットボックス位置でヒット判定を行う
	/// </summary>
	/// <param name="HitCharacter">ヒットしたキャラクター</param>
	/// <param name="TraceStart">トレースの開始位置</param>
	/// <param name="HitLocation">ヒットした位置</param>
	/// <param name="HitTime">ヒットが発生した時刻</param>
	/// <returns>巻き戻しの結果</returns>
	FServerSideRewindResult ServerSideRewind(
		class ABlasterCharacter* HitCharacter, 
		const FVector_NetQuantize& TraceStart, 
		const FVector_NetQuantize& HitLocation, 
		float HitTime);

	/// <summary>
	/// プロジェクタイル武器用のサーバーサイド巻き戻し
	/// 弾道をシミュレートしてヒット判定を行う
	/// </summary>
	/// <param name="HitCharacter">ヒットしたキャラクター</param>
	/// <param name="TraceStart">トレースの開始位置</param>
	/// <param name="InitialVelocity">プロジェクタイルの初速度</param>
	/// <param name="HitTime">ヒットが発生した時刻</param>
	/// <returns>巻き戻しの結果</returns>
	FServerSideRewindResult ProjectileServerSideRewind(
		ABlasterCharacter* HitCharacter,
		const FVector_NetQuantize& TraceStart,
		const FVector_NetQuantize100& InitialVelocity,
		float HitTime
	);

	/// <summary>
	/// ショットガン用のサーバーサイド巻き戻し
	/// 複数のキャラクターに対するヒット判定を行う
	/// </summary>
	/// <param name="HitCharacters">ヒットしたキャラクターの配列</param>
	/// <param name="TraceStart">トレースの開始位置</param>
	/// <param name="HitLocations">各ヒット位置の配列</param>
	/// <param name="HitTime">ヒットが発生した時刻</param>
	/// <returns>ショットガン用の巻き戻し結果</returns>
	FShotgunServerSideRewindResult ShotgunServerSideRewind(
		const TArray<ABlasterCharacter*>& HitCharacters,
		const FVector_NetQuantize& TraceStart,
		const TArray<FVector_NetQuantize>& HitLocations,
		float HitTime);

	/// <summary>
	/// ヒットスキャン武器用のサーバーRPC
	/// クライアントからのスコア要求を処理
	/// </summary>
	UFUNCTION(Server, Reliable)
	void ServerScoreRequest(
		ABlasterCharacter* HitCharacter,
		const FVector_NetQuantize& TraceStart,
		const FVector_NetQuantize& HitLocation,
		float HitTime
	);

	/// <summary>
	/// プロジェクタイル武器用のサーバーRPC
	/// クライアントからのスコア要求を処理
	/// </summary>
	UFUNCTION(Server, Reliable)
	void ProjectileServerScoreRequest(
		ABlasterCharacter* HitCharacter,
		const FVector_NetQuantize& TraceStart,
		const FVector_NetQuantize100& InitialVelocity,
		float HitTime
	);

	/// <summary>
	/// ショットガン用のサーバーRPC
	/// クライアントからのスコア要求を処理
	/// </summary>
	UFUNCTION(Server, Reliable)
	void ShotgunServerScoreRequest(
		const TArray<ABlasterCharacter*>& HitCharacters,
		const FVector_NetQuantize& TraceStart,
		const TArray<FVector_NetQuantize>& HitLocations,
		float HitTime
	);

protected:
	virtual void BeginPlay() override;

	/// <summary>
	/// 現在のフレームパッケージを保存
	/// </summary>
	/// <param name="Package">保存するフレームパッケージ</param>
	void SaveFramePackage(FFramePackage& Package);

	/// <summary>
	/// 2つのフレーム間を補間して指定時刻のフレームを生成
	/// </summary>
	/// <param name="OlderFrame">古いフレーム</param>
	/// <param name="YoungerFrame">新しいフレーム</param>
	/// <param name="HitTime">補間する時刻</param>
	/// <returns>補間されたフレームパッケージ</returns>
	FFramePackage InterpBetweenFrames(const FFramePackage& OlderFrame, const FFramePackage& YoungerFrame, float HitTime);

	/// <summary>
	/// キャラクターの現在のヒットボックス位置をキャッシュ
	/// </summary>
	/// <param name="HitCharacter">対象のキャラクター</param>
	/// <param name="OutFramePackage">出力先のフレームパッケージ</param>
	void CacheBoxPositions(ABlasterCharacter* HitCharacter, FFramePackage& OutFramePackage);

	/// <summary>
	/// フレームパッケージの情報に基づいてヒットボックスを移動
	/// </summary>
	/// <param name="HitCharacter">対象のキャラクター</param>
	/// <param name="Package">適用するフレームパッケージ</param>
	void MoveBoxes(ABlasterCharacter* HitCharacter, const FFramePackage& Package);

	/// <summary>
	/// ヒットボックスを元の位置にリセット
	/// </summary>
	/// <param name="HitCharacter">対象のキャラクター</param>
	/// <param name="Package">元の位置情報を含むフレームパッケージ</param>
	void ResetHitBoxes(ABlasterCharacter* HitCharacter, const FFramePackage& Package);

	/// <summary>
	/// キャラクターメッシュのコリジョンを有効/無効化
	/// </summary>
	/// <param name="HitCharacter">対象のキャラクター</param>
	/// <param name="CollisionEnabled">設定するコリジョンタイプ</param>
	void EnableCharacterMeshCollision(ABlasterCharacter* HitCharacter, ECollisionEnabled::Type CollisionEnabled);

	/// <summary>
	/// 現在のフレームパッケージを履歴に保存
	/// </summary>
	void SaveFramePackage();

	/// <summary>
	/// 指定時刻のフレームを取得（補間を含む）
	/// </summary>
	/// <param name="HitCharacter">対象のキャラクター</param>
	/// <param name="HitTime">取得する時刻</param>
	/// <returns>指定時刻のフレームパッケージ</returns>
	FFramePackage GetFrameToCheck(ABlasterCharacter* HitCharacter, float HitTime);

	/// <summary>
	/// ヒットスキャン武器用のヒット確認
	/// </summary>
	/// <param name="Package">検証するフレームパッケージ</param>
	/// <param name="HitCharacter">ヒットしたキャラクター</param>
	/// <param name="TraceStart">トレースの開始位置</param>
	/// <param name="HitLocation">ヒットした位置</param>
	/// <returns>確認結果</returns>
	FServerSideRewindResult ConfirmHit(
		const FFramePackage& Package,
		ABlasterCharacter* HitCharacter,
		const FVector_NetQuantize& TraceStart,
		const FVector_NetQuantize& HitLocation);

	/// <summary>
	/// プロジェクタイル武器用のヒット確認
	/// </summary>
	/// <param name="Package">検証するフレームパッケージ</param>
	/// <param name="HitCharacter">ヒットしたキャラクター</param>
	/// <param name="TraceStart">トレースの開始位置</param>
	/// <param name="InitialVelocity">プロジェクタイルの初速度</param>
	/// <param name="HitTime">ヒット時刻</param>
	/// <returns>確認結果</returns>
	FServerSideRewindResult ProjectileConfirmHit(
		const FFramePackage& Package,
		ABlasterCharacter* HitCharacter,
		const FVector_NetQuantize& TraceStart,
		const FVector_NetQuantize100& InitialVelocity,
		float HitTime
	);

	/// <summary>
	/// ショットガン用のヒット確認
	/// </summary>
	/// <param name="FramePackages">検証するフレームパッケージの配列</param>
	/// <param name="TraceStart">トレースの開始位置</param>
	/// <param name="HitLocations">各ヒット位置の配列</param>
	/// <returns>ショットガン用の確認結果</returns>
	FShotgunServerSideRewindResult ShotgunConfirmHit(
		const TArray<FFramePackage>& FramePackages,
		const FVector_NetQuantize& TraceStart,
		const TArray<FVector_NetQuantize>& HitLocations
	);

private:
	/// <summary>
	/// このコンポーネントを所有するキャラクター
	/// </summary>
	UPROPERTY()
	ABlasterCharacter* Character;

	/// <summary>
	/// キャラクターのプレイヤーコントローラー
	/// </summary>
	UPROPERTY()
	class ABlasterPlayerController* Controller;

	/// <summary>
	/// フレームパッケージの履歴を保持する双方向リンクリスト
	/// </summary>
	TDoubleLinkedList<FFramePackage> FrameHistory;

	/// <summary>
	/// フレーム履歴の最大記録時間（秒）
	/// </summary>
	UPROPERTY(EditAnywhere)
	float MaxRecordTime = 4.f;
};
