#!/usr/bin/env python
import sys
import os

if len(sys.argv) < 2:
    exit()

SCRIPTS = os.path.dirname(os.path.abspath(sys.argv[0]))
ROOT = os.path.abspath(os.path.join(SCRIPTS, ".."))
os.chdir(ROOT)

tables = []
BUILD = "build"
if not os.path.exists(BUILD) and len(sys.argv) > 1:
    os.mkdir(BUILD)
with open(sys.argv[1], "r", encoding="UTF-8") as f:
    textlines = f.read().split('\n')
for textline in textlines:
    if not textline or ';' in textline[0] or '=' not in textline:
        continue
    values = textline.split('=')
    source = values[0].strip()
    target = '='.join(values[1:]).strip()
    for string in (("\\r", '\r'), ("\\t", '\t'), ("\\n", '\n'), ("\\\\", '\\')):
        source = source.replace(string[0], string[1])
        target = target.replace(string[0], string[1])
    tables.append((source, target))

NAME = os.path.splitext(os.path.basename(sys.argv[1]))[0]

for input in sys.argv[2:]:
    source = os.path.basename(input)
    define = os.path.splitext(source)[0]
    suffix = os.path.splitext(source)[-1]
    output = os.path.join(BUILD, NAME + suffix)

    with open(input, "r", encoding="UTF-8") as f:
        text = f.read()

    for string in tables:
        text = text.replace(string[0], string[1])

    for string in ("__public__ ", "__hidden__ "):
        text = text.replace(string, '')

    for string in (
        (define, NAME),
        ("o_zero_o", "0"),
        ("o_success_o", "0"),
        ("o_failure_o", "1"),
        ("o_invalid_o", "2"),
        ("o_type_o", NAME + "_s"),
        (define.upper(), NAME.upper()),
        ("o_inline_o", "static inline"),
    ):
        text = text.replace(string[0], string[1])

    with open(output, "wb") as f:
        f.write(text.encode("UTF-8"))

    print(input, "->", output)
