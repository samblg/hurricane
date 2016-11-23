./deploy.sh

export LD_LIBRARY_PATH=. 

config_file="supervisor$1".properties
./supervisor "${config_file}"
