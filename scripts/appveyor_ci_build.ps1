
# Helper function to extract vars out of the vsvars batch file
function Get-Batchfile ($file) {
    $cmd = "`"$file`" & set"
    cmd /c $cmd | Foreach-Object {
        $p, $v = $_.split('=')
        Set-Item -path env:$p -value $v
    }
}

function Invoke-BuildCommand($command, $directory = '.')
{
    $command_wrapped = "$command;`$err = `$?"
    Write-Host $command

    Push-Location $directory
    Invoke-Expression $command_wrapped

    if ($LASTEXITCODE -ne 0)
    {
        Pop-Location
        Write-Host "Command Returned error: $LASTEXITCODE"
        Exit $LASTEXITCODE
    }

    Pop-Location
}

function Invoke-CygwinCommand($command, $directory = '.')
{
    # Assume cygwin is located at C:\cygwin for now
    $cygwin_bin = "C:\cygwin\bin"
    $cygwin_directory = (. "${cygwin_bin}\cygpath.exe" (Resolve-Path $directory))
    $command_wrapped = "${cygwin_bin}\bash.exe --login -c 'cd $cygwin_directory ; $command' ; `$err = `$?"
    
    Write-Host $command
    Invoke-Expression $command_wrapped

    if ($LASTEXITCODE -ne 0)
    {
        Write-Host "Command Returned error: $LASTEXITCODE"
        Exit $LASTEXITCODE
    }
}

function Remove-PathFolder($folder)
{
    [System.Collections.ArrayList]$pathFolders = New-Object System.Collections.ArrayList
    $env:Path -split ";" | foreach { $pathFolders.Add($_) | Out-Null }

    for ([int]$i = 0; $i -lt $pathFolders.Count; $i++)
    {
        if ([string]::Compare($pathFolders[$i], $folder, $true) -eq 0)
        {
            Write-Host "Removing $folder from the PATH"
            $pathFolders.RemoveAt($i)
            $i--
        }
    }

    $env:Path = $pathFolders -join ";"
}

function Add-PathFolder($folder)
{
    if (-not (Test-Path $folder))
    {
        Write-Host "Not adding $folder to the PATH, it does not exist"
    }

    [bool]$alreadyInPath = $false
    [System.Collections.ArrayList]$pathFolders = New-Object System.Collections.ArrayList
    $env:Path -split ";" | foreach { $pathFolders.Add($_) | Out-Null }

    for ([int]$i = 0; $i -lt $pathFolders.Count; $i++)
    {
        if ([string]::Compare($pathFolders[$i], $folder, $true) -eq 0)
        {
            $alreadyInPath = $true
            break
        }
    }

    if (-not $alreadyInPath)
    {
        Write-Host "Adding $folder to the PATH"
        $pathFolders.Insert(0, $folder)
        $env:Path = $pathFolders -join ";"
    }
}

# The project files that will get built
$VS2008ProjectFiles = @( 'CppUTest.vcproj' , 'tests\AllTests.vcproj'  )
$VS2010ProjectFiles = @( 'CppUTest.vcxproj', 'tests\AllTests.vcxproj' )

if ($env:APPVEYOR)
{
    $logger_arg = '/logger:"C:\Program Files\AppVeyor\BuildAgent\Appveyor.MSBuildLogger.dll"'
}
else
{
    $logger_arg = ''
}

if ($env:PlatformToolset -eq 'v90')
{
    $vsvarspath = Join-Path $env:VS90COMNTOOLS vsvars32.bat
    Get-BatchFile($vsvarspath)

    $VS2008ProjectFiles | foreach {
        Invoke-BuildCommand "vcbuild /upgrade $_"
    }

    $VS2008ProjectFiles | foreach {
        Invoke-BuildCommand "vcbuild $_ $env:CONFIGURATION"
    }
}

if ($env:PlatformToolset -eq 'v100')
{
    $VS2010ProjectFiles | foreach {
        Invoke-BuildCommand "msbuild $logger_arg $_"
    }
}

if ($env:PlatformToolset -eq 'Cygwin')
{
    Invoke-CygwinCommand "autoreconf -i .. ; ../configure ; make CppUTestTests.exe CppUTestExtTests.exe" "cpputest_build"
}

if ($env:PlatformToolset -eq 'MinGW')
{
    $mingw_path = 'C:\Tools\mingw32\bin'
    if ($env:Platform -eq 'x64')
    {
        $mingw_path = 'C:\Tools\mingw64\bin'
    }

    Write-Host "Initial Path: $env:Path"

    # Need to do some path cleanup first
    Remove-PathFolder "C:\MinGW\bin"
    Remove-PathFolder "C:\Program Files\Git\bin"
    Remove-PathFolder "C:\Program Files\Git\cmd"
    Remove-PathFolder "C:\Program Files\Git\usr\bin"
    Remove-PathFolder "C:\Program Files (x86)\Git\bin"
    Remove-PathFolder "C:\Program Files (x86)\Git\cmd"

    # Add mingw to the path
    Add-PathFolder $mingw_path

    Write-Host "Building with Path: $env:Path"

    Invoke-BuildCommand "cmake -G 'MinGW Makefiles' .." 'cpputest_build'
    Invoke-BuildCommand "mingw32-make all" 'cpputest_build'

    Remove-PathFolder $mingw_path
}
