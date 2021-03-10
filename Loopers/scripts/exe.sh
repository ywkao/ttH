#!/bin/bash
#time python do_fcnc.py --baby_version "v5.7" --tag "v5.7_dataOnlyV2" 2>&1 | tee -a log_debug_20210301_v3.txt
#time python do_fcnc.py --baby_version "v5.7" --tag "v5.7_tprimetH" 2>&1 | tee -a log_tprimetH_20210303_plots.txt
time python do_fcnc.py --baby_version "v5.7" --tag "v5.7_tprimetH" 2>&1 | tee -a log_tprimetH_20210310_mvaBaby.txt
