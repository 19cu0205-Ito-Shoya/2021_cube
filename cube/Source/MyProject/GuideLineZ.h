//-------------------------------------------------------------------
// ファイル		：GuideLineZ.h
// 概要			：回転用ガイドラインの処理
// 作成者		：19CU0236 林雲暉 
// 作成日		：2021/08/12		回転用ガイドラインの基本構成制作
// 更新日		：2021/08/13		選択機能実装、マテリアル変えれる
//				：2021/08/14		所属軸情報追加、表示非表示追加
//-------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "GuideLineZ.generated.h"

UCLASS()
class MYPROJECT_API AGuideLineZ : public AActor
{
	GENERATED_BODY()
	
public:	
	// コンストラクタ
	AGuideLineZ();

	// デストラクタ
	~AGuideLineZ();

protected:
	// 実行時に一度呼ばれる
	virtual void BeginPlay() override;

public:	
	// 毎フレーム呼ばれる
	virtual void Tick(float DeltaTime) override;


public:
	// 本体のメッシュ
	UPROPERTY(EditAnywhere, Category = "GuideLineDetails|Mesh")
		UStaticMeshComponent* mGuideLineMesh;

	// シーンコンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GuideLineDetails", meta = (AllowPrivateAccess = "true"))
		USceneComponent* mRootComponent;

	// 選択されましたか?
	UPROPERTY(VisibleAnywhere, Category = "GuideLineDetails")
		bool mIsSelected;

	// 表示されましたか?
	UPROPERTY(VisibleAnywhere, Category = "GuideLineDetails")
		bool mIsVisible;

	//軸はどっち？
	// mode 0 = XY-Plane(Z-axis),  1 = YZ-Plane(X-axis),  2 = XZ-Plane(Y-axis)
	UPROPERTY(VisibleAnywhere, Category = "GuideLineDetails")
		int mode;

private:
	// Default Material デフォルトマテリアル
	UPROPERTY(EditAnywhere, Category = "GuideLineDetails|Mesh")
		UMaterial* mLineMaterial_1;

	// Chosen Material 選択されたマテリアル
	UPROPERTY(EditAnywhere, Category = "GuideLineDetails|Mesh")
		UMaterial* mLineMaterial_2;

	// Cursor Over's Material マウスが上にいる時のマテリアル
	UPROPERTY(EditAnywhere, Category = "GuideLineDetails|Mesh")
		UMaterial* mLineMaterial_3;


private:
	UFUNCTION()
		void OnSelected(AActor* Target, FKey ButtonPressed);

public:
	// マテリアルを変える
	UFUNCTION()
		void ChangeMaterialFunc();

	// 可視性を変える
	UFUNCTION()
		void ChangeVisibilityFunc();

};
