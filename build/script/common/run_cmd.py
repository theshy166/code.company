#!/usr/bin/env python3
import os
import subprocess
import sys

print("hello, this is run_cmd.py")

def main(argv):
    print("Environment Variables:")
    for key, value in os.environ.items():
        print(f"{key}: {value}")

    if len(sys.argv) == 3:
        subprocess.call(['/bin/bash', sys.argv[1], sys.argv[2]])
    elif len(sys.argv) == 4:
        subprocess.call(['/bin/bash', sys.argv[1], sys.argv[2], sys.argv[3]])
    else:
        subprocess.call(['/bin/bash', sys.argv[1]])

if __name__ == "__main__":
    print(f"Arguments count: {len(sys.argv)}")
    for i, arg in enumerate(sys.argv):
        print(f"Argument: {i:>6}: {arg}")
    main(sys.argv)

