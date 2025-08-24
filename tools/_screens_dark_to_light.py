import os
import re

replacements = [
    (r'Resrc="dark\\', 'Resrc="'),
    (r'ColorSelect="0x40ffffff" ColorHighlight="0x20ffffff" ColorText="0xfff1f1f1"',
     'ColorSelect="0x22b1613e" ColorHighlight="0x185e3925"'),
    (r'ColorActive="0xffe1e1e1" ColorInactive="0xfff1f1f1" ColorHighlight="0xffffffff"',
     'ColorActive="0xff4e7985" ColorInactive="0xff23363b" ColorHighlight="0xff505217"'),
    (r'ColorActive="0xfff1f1f1" ColorInactive="0xfff1f1f1" ColorHighlight="0xfff1f1f1" ColorDisabled="0xff717171"',
     'ColorActive="0xff23363b" ColorInactive="0xff23363b" ColorHighlight="0xff23363b" ColorDisabled="0x7f23363b"')
]

UTF8_BOM = b'\xef\xbb\xbf'

for filename in os.listdir('.'):
    if filename.lower().endswith('.xml'):
        with open(filename, 'rb') as f:
            raw = f.read()

        has_bom = raw.startswith(UTF8_BOM)
        text = raw.decode('utf-8-sig')  # This will strip BOM if present

        original_text = text

        for pattern, replacement in replacements:
            text = re.sub(pattern, replacement, text)

        if text != original_text:
            with open(filename, 'wb') as f:
                if has_bom:
                    f.write(UTF8_BOM)
                f.write(text.encode('utf-8'))
            print(f'Modified: {filename}')
        else:
            print(f'No changes: {filename}')