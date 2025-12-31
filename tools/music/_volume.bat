@echo off
SETLOCAL ENABLEDELAYEDEXPANSION

REM Process all WAV files in the current folder and overwrite them safely
for %%f in (*.wav) do (
    REM Create a temporary output file with proper extension
    set "tmpfile=%%~nf_tmp.wav"

    REM Run FFmpeg
    ffmpeg -y -i "%%f" -filter:a "volume=0.5" "!tmpfile!"

    REM Replace the original file
    move /Y "!tmpfile!" "%%f"
)

echo Done! All WAV files have been updated.
pause
