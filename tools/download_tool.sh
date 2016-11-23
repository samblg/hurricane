DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

TOOL_NAME=$1
TOOL_URL=$2

wget "${TOOL_URL}" -o "${TOOL_NAME}"

if [ -f "${DIR}/../deps/${TOOL_NAME}" ]; then
    echo "${DIR}/../deps/${TOOL_NAME}"
fi
