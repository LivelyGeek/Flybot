// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

using UnrealBuildTool;
using System.Collections.Generic;

public class FlybotEditorTarget : TargetRules
{
	public FlybotEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "Flybot" } );
	}
}
