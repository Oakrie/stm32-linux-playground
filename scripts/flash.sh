#!/usr/bin/env bash
#
# flash.sh - detect an attached ST-LINK (Nucleo-F446RE), build with CMake,
#            and program the resulting ELF onto the STM32F446RE with OpenOCD.
#
# usage: scripts/flash.sh [-t Debug|Release] [-c] [-b] [-h]
#   -t  CMake build type (default: Debug)
#   -c  clean build directory before building
#   -b  build only, skip device detection and flashing
#   -h  show this help

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(dirname "${SCRIPT_DIR}")"
BUILD_DIR="${PROJECT_DIR}/build"   # must stay one level deep: LINKER_SCRIPT is "../linker/..."
PROJECT_NAME="stm-devboard"
ELF="${BUILD_DIR}/${PROJECT_NAME}.elf"

BUILD_TYPE="Debug"
CLEAN=0
BUILD_ONLY=0

# ST-LINK USB IDs (vendor 0483): V2, V2.1 (Nucleo), V2.1 no-MSD, V3 variants
STLINK_VID="0483"
STLINK_PIDS=("3748" "374b" "3752" "374d" "374e" "374f" "3753" "3754")

info()  { printf '\033[1;34m[*]\033[0m %s\n' "$*"; }
ok()    { printf '\033[1;32m[+]\033[0m %s\n' "$*"; }
die()   { printf '\033[1;31m[!]\033[0m %s\n' "$*" >&2; exit 1; }

usage() { sed -n '3,11p' "${BASH_SOURCE[0]}" | sed 's/^# \{0,1\}//'; exit "${1:-0}"; }

while getopts ":t:cbh" opt; do
    case "${opt}" in
        t) BUILD_TYPE="${OPTARG}" ;;
        c) CLEAN=1 ;;
        b) BUILD_ONLY=1 ;;
        h) usage 0 ;;
        *) usage 1 ;;
    esac
done

require() {
    command -v "$1" >/dev/null 2>&1 || die "'$1' not found in PATH"
}

detect_device() {
    info "Looking for ST-LINK debugger..."
    require lsusb

    local pid line
    for pid in "${STLINK_PIDS[@]}"; do
        line="$(lsusb -d "${STLINK_VID}:${pid}" 2>/dev/null || true)"
        if [[ -n "${line}" ]]; then
            ok "Found: ${line}"
            return 0
        fi
    done

    local msg="No ST-LINK found. Is the Nucleo-F446RE plugged in?"
    if grep -qi microsoft /proc/version 2>/dev/null; then
        msg+=$'\n    WSL detected: attach it from Windows (admin PowerShell):'
        msg+=$'\n      usbipd list'
        msg+=$'\n      usbipd bind --busid <BUSID>'
        msg+=$'\n      usbipd attach --wsl --busid <BUSID>'
    fi
    die "${msg}"
}

build() {
    require cmake
    require arm-none-eabi-gcc

    if [[ "${CLEAN}" -eq 1 && -d "${BUILD_DIR}" ]]; then
        info "Cleaning ${BUILD_DIR}"
        rm -rf "${BUILD_DIR}"
    fi

    info "Configuring (${BUILD_TYPE})..."
    cmake -S "${PROJECT_DIR}" -B "${BUILD_DIR}" -DCMAKE_BUILD_TYPE="${BUILD_TYPE}"

    info "Building..."
    cmake --build "${BUILD_DIR}" -j"$(nproc)"

    [[ -f "${ELF}" ]] || die "Build finished but ${ELF} was not produced"
    ok "Built ${ELF}"
}

flash() {
    require openocd

    info "Flashing ${ELF} to STM32F446RE..."
    openocd \
        -f interface/stlink.cfg \
        -f target/stm32f4x.cfg \
        -c "program ${ELF} verify reset exit"

    ok "Flash complete, target reset"
}

if [[ "${BUILD_ONLY}" -eq 0 ]]; then
    detect_device
fi

build

if [[ "${BUILD_ONLY}" -eq 0 ]]; then
    flash
fi
