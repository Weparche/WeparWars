param([string]$Compiler = "g++")
$ErrorActionPreference = "Stop"
$Root = Split-Path -Parent $PSScriptRoot
$Exe = Join-Path $env:TEMP "wepar-rules-tests.exe"
& $Compiler "-std=c++17" "-Wall" "-Wextra" "-Werror" (Join-Path $Root "Tests/RulesTests.cpp") "-o" $Exe
if ($LASTEXITCODE -ne 0) { throw "C++ compilation failed" }
& $Exe
if ($LASTEXITCODE -ne 0) { throw "Combat tests failed" }
