This plugin allows to create custom filtered gameplay tags. When variable of such type is used in BP only child tags will be allowed.
Filtering tag will be automatically added as native tag.

Filtered tags can be defined by inheriting FGameplayTag or other filtered tag:

```
// Filtered tag that can only accept child tags of "Action" category
 USTRUCT(meta = (GameplayTagFilter = "Action"))
 struct FActionTag : public FGameplayTag
 {
 	GENERATED_BODY()
 	END_FILTERED_TAG_DECL(FActionTag, TEXT("Action"))
 };

 USTRUCT(meta = (GameplayTagFilter = "Action.Melee"))
 struct FMeleeTag : public FActionTag
 {
 	GENERATED_BODY()
 	END_FILTERED_TAG_DECL(FMeleeTag, TEXT("Action.Melee"))
 };
```

Specific filtered tags can be added from C++ using FGameplayTagNativeAdder, which allows to use such tags anywhere else in C++ code:

```
// "YourTags.h"
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
FNativeActionTags FNativeActionTags::StaticInstance;

// Usage
FNativeActionTags::Get().Jump;
FNativeActionTags::Get().Melee_Cut;
```

There is two downsides now: 
 - filtered native tags can't have comments due to logic of handling native tags in GameplayTagsManager (only FNativeGameplayTag can have comments)
 - for every custom filtered tag struct custom IPropertyTypeCustomization needs to be registered (though it is the same as for FGameplayTag), otherwise you won't be able to set default values for tag variables
```
class FYourEditorModuleModule : public IModuleInterface
{
	public:
	virtual void StartupModule() override
	{
		...
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));
		PropertyModule.RegisterCustomPropertyTypeLayout(FActionTag::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FGameplayTagCustomizationPublic::MakeInstance));
		PropertyModule.RegisterCustomPropertyTypeLayout(FMeleeTag::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FGameplayTagCustomizationPublic::MakeInstance));
 	 	...
	}
	...
};
```


How it is looks in BP?
For example, we have a function SomeFunction with parameter Tag of type FActionTag. GameplayTagPicker will automatically filter tags and show only child tags of "Action"

![image](https://github.com/MaksymKapelianovych/FilteredGameplayTags/assets/48297221/80405499-d1bc-495d-bd50-ece2667c2a02)


