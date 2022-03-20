@echo off
SET TARGET=windows
SET REQUIRED_DLLS=csfml-audio-2.dll csfml-graphics-2.dll csfml-system-2.dll openal32.dll
REM Using UPX 3.96 results in the executable being marked as a virus by Windows.
SET USE_UPX="n"
IF %USE_UPX% == "y" (
	IF NOT EXIST upx.exe (
		ECHO Can't find upx.exe. You can download the latest UPX release from GitHub: https://github.com/upx/upx/releases
		EXIT /b
	)
)
SET missing_ddls="n"
for %%a in (%REQUIRED_DLLS%) do (
	IF NOT EXIST ..\target\release\%%a (
		ECHO Can't find %%a in target\release.
		SET missing_ddls="y"
	)
)
IF %missing_ddls% == "y" (
	ECHO You can download the required DDLs from SFML's website.
	ECHO SFML 2.5.1:  https://www.sfml-dev.org/download/sfml/2.5.1/
	ECHO CSFML 2.5.1: https://www.sfml-dev.org/download/csfml/
	ECHO After downloading, copy them to target\debug and target\release, then try running this script again.
	EXIT /b
)
IF EXIST %TARGET% (
	RMDIR /S /Q %TARGET%
)
MD %TARGET%
CD ..
cargo build --release || (
	ECHO Seems like something went wrong building the project, even though all required DDLs were found.
	ECHO Have you copied all the appropriate .lib files for SFML and CSFML into your Rust toolchain's libraries folder?
	ECHO If you're having trouble, please take a look at the following guides.
	ECHO Rust SFML MSCV Windows Setup: https://www.youtube.com/watch?v=nnojR-8PT4M
	ECHO rust-sfml Windows wiki page:  https://github.com/jeremyletang/rust-sfml/wiki/Windows
	CD build
	EXIT /b
)
COPY target\release\septadrop.exe build\%TARGET%\septadrop.exe > NUL
for %%a in (%REQUIRED_DLLS%) do (
	COPY target\release\%%a build\%TARGET% > NUL
)
XCOPY /E /D res build\%TARGET%\res\ > NUL
CD build
IF %USE_UPX% == "y" (
	upx.exe --best --lzma %TARGET%/septadrop.exe
)
ECHO Finished building and packaging successfully! 