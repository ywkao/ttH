import ROOT
import numpy
import math

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--input", help = "file with histograms to derive shape from", type=str)
parser.add_argument("--hist_name", help = "which histogram to fit", type=str, default = "hPhotonMinIDMVA_fine")
parser.add_argument("--two_dimensional", help = "fit 2d function in min/max ID MVA", action='store_true')
args = parser.parse_args()

f = ROOT.TFile(args.input)

hist_name = args.hist_name 

minIDMVA = f.Get(hist_name + "_Data")

for bkg in ["TTGG", "TTGJets", "TTJets", "DiPhoton"]:
  hist = f.Get(hist_name + "_" + bkg)
  minIDMVA.Add(hist, -1)

minIDMVA.GetXaxis().SetRangeUser(-0.7,1.0)
if args.two_dimensional:
  minIDMVA.GetYaxis().SetRangeUser(-0.7,1.0)

#N = 3

#def nd_polynomial(x, par):
#  sum = 0
#  idx = 0
#  for i in range(N):
#    sum += par[idx]*(x[0]**(N-i) * x[1]**(i))
#    idx += 1

N = 3 
def nd_polynomial(x, par):
  if x[1] >= x[0]:
    return 0
  else:
    return par[0] + par[1]*x[0] + par[2]*x[1] + par[3]*(x[0]**2) + par[4]*(x[0]*x[1]) + par[5]*(x[1]**2) + par[6]*(x[0]**3) + par[7]*(x[1]**3) + par[8]*((x[0]**1)*(x[1]**2)) + par[9]*((x[0]**2)*(x[1]**1)) + par[10]*(x[0]**4) + par[11]*(x[1]**4) + par[12]*(x[0]**5) + par[13]*(x[1]**5) + par[14]*(x[0]**6) + par[15]*(x[1]**6) + par[16]*(x[0]**7) + par[17]*(x[1]**7) + par[18]*((x[0]**3)*(x[1]**1)) + par[19]*((x[0]**1)*(x[1]**3)) + par[20]*((x[0]**2)*(x[1]**2)) + par[21]*((x[0]**4)*(x[1]**1)) + par[22]*((x[0]**3)*(x[1]**2)) + par[23]*((x[0]**2)*(x[1]**3)) + par[24]*((x[0]**1)*(x[1]**4)) + par[25]*((x[0]**5)*(x[1]**1)) + par[26]*((x[0]**4)*(x[1]**2)) + par[27]*((x[0]**3)*(x[1]**3)) + par[28]*((x[0]**2)*(x[1]**4)) + par[29]*((x[0]**1)*(x[1]**5)) + par[30]*((x[0]**6)*(x[1]**1)) + par[31]*((x[0]**5)*(x[1]**2)) + par[32]*((x[0]**4)*(x[1]**3)) + par[33]*((x[0]**3)*(x[1]**4)) + par[34]*((x[0]**2)*(x[1]**5)) + par[35]*((x[0]**1)*(x[1]**6)) 


def bivariate_gaussian(x, par):
  const = par[0] / (2 * math.pi * par[2] * par[4] * math.sqrt(1 - (par[5]**2)))
  exp = (-1 / (2*(par[5]**2))) * (( (x[0] - par[1]) / par[2])**2 + ( (x[1] - par[3]) / par[4])**2  - ((2*par[5]*(x[0]-par[1])*(x[1]-par[3]))/(par[2]*par[4])))
  return const*math.exp(exp)

def sum_of_gaussians(x, par):
  if x[1] >= x[0]:
    return 0
  else:
    gaus1 = bivariate_gaussian(x, par) 
    return gaus1


def fit_func(x, par):
  if x[1] >= x[0]:
    return 0
  else:
    return par[0] + par[1]*x[0] + par[2]*x[1] + par[3]*x[0]**2 + par[4]*x[0]*x[1] + par[5]*x[1]**2 + par[6]*x[0]**3 + par[7]*x[0]**2*x[1] + par[8]*x[0]*x[1]**2 + par[9]*x[1]**3 + par[10]*x[0]**4 + par[11]*x[0]**3*x[1] + par[12]*x[0]**2*x[1]**2 + par[13]*x[0]*x[1]**3 + par[14]*x[1]**4 + par[15]*x[0]**5 + par[16]*x[0]**4*x[1] + par[17]*x[0]**3*x[1]**2 + par[18]*x[0]**2*x[1]**3 + par[19]*x[0]*x[1]**4 + par[20]*x[1]**5 + par[21]*x[0]**6 + par[22]*x[0]**5*x[1] + par[23]*x[0]**4*x[1]**2 + par[24]*x[0]**3*x[1]**3 + par[25]*x[0]**2*x[1]**4 + par[26]*x[0]*x[1]**5 + par[27]*x[1]**6 + par[28]*x[0]**7 + par[29]*x[0]**6*x[1] + par[30]*x[0]**5*x[1]**2 + par[31]*x[0]**4*x[1]**3 + par[32]*x[0]**3*x[1]**4 + par[33]*x[0]**2*x[1]**5 + par[34]*x[0]*x[1]**6 + par[36]*x[1]**7 

