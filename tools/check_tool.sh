DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

TOOL_NAME=$1
TOOL_EXEC=$2
TOOL_DIR="${DIR}/../deps"

TOOL_PATH=$(which ${TOOL_EXEC})

if [ -z "${TOOL_PATH}" ]; then
    if [ -f "${TOOL_DIR}/${TOOL_NAME}" ]; then
        echo "${TOOL_DIR}/${TOOL_NAME}"
    fi
fi
    
echo ${TOOL_PATH}
