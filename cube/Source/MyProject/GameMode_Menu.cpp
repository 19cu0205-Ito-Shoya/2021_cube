// ファイル名：GameMode_Menu.cpp
// 作成者：19CU0217-朱適
// 作成日：2021/08/03
// 更新履歴：
// 2021/08/03 ゲームモードの作成
#include "GameMode_Menu.h"

AGameMode_Menu::AGameMode_Menu()
	:Super()
{
	FString Path = "/Game/a/BP_PlayerContorller_Menu.BP_PlayerContorller_Menu_C";
	ConstructorHelpers::FObjectFinder<UClass> PlayerControllerBP(*Path);
	TSubclassOf<AActor> PlayerController = PlayerControllerBP.Object;

	if (PlayerController != nullptr)
	{
		PlayerControllerClass = PlayerController;
	}
}