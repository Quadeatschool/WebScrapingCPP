param(
    [ValidateSet("Debug", "Release")]
    [string]$Config = "Debug"
)

$ErrorActionPreference = "Stop"

$projectRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $projectRoot

$vswherePath = "${env:ProgramFiles(x86)}/Microsoft Visual Studio/Installer/vswhere.exe"
$vsInstallPath = $null
if (Test-Path $vswherePath) {
    $vsInstallPath = (& $vswherePath -latest -products * -requires Microsoft.Component.MSBuild -property installationPath | Select-Object -First 1)
}

$cmakeCmd = Get-Command cmake -ErrorAction SilentlyContinue
if ($cmakeCmd) {
    $cmakeExe = $cmakeCmd.Source
} else {
    $cmakeCandidates = @(
        "C:/Program Files/CMake/bin/cmake.exe",
        "C:/Program Files (x86)/CMake/bin/cmake.exe",
        "C:/Program Files/Microsoft Visual Studio/2022/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe",
        "C:/Program Files/Microsoft Visual Studio/2022/Professional/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe",
        "C:/Program Files/Microsoft Visual Studio/2022/Enterprise/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe"
    )

    if ($vsInstallPath) {
        $cmakeCandidates += "$vsInstallPath/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe"
    }

    $cmakeExe = $cmakeCandidates | Where-Object { Test-Path $_ } | Select-Object -First 1
}

$msbuildExe = $null
$msbuildCmd = Get-Command msbuild -ErrorAction SilentlyContinue
if ($msbuildCmd) {
    $msbuildExe = $msbuildCmd.Source
} else {
    $msbuildCandidates = @(
        "C:/Program Files/Microsoft Visual Studio/2022/BuildTools/MSBuild/Current/Bin/MSBuild.exe",
        "C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/MSBuild/Current/Bin/MSBuild.exe",
        "C:/Program Files/Microsoft Visual Studio/2022/Community/MSBuild/Current/Bin/MSBuild.exe",
        "C:/Program Files/Microsoft Visual Studio/2022/Professional/MSBuild/Current/Bin/MSBuild.exe",
        "C:/Program Files/Microsoft Visual Studio/2022/Enterprise/MSBuild/Current/Bin/MSBuild.exe"
    )

    if ($vsInstallPath) {
        $msbuildCandidates += "$vsInstallPath/MSBuild/Current/Bin/MSBuild.exe"
    }

    $msbuildExe = $msbuildCandidates | Where-Object { Test-Path $_ } | Select-Object -First 1
}

if ($cmakeExe) {
    Write-Host "[1/3] Configuring CMake..." -ForegroundColor Cyan
    & $cmakeExe -S . -B build

    Write-Host "[2/3] Building ($Config)..." -ForegroundColor Cyan
    & $cmakeExe --build build --config $Config
} else {
    $solutionPath = Join-Path $projectRoot "build/WebScraper.sln"
    if (-not $msbuildExe -or -not (Test-Path $solutionPath)) {
        throw "No usable CLI build path found. Install CMake or generate build/WebScraper.sln in VS Code first."
    }

    Write-Host "[1/3] Skipping configure (CMake CLI not available)..." -ForegroundColor Yellow
    Write-Host "[2/3] Building with MSBuild ($Config)..." -ForegroundColor Cyan
    & $msbuildExe $solutionPath /m /p:Configuration=$Config /p:Platform=x64
}

$exeCandidates = @(
    (Join-Path $projectRoot "build/$Config/WebScraper.exe"),
    (Join-Path $projectRoot "build/x64/$Config/WebScraper.exe"),
    (Join-Path $projectRoot "build/WebScraper.exe")
)

$exePath = $exeCandidates | Where-Object { Test-Path $_ } | Select-Object -First 1

if (-not $exePath) {
    throw "WebScraper.exe was not found after build. Checked: $($exeCandidates -join ', ')"
}

Write-Host "[3/3] Running WebScraper..." -ForegroundColor Cyan
& $exePath
