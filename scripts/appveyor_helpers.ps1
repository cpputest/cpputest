
# Helper function to extract vars out of the vsvars batch file
function Get-Batchfile ($file) {
    $cmd = "`"$file`" & set"
    cmd /c $cmd | Foreach-Object {
        $p, $v = $_.split('=')
        Set-Item -path env:$p -value $v
    }
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

