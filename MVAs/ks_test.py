from scipy.stats import ks_2samp

def logical_vector(values, labels, label_target):
  array = []
  if not(len(values) == len(labels)):
    raise RuntimeError("length of target array must be the same as label array")
  for i in range(len(values)):
    if labels[i] == label_target:
      array.append(values[i])
  return array


def ks_test(pred_train, pred_test, y_train, y_test):
  pred_train_sig = logical_vector(pred_train, y_train, 1)
  pred_train_bkg = logical_vector(pred_train, y_train, 0)
  pred_test_sig = logical_vector(pred_test, y_test, 1)
  pred_test_bkg = logical_vector(pred_test, y_test, 0)

  d_sig, p_value_sig = ks_2samp(pred_train_sig, pred_test_sig)
  d_bkg, p_value_bkg = ks_2samp(pred_train_bkg, pred_test_bkg)

  return d_sig, p_value_sig, d_bkg, p_value_bkg

