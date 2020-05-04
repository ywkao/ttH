#!/bin/bash
time python do_fcnc.py --baby_version "v4.11" --tag "v4.11_all" 2>&1 | tee log_all
#time python do_fcnc.py --baby_version "v4.11" --tag "v4.11_topTagger" 2>&1 | tee log_topTagger
#time python do_fcnc.py --baby_version "v4.11" --tag "v4.11_chi2" 2>&1 | tee log_chi2
#time python do_fcnc.py --baby_version "v4.11" --tag "v4.11_none" 2>&1 | tee log_none
