// Copyright Jose Sotomayor


#include "AbilitySystem/Data/AttributeInfo.h"
#include "Aura/AuraLogChannels.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (const FAuraAttributeInfo& Info : AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("Can't Find Info for Attribute Tag [%s] on Attribute Info [%s]."), *AttributeTag.ToString(), *GetNameSafe(this));
	}
	return FAuraAttributeInfo();

}
