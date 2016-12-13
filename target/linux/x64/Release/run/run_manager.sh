./deploy.sh

export LD_LIBRARY_PATH=. 

config_file="manager$1".properties
./manager "${config_file}"
