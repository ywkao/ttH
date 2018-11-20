std::map<double, double> m = {
  {-0.8, 0.625029},
  {-0.6, 1.1893},
  {-0.4, 1.3432},
  {-0.2, 1.54542},
  { 0.0, 1.67213},
  { 0.2, 1.84241},
  { 0.4, 2.06992},
  { 0.6, 2.24219},
  { 0.8, 2.9558},
  { 1.0, 4.24346}
};

double prob_ratio_from_madgraph(double mva_score) {
  std::map<double, double>::iterator it;
  for (it = m.begin(); it != m.end(); it++) {
    if (mva_score <= it->first)
      return it->second;
  }
}
