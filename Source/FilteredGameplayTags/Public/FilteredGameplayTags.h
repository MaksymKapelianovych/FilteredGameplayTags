// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagsManager.h"

// Example of adding native filtered tags from C++:
//
// In "YourTags.h"
//
// USTRUCT(meta = (GameplayTagFilter = "Action"))
// struct FActionTag : public FGameplayTag
// {
// 	GENERATED_BODY()
// 	END_FILTERED_TAG_DECL(FActionTag, TEXT("Action"))
// };
//
// USTRUCT(meta = (GameplayTagFilter = "Action.Melee"))
// struct FMeleeTag : public FActionTag
// {
// 	GENERATED_BODY()
// 	END_FILTERED_TAG_DECL(FMeleeTag, TEXT("Action.Melee"))
// };
//
// struct FNativeActionTags : public FGameplayTagNativeAdder
// {
// 	virtual ~FNativeActionTags() {}
// 	
// 	FActionTag Walk;
// 	FActionTag Run;
// 	FActionTag Jump;
//
// 	FMeleeTag Melee_Cut;
// 	FMeleeTag Melee_Slash;
// 	FMeleeTag Melee_Pierce;
// 	
// 	virtual void AddTags() override
// 	{
// 		Walk = FActionTag::AddNativeTag("Walk");
// 		Run = FActionTag::AddNativeTag("Run");
// 		Jump = FActionTag::AddNativeTag("Jump");
// 		Melee_Cut = FMeleeTag::AddNativeTag("Cut");
// 		Melee_Slash = FMeleeTag::AddNativeTag("Slash");
// 		Melee_Pierce = FMeleeTag::AddNativeTag("Pierce");
// 	}
//
// 	FORCEINLINE static const FNativeActionTags& Get()
// 	{
// 		return StaticInstance;
// 	}
//
// private:
// 	static FNativeActionTags StaticInstance;
// };
//
// In "YourTags.cpp"
//
// #include "YourTags.h"
// FNativeActionTags FNativeActionTags::StaticInstance;
//
// In "YourEditorModule.cpp"
//
// #include "GameplayTagsEditorModule.h"
// ...
//
// class FYourEditorModuleModule : public IModuleInterface
// {
// public:
// 	virtual void StartupModule() override
// 	{
//		...
// 		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));
// 		PropertyModule.RegisterCustomPropertyTypeLayout(FActionTag::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FGameplayTagCustomizationPublic::MakeInstance));
// 		PropertyModule.RegisterCustomPropertyTypeLayout(FMeleeTag::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FGameplayTagCustomizationPublic::MakeInstance));
//      ...
// 	}
//  ...
// };
// 

template <typename TagT>
class TTypedTagStaticImpl
{
	friend TagT;

	static TagT AddNativeTag(const FString& TagBody)
	{
		if (!ensure(!TagBody.IsEmpty()))
		{
			return TagT();
		}

		FString TagStr;
		FString RootTagStr = FString::Printf(TEXT("%s."), TagT::GetRootTagStr());
		if (!TagBody.StartsWith(RootTagStr))
		{
			TagStr = RootTagStr + TagBody;
		}
		else
		{
			TagStr = TagBody;
#if !UE_BUILD_SHIPPING && !UE_BUILD_TEST
			ensureAlwaysMsgf(false, TEXT("Passed unnecessary prefix [%s] when creating a tag of type [%s] with the body [%s]"),
				*RootTagStr, TNameOf<TagT>::GetName(), *TagBody);
#endif
		}

		return UGameplayTagsManager::Get().AddNativeGameplayTag(FName(*TagStr));
	}

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	/** Intended for console commands/cheats: not for shipping code! */
	static FORCEINLINE TagT FindFromString_DebugOnly(const FString& PartialTagName)
	{
		return UGameplayTagsManager::Get().FindGameplayTagFromPartialString_Slow(PartialTagName);
	}
#endif

	static bool ExportTextItem(const TagT& Tag, FString& ValueStr, int32 PortFlags)
	{
		ValueStr += Tag.GetTagName().ToString();
		return true;
	}

	static TagT TryConvert(FGameplayTag VanillaTag, bool bChecked)
	{
		if (VanillaTag.MatchesTag(StaticImpl.RootTag))
		{
			return TagT(VanillaTag);
		}
		else if (VanillaTag.IsValid() && bChecked)
		{
			check(false);
		}
		return TagT();
	}

	TTypedTagStaticImpl()
	{ 
		LLM_SCOPE(ELLMTag::UI);
		UGameplayTagsManager::OnLastChanceToAddNativeTags().AddLambda([this]()
			{
				StaticImpl.RootTag = UGameplayTagsManager::Get().AddNativeGameplayTag(TagT::GetRootTagStr());
			});
	}
	TagT RootTag;
	static TTypedTagStaticImpl StaticImpl;
};

template <typename TagT>
TTypedTagStaticImpl<TagT> TTypedTagStaticImpl<TagT>::StaticImpl;

// Intended to be the absolute last thing in the definition of a tag
#define END_FILTERED_TAG_DECL(TagType, TagRoot)	\
public:	\
	TagType() { }	\
	static TagType GetRootTag() { return TTypedTagStaticImpl<TagType>::StaticImpl.RootTag; }	\
	static TagType TryConvert(FGameplayTag FromTag) { return TTypedTagStaticImpl<TagType>::TryConvert(FromTag, false); }	\
	static TagType ConvertChecked(FGameplayTag FromTag) { return TTypedTagStaticImpl<TagType>::TryConvert(FromTag, true); }	\
	static TagType AddNativeTag(const FString& TagBody) { return TTypedTagStaticImpl<TagType>::AddNativeTag(TagBody); }	\
	bool ExportTextItem(FString& ValueStr, const TagType& DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope) const	\
	{	\
		return TTypedTagStaticImpl<TagType>::ExportTextItem(*this, ValueStr, PortFlags);	\
	}	\
protected:	\
	TagType(FGameplayTag Tag) { TagName = Tag.GetTagName(); }	\
	static const TCHAR* GetRootTagStr() { return TagRoot; }	\
	friend class TTypedTagStaticImpl<TagType>;	\
};	\
Expose_TNameOf(TagType)	\
template<>	\
struct TStructOpsTypeTraits<TagType> : public TStructOpsTypeTraitsBase2<TagType>	\
{	\
	enum	\
	{	\
		WithExportTextItem = true,	\
		WithImportTextItem = true	\
	};


