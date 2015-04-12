
# Helper function to extract vars out of the vsvars batch file
function Get-Batchfile ($file) {
    $cmd = "`"$file`" & set"
    cmd /c $cmd | Foreach-Object {
        $p, $v = $_.split('=')
        Set-Item -path env:$p -value $v
    }
}

function Invoke-BuildCommand($command)
{
    Write-Host $command
    Invoke-Expression $command
    if (-not $?)
    {
        Exit $LASTEXITCODE
    }
}

function Invoke-Tests($executable)
{
    # Run tests using Chutzpah and export results as JUnit format to chutzpah-results.xml
    $TestCommand = "$executable -ojunit"
    Write-Host $TestCommand
    Invoke-Expression $TestCommand

    $anyFailures = $FALSE

    # Upload results to AppVeyor one by one
    Get-ChildItem *.xml | foreach
    {
        $testsuites = [xml](get-content $_.Name)

        foreach ($testsuite in $testsuites.testsuites.testsuite) {
            write-host " $($testsuite.name)"
            foreach ($testcase in $testsuite.testcase){
                $failed = $testcase.failure
                if ($failed) {
                    Add-AppveyorTest $testcase.name -Outcome Failed -FileName $testsuite.name -ErrorMessage $testcase.failure.message
                    Add-AppveyorMessage "$($testcase.name) failed" -Category Error
                    $anyFailures = $TRUE
                }
                else {
                    Add-AppveyorTest $testcase.name -Outcome Passed -FileName $testsuite.name
                }

            }
        }
    }

    if ($anyFailures -eq $TRUE){
        write-host "Failing build as there are broken tests"
        $host.SetShouldExit(1)
    }
}

# The project files that will get built
$VS2008ProjectFiles = @( 'CppUTest.vcproj' , 'tests\AllTests.vcproj'  )
$VS2010ProjectFiles = @( 'CppUTest.vcxproj', 'tests\AllTests.vcxproj' )
$VS2008TestCommand = 'tests\Debug\AllTests.exe'
$VS2010TestCommand = 'tests\Debug\AllTests.exe'

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
        Invoke-BuildCommand("vcbuild /upgrade $_")
    }

    $VS2008ProjectFiles | foreach {
        Invoke-BuildCommand("vcbuild $_ Debug")
    }

    Invoke-Test($VS2008TestCommand)
}

if ($env:PlatformToolset -eq 'v100')
{
    $VS2010ProjectFiles | foreach {
        Invoke-BuildCommand("msbuild $logger_arg $_")
    }

    Invoke-Tests($VS2010TestCommand)
}
