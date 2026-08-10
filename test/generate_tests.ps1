<#
.SYNOPSIS
    Auto-generates AllTests.h, which #includes every .h file found in the tests folder.

.DESCRIPTION
    Run this as a Visual Studio Pre-Build Event so AllTests.h always reflects
    whatever .h files currently exist in the tests folder - no manual editing needed.

.PARAMETER TestsDir
    Folder containing the individual test headers (e.g. ...\tests)

.PARAMETER OutputFile
    Path of the generated header (e.g. ...\AllTests.h)
#>
param(
    [Parameter(Mandatory = $true)]
    [string]$TestsDir,

    [Parameter(Mandatory = $true)]
    [string]$OutputFile
)

$ErrorActionPreference = "Stop"

if (-not (Test-Path $TestsDir)) {
    Write-Error "Tests folder not found: $TestsDir"
    exit 1
}

# Only .h files directly inside the tests folder (not recursive), sorted alphabetically.
$headers = Get-ChildItem -Path $TestsDir -Filter *.h -File | Sort-Object Name

# Use the tests folder's own name in the #include path (e.g. "tests/Test1.h"),
# so this keeps working no matter where OutputFile itself lives.
$testsDirName = Split-Path -Leaf $TestsDir

$lines = New-Object System.Collections.Generic.List[string]
$lines.Add("// ============================================================")
$lines.Add("// AUTO-GENERATED FILE - DO NOT EDIT MANUALLY")
$lines.Add("// Regenerated on every build by generate_tests.ps1.")
$lines.Add("// Add or remove test headers in the '$testsDirName' folder instead.")
$lines.Add("// ============================================================")
$lines.Add("#pragma once")
$lines.Add("")

if ($headers.Count -eq 0) {
    $lines.Add("// (no test headers found in $testsDirName)")
} else {
    foreach ($h in $headers) {
        $lines.Add("#include ""$testsDirName/$($h.Name)""")
    }
}
$lines.Add("")

$newContent = $lines -join "`r`n"

# Only write if the content actually changed, so we don't touch the file's
# timestamp and trigger a needless downstream rebuild every single time.
$shouldWrite = $true
if (Test-Path $OutputFile) {
    $existing = Get-Content -Raw -Path $OutputFile -ErrorAction SilentlyContinue
    if ($existing -eq $newContent) {
        $shouldWrite = $false
    }
}

if ($shouldWrite) {
    Set-Content -Path $OutputFile -Value $newContent -Encoding UTF8 -NoNewline
    Write-Host "Generated $OutputFile with $($headers.Count) test header(s)."
} else {
    Write-Host "$OutputFile is already up to date ($($headers.Count) test header(s))."
}
