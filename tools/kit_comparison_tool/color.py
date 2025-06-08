import math
from typing import List, Tuple, Union
from colormath.color_objects import sRGBColor, LabColor
from colormath.color_conversions import convert_color
from colormath.color_diff import delta_e_cie2000
import numpy

def patch_asscalar(a):
    return a.item()
    
setattr(numpy, "asscalar", patch_asscalar)

class Utils:
    @staticmethod
    def format(fmt: str, *args):
        return fmt % args

class Color:
    def __init__(self, red: Union[int, None] = None, green: int = None, blue: int = None):
        if red is None:
            self.r, self.g, self.b = 255, 255, 255
        elif green is None and blue is None:
            self.set_from_int(red)
        else:
            self.set(red, green, blue)

    def set(self, red: int, green: int, blue: int):
        self.r, self.g, self.b = red, green, blue

    def set_from_int(self, int_value: int):
        self.r = (int_value >> 16) & 0xFF
        self.g = (int_value >> 8) & 0xFF
        self.b = int_value & 0xFF

    @staticmethod
    def distance(e1: 'Color', e2: 'Color') -> float:
        srgb1 = sRGBColor(e1.r / 255.0, e1.g / 255.0, e1.b / 255.0)
        srgb2 = sRGBColor(e2.r / 255.0, e2.g / 255.0, e2.b / 255.0)
        lab1 = convert_color(srgb1, LabColor)
        lab2 = convert_color(srgb2, LabColor)
        return delta_e_cie2000(lab1, lab2)

    def __eq__(self, other: 'Color') -> bool:
        return self.r == other.r and self.g == other.g and self.b == other.b

    def __ne__(self, other: 'Color') -> bool:
        return not self == other

    def __lt__(self, other: 'Color') -> bool:
        return self.to_hex_str() <= other.to_hex_str()

    def find_index_in_table(self, table: List[Tuple[int, 'Color']], table_min=0, table_max=0) -> int:
        if table_max == 0:
            table_max = 9999
        for index, color in table:
            if table_min <= index <= table_max and self == color:
                return index

        closest_index = 0
        min_dist = float('inf')
        for index, color in table:
            if table_min <= index <= table_max:
                dist = self.distance(self, color)
                if dist < min_dist:
                    min_dist = dist
                    closest_index = index
        return closest_index

    def set_from_table(self, table: List[Tuple[int, 'Color']], index: int, table_min=0, table_max=0):
        if not table:
            self.set(255, 255, 255)
            return

        if table_max == 0:
            table_max = 9999

        for idx, color in table:
            if idx == index and table_min <= idx <= table_max:
                self.r, self.g, self.b = color.r, color.g, color.b
                return

        self.set(255, 255, 255)

    def to_str(self) -> str:
        return Utils.format("(%u,%u,%u)", self.r, self.g, self.b)

    def to_hex_str(self) -> str:
        return Utils.format("#%02X%02X%02X", self.r, self.g, self.b)

    def to_int(self) -> int:
        return (0xFF << 24) | (self.r << 16) | (self.g << 8) | self.b


GenColor = Color


class ColorPair:
    def __init__(self, color1: Color = None, color2: Color = None):
        if color1 is None:
            self.first = Color()
            self.second = Color()
        else:
            self.set(color1, color2)

    def set(self, color1: Color, color2: Color):
        self.first = color1
        self.second = color2

    @staticmethod
    def distance(e1: 'ColorPair', e2: 'ColorPair') -> float:
        d11 = Color.distance(e1.first, e2.first)
        d22 = Color.distance(e1.second, e2.second)
        d12 = Color.distance(e1.first, e2.second)
        d21 = Color.distance(e1.second, e2.first)
        best_match = min(d11, d22, d12, d21)
        average_all = (d11 + d22 + d12 + d21) / 4.0
        return (best_match + average_all) / 2.0

    def __eq__(self, other: 'ColorPair') -> bool:
        return ((self.first == other.first and self.second == other.second) or
                (self.first == other.second and self.second == other.first))

    def find_index_in_table(self, table: List[Tuple[int, 'ColorPair']], table_min=0, table_max=0) -> int:
        if table_max == 0:
            table_max = 9999
        for index, pair in table:
            if table_min <= index <= table_max and self == pair:
                return index

        closest_index = 0
        min_dist = float('inf')
        for index, pair in table:
            if table_min <= index <= table_max:
                dist = ColorPair.distance(self, pair)
                if dist < min_dist:
                    min_dist = dist
                    closest_index = index
        return closest_index

    def set_from_table(self, table: List[Tuple[int, 'ColorPair']], index: int, table_min=0, table_max=0):
        if not table:
            self.set(Color(), Color())
            return

        if table_max == 0:
            table_max = 9999

        for idx, pair in table:
            if idx == index and table_min <= idx <= table_max:
                self.first = pair.first
                self.second = pair.second
                return

        self.set(Color(), Color())

    def to_str(self) -> str:
        return f"{self.first.to_str()}:{self.second.to_str()}"

    def to_hex_str(self) -> str:
        return f"{self.first.to_hex_str()}:{self.second.to_hex_str()}"


GenColorPair = ColorPair
