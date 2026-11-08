param(
    [Parameter(Mandatory = $true)][string]$LyraProjectRoot
)
$ErrorActionPreference = "Stop"
$Root = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$Project = (Resolve-Path $LyraProjectRoot).Path
if (-not (Get-ChildItem $Project -Filter "*.uproject" -File)) {
    throw "The directory must contain an existing Lyra .uproject file."
}
$TargetParent = Join-Path $Project "Plugins"
$Target = Join-Path $TargetParent "WeparCombat"
if (Test-Path $Target) {
    throw "Target plugin exists: $Target. Refusing to overwrite; use git or compare changes."
}
New-Item -ItemType Directory -Force -Path $TargetParent | Out-Null
Copy-Item -Path (Join-Path $Root "Plugins/WeparCombat") -Destination $TargetParent -Recurse
Write-Host "Plugin copied to $Target"
Write-Host "Next: enable WeparCombat, generate project files and compile in Visual Studio."
