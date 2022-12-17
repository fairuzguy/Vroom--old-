$ErrorActionPreference = "Stop"

Push-Location -Path "..\..\"
if (!(Test-Path "third-party")) {
	New-Item -ItemType Directory -Path "third-party"
}
Pop-Location

if (!(Test-Path "..\..\third-party\sdl-windows")) {
	Write-Host "Downloading SDL Windows Dev library into third party folder sdl-windows..."
	$source = 'https://www.libsdl.org/release/SDL2-devel-2.0.9-VC.zip'
	$destination = '..\..\third-party\SDL2-devel-2.0.9-VC.zip'
	Invoke-WebRequest -Uri $source -OutFile $destination

	Push-Location -Path "..\..\third-party"
		Write-Host "Unzipping SDL Windows Dev library into third-party/sdl-windows..."
		cmd.exe /c 'tar -xf SDL2-devel-2.0.9-VC.zip'
		Move-Item -Path SDL2-2.0.9 -Destination sdl-windows
		Remove-Item -Path SDL2-devel-2.0.9-VC.zip
	Pop-Location
}

if (!(Test-Path "..\..\third-party\glew")) {
	Write-Host "Downloading GLEW into third party folder glew..."
	$source = 'https://github.com/nigels-com/glew/releases/download/glew-2.1.0/glew-2.1.0-win32.zip'
	$destination = '..\..\third-party\glew-2.1.0-win32.zip'
	Invoke-WebRequest -Uri $source -OutFile $destination

	Push-Location -Path "..\..\third-party"
		Write-Host "Unzipping GLEW library into third-party/glew..."
		cmd.exe /c 'tar -xf glew-2.1.0-win32.zip'
		Move-Item -Path glew-2.1.0 -Destination glew
		Remove-Item -Path glew-2.1.0-win32.zip
	Pop-Location
}

if (!(Test-Path "..\..\third-party\glm")) {
	Write-Host "Downloading GLM into third party folder glm ..."
	$source = 'https://github.com/g-truc/glm/releases/download/0.9.9.3/glm-0.9.9.3.zip'
	$destination = '..\..\third-party\glm-0.9.9.3.zip'
	Invoke-WebRequest -Uri $source -OutFile $destination

	Push-Location -Path "..\..\third-party"
		Write-Host "Unzipping GLM into third-party\glm ..."
		cmd.exe /c 'tar -xf glm-0.9.9.3.zip'
		Remove-Item -Path glm-0.9.9.3.zip
	Pop-Location
}

if (!(Test-Path "..\..\third-party\tiny-obj-loader")) {
	Write-Host "Downloading Tiny OBJ Loader into third party folder tiny-obj-loader..."
	Write-Host "Tiny OBJ Loader is unavailable for remote install, download the header file from master branch."
}

if (!(Test-Path "..\..\third-party\sdl-image-windows")) {
	Write-Host "Downloading SDL Image Windows library into third party folder sdl-image-windows..."
	$source = 'https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.0.4-VC.zip'
	$destination = '..\..\third-party\SDL2_image-devel-2.0.4-VC.zip'
	Invoke-WebRequest -Uri $source -OutFile $destination

	Push-Location -Path "..\..\third-party"
		Write-Host "Unzipping SDL2 Image Windows library into third-party/sdl-image-windows..."
		cmd.exe /c 'tar -xf SDL2_image-devel-2.0.4-VC.zip'
		Move-Item -Path SDL2_image-2.0.4 -Destination sdl-image-windows
		Remove-Item -Path SDL2_image-devel-2.0.4-VC.zip
	Pop-Location
}
