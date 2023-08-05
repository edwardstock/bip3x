#!/usr/bin/env python3
import argparse
import os
import platform
import subprocess

###### DEFINE PROJECT CONFIG HERE ######
CONAN_PROJECT_NAME = "bip3x"
CONAN_REPO = "edwardstock"
CONAN_REPO_TAG = "latest"
# only boolean options supported
BUILD_OPTIONS = ["shared", "enable_jni", "enable_c", "with_openssl_rand"]
###### END PROJECT CONFIG ##############


def get_version():
    if os.path.isfile("version.info"):
        version = open("version.info").read().strip()
    elif os.path.isfile("../version.info"):
        version = open("../version.info").read().strip()
    else:
        version = ""

    return version


def combine_options(build_options, build_values):
    build_options_tmp = []
    result = []
    opts_cnt = len(build_options)
    values_cnt = len(build_values)
    tmp_cnt = values_cnt * opts_cnt

    # Populate build_options_tmp
    for value_idx in range(values_cnt):
        for opt_idx in range(opts_cnt):
            option = f"{build_options[opt_idx]}={build_values[value_idx]}"
            build_options_tmp.append(option)

    # Combine options
    for i in range(tmp_cnt):
        option = ""
        for n in range(opts_cnt):
            next_index = (i + n) % tmp_cnt
            option += " " + build_options_tmp[next_index]
        result.append(option)

    print(result)
    return result


def upload(version):
    cmd_upload = [
        "conan",
        "upload",
        "{}/{}@{}/{}".format(CONAN_PROJECT_NAME, version, CONAN_REPO, CONAN_REPO_TAG),
        "-r={}".format(CONAN_REPO),
    ]

    subprocess.run(cmd_upload, check=True)


def build():
    sysname = os.name
    std_lib_names = []
    values = ["True", "False"]

    if sysname == "posix":
        if platform.system() == "Darwin":
            std_lib_names = ["libc++"]
        else:
            std_lib_names = ["libstdc++", "libstdc++11"]
    else:
        std_lib_names = []

    version = get_version()
    target_options = combine_options(BUILD_OPTIONS, values)

    if len(std_lib_names) == 0:
        # iterate over available c++ stdlib
        for optionSet in target_options:
            cmd_debug = [
                "conan",
                "create",
                ".",  # current dir, call this script from project root
                "-s",
                "build_type=Debug",
                "--build=missing",
                "--user={}".format(CONAN_REPO),
                "--channel={}".format(CONAN_REPO_TAG),
            ]

            cmd_options = []
            for optionItem in optionSet.split(" "):
                cmd_options.append("-o")
                cmd_options.append(optionItem)

            cmd_debug.extend(cmd_options)

            cmd_release = cmd_debug.copy()
            cmd_release[4] = "build_type=Release"

            subprocess.run(cmd_debug, check=True)
            subprocess.run(cmd_release, check=True)
    else:
        for std_lib in std_lib_names:
            for optionSet in target_options:
                cmd_debug = [
                    "conan",
                    "create",
                    ".",  # current dir, call this script from project root
                    "-s",
                    "build_type=Debug",
                    "-s",
                    "compiler.libcxx={}".format(std_lib),
                    "--build=missing",
                    "--user={}".format(CONAN_REPO),
                    "--channel={}".format(CONAN_REPO_TAG),
                ]
                cmd_options = []
                for optionItem in optionSet.split(" "):
                    cmd_options.append("-o")
                    cmd_options.append(optionItem)

                cmd_debug.extend(cmd_options)

                cmd_release = cmd_debug.copy()
                cmd_release[4] = "build_type=Release"

                subprocess.run(cmd_debug, check=True)
                subprocess.run(cmd_release, check=True)


def clear_cache():
    cmd_remove_all = [
        "conan",
        "remove",
        "{}/*".format(CONAN_PROJECT_NAME),
        "-c"
    ]

    cmd_remove_specific = [
        "conan",
        "remove",
        "{}/*@{}/{}".format(CONAN_PROJECT_NAME, CONAN_REPO, CONAN_REPO_TAG),
        "-c"
    ]

    cmd_cache_clean_all = [
        "conan",
        "cache",
        "clean",
        "{}/*".format(CONAN_PROJECT_NAME),
        "--source",
        "--build",
        "--download"
    ]

    cmd_cache_clean_specific = [
        "conan",
        "cache",
        "clean",
        "{}/*@{}/{}".format(CONAN_PROJECT_NAME, CONAN_REPO, CONAN_REPO_TAG),
        "--source",
        "--build",
        "--download"
    ]

    subprocess.run(cmd_remove_all, check=True)
    subprocess.run(cmd_remove_specific, check=True)
    subprocess.run(cmd_cache_clean_all, check=True)
    subprocess.run(cmd_cache_clean_specific, check=True)


def main():
    parser = argparse.ArgumentParser(description='Conan Build and Upload script.')
    parser.add_argument('command', choices=['build', 'upload', 'clear-cache'], help='The command to execute.')
    parser.add_argument('--force', action='store_true', help='If set, clear cache before build.')

    args = parser.parse_args()

    version = get_version()

    if args.command == 'build':
        if args.force:
            clear_cache()
        build()
    elif args.command == 'upload':
        upload(version)
    elif args.command == 'clear-cache':
        clear_cache()


if __name__ == "__main__":
    main()
