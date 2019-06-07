# This script gets you set up to train DNNs on kubernetes
# 1. Make pod: kubectl create -f mytensorflow.yaml
# 2. Connect to pod: kubectl exec -it mytensorflow-sam  -n cms -- /bin/bash
# 3. apt-get install wget
# 4. wget http://uaf-8.t2.ucsd.edu/~sjmay/ttH/nautilus/kubernetes_setup.sh 
apt-get install openssh-client
apt-get install vim
apt-get install git
apt-get install python2.7
wget https://bootstrap.pypa.io/get-pip.py
python2.7 get-pip.py
rm /usr/local/bin/python
ln -s /usr/bin/python2.7 /usr/local/bin/python
mkdir work
cd work
git clone https://github.com/sam-may/ttH
cd ttH/MVAs
wget http://uaf-8.t2.ucsd.edu/~sjmay/ttH/nautilus/ttHHadronic_v1.6_28May2019_RunII_MVA_Presel_impute_dnn_features_ttGG.hdf5
pip install tensorflow-gpu
pip install keras
pip install sklearn
apt-get install -y python-subprocess32
pip install matplotlib
pip install bayesian-optimization
mkdir dnn_weights
