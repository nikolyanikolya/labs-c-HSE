#!/usr/bin/env python3
import logging
import os
import subprocess
import sys
import threading
import time


def read_exact(f, expected):
    result = b""
    remaining = len(expected)
    while remaining > 0:
        chunk = f.read(remaining)
        assert chunk
        assert len(chunk) <= remaining
        result += chunk
        remaining -= len(chunk)
    if expected != result:
        print(f"Expected {expected}, got {result}")
        sys.exit(1)


def main():
    with subprocess.Popen("./employees", stdin=subprocess.PIPE, stdout=subprocess.PIPE) as proc:
        def thread_func():
            time.sleep(2)
            proc.poll()
            if proc.returncode is None:
                print("Process did not terminate in time, killing.")
                print("Did you flush stdout after printing the prompt?")
                proc.kill()
        th = threading.Thread(target=thread_func, daemon=True)
        th.start()

        read_exact(proc.stdout, b"> ")
        print("Read prompt successfully")
        proc.stdin.write(b"list-devs" + os.linesep.encode("ascii"))
        proc.stdin.flush()

        read_exact(proc.stdout, b"0 developer(s) found:" + os.linesep.encode("ascii"))
        print("Read response successfully")

        read_exact(proc.stdout, b"> ")
        print("Read second prompt successfully")
        proc.stdin.close()

        proc.wait()

    if proc.returncode != 0:
        print(f"Process terminated with {proc.returncode}")
        sys.exit(1)

    print("SUCCESS")


if __name__ == "__main__":
    main()
