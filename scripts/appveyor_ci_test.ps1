
# Load functions from the helper file
. (Join-Path (Split-Path $MyInvocation.MyCommand.Path) 'appveyor_helpers.ps1')

function Publish-TestResults($files)
{
    $anyFailures = $FALSE

    # Upload results to AppVeyor one by one
    $files | foreach {
        $testsuite = ([xml](get-content $_.Name)).testsuite

        foreach ($testcase in $testsuite.testcase) {
            if ($testcase.failure) {
                Add-AppveyorTest $testcase.name -Outcome Failed -FileName $testsuite.name -ErrorMessage $testcase.failure.message
                Add-AppveyorMessage "$($testcase.name) failed" -Category Error
                $anyFailures = $TRUE
            }
            elseif ($testcase.skipped) {
                Add-AppveyorTest $testcase.name -Outcome Ignored -Filename $testsuite.name
            }
            else {
                Add-AppveyorTest $testcase.name -Outcome Passed -FileName $testsuite.name
            }
        }

        Remove-Item $_.Name
    }

    if ($anyFailures -eq $TRUE){
        write-host "Failing build as there are broken tests"
        $host.SetShouldExit(1)
    }
}

function Invoke-Tests($executable)
{
    # Run tests and output the results using junit
    $TestCommand = "$executable -ojunit"
    Write-Host $TestCommand -NoNewline
    Invoke-Expression $TestCommand
    Write-Host " - return code: $LASTEXITCODE"
    if ($LASTEXITCODE -lt 0) {
        Write-Error "Runtime Exception during test execution"
    }
}

$TestCount = 0

if (-not $env:APPVEYOR)
{
    function Add-AppVeyorTest()
    {
        # Wacky way to access a script variable, but it works
        $count = Get-Variable -Name TestCount -Scope script
        Set-Variable -Name TestCount -Scope script -Value ($count.Value + 1)
    }

    function Add-AppVeyorMessage($Message, $Category)
    {
        if ($Category -eq 'Error')
        {
            Write-Error $Message
        }
        else
        {
            Write-Host $Message
        }
    }
}

switch -Wildcard ($env:Platform)
{
    'MinGW*'
    {
        $mingw_path = Get-MinGWBin

        Set-Path "$mingw_path;C:\Windows;C:\Windows\System32"
        Invoke-Tests '.\cpputest_build\tests\CppUTest\CppUTestTests.exe'
        Invoke-Tests '.\cpputest_build\tests\CppUTestExt\CppUTestExtTests.exe'
        Restore-Path
    }

    default
    {
        Invoke-Tests '.\cpputest_build\AllTests.exe'
    }
}

Publish-TestResults (Get-ChildItem 'cpputest_*.xml')

if (-not $env:APPVEYOR)
{
    Write-Host "Tests Ran: $TestCount"
}
