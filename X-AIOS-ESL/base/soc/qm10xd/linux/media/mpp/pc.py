#!/usr/bin/python
# -*- coding: UTF-8 -*-

import sys

if len(sys.argv) != 3:
    print("Usage: {} <input_file> <output_file>".format(sys.argv[0]))
    sys.exit(1)

input_file_path = sys.argv[1]
output_file_path = sys.argv[2]

offset = 0x800
with open(input_file_path, 'rb') as f_in:
    f_in.seek(offset)
    data = f_in.read()

    with open(output_file_path, 'wb') as f_out:
        f_out.write(data)
