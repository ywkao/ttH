import parallel_utils

scan_list = [
	'./ttHHadronicLooper "ttHHadronic_data_sideband_0b_train" "2017" "" "" "binned_NJets"',
	'./ttHHadronicLooper "ttHHadronic_data_sideband_0b_test" "2017" "" "" "binned_NJets"',
	'./ttHHadronicLooper "ttHHadronic_data_sideband_phoID_train" "2017" "" "" "binned_NJets"',
	'./ttHHadronicLooper "ttHHadronic_data_sideband_phoID_test" "2017" "" "" "binned_NJets"',
]

parallel_utils.submit_jobs(scan_list, 8)
