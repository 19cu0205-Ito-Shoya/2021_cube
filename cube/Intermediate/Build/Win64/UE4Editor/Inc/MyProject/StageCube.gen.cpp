// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "MyProject/StageCube.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeStageCube() {}
// Cross Module References
	MYPROJECT_API UClass* Z_Construct_UClass_AStageCube_NoRegister();
	MYPROJECT_API UClass* Z_Construct_UClass_AStageCube();
	ENGINE_API UClass* Z_Construct_UClass_APawn();
	UPackage* Z_Construct_UPackage__Script_MyProject();
// End Cross Module References
	void AStageCube::StaticRegisterNativesAStageCube()
	{
	}
	UClass* Z_Construct_UClass_AStageCube_NoRegister()
	{
		return AStageCube::StaticClass();
	}
	struct Z_Construct_UClass_AStageCube_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AStageCube_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_APawn,
		(UObject* (*)())Z_Construct_UPackage__Script_MyProject,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AStageCube_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Navigation" },
		{ "IncludePath", "StageCube.h" },
		{ "ModuleRelativePath", "StageCube.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AStageCube_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AStageCube>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_AStageCube_Statics::ClassParams = {
		&AStageCube::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009000A4u,
		METADATA_PARAMS(Z_Construct_UClass_AStageCube_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AStageCube_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AStageCube()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_AStageCube_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(AStageCube, 1792915999);
	template<> MYPROJECT_API UClass* StaticClass<AStageCube>()
	{
		return AStageCube::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_AStageCube(Z_Construct_UClass_AStageCube, &AStageCube::StaticClass, TEXT("/Script/MyProject"), TEXT("AStageCube"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(AStageCube);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
