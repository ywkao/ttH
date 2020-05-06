import numpy

process_dict = {
    "tth"           : 0,
    "dy"            : 1,
    "dipho"         : 2,
    "gjets"         : 3,
    "qcd"           : 4,
    "ttgg"          : 5,
    "ttg"           : 6,
    "vgamma"        : 7,
    "wjets"         : 8,
    "tt"            : 9,
    "data"          : 10,
    "thq"           : 11,
    "thw"           : 12,
    "tgamma"        : 13,
    "ggh"           : 14,
    "vbf"           : 15,
    "vh"            : 16,
    "qcd_gjets"     : 18,
    "ttz"           : 19,
    "vv"            : 20,
    "tv"            : 21,
    "tt_fcnc_hut"   : 22,
    "tt_fcnc_hct"   : 23,
    "st_fcnc_hut"   : 24,
    "st_fcnc_hct"   : 25,
    "ttw"           : 26,
}

process_dict_inverse = {v : k for k, v in process_dict.items()}

def selection(signal, bkg, type):
    sel_string = "("
    for proc in (signal + bkg):
        if proc.lower() in process_dict.keys():
            sel_string += "process_id_ == %d || " % process_dict[proc.lower()]
    
    sel_string = sel_string[:-3]
    sel_string += ")"

    if type == 0: # train
        set = "Training"
        sel_string += " && rand_ > 0.5" #" && (signal_mass_label_ != 0)"
    elif type == 1: # validation
        set = "Testing"
        sel_string += " && rand_ < 0.5 && (signal_mass_category_ == 125 || signal_mass_category_ == -1)"
    elif type == 2: # data
        set = "Data"
        sel_string = "label_ == 2"
    elif type == 3: # final fit
        set = "Final Fit"
        sel_string = " label_ == 2 && rand_ < 0.01" # dummy selection to prevent crashes later

    print "[prep_utils.py] selection for %s set: %s" % (set, sel_string)

    return sel_string

def z_score(array, mean, std, pad_value = -9):
    preprocessed_array = array
    preprocessed_array[array != pad_value] += -mean
    preprocessed_array[array != pad_value] *= 1./std
    return preprocessed_array

def create_features_and_label(features, feature_names, signal, bkg, preprocess_dict = None, z_score = True):
    label = []
    features_array = []

    for name in feature_names:
        feat = numpy.array(features[name])
        if preprocess_dict and z_score:
            feat = z_score(feat, preprocess_dict[name]["mean"], preprocess_dict[name]["std_dev"])
        features_array.append(feat)

    for evt in features["process_id_"]:
        proc = process_dict_inverse[evt]
        if proc in signal:
            label.append(1)
        elif proc in bkg:
            label.append(0)
        elif proc == "data":
            label.append(2)
        else:
            print "[prep_utils.py] proc: %s not included in list of signals or bkgs!! Please check!!" % proc
            label.append(-1)

    return numpy.transpose(numpy.array(features_array)), numpy.array(label)

