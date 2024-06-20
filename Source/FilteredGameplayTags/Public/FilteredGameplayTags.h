#pragma once

#include "CoreMinimal.h"
#include "GameplayTagsManager.h"
#include "NativeGameplayTags.h"

// Example of adding native filtered tags from C++:

// In "YourTags.h"
//
// USTRUCT(meta = (GameplayTagFilter = "Action", PresentAsType = "GameplayTag")) // instead of PresentAsType you can manually register IPropertyTypeCustomization)
// struct FActionTag : public FGameplayTag
// {
// 	GENERATED_BODY()
// 	END_FILTERED_TAG_DECL( FActionTag, TEXT( "Action" ) )
// };
//
// USTRUCT(meta = (GameplayTagFilter = "Action.Melee", PresentAsType = "GameplayTag")) // instead of PresentAsType you can manually register IPropertyTypeCustomization)
// struct FMeleeTag : public FActionTag
// {
// 	GENERATED_BODY()
// 	END_FILTERED_TAG_DECL( FMeleeTag, TEXT( "Action.Melee" ) )
// };
//
//
// // You can define tag with macros (allows to specify comments)
// namespace ActionTags // namespace is not necessary, but it helps to keep things organized
// {
//		DECLARE_FILTERED_GAMEPLAY_TAG_EXTERN( FActionTag, Equip );
//		DECLARE_FILTERED_GAMEPLAY_TAG_EXTERN( FActionTag, Unequip );
// }
//
//
// // Or you can use FGameplayTagNativeAdder
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
// 		Walk = FActionTag::AddNativeTag( "Walk" );
// 		Run = FActionTag::AddNativeTag( "Run" );
// 		Jump = FActionTag::AddNativeTag( "Jump" );
//
// 		Melee_Cut = FMeleeTag::AddNativeTag( "Cut" );
// 		Melee_Slash = FMeleeTag::AddNativeTag( "Slash" );
// 		Melee_Pierce = FMeleeTag::AddNativeTag( "Pierce" );
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


// In "YourTags.cpp"
//
// #include "YourTags.h"
//
// // You can define tag with macros (allows to specify comments)
// namespace ActionTags // namespace is not necessary, but it helps to keep things organized
// {
//		DEFINE_FILTERED_GAMEPLAY_TAG_COMMENT(		 FActionTag, Equip, "Equip", "Tag to define equip ability" );
//		DEFINE_FILTERED_GAMEPLAY_TAG(				 FActionTag, Unequip, "Unequip" );
//		DEFINE_FILTERED_GAMEPLAY_TAG_STATIC_COMMENT( FActionTag, Reload, "Reload", "Tag to define reload ability" );
//		DEFINE_FILTERED_GAMEPLAY_TAG_STATIC(		 FActionTag, Fire, "Fire" );
// }
//
//
// // Or you can use FGameplayTagNativeAdder
// FNativeActionTags FNativeActionTags::StaticInstance;



/**
 * Declares a native gameplay tag that is defined in a cpp with UE_DEFINE_GAMEPLAY_TAG to allow other modules or code to use the created tag variable.
 */
#define DECLARE_FILTERED_GAMEPLAY_TAG_EXTERN( TagType, TagName ) extern TagType TagName;

/**
 * Defines a native gameplay tag with a comment that is externally declared in a header to allow other modules or code to use the created tag variable.
 */
#define DEFINE_FILTERED_GAMEPLAY_TAG_COMMENT( TagType, TagName, Tag, Comment ) \
	TagType TagName = []() \
	{ \
		static FNativeGameplayTag Internal{ UE_PLUGIN_NAME, UE_MODULE_NAME, FName( TagType::GetFullTagStr( Tag ) ), TEXT( Comment ), ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD }; \
		return TagType::ConvertChecked( Internal );	\
	}(); \
	static_assert( UE::GameplayTags::Private::HasFileExtension( __FILE__ ), "DEFINE_FILTERED_GAMEPLAY_TAG_COMMENT can only be used in .cpp files, if you're trying to share tags across modules, use DECLARE_FILTERED_GAMEPLAY_TAG_EXTERN in the public header, and DEFINE_FILTERED_GAMEPLAY_TAG_COMMENT in the private .cpp" );

/**
 * Defines a native gameplay tag with no comment that is externally declared in a header to allow other modules or code to use the created tag variable.
 */
#define DEFINE_FILTERED_GAMEPLAY_TAG( TagType, TagName, Tag ) \
	TagType TagName = []() \
	{ \
		static FNativeGameplayTag Internal{ UE_PLUGIN_NAME, UE_MODULE_NAME, FName( TagType::GetFullTagStr( Tag ) ), TEXT(""), ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD }; \
		return TagType::ConvertChecked( Internal );	\
	}(); \
	static_assert( UE::GameplayTags::Private::HasFileExtension( __FILE__ ), "DEFINE_FILTERED_GAMEPLAY_TAG can only be used in .cpp files, if you're trying to share tags across modules, use DECLARE_FILTERED_GAMEPLAY_TAG_EXTERN in the public header, and DEFINE_FILTERED_GAMEPLAY_TAG in the private .cpp" );

/**
 * Defines a native gameplay tag with a comment such that it's only available to the cpp file you define it in.
 */
