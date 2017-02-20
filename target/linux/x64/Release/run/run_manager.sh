./deploy.sh

source environment.sh

config_file="manager$1".properties
./manager "${config_file}"
