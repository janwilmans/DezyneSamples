"""Simple text filter
"""

from __future__ import print_function
import traceback
import sys
import os

def filter(keyword):
    for line in sys.stdin:
        if not keyword in line:
            sys.stdout.write(line)
        else:
            sys.stdout.write("// " + line)

def main():
    if (len(sys.argv) != 2):
        print ("Usage: " + os.path.basename(sys.argv[0]) + " <keyword>]")
        return
    filter(sys.argv[1]);

if __name__ == "__main__":
    try:
        main()
    except SystemExit :
        pass
    except KeyboardInterrupt:
        pass
    except Exception:
        traceback.print_exc(file=sys.stdout) 
    sys.exit(0)
    