if not args.two_dimensional:
  fit = (minIDMVA.Fit("pol7")).Get()
else:
  #func = ROOT.TF2('f2', nd_polynomial, -10, 10, -99999999, 99999999) 
  #func = ROOT.TF2("func", nd_polynomial, -0.7, 1.0, -0.7, 1.0, 36)

  #func1 = ROOT.TF2("func1", "bigaus", -0.7, 1.0, -0.7, 1.0)
  #func2 = ROOT.TF2("func2", "bigaus", -0.7, 1.0, -0.7, 1.0)
  #func3 = ROOT.TF2("func3", "bigaus", -0.7, 1.0, -0.7, 1.0)
  #func = ROOT.TF2("func", "xygaus",  -0.7, 1.0, -0.7, 1.0)
  #func.SetParameters(1.47177e+06, -0.6, 3.06113e+00, -0.6, 2.91405e+00, 7.20010e-01, 1.47177e+06, 0.9, 3.06113e+00, -0.6, 2.91405e+00, 7.20010e-01, 1.47177e+06, 0.9, 3.06113e+00, 0.9, 2.91405e+00, 7.20010e-01)

  func = ROOT.TF2("func", fit_func, -0.7, 1.0, -0.7, 1.0, 37)
  #func.SetParLimits(0, 0.0001, 10**10)
  #func.SetParLimits(1, 0.0001, 10**10)
  #func.SetParLimits(2, 0.0001, 10**10)
  #func.SetParLimits(3, 0.0001, 10**10)
  #func.SetParLimits(4, 0.0001, 10**10)
  #func.SetParLimits(5, 0.0001, 0.999)
  #func.SetParLimits(6, 0.0001, 0.999) 

  #func = ROOT.TF2("func", "[0] + [1]*x + [2]*y + [3]*(x**2) + [4]*(x*y) + [5]*(y**2) + [6]*(x**3) + [7]*((x**2)*y) + [8]*((x) * (y**2)) + [9]*(y**3) + [10]*(x**4) + [11]*(y**4) + [12]*(x**5) + [13]*(y**5) + [14]*(x**6) + [15]*(y**6) + [16]*(x**7) + [17]*(y**7) + [18]*((x**2) * (y**2)) + [19]*(x*(y**3)) + [20]*((x**3)*y) + [21]*(x**8) + [22]*(y**8) + [23]*(x**9) + [24]*(y**9) + [25]*(x*(y**4)) + [26]*((x**2)*(y**3)) + [27]*((x**3)*(y**2)) + [28]*((x**4)*y)", -0.7, 1.0, -0.7, 1.0)



  #func = ROOT.TF2("func", "[0] + [1]*((x**7) * (y**0)) + [2]*( (x**6) * (y**1)) + [3]*( (x**5) * (y**2)) + [4]*((x**4) * (y**3)) + [5]*((x**3) * (y**4)) + [6]*((x**2) * (y**5)) + [7]*((x**1) * (y**6)) + [8]*((x**0) * (y**7))", -0.7, 1.0, -0.7, 1.0)
  func.SetContour(100) 
  #func.SetRange(-0.7, -0.7, 0.8, 1.0)
  fit = (minIDMVA.Fit("func")).Get()

#func = ROOT.TF2("myfunc", "(1.39138e+02) + (9.61460e+02)*((x**7) * (y**0)) + (-1.24691e+03)*( (x**6) * (y**1)) + (-9.16042e+02)*( (x**5) * (y**2)) + (-1.98543e+03)*((x**4) * (y**3)) + [9.39347e+03]*((x**3) * (y**4)) + (6.96885e+03)*((x**2) * (y**5)) + (-5.56256e+03)*((x**1) * (y**6)) + (-7.62668e+03)*((x**0) * (y**7))", -0.7, 1.0, -0.7, 1.0) 
#func.SetRange(-0.7, -0.7, 0.0, 1.0, 1.0, 9999999)

#minIDMVA.GetYaxis().SetRangeUser(-0.7, 0.4)
#minIDMVA.GetZaxis().SetRangeUser(0.0, 10000)
minIDMVA.Draw("LEGO")

while True:
  continue
