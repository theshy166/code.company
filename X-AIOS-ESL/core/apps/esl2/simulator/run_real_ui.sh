#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

SN="${SN:-${ESL2_SIM_SN:-}}"
REAL_RES_DIR="${REAL_RES_DIR:-${ESL2_SIM_REAL_RES_DIR:-}}"
MODEL_JSON="${MODEL_JSON:-${ESL2_SIM_MODEL_JSON:-}}"

mkdir -p "${BUILD_DIR}"
cmake -S "${SCRIPT_DIR}" -B "${BUILD_DIR}"
cmake --build "${BUILD_DIR}" -j"$(nproc)"

if [[ -n "${MODEL_JSON}" && -z "${SN}" ]]; then
    base="$(basename "${MODEL_JSON}")"
    if [[ "${base}" =~ ^product_base_model_(.*)_1\.json$ ]]; then
        SN="${BASH_REMATCH[1]}"
    fi
fi

if [[ -n "${REAL_RES_DIR}" ]]; then
    if [[ ! -d "${REAL_RES_DIR}" ]]; then
        echo "REAL_RES_DIR does not exist: ${REAL_RES_DIR}" >&2
        exit 1
    fi

    mkdir -p "${BUILD_DIR}/out/simulator/xos/res"
    for name in json img logo ttf esl2; do
        if [[ -e "${REAL_RES_DIR}/${name}" ]]; then
            rm -rf "${BUILD_DIR}/out/simulator/xos/res/${name}"
            cp -a "${REAL_RES_DIR}/${name}" "${BUILD_DIR}/out/simulator/xos/res/${name}"
        fi
    done
fi

if [[ -n "${MODEL_JSON}" ]]; then
    if [[ -z "${SN}" ]]; then
        echo "Set SN when MODEL_JSON is not named product_base_model_<SN>_1.json" >&2
        exit 1
    fi
    if [[ ! -f "${MODEL_JSON}" ]]; then
        echo "MODEL_JSON does not exist: ${MODEL_JSON}" >&2
        exit 1
    fi
    mkdir -p "${BUILD_DIR}/out/simulator/xos/res/json"
    cp -f "${MODEL_JSON}" "${BUILD_DIR}/out/simulator/xos/res/json/product_base_model_${SN}_1.json"
fi

cd "${BUILD_DIR}"

if [[ -n "${SN}" ]]; then
    export ESL2_SIM_SN="${SN}"
fi

echo "Starting ESL2 real UI simulator"
echo "  build: ${BUILD_DIR}"
echo "  SN: ${ESL2_SIM_SN:-SIM-ESL2-001}"
echo "  REAL_RES_DIR: ${REAL_RES_DIR:-<not set>}"
echo "  MODEL_JSON: ${MODEL_JSON:-<not set>}"

exec ./esl2_sim
