import os
import sys


def find_imgui_ini_files(rootDir, res):
    for item in os.listdir(rootDir):
        pathname = os.path.join(rootDir, item)
        if os.path.isfile(pathname) and item == "imgui.ini":
            res.append(pathname)
        elif os.path.isdir(pathname):
            find_imgui_ini_files(pathname, res)


def main():
    args = sys.argv
    rootDir = os.path.curdir
    res = []

    if len(args) == 2:
        rootDir = args[1]
    elif len(args) > 2:
        print("Usage: python script.py [rootDir]")
        sys.exit(1)

    find_imgui_ini_files(rootDir, res)
    for file in res:
        print(file)


if __name__ == "__main__":
    main()
