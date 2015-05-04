

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

        Remove-Item $_.Name
    }

    if ($anyFailures -eq $TRUE){
        write-host "Failing build as there are broken tests"
        $host.SetShouldExit(1)
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

if ($env:PlatformToolset -ne 'MinGW')
{
    Invoke-Tests('.\cpputest_build\AllTests.exe')
}
else
{
    Add-PathFolder 'C:\Tools\mingw32\bin'
    Invoke-Tests('.\cpputest_build\tests\CppUTestTests.exe')
    Invoke-Tests('.\cpputest_build\tests\CppUTestExt\CppUTestExtTests.exe')
    Remove-PathFolder 'C:\Tools\mingw32\bin'
}
