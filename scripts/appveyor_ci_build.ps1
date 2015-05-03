
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

    if ($LASTEXITCODE > 0)
    {
        Pop-Location
        Exit $LASTEXITCODE
    }

    Pop-Location
}

function Remove-PathFolder($folder)
{
    [string[]]$pathFolders = $env:Path -split ";"
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
    $env:Path = "$folder;$env:Path"
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

if ($env:PlatformToolset -eq 'MinGW')
{
    Write-Host "Initial Path: $env:Path"

    # Need to do some path cleanup first
    Remove-PathFolder "C:\Program Files\Git\bin"
    Remove-PathFolder "C:\Program Files (x86)\Git\bin"

    # Add cmake and mingw to the path
    Add-PathFolder "C:\Program Files (x86)\CMake 2.8\bin"
    Add-PathFolder "C:\MinGW\bin"

    Write-Host "Building with Path: $env:Path"

    Invoke-BuildCommand "cmake -G 'MinGW Makefiles' .." 'cpputest_build'
    Invoke-BuildCommand "mingw32-make all" 'cpputest_build'
}
