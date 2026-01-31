import os
import time
import argparse
import logging
import sys

try:
    import pyautogui
    import pyperclip
except Exception as e:
    raise SystemExit("Missing dependency: please install pyautogui and pyperclip (pip install pyautogui pyperclip)")

# ----------------- CONFIG -----------------
ROOT_DIR = "D:\\Games\\fc26_files\\content\\character\\balls\\"
# Offsets and coordinates (will be scaled from screen size by default)
LEFT_CENTER_X_RATIO = 0.25
RIGHT_CENTER_X_RATIO = 0.75
CENTER_Y_RATIO = 0.5

# Menu selection offsets (pixels) - adjust if menu entry positions differ
MENU_FIRST_ENTRY_OFFSET = (8, 8)      # small nudge from context-click position to hit first item
MENU_ENTRY_HEIGHT = 40                # vertical distance between menu items (approx)

# Timing (seconds) - tweak if xNormal is slow on your machine
GLOBAL_PAUSE = 0.05   # pyautogui pause between calls
SMALL_DELAY = 0.4    # after opening context menu before selecting
OPEN_DELAY = 0.8      # after opening file dialog / after selecting file before proceed
GENERATION_DELAY = 4.0  # wait for generation to finish (increase if generation takes longer)
SAVE_DIALOG_DELAY = 0.6
SAVE_DELAY = 1.0
POST_SAVE_DELAY = 1.0

# Safety
pyautogui.FAILSAFE = True
pyautogui.PAUSE = GLOBAL_PAUSE

# ------------------------------------------

logging.basicConfig(level=logging.INFO, format='%(asctime)s %(levelname)s: %(message)s')


def list_target_folders(root_dir):
    """Return list of (folder_path, folder_name, normal_path) for immediate subfolders that contain <name>_normal.png"""
    targets = []
    if not os.path.isdir(root_dir):
        raise FileNotFoundError(f"Root folder not found: {root_dir}")

    for name in sorted(os.listdir(root_dir)):
        folder_path = os.path.join(root_dir, name)
        if not os.path.isdir(folder_path):
            continue
        candidate = os.path.join(folder_path, f"{name}_normal.png")
        if os.path.isfile(candidate):
            targets.append((folder_path, name, candidate))
    return targets


def move_and_right_click(x, y, dry_run=False):
    logging.debug(f"move to ({x},{y}) and right click")
    if dry_run:
        print(f"[DRY] move to ({x},{y}) right click")
        return
    pyautogui.moveTo(x, y, duration=0.12)
    pyautogui.click(button='right')


def click_at(x, y, button='left', dry_run=False):
    logging.debug(f"click {button} at ({x},{y})")
    if dry_run:
        print(f"[DRY] click {button} at ({x},{y})")
        return
    pyautogui.moveTo(x, y, duration=0.08)
    pyautogui.click(button=button)


def paste_and_enter(text, dry_run=False):
    logging.debug(f"paste text and press Enter: {text}")
    if dry_run:
        print(f"[DRY] paste: {text} then Enter")
        return
    pyperclip.copy(text)
    time.sleep(0.06)
    pyautogui.hotkey('ctrl', 'v')
    time.sleep(0.06)
    pyautogui.press('enter')


def process_one(folder_path, folder_name, normal_path, screen_w, screen_h, config, dry_run=False):
    """Perform the exact sequence for a single folder

    Sequence (summary of your steps, implemented robustly):
      1) Right-click center-left -> open context -> select Browse -> paste normal path -> Enter
      2) Wait for file load
      3) Right-click center-right -> open context -> select Generate (first item) -> wait for generation
      4) Right-click center-right -> open context -> select Save (second item) -> paste filename 'cavity' -> Enter -> press Enter on warning

    """
    left_x = int(screen_w * LEFT_CENTER_X_RATIO)
    right_x = int(screen_w * RIGHT_CENTER_X_RATIO)
    center_y = int(screen_h * CENTER_Y_RATIO)

    mx_first = MENU_FIRST_ENTRY_OFFSET[0]
    my_first = MENU_FIRST_ENTRY_OFFSET[1]
    entry_h = MENU_ENTRY_HEIGHT

    logging.info(f"Processing: {folder_name}")

    # 1) left center -> open context
    move_and_right_click(left_x, center_y, dry_run)
    time.sleep(SMALL_DELAY)

    # move a few pixels to first entry and click (select Browse)
    click_at(left_x + mx_first, center_y + my_first, button='left', dry_run=dry_run)
    time.sleep(OPEN_DELAY)

    # 6) paste input path and Enter
    input_path = normal_path
    paste_and_enter(input_path, dry_run=dry_run)
    time.sleep(OPEN_DELAY)

    # 10) right center -> generate
    move_and_right_click(right_x, center_y, dry_run)
    time.sleep(SMALL_DELAY)
    click_at(right_x + mx_first, center_y + my_first, button='left', dry_run=dry_run)  # Generate
    time.sleep(config['generation_delay'])

    # 15) open context again and click on "Save" (second entry)
    move_and_right_click(right_x, center_y, dry_run)
    time.sleep(SMALL_DELAY)
    # second menu entry is one entry down
    click_at(right_x + mx_first, center_y + my_first + entry_h, button='left', dry_run=dry_run)
    time.sleep(SAVE_DIALOG_DELAY)

    # 16) paste filename 'cavity' and Enter
    paste_and_enter('cavity', dry_run=dry_run)
    time.sleep(0.12)

    # 17) warning message box -> press Enter to confirm
    pyautogui.press('enter')
    
    time.sleep(SAVE_DELAY)
    pyautogui.press('enter')

    time.sleep(POST_SAVE_DELAY)


def main(argv=None):
    parser = argparse.ArgumentParser(description='Automate cavity map creation in xNormal for many folders')
    parser.add_argument('--root', default=ROOT_DIR, help='Root folder containing subfolders')
    parser.add_argument('--dry-run', action='store_true', help='Only print actions without moving mouse')
    parser.add_argument('--generation-delay', type=float, default=GENERATION_DELAY, help='Time to wait for generation')
    parser.add_argument('--start-delay', type=float, default=3.0, help='Seconds to wait before starting (give you time to switch to xNormal)')
    args = parser.parse_args(argv)

    config = {
        'generation_delay': args.generation_delay
    }

    targets = list_target_folders(args.root)
    if not targets:
        logging.info('No target folders found. Nothing to do.')
        return

    logging.info(f'Found {len(targets)} targets. First few: { [t[1] for t in targets[:6]] }')

    if args.dry_run:
        logging.info('Running in DRY RUN mode. No mouse/keyboard events will be sent.')

    screen_w, screen_h = pyautogui.size()
    logging.info(f'Screen size detected: {screen_w}x{screen_h}')

    if not args.dry_run and args.start_delay > 0:
        logging.info(f'Starting in {args.start_delay} seconds. Move mouse to top-left corner to abort (pyautogui failsafe).')
        time.sleep(args.start_delay)

    for folder_path, folder_name, normal_path in targets:
        try:
            process_one(folder_path, folder_name, normal_path, screen_w, screen_h, config, dry_run=args.dry_run)
        except Exception as e:
            logging.exception(f'Error processing {folder_name}: {e}')
            # continue with next folder
            time.sleep(0.8)

    logging.info('All done.')


if __name__ == '__main__':
    main()
