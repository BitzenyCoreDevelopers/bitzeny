#!/usr/bin/env python3
# python3 replace-locale.py < bitcoin_ja.ts > bitcoin_ja.ts.replaced

import fileinput
import re

w = {
    'bitcoin': 'bitzeny',
    'Bitcoin': 'BitZeny',
    'ビットコイン': 'BitZeny'
}

pattern = re.compile('(' + '|'.join(w.keys()) + ')')

skip_line = False

for line in fileinput.input():
    if '<source>Bitcoin Core</source>' in line:  # Skip copyright string
        skip_line = True
        print(line, end='')
        continue

    if not skip_line and '<translation>' in line:
        line = pattern.sub(lambda x: w[x.group()], line)

    if skip_line:
        skip_line = False

    print(line, end='')
