
# Helper function to extract vars out of the vsvars batch file
function Get-Batchfile ($file) {
    $cmd = "`"$file`" & set"
    cmd /c $cmd | Foreach-Object {
        $p, $v = $_.split('=')
        Set-Item -path env:$p -value $v
    }
}

# Helper function to provide the bin-folder path to mingw
function Get-MinGWBin() {
    if ($env:Platform -like '*64') {
        Write-Output 'C:\Tools\mingw64\bin'
    }
    else {
        Write-Output 'C:\Tools\mingw32\bin'
    }
}

# Helper function to provide the toolchain file for clang
function Get-ClangToolchainFilename() {
    if ($env:Platform -like '*64') {
        Write-Output 'clang+mingw-win64.toolchain.cmake'
    }
    else {
        Write-Output 'clang+mingw-win32.toolchain.cmake'
    }
}

# Helper function to provide the bin-folder path to cygwin
function Get-CygwinBin() {
    if ($env:Platform -like '*64') {
        Write-Output 'C:\cygwin64\bin'
    }
    else {
        Write-Output 'C:\cygwin\bin'
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

function Set-Path($newPath)
{
    $env:RestorePath = $env:Path
    $env:Path = $newPath
}

function Restore-Path()
{
    $env:Path = $env:RestorePath
}
