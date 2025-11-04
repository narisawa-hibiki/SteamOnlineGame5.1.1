// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractWithCrosshairsInterface.generated.h"

/**
 * クロスヘア相互作用インターフェース用のUObjectラッパークラス
 * Unreal Engine のリフレクションシステムで必要
 * このクラスは変更不要
 */
UINTERFACE(MinimalAPI)
class UInteractWithCrosshairsInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * クロスヘア相互作用インターフェース
 * クロスヘアのトレースによってヒット可能なオブジェクトを識別するためのマーカーインターフェース
 * このインターフェースを実装するクラス（キャラクター、武器など）は、
 * クロスヘアがその上に重なった時にクロスヘアの色や拡散を変更するために使用される
 * 
 * 現在は関数を持たないマーカーインターフェースとして機能
 */
class BLASTER_API IInteractWithCrosshairsInterface
{
	GENERATED_BODY()

	// このクラスにインターフェース関数を追加可能
	// このインターフェースを実装するクラスで継承される
public:
};
