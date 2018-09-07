// The Family

//#if WITH_EDITOR
//#include "SVRAssetFactory.h"
//#include "SVRConfigAsset.h"
//#include "AssetTypeCategories.h"
//
//USVRAssetFactory::USVRAssetFactory()
//{
//	bCreateNew = true;
//	bEditAfterNew = true;
//	//Configure the class that this factory creates
//	SupportedClass = USVRConfigAsset::StaticClass();
//}
//
//uint32 USVRAssetFactory::GetMenuCategories() const
//{
//	//Let's place this asset in the Blueprints category in the Editor
//	return EAssetTypeCategories::Misc;
//}
//
//UObject* USVRAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
//{
//	//Create the editor asset 
//	USVRConfigAsset* ConfigAsset = NewObject<USVRConfigAsset>(InParent, InClass, InName, Flags);
//	return ConfigAsset;
//}
//#endif
