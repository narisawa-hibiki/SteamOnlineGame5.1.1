// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BlasterHUD.generated.h"

/**
 * HUDパッケージ構造体
 * クロスヘアの表示に必要な全ての情報をパッケージ化
 * 武器ごとに異なるクロスヘアテクスチャ、拡散、色を設定可能
 */
USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()
public:
	/// <summary>
	/// クロスヘア中央のテクスチャ
	/// </summary>
	class UTexture2D* CrosshairsCenter;

	/// <summary>
	/// クロスヘア左側のテクスチャ
	/// </summary>
	UTexture2D* CrosshairsLeft;

	/// <summary>
	/// クロスヘア右側のテクスチャ
	/// </summary>
	UTexture2D* CrosshairsRight;

	/// <summary>
	/// クロスヘア上側のテクスチャ
	/// </summary>
	UTexture2D* CrosshairsTop;

	/// <summary>
	/// クロスヘア下側のテクスチャ
	/// </summary>
	UTexture2D* CrosshairsBottom;

	/// <summary>
	/// クロスヘアの拡散量
	/// 移動、射撃、ジャンプなどで動的に変化
	/// </summary>
	float CrosshairSpread;

	/// <summary>
	/// クロスヘアの色
	/// 敵にエイムしている時などに色を変更
	/// </summary>
	FLinearColor CrosshairsColor;
};

/**
 * ブラスターHUDクラス
 * ゲーム中の全てのHUD要素を管理
 * クロスヘア、キャラクターオーバーレイ、アナウンスメント、エリミネーション通知を表示
 */
UCLASS()
class BLASTER_API ABlasterHUD : public AHUD
{
	GENERATED_BODY()
public:
	/// <summary>
	/// HUD描画処理
	/// 毎フレーム呼ばれ、クロスヘアなどのカスタムHUD要素を描画
	/// </summary>
	virtual void DrawHUD() override;

	/// <summary>
	/// キャラクターオーバーレイウィジェットのクラス
	/// 体力、弾薬、スコアなどのゲームプレイ情報を表示
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<class UUserWidget> CharacterOverlayClass;

	/// <summary>
	/// キャラクターオーバーレイを画面に追加
	/// マッチ開始時に呼ばれる
	/// </summary>
	void AddCharacterOverlay();

	/// <summary>
	/// キャラクターオーバーレイウィジェットのインスタンス
	/// </summary>
	UPROPERTY()
	class UCharacterOverlay* CharacterOverlay;

	/// <summary>
	/// アナウンスメントウィジェットのクラス
	/// マッチ開始前のカウントダウンや終了時の結果を表示
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Announcements")
	TSubclassOf<UUserWidget> AnnouncementClass;

	/// <summary>
	/// アナウンスメントウィジェットのインスタンス
	/// </summary>
	UPROPERTY()
	class UAnnouncement* Announcement;

	/// <summary>
	/// アナウンスメントを画面に追加
	/// ウォームアップ時やクールダウン時に呼ばれる
	/// </summary>
	void AddAnnouncement();

	/// <summary>
	/// エリミネーション通知を追加
	/// プレイヤーがエリミネートされた時にキルフィードを表示
	/// </summary>
	/// <param name="Attacker">攻撃者のプレイヤー名</param>
	/// <param name="Victim">犠牲者のプレイヤー名</param>
	void AddElimAnnouncement(FString Attacker, FString Victim);

protected:
	virtual void BeginPlay() override;

private:
	/// <summary>
	/// このHUDを所有するプレイヤーコントローラー
	/// </summary>
	UPROPERTY()
	class APlayerController* OwningPlayer;

	/// <summary>
	/// 現在のクロスヘア情報を格納するパッケージ
	/// CombatComponentから更新される
	/// </summary>
	FHUDPackage HUDPackage;

	/// <summary>
	/// 個別のクロスヘアテクスチャを描画
	/// </summary>
	/// <param name="Texture">描画するテクスチャ</param>
	/// <param name="ViewportCenter">ビューポートの中心座標</param>
	/// <param name="Spread">クロスヘアの拡散オフセット</param>
	/// <param name="CrosshairColor">クロスヘアの色</param>
	void DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor);

	/// <summary>
	/// クロスヘア拡散の最大値
	/// これを超えてクロスヘアが広がらないように制限
	/// </summary>
	UPROPERTY(EditAnywhere)
	float CrosshairSpreadMax = 16.f;

	/// <summary>
	/// エリミネーション通知ウィジェットのクラス
	/// </summary>
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UElimAnnouncement> ElimAnnouncementClass;

	/// <summary>
	/// エリミネーション通知の表示時間（秒）
	/// この時間が経過すると通知が自動的に消える
	/// </summary>
	UPROPERTY(EditAnywhere)
	float ElimAnnouncementTime = 2.5f;

	/// <summary>
	/// エリミネーション通知タイマー完了時のコールバック
	/// 指定された通知を画面から削除
	/// </summary>
	/// <param name="MsgToRemove">削除する通知ウィジェット</param>
	UFUNCTION()
	void ElimAnnouncementTimerFinished(UElimAnnouncement* MsgToRemove);

	/// <summary>
	/// 現在表示中のエリミネーション通知の配列
	/// キルフィードとして複数の通知を同時に表示
	/// </summary>
	UPROPERTY()
	TArray<UElimAnnouncement*> ElimMessages;

public:
	/// <summary>
	/// HUDパッケージを設定
	/// CombatComponentから毎フレーム呼ばれ、クロスヘア情報を更新
	/// </summary>
	/// <param name="Package">設定するHUDパッケージ</param>
	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package) { HUDPackage = Package; }
};
