# This script gets you set up to train DNNs on kubernetes
# 1. Make pod: kubectl create -f optimize_dnn_hyperparams.yaml 
# 2. Connect to pod: kubectl exec -it mytensorflow-sam  -n cms -- /bin/bash

INPUT=$1
TAG=$2

# Set up ssh key
apt-get install -y openssh-client
mkdir ~/.ssh
cp /etc/secret-volume/ssh-publickey ~/.ssh/id_rsa.pub
cp /etc/secret-volume/ssh-privatekey ~/.ssh/id_rsa
chmod 600 ~/.ssh/id_rsa
echo "Blah blah blah" >> blah.txt
scp -o StrictHostKeyChecking=no blah.txt sjmay@uaf-10.t2.ucsd.edu:~/ttH/MVAs/nautilus/blah_test.txt

# Install needed libs
apt-get install -y vim
apt-get install -y git
apt-get install -y python2.7
wget https://bootstrap.pypa.io/get-pip.py
python2.7 get-pip.py
rm /usr/local/bin/python
ln -s /usr/bin/python2.7 /usr/local/bin/python
mkdir work
cd work
git clone https://github.com/sam-may/ttH 
cd ttH/MVAs
wget "http://uaf-8.t2.ucsd.edu/~sjmay/ttH/nautilus/$INPUT"
pip install tensorflow-gpu
pip install keras
pip install sklearn
apt-get install -y python-subprocess32
pip install matplotlib
pip install bayesian-optimization
pip install keras-layer-normalization

# Optimize DNN
mkdir dnn_weights

cp nautilus/copy_jsons.sh .
./copy_jsons.sh &

python optimize_dnn.py --input "$INPUT" --tag "fixed_$TAG" --channel "Hadronic" --no_bootstrap --n_points "5" --pbounds "pbounds_fixed"
python optimize_dnn.py --input "$INPUT" --tag "$TAG" --channel "Hadronic" --no_bootstrap --n_points "5" --pbounds "pbounds_light"
python optimize_dnn.py --input "$INPUT" --tag "$TAG" --channel "Hadronic" --no_bootstrap --n_points "10" --pbounds "pbounds_full"

scp *.json sjmay@uaf-10.t2.ucsd.edu:~/ttH/MVAs/nautilus/results/
