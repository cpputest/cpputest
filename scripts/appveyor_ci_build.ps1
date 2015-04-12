
# Helper function to extract vars out of the vsvars batch file
function Get-Batchfile ($file) {
    $cmd = "`"$file`" & set"
    cmd /c $cmd | Foreach-Object {
        $p, $v = $_.split('=')
        Set-Item -path env:$p -value $v
    }
}

# The project files that will get built
$VS2008ProjectFiles = @( 'CppUTest.vcproj' , 'tests/AllTests.vcproj'  )
$VS2010ProjectFiles = @( 'CppUTest.vcxproj', 'tests/AllTests.vcxproj' )

if ($env:PlatformToolset -eq 'v90')
{
    $vsvarspath = Join-Path $env:VS90COMNTOOLS vsvars32.bat
    Get-BatchFile($vsvarspath)

    $VS2008ProjectFiles | foreach {
        vcbuild /upgrade $_
    }

    $VS2008ProjectFiles | foreach {
        vcbuild $_ Debug
    }
}

if ($env:PlatformToolset -eq 'v100')
{
    $VS2010ProjectFiles | foreach {
        msbuild /logger:"C:\Program Files\AppVeyor\BuildAgent\Appveyor.MSBuildLogger.dll" $_
    }
}
