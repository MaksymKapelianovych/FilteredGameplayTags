This plugin allows to create custom filtered gameplay tags. When variable of such type is used in BP only child tags will be allowed.
Filtering tag will be automatically added as native tag.

How to define filtered tag?

```
// Filtered tag that can only accept child tags of "Action" category
USTRUCT(BlueprintType, Blueprintable, meta = (GameplayTagFilter = "Action"))
struct EXAMPLE_API FActionTag : public FGameplayTag
{
	GENERATED_BODY()
	END_FILTERED_TAG_DECL(FActionTag, TEXT("Action"))
}; 
```

How it is looks in BP?
For example, we have a function SomeFunction with parameter Tag of type FInputTag. GameplayTagPicker will automatically filter tags and show only child tags of "Action"

![image](https://github.com/MaksymKapelianovych/FilteredGameplayTags/assets/48297221/bdd5389a-77c7-4225-9ad6-69a862ddaa1a)

![image](https://github.com/MaksymKapelianovych/FilteredGameplayTags/assets/48297221/a69111de-ed53-48d6-be2a-79df620ba3bd)

