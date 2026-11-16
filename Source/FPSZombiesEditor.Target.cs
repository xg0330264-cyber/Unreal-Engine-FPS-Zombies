using UnrealBuildTool;
using System.Collections.Generic;

public class FPSZombiesEditorTarget : TargetRules
{
	public FPSZombiesEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_2;
		ExtraModuleNames.Add("FPSZombies");
	}
}
