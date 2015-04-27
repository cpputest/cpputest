

function Invoke-Tests($executable)
{
    # Run tests and output the results using junit
    $TestCommand = "$executable -ojunit"
    Write-Host $TestCommand
    Invoke-Expression $TestCommand

    $anyFailures = $FALSE

    # Upload results to AppVeyor one by one
    Get-ChildItem cpputest_*.xml | foreach {
        $testsuite = ([xml](get-content $_.Name)).testsuite

        write-host " $($testsuite.name)"
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
    }

    if ($anyFailures -eq $TRUE){
        write-host "Failing build as there are broken tests"
        $host.SetShouldExit(1)
    }
}

if ($env:PlatformToolset -eq 'v90') {
    Invoke-Tests('.\tests\Debug\AllTests.exe')
}
else {
    Invoke-Tests('.\cpputest_build\AllTests.exe')
}
