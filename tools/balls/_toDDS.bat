@echo off

echo Starting conversion with full mipmap chains...

REM Convert all PNGs starting with "ball" to DDS DXT1 with full mipmaps
for /R %%I in (ball*.png) do (
    CompressonatorCLI.exe -fd DXT1 -mipsize 1 "%%I" "%%~dpnI.dds"
)

REM Convert all PNGs starting with "baln" to DDS DXT5 with full mipmaps
for /R %%I in (baln*.png) do (
    CompressonatorCLI.exe -fd DXT5 -mipsize 1 "%%I" "%%~dpnI.dds"
)

echo Conversion complete!
pause