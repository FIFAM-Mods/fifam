@echo off
setlocal enabledelayedexpansion

:: Define duration (3.5 minutes = 210 seconds)
set duration=210
set fade_duration=2

:: Create output folder if it doesn't exist
if not exist "output" mkdir "output"

:: Loop through all WAV files in the current directory
for %%F in (*.wav) do (
    echo Processing: %%F

    :: Calculate fade-out start time (210 - 2 = 208)
    set /A fade_out_start=%duration%-%fade_duration%

    ffmpeg -i "%%F" -af "afade=t=in:ss=0:d=%fade_duration%,afade=t=out:st=!fade_out_start!:d=%fade_duration%" -t %duration% "output\%%~nF.wav"
)

echo All files processed!
pause