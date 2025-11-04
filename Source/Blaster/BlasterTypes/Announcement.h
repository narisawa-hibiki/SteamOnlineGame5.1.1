#pragma once

/**
 * ゲーム内アナウンスメント用の文字列定数を定義する名前空間
 * マッチ開始、終了、勝敗結果などの表示テキストを管理
 */
namespace Announcement
{
	/// <summary>
	/// 新しいマッチ開始までのカウントダウン表示
	/// </summary>
	const FString NewMatchStartsIn(TEXT("New match starts in:"));

	/// <summary>
	/// 勝者なし（引き分け）の表示
	/// </summary>
	const FString ThereIsNoWinner(TEXT("There is no winner."));

	/// <summary>
	/// プレイヤー自身が勝者の場合の表示
	/// </summary>
	const FString YouAreTheWinner(TEXT("You are the winner!"));

	/// <summary>
	/// 複数プレイヤーが同点で勝利した場合の表示
	/// </summary>
	const FString PlayersTiedForTheWin(TEXT("Players tied for the win:"));

	/// <summary>
	/// 複数チームが同点で勝利した場合の表示
	/// </summary>
	const FString TeamsTiedForTheWin(TEXT("Teams tied for the win:"));

	/// <summary>
	/// 赤チームの名称
	/// </summary>
	const FString RedTeam(TEXT("Red team"));

	/// <summary>
	/// 青チームの名称
	/// </summary>
	const FString BlueTeam(TEXT("Blue team"));

	/// <summary>
	/// 赤チーム勝利時の表示
	/// </summary>
	const FString RedTeamWins(TEXT("Red team wins!"));

	/// <summary>
	/// 青チーム勝利時の表示
	/// </summary>
	const FString BlueTeamWins(TEXT("Blue team wins!"));
}