DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
KAKEFILE_DIR="${DIR}/../kake/Hurricane"
BUILD_DIR="${DIR}/../target/build/linux/x64/Release"

source "${DIR}/deps.sh"

echo "[INFO] Check NodeJS"
NODE_PATH=$("${DIR}/check_tool.sh" NodeJS node ../deps/node)
NODE_DIR=$(dirname ${NODE_PATH})

echo "[INFO] Check Kake"
KAKE_PATH=$("${DIR}/check_tool.sh" Kake kake ../deps/kake)
KAKE_DIR=$(dirname ${KAKE_PATH})

echo "${NODE_DIR}"
echo "${KAKE_DIR}"

if [ -z "${KAKE_DIR}" ]; then
    echo "You have not installed kake, do you want to install it?(Y/N)"
    read to_install_kake

    if [ "${to_install_kake}" == "N" ]; then
        make install
        exit 0
    fi
fi

if [ -z "${NODE_DIR}" ]; then
    echo "[INFO] Download NodeJS"
    "${DIR}/download_tool.sh" node
fi

if [ -z "${KAKE_DIR}" ]; then
    echo "[INFO] Download Kake"
    "${DIR}/download_tool.sh" kake "${KAKE_URL}"
fi

pushd .

cd "${KAKEFILE_DIR}"
npm install
"${KAKE_DIR}/kake" generate

cd "${BUILD_DIR}"
make

popd
