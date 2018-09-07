// The Family

#include "SVRConfigAsset.h"
#include "Runtime/Json/Public/Dom/JsonObject.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "EngineMinimal.h"
#include "HAL/FileManager.h"

USVRConfigAsset::USVRConfigAsset()
{
	LoadConfig();
}

#if WITH_EDITOR
void USVRConfigAsset::PostEditChangeProperty(struct FPropertyChangedEvent& Event)
{
	FName PropertyName = (Event.Property != NULL) ? Event.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(USVRConfigAsset, ResetSettings))
	{
		ResetSettings = false;
		LoadConfig();
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(USVRConfigAsset, SaveSettings))
	{
		SaveSettings = false;
		SaveConfig();
	}
	Super::PostEditChangeProperty(Event);
}
#endif

void USVRConfigAsset::LoadConfig()
{
	FString FullPath = FPaths::ProjectDir() + RelativeConfigPath;
	FString JsonStr;
	FFileHelper::LoadFileToString(JsonStr, *FullPath);
	TSharedRef<TJsonReader<TCHAR>> JsonReader = FJsonStringReader::Create(JsonStr);
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	bool Serialized = FJsonSerializer::Deserialize(JsonReader, JsonObject);
	if (!Serialized)
	{
		return;
	}
	
	auto Maps = JsonObject->GetArrayField("Maps");
	MapsNames.Empty();
	for (int32 i = 0; i < Maps.Num(); i++)
	{
		MapsNames.Add(Maps[i]->AsString());
	}
	StartupMapName = JsonObject->GetStringField(TEXT("StartupMapName"));

	MaxPlayers = JsonObject->GetNumberField(TEXT("MaxPlayers"));
	PlayerMaxSpeed = JsonObject->GetIntegerField(TEXT("PlayerMaxSpeed"));	

	ServerConsoleCommandsOnStartup.Empty();
	auto Commands = JsonObject->GetArrayField("ServerConsoleCommandsOnStartup");
	for (int32 i = 0; i < Commands.Num(); i++)
	{
		ServerConsoleCommandsOnStartup.Add(Commands[i]->AsString());
	}

	ClientConsoleCommandsOnStartup.Empty();
	Commands = JsonObject->GetArrayField("ClientConsoleCommandsOnStartup");
	for (int32 i = 0; i < Commands.Num(); i++)
	{
		ClientConsoleCommandsOnStartup.Add(Commands[i]->AsString());
	}

	FString _ClientLaunchType = JsonObject->GetStringField(TEXT("ClientLaunchType"));
	ClientLaunchType = GetEnumValueFromString<EClientLaunchType>("EClientLaunchType", _ClientLaunchType);

	const FString _ClientControllerType = JsonObject->GetStringField(TEXT("ClientControllerType"));
	ClientControllerType = GetEnumValueFromString<EClientControllerType>("EClientControllerType", _ClientControllerType);


	auto TeamDataJson = JsonObject->GetArrayField("TeamData");
	TeamData.Empty();
	for (int32 i = 0; i < TeamDataJson.Num(); i++)
	{
		auto TeamObject = TeamDataJson[i]->AsObject();
		FString Name = TeamObject->GetStringField("Name");
		int32 ID = TeamObject->GetIntegerField("ID");
		ETeamID TeamIDl = ETeamID(ID);

		auto ColorJson = TeamObject->GetStringField("Color");
		FLinearColor Color;
		Color.InitFromString(ColorJson);

		TeamData.Add(ETeamID(ID), FTeamData(TeamIDl, Name, Color));
	}

	PlayerName = JsonObject->GetStringField(TEXT("PlayerName"));
	TeamID = ETeamID(JsonObject->GetIntegerField(TEXT("TeamID")));
}

FTeamData USVRConfigAsset::GetTeamDataByID(ETeamID ID)
{
	return TeamData[ID];
}

FString USVRConfigAsset::ChangeToNextMap()
{
	CurrentMapIndex = ++CurrentMapIndex % MapsNames.Num();
	return MapsNames[CurrentMapIndex];
}

FString USVRConfigAsset::GetCurrentMapName()
{
	return MapsNames[CurrentMapIndex];
}

void USVRConfigAsset::SaveConfig()
{
	// Create a writer and hold it in this FString
	FString JsonStr;
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	JsonObject->SetStringField("StartupMapName", StartupMapName);
	JsonObject->SetStringField("PlayerName", PlayerName);
	JsonObject->SetNumberField("MaxPlayers", MaxPlayers);
	JsonObject->SetNumberField("PlayerMaxSpeed", PlayerMaxSpeed);
	JsonObject->SetNumberField("TeamID", int(TeamID));	

	TArray< TSharedPtr<FJsonValue> > JsonObjectMaps;
	for (int32 i = 0; i < MapsNames.Num(); i++)
	{
		TSharedPtr<FJsonValue> MapName = MakeShareable(new FJsonValueString(MapsNames[i]));
		JsonObjectMaps.Add(MapName);
	}
	JsonObject->SetArrayField("Maps", JsonObjectMaps);

	TArray< TSharedPtr<FJsonValue> > JsonObjectConsole;
	for (int32 i = 0; i < ServerConsoleCommandsOnStartup.Num(); i++)
	{
		TSharedPtr<FJsonValue> Console = MakeShareable(new FJsonValueString(ServerConsoleCommandsOnStartup[i]));
		JsonObjectConsole.Add(Console);
	}
	JsonObject->SetArrayField("ServerConsoleCommandsOnStartup", JsonObjectConsole);

	JsonObjectConsole.Empty();
	for (int32 i = 0; i < ClientConsoleCommandsOnStartup.Num(); i++)
	{
		TSharedPtr<FJsonValue> Console = MakeShareable(new FJsonValueString(ClientConsoleCommandsOnStartup[i]));
		JsonObjectConsole.Add(Console);
	}
	JsonObject->SetArrayField("ClientConsoleCommandsOnStartup", JsonObjectConsole);
	
	
	FString ClientLaunchTypeString = GetEnumDisplayName<EClientLaunchType>("EClientLaunchType", ClientLaunchType);
	//auto ClientLaunchTypeString1 = GetEnumMetaText<EClientLaunchType>("EClientLaunchType", FName(*ClientLaunchTypeString));
	JsonObject->SetStringField("ClientLaunchType", ClientLaunchTypeString);

	FString ClientControllerTypeString = GetEnumDisplayName<EClientControllerType>("EClientControllerType", ClientControllerType);
	JsonObject->SetStringField("ClientControllerType", ClientControllerTypeString);	
	
	TArray< TSharedPtr<FJsonValue> > JsonTeamData;	
	for (auto& OneTeam : TeamData)
	{
		TSharedPtr<FJsonObject> JsonObjectTeam = MakeShareable(new FJsonObject);
		JsonObjectTeam->SetStringField("Name", OneTeam.Value.Name);
		JsonObjectTeam->SetNumberField("ID", int32(OneTeam.Value.ID));
		JsonObjectTeam->SetStringField("Color", OneTeam.Value.Color.ToString());
		TSharedPtr<FJsonValue> JsonTeam = MakeShareable(new FJsonValueObject(JsonObjectTeam));
		JsonTeamData.Add(JsonTeam);
	}	
	JsonObject->SetArrayField("TeamData", JsonTeamData);


	//saving
	FString SaveJsonString;
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&SaveJsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	FString FullPath = FPaths::ProjectDir() + RelativeConfigPath;
	FFileHelper::SaveStringToFile(SaveJsonString, *FullPath);
}

