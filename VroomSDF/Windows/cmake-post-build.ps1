$ErrorActionPreference = "Stop"

Push-Location -Path "out"
	if (!(Test-Path "SDL2.dll")) {
		Copy-Item -Path "..\..\..\third-party\sdl-windows\lib\x64\SDL2.dll"
	}
	if (!(Test-Path "SDL2_image.dll")) {
		Copy-Item -Path "..\..\..\third-party\sdl-image-windows\lib\x64\SDL2_image.dll"
	}
	if (!(Test-Path "libpng16-16.dll")) {
		Copy-Item -Path "..\..\..\third-party\sdl-image-windows\lib\x64\libpng16-16.dll"
	}
	if (!(Test-Path "zlib1.dll")) {
		Copy-Item -Path "..\..\..\third-party\sdl-image-windows\lib\x64\zlib1.dll"
	}
	if (!(Test-Path "assets")) {
		cmd.exe /c 'mklink /d assets ..\..\main\assets'
	}
Pop-Location

Push-Location -Path ".\out\Debug"
	if (!(Test-Path "SDL2.dll")) {
		Copy-Item -Path "..\..\..\..\third-party\sdl-windows\lib\x64\SDL2.dll"
	}
	if (!(Test-Path "SDL2_image.dll")) {
    	Copy-Item -Path "..\..\..\..\third-party\sdl-image-windows\lib\x64\SDL2_image.dll"
	}
	if (!(Test-Path "libpng16-16.dll")) {
    	Copy-Item -Path "..\..\..\..\third-party\sdl-image-windows\lib\x64\libpng16-16.dll"
	}
	if (!(Test-Path "zlib1.dll")) {
  		Copy-Item -Path "..\..\..\..\third-party\sdl-image-windows\lib\x64\zlib1.dll"
	}
	if (!(Test-Path "assets")) {
		cmd.exe /c 'mklink /d assets ..\..\..\main\assets'
	}
Pop-Location


