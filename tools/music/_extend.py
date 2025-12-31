import os
import subprocess
from pathlib import Path

def extend_wav_files(repeat_times=10):
    # Get current directory
    current_dir = Path.cwd()
    
    # Create output directory if it doesn't exist
    output_dir = current_dir / "output"
    output_dir.mkdir(exist_ok=True)
    
    # Find all .wav files in current directory
    wav_files = list(current_dir.glob("*.wav"))
    
    if not wav_files:
        print("No .wav files found in the current directory.")
        return
    
    print(f"Found {len(wav_files)} .wav file(s) to process.")
    print(f"Each file will be repeated {repeat_times} times.\n")
    
    # Process each wav file
    for wav_file in wav_files:
        print(f"Processing: {wav_file.name}")
        
        # Output file path
        output_file = output_dir / wav_file.name
        
        # Create a filter complex string for repeating the audio
        # Using concat filter: [0:a][0:a][0:a]... concat=n=10:v=0:a=1
        filter_parts = "[0:a]" * repeat_times
        filter_complex = f"{filter_parts}concat=n={repeat_times}:v=0:a=1[out]"
        
        # Build ffmpeg command
        cmd = [
            "ffmpeg",
            "-i", str(wav_file),
            "-filter_complex", filter_complex,
            "-map", "[out]",
            "-y",  # Overwrite output file if it exists
            str(output_file)
        ]
        
        try:
            # Run ffmpeg command
            result = subprocess.run(
                cmd,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True
            )
            
            if result.returncode == 0:
                # Get file sizes for confirmation
                original_size = wav_file.stat().st_size / (1024 * 1024)  # MB
                new_size = output_file.stat().st_size / (1024 * 1024)  # MB
                
                print(f"  ✓ Success! {original_size:.2f} MB → {new_size:.2f} MB")
                print(f"  Saved to: {output_file}\n")
            else:
                print(f"  ✗ Error processing {wav_file.name}")
                print(f"  {result.stderr}\n")
                
        except FileNotFoundError:
            print("  ✗ Error: ffmpeg not found. Please ensure ffmpeg is installed and in PATH.")
            return
        except Exception as e:
            print(f"  ✗ Error: {str(e)}\n")
    
    print(f"Processing complete! Extended files saved to '{output_dir}'")

if __name__ == "__main__":
    extend_wav_files(repeat_times=10)