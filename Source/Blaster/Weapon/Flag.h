// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Flag.generated.h"

/**
 * フラグクラス
 * キャプチャー・ザ・フラッグ（CTF）モード用の旗を表現
 * 武器クラスを継承しているが、攻撃機能は持たず、運搬対象として機能
 * 初期位置を記憶し、リセット時に元の場所に戻る
 */
UCLASS()
class BLASTER_API AFlag : public AWeapon
{
	GENERATED_BODY()
public:
	AFlag();
	
	/// <summary>
	/// フラグをドロップ
	/// プレイヤーがフラグを手放した時の処理
	/// </summary>
	virtual void Dropped() override;
	
	/// <summary>
	/// フラグを初期位置にリセット
	/// スコア成功時やタイムアウト時に呼ばれる
	/// </summary>
	void ResetFlag();

protected:
	/// <summary>
	/// フラグが装備された時の処理
	/// </summary>
	virtual void OnEquipped() override;
	
	/// <summary>
	/// フラグがドロップされた時の処理
	/// </summary>
	virtual void OnDropped() override;
	
	virtual void BeginPlay() override;

private:
	/// <summary>
	/// フラグのメッシュコンポーネント
	/// 旗の見た目を表示
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* FlagMesh;

	/// <summary>
	/// フラグの初期位置とトランスフォーム
	/// リセット時に元の位置に戻すために保存
	/// </summary>
	FTransform InitialTransform;

public:
	/// <summary>
	/// フラグの初期トランスフォームを取得
	/// </summary>
	/// <returns>初期トランスフォーム</returns>
	FORCEINLINE FTransform GetInitialTransform() const { return InitialTransform; }
};