#define DEFINE_FILTERED_GAMEPLAY_TAG_STATIC_COMMENT( TagType, TagName, Tag, Comment ) \
	static TagType TagName = []() \
	{ \
		static FNativeGameplayTag Internal{ UE_PLUGIN_NAME, UE_MODULE_NAME, FName( TagType::GetFullTagStr( Tag ) ), TEXT( Comment ), ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD }; \
		return TagType::ConvertChecked( Internal );	\
	}(); \
	static_assert( UE::GameplayTags::Private::HasFileExtension( __FILE__ ), "DEFINE_FILTERED_GAMEPLAY_TAG_STATIC_COMMENT can only be used in .cpp files, if you're trying to share tags across modules, use DECLARE_FILTERED_GAMEPLAY_TAG_EXTERN in the public header, and DEFINE_FILTERED_GAMEPLAY_TAG_COMMENT in the private .cpp" );

/**
 * Defines a native gameplay tag such that it's only available to the cpp file you define it in.
 */
#define DEFINE_FILTERED_GAMEPLAY_TAG_STATIC( TagType, TagName, Tag ) \
	static TagType TagName = []() \
	{ \
		static FNativeGameplayTag Internal{ UE_PLUGIN_NAME, UE_MODULE_NAME, FName( TagType::GetFullTagStr( Tag ) ), TEXT(""), ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD }; \
		return TagType::ConvertChecked( Internal );	\
	}(); \
	static_assert( UE::GameplayTags::Private::HasFileExtension( __FILE__ ), "DEFINE_FILTERED_GAMEPLAY_TAG_STATIC can only be used in .cpp files, if you're trying to share tags across modules, use DECLARE_FILTERED_GAMEPLAY_TAG_EXTERN in the public header, and DEFINE_FILTERED_GAMEPLAY_TAG in the private .cpp" );


template <typename TagT>
class TTypedTagStaticImpl
{
	friend TagT;

	static FString GetFullTagStr( const FString& TagBody )
	{
		checkf( !TagBody.IsEmpty(), TEXT( "Passed empty tag body!" ) );

		FString TagStr;
		FString RootTagStr = FString::Printf( TEXT( "%s." ), TagT::GetRootTagStr());
		if ( TagBody.StartsWith( RootTagStr ) == false)
		{
			TagStr = RootTagStr + TagBody;
		}
		else
		{
			TagStr = TagBody;
#if !UE_BUILD_SHIPPING && !UE_BUILD_TEST
			ensureAlwaysMsgf( false, TEXT( "Passed unnecessary prefix [%s] when creating a tag of type [%s] with the body [%s]" ),
				*RootTagStr, TNameOf<TagT>::GetName(), *TagBody );
#endif
		}

		return TagStr;
	}

	static TagT AddNativeTag( const FString& TagBody )
	{
		if ( ensure( TagBody.IsEmpty() == false ) == false )
		{
			return TagT();
		}

		FString TagStr = GetFullTagStr( TagBody );

		return UGameplayTagsManager::Get().AddNativeGameplayTag( FName( *TagStr ) );
	}

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	/** Intended for console commands/cheats: not for shipping code! */
	static FORCEINLINE TagT FindFromString_DebugOnly( const FString& PartialTagName )
	{
		return UGameplayTagsManager::Get().FindGameplayTagFromPartialString_Slow( PartialTagName );
	}
#endif

	static bool ExportTextItem( const TagT& Tag, FString& ValueStr, int32 PortFlags )
	{
		ValueStr += Tag.GetTagName().ToString();
		return true;
	}

	static TagT TryConvert( FGameplayTag VanillaTag, bool bChecked )
	{
		if ( VanillaTag.MatchesTag( StaticImpl.RootTag ) )
		{
			return TagT( VanillaTag );
		}
		else if ( VanillaTag.IsValid() && bChecked )
		{
			check( false );
		}
		return TagT();
	}

	TTypedTagStaticImpl()
	{
		// Force generate root tag, in case there is only filtered tags definition in C++ 
		StaticImpl.RootTag = UGameplayTagsManager::Get().AddNativeGameplayTag( TagT::GetRootTagStr() );
	}
	TagT RootTag;
	static TTypedTagStaticImpl StaticImpl;
};

template <typename TagT>
TTypedTagStaticImpl< TagT > TTypedTagStaticImpl< TagT >::StaticImpl;

// Intended to be the absolute last thing in the definition of a tag
#define END_FILTERED_TAG_DECL( TagType, TagRoot )	\
public:	\
	TagType() { }	\
	static TagType GetRootTag() { return TTypedTagStaticImpl<TagType>::StaticImpl.RootTag; }	\
	static const TCHAR* GetRootTagStr() { return TagRoot; }	\
	static FString GetFullTagStr( const FString& TagBody ) { return TTypedTagStaticImpl< TagType >::GetFullTagStr( TagBody ); }	\
	static TagType TryConvert( FGameplayTag FromTag ) { return TTypedTagStaticImpl< TagType >::TryConvert( FromTag, false ); }	\
	static TagType ConvertChecked( FGameplayTag FromTag ) { return TTypedTagStaticImpl< TagType >::TryConvert( FromTag, true ); }	\
	static TagType AddNativeTag( const FString& TagBody ) { return TTypedTagStaticImpl< TagType >::AddNativeTag( TagBody ); }	\
	bool ExportTextItem( FString& ValueStr, const TagType& DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope ) const	\
	{	\
		return TTypedTagStaticImpl< TagType >::ExportTextItem( *this, ValueStr, PortFlags );	\
	}	\
protected:	\
	TagType( FGameplayTag Tag ) { TagName = Tag.GetTagName(); }	\
	friend class TTypedTagStaticImpl< TagType >;	\
};	\
Expose_TNameOf( TagType )	\
template<>	\
struct TStructOpsTypeTraits< TagType > : public TStructOpsTypeTraitsBase2< TagType >	\
{	\
	enum	\
	{	\
		WithExportTextItem = true,	\
		WithImportTextItem = true	\
	};


