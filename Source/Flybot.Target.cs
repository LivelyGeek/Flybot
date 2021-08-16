// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

using UnrealBuildTool;
using System.Collections.Generic;

public class FlybotTarget : TargetRules
{
	public FlybotTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "Flybot" } );
	}
}
