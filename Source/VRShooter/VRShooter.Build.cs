// The Family

using UnrealBuildTool;

public class VRShooter : ModuleRules
{
	public VRShooter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore"/*, "UnrealEd"*/ });
		PrivateDependencyModuleNames.AddRange(new string[] {  });

        if (Target.Type == TargetType.Editor)
        {
            PublicDependencyModuleNames.AddRange(new string[] { "UnrealEd" });//for ConfigAsset
        }

        // Online features
        PrivateDependencyModuleNames.AddRange(new string[] { "HeadMountedDisplay", "OnlineSubsystem", "OnlineSubsystemUtils", "Sockets", "Networking", "Json", "JsonUtilities" });
        DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");


        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
