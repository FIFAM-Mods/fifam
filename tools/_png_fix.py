from pathlib import Path
import sys

try:
    from PIL import Image
except ImportError:
    print("Pillow is required. Install with: pip install pillow")
    sys.exit(1)

def needs_fix(p: Path) -> bool:
    try:
        with p.open("rb") as f:
            buf = f.read(26)
            if len(buf) < 26:
                return False
            return (buf[24] != 8) or (buf[25] != 6)
    except Exception as e:
        print(f"  [!] Error reading {p}: {e}")
        return False

def fix_image(p: Path) -> bool:
    try:
        with Image.open(p) as img:
            img = img.convert("RGBA")
            img.save(p, format="PNG", optimize=True)
        return True
    except Exception as e:
        print(f"  [!] Failed to fix {p}: {e}")
        return False

def main():
    cwd = Path.cwd()
    print(f"Scanning: {cwd}")

    fixed_count = 0
    checked_count = 0

    for p in cwd.rglob("*"):
        if p.is_file() and p.suffix.lower() == ".png":
            checked_count += 1
            try:
                if needs_fix(p):
                    print(f"Fix {p.name}")
                    if fix_image(p):
                        fixed_count += 1
            except Exception as e:
                print(f"  [!] Unexpected error with {p}: {e}")

    print()
    print(f"Checked PNG files: {checked_count}")
    print(f"Fixed files: {fixed_count}")

    try:
        input("\nPress Enter to exit...")
    except EOFError:
        pass

if __name__ == "__main__":
    main()
