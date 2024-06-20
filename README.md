This plugin allows to create custom filtered gameplay tags. When variable of such type is used in BP only child tags will be allowed.
Filtering tag will be automatically added as native tag.

Filtered tags can be defined by inheriting FGameplayTag or other filtered tag:

```
// Filtered tag that can only accept child tags of "Action" category
 USTRUCT(meta = (GameplayTagFilter = "Action", PresentAsType = "GameplayTag"))
 struct FActionTag : public FGameplayTag
 {
 	GENERATED_BODY()
 	END_FILTERED_TAG_DECL(FActionTag, TEXT("Action"))
 };

 USTRUCT(meta = (GameplayTagFilter = "Action.Melee", PresentAsType = "GameplayTag"))
 struct FMeleeTag : public FActionTag
 {
 	GENERATED_BODY()
 	END_FILTERED_TAG_DECL(FMeleeTag, TEXT("Action.Melee"))
 };
```

`GameplayTagFilter` is responsible for indicating parent tag for filtering in TagPicker widget.

`PresentAsType` is responsible for setting type customization the same as for FGameplayTag (otherwise you need to register you own IPropertyTypeCustomization, but 99% you will want to use customization for FGameplayTag)

`END_FILTERED_TAG_DECL` register parent tag in TagManager, provides a bunch ob useful functions to add tags (such as `AddNativeTag`)

Specific filtered tags can be added from C++ using set of macros or FGameplayTagNativeAdder, which allows to use such tags anywhere else in C++ code.

```
// "YourTags.h"
namespace ActionTags // namespace is not necessary, but it helps to keep things organized
{
 	DECLARE_FILTERED_GAMEPLAY_TAG_EXTERN(FActionTag, Equip);
	DECLARE_FILTERED_GAMEPLAY_TAG_EXTERN(FActionTag, Unequip);
}

struct FNativeActionTags : public FGameplayTagNativeAdder
{
	virtual ~FNativeActionTags() {}
	
 	FActionTag Walk;
 	FActionTag Run;
 	FActionTag Jump;

 	FMeleeTag Melee_Cut;
 	FMeleeTag Melee_Slash;
 	FMeleeTag Melee_Pierce;
 	
 	virtual void AddTags() override
 	{
 		Walk = FActionTag::AddNativeTag("Walk");
 		Run = FActionTag::AddNativeTag("Run");
 		Jump = FActionTag::AddNativeTag("Jump");

 		Melee_Cut = FMeleeTag::AddNativeTag("Cut");
 		Melee_Slash = FMeleeTag::AddNativeTag("Slash");
 		Melee_Pierce = FMeleeTag::AddNativeTag("Pierce");
 	}

 	FORCEINLINE static const FNativeActionTags& Get()
 	{
 		return StaticInstance;
	}

private:
	static FNativeActionTags StaticInstance;
};


// "YourTags.cpp"
namespace ActionTags // namespace is not necessary, but it helps to keep things organized
{
	DEFINE_FILTERED_GAMEPLAY_TAG_COMMENT(FActionTag, Equip, "Equip", "Tag to define equip ability");
	DEFINE_FILTERED_GAMEPLAY_TAG(FActionTag, Unequip, "Unequip");
	DEFINE_FILTERED_GAMEPLAY_TAG_STATIC_COMMENT(FActionTag, Reload, "Reload", "Tag to define reload ability");
	DEFINE_FILTERED_GAMEPLAY_TAG_STATIC(FActionTag, Fire, "Fire");
}

FNativeActionTags FNativeActionTags::StaticInstance;


// Usage
ActionTags::Equip;

FNativeActionTags::Get().Jump;
FNativeActionTags::Get().Melee_Cut;
```

There is one downside now: 
 - tags added by `FYourTag::AddNativeTag` function can't have comments due to logic of handling native tags in GameplayTagsManager (only FNativeGameplayTag can have comments). But you can use macros to add tags with comment


How it is looks in BP?
For example, we have a function SomeFunction with parameter Tag of type FActionTag. GameplayTagPicker will automatically filter tags and show only child tags of "Action"

![image](https://github.com/MaksymKapelianovych/FilteredGameplayTags/assets/48297221/80405499-d1bc-495d-bd50-ece2667c2a02)


