// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * マルチプレイヤーセッションプラグインのモジュールクラス
 * プラグインのロード・アンロード時の初期化とクリーンアップを管理
 */
class FMultiplayerSessionsModule : public IModuleInterface
{
public:
	/// <summary>
	/// モジュールの起動時に呼ばれる
	/// モジュールがメモリにロードされた後に実行される
	/// </summary>
	virtual void StartupModule() override;

	/// <summary>
	/// モジュールのシャットダウン時に呼ばれる
	/// モジュールのクリーンアップ処理を行う
	/// </summary>
	virtual void ShutdownModule() override;
};
