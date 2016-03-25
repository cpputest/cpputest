
# Load functions from the helper file
. (Join-Path (Split-Path $MyInvocation.MyCommand.Path) 'appveyor_helpers.ps1')

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
    # Assume cygwin is located at C:\cygwin on x86 and C:\cygwin64 on x64 for now
    $cygwin_bin = Get-CygwinBin

    $cygwin_directory = (. "${cygwin_bin}\cygpath.exe" (Resolve-Path $directory))
    $command_wrapped = "${cygwin_bin}\bash.exe --login -c 'cd $cygwin_directory ; $command'"
    
    Write-Host "Executing <$command> in <$cygwin_directory>"
    Invoke-Expression $command_wrapped

    if ($LASTEXITCODE -ne 0)
    {
        Write-Host "Command Returned error: $LASTEXITCODE"
        Exit $LASTEXITCODE
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

# Clean up some paths for any configuration
Remove-PathFolder "C:\MinGW\bin"
Remove-PathFolder "C:\Program Files\Git\bin"
Remove-PathFolder "C:\Program Files\Git\cmd"
Remove-PathFolder "C:\Program Files\Git\usr\bin"
Remove-PathFolder "C:\Program Files (x86)\Git\bin"
Remove-PathFolder "C:\Program Files (x86)\Git\cmd"
Remove-PathFolder "C:\Program Files (x86)\Git\usr\bin"

switch -Wildcard ($env:Platform)
{
    'Cygwin*'
    {
        Invoke-CygwinCommand "autoreconf -i .." "cpputest_build"
        Invoke-CygwinCommand "../configure" "cpputest_build"
        Invoke-CygwinCommand "make CppUTestTests.exe CppUTestExtTests.exe" "cpputest_build"
    }

    'MinGW*'
    {
        $mingw_path = Get-MinGWBin

        # Add mingw to the path
        Add-PathFolder $mingw_path

        Invoke-BuildCommand "cmake -G 'MinGW Makefiles' .." 'cpputest_build'
        Invoke-BuildCommand "mingw32-make all" 'cpputest_build'

        Remove-PathFolder $mingw_path
    }

    default   # Assume that anything else uses Visual C++
    {
        if ($env:PlatformToolset -eq 'v90')
        {
            # Load environment variables from vsvars32.bat
            $vsvarspath = Join-Path $env:VS90COMNTOOLS vsvars32.bat
            Get-BatchFile($vsvarspath)

            $VS2008ProjectFiles | foreach {
                Invoke-BuildCommand "vcbuild /upgrade $_"
            }

            $VS2008ProjectFiles | foreach {
                Invoke-BuildCommand "vcbuild $_ $env:CONFIGURATION"
            }
        }
        else
        {
            $VS2010ProjectFiles | foreach {
                Invoke-BuildCommand "msbuild /ToolsVersion:14.0 $logger_arg $_"
            }
        }
    }
}
