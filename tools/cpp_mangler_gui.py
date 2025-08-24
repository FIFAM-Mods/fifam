import sys
import re
from PyQt5.QtWidgets import QApplication, QWidget, QVBoxLayout, QTextEdit, QLabel

def mangle_itanium(function_signature):
    """
    Mangle a C++ function name according to the Itanium ABI.
    Supports namespaces, templates, pointers, references, constructors, destructors, and operators.
    """
    # Helper function to encode a single identifier (type or namespace)
    def encode_identifier(name):
        return f"{len(name)}{name}"

    # Helper function to encode a type (handles pointers, references, templates)
    def encode_type(type_str):
        type_str = type_str.strip()
        if type_str.endswith('*'):
            return 'P' + encode_type(type_str[:-1].strip())
        if type_str.endswith('&'):
            return 'R' + encode_type(type_str[:-1].strip())
        # Handle template types, e.g., CFMEntityTraits<CDBCountry *>
        if '<' in type_str:
            base, template = re.match(r'([^\<]+)\<(.+)\>', type_str).groups()
            template_types = split_template_args(template)
            encoded_templates = ''.join(encode_type(t) for t in template_types)
            return f"I{encode_identifier(base)}{encoded_templates}E"
        return encode_identifier(type_str)

    # Split template arguments, handling nested templates
    def split_template_args(template_str):
        args = []
        current = ""
        nest_level = 0
        for char in template_str:
            if char == '<':
                nest_level += 1
            elif char == '>':
                nest_level -= 1
            elif char == ',' and nest_level == 0:
                args.append(current.strip())
                current = ""
                continue
            current += char
        if current.strip():
            args.append(current.strip())
        return args

    # Encode operator names
    def encode_operator(op):
        operator_map = {
            '++': 'pp', '--': 'mm', '+': 'pl', '-': 'mi', '*': 'ml',
            '/': 'dv', '%': 'mo', '&': 'an', '|': 'or', '^': 'eo',
            '=': 'aS', '==': 'eq', '!=': 'ne', '<': 'lt', '>': 'gt',
            '<=': 'le', '>=': 'ge', '<<': 'ls', '>>': 'rs', '()': 'cl',
            '[]': 'ix'
        }
        return operator_map.get(op, None)

    # Parse the function signature
    # Format: [namespace::]class[<template>][::function]([args])
    match = re.match(r'((?:[a-zA-Z0-9_]+::)*)?([a-zA-Z0-9_]+(?:\<.+?\>)?)(::([a-zA-Z0-9_~]+|\S+))?\((.*?)\)', function_signature)
    if not match:
        raise ValueError(f"Invalid function signature: {function_signature}")

    namespaces, class_name, _, func_name, args = match.groups()
    
    # Initialize mangled name with _Z
    mangled = "_Z"

    # Encode namespaces
    if namespaces:
        ns_list = namespaces.strip(':').split('::')
        mangled += 'N' + ''.join(encode_identifier(ns) for ns in ns_list)
    else:
        mangled += 'N'

    # Encode class name (including template if present)
    mangled += encode_type(class_name)

    # Handle function name
    if func_name:
        if func_name.startswith('operator'):
            op = func_name[len('operator'):]
            encoded_op = encode_operator(op)
            if not encoded_op:
                raise ValueError(f"Unsupported operator: {op}")
            mangled += encoded_op
        elif func_name == class_name.split('<')[0]:  # Constructor
            mangled += 'C1'
        elif func_name == '~' + class_name.split('<')[0]:  # Destructor
            mangled += 'D1'
        else:
            mangled += encode_identifier(func_name)
    else:
        mangled += 'C1'  # Constructor if no function name

    # Close nested scope
    mangled += 'E'

    # Encode arguments
    if args:
        arg_types = split_template_args(args) if args else []
        mangled += ''.join(encode_type(arg.strip()) for arg in arg_types)
    else:
        mangled += 'v'  # Void for no arguments

    return mangled

class CppMangler(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("C++ Name Mangler (Itanium ABI via cppmangle)")

        self.inputBox = QTextEdit()
        self.outputBox = QTextEdit()
        self.outputBox.setReadOnly(True)

        self.inputBox.textChanged.connect(self.onTextChanged)

        layout = QVBoxLayout()
        layout.addWidget(QLabel("C++ Signatures (e.g. int foo(int, float))"))
        layout.addWidget(self.inputBox)
        layout.addWidget(QLabel("Mangled Names:"))
        layout.addWidget(self.outputBox)
        self.setLayout(layout)

    def onTextChanged(self):
        input_text = self.inputBox.toPlainText()
        output_lines = []
        for line in input_text.splitlines():
            mangled = self.mangle_line(line.strip())
            output_lines.append(mangled)
        self.outputBox.setPlainText("\n".join(output_lines))

    def mangle_line(self, line):
        try:
            return mangle_itanium(line)
    
        except Exception as e:
            return f"error: {e}"

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = CppMangler()
    window.resize(800, 600)
    window.show()
    sys.exit(app.exec_())
