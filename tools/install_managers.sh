DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

source "${DIR}/config.sh"

while read node; do
    scp "${DIR}../" -r "${MANAGER_USER}"@"$node":"${HURRICANE_HOME}"
    ssh "$node" 'cd ${HURRICANE_HOME};tools/build.sh'
done < nodes.list
