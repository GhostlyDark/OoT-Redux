#!/usr/bin/env bash
set -ex

script_dir="$(dirname "$0")"
toplvl_dir="$(realpath "$script_dir")"
rom_dir="$toplvl_dir/roms"
tool_dir="$toplvl_dir/tools"

./build.py --compile-c

if [[ $(uname -s) = MINGW32* ]]; then
    rm -f "$rom_dir"/redux.ppf
    "$tool_dir"/MakePPF3 c "$rom_dir"/base.z64 "$rom_dir"/patched.z64 "$rom_dir"/redux.ppf
fi
