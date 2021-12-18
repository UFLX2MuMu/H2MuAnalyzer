################################################################
###    make mass calibration plots from mass histograms      ###
################################################################

## Basic python includes for manipulating files
import os
import sys

sys.path.insert(0, '%s/lib' % os.getcwd())
from ROOT import *
from MassCal_Helper import FitVoigtian, GetColor, WriteOverlay, WriteSummary
from CMS_style import setTDRStyle

gROOT.SetBatch(True)

## Configure the script user
if 'abrinke1' in os.getcwd(): USER = 'abrinke1'
if 'bortigno' in os.getcwd(): USER = 'bortigno'
if 'eyigitba' in os.getcwd(): USER = 'eyigitba'
if 'xzuo'     in os.getcwd(): USER = 'xzuo'

YEAR = 'Run2'

## Directory for input histograms and output plots
if USER == 'abrinke1': PLOT_DIR = '/afs/cern.ch/work/a/abrinke1/public/H2Mu/%s/Histograms'%YEAR
if USER == 'eyigitba': PLOT_DIR = '/afs/cern.ch/work/e/eyigitba/public/H2Mu/%s/Histograms'%YEAR
if USER == 'xzuo':     PLOT_DIR = '/afs/cern.ch/work/x/xzuo/public/H2Mu/%s/Histograms'%YEAR

LABEL = 'MassCal_Thesis/GeoBSRoch_2D_muP_d0_muN_d0'
nameX = "muP_d0"
nameY = "muN_d0"
haveY = True
plotVar = nameX
CAT   = "NONE_NONE"

def main():
    file_dir = PLOT_DIR+"/"+LABEL
    out_file = TFile( file_dir + "/mass_cal_plots_" + plotVar + ".root", "RECREATE")
    in_file  = TFile.Open( file_dir + "/all_samples" + ".root", "READ")

    sum_dir  = out_file.mkdir("summary")
    ovl_dir  = out_file.mkdir("overlay")
    cal_dir  = out_file.mkdir("individual_cal_plots")
    fit_dir  = out_file.mkdir("fits_specifics")

    samples = ["ZJets_AMC", "data"]
    if YEAR == '2018': samples = ["ZJets_MG_1", "data"]
    pt_cals = ["PF", "Roch", "GeoBSRoch"]#, "Kinfit", "KinRoch"]
#    pt_cals = ["PF", "Kinfit", "good_Kinfit", "Kin_vs_d0kin"]
#    pt_cals = ["Kin_vs_d0kin", "Kin_vs_d0kin_BB", "Kin_vs_d0kin_BE", "Kin_vs_d0kin_EE"]
#    pt_cals = ["Kin_vs_d0kin_d0PV_N50_N15", "Kin_vs_d0kin_d0PV_N15_N05", "Kin_vs_d0kin_d0PV_P05_P15", "Kin_vs_d0kin_d0PV_P15_P50"]

    summary_info = in_file.Get( pt_cals[0] + "/summary_" + samples[0] + "_" + CAT + "_" + pt_cals[0]).Clone()
    binningX = summary_info.GetXaxis().GetXbins()  ## return type is TArrayD
    binningY = False
    if haveY: binningY = summary_info.GetYaxis().GetXbins()

    mean_plots = {}
    reso_plots = {}
    pull_plots = {}
    for pt_cal in pt_cals:
      for sample in samples:
        if pt_cal == 'gen' and sample == 'data': continue
	mean_plots[sample+pt_cal] = TGraphErrors()
	reso_plots[sample+pt_cal] = TGraphErrors()
	mean_plots[sample+pt_cal].SetName("Mean_" + sample + "_" + pt_cal)
	reso_plots[sample+pt_cal].SetName("Reso_" + sample + "_" + pt_cal)

        plot_bins = binningX
        if haveY and plotVar == nameY: plot_bins = binningY
	for i in range(plot_bins.GetSize()-1):
	  a_low  = plot_bins.GetAt(i)
	  a_high = plot_bins.GetAt(i+1)
	  a_val = ( a_high + a_low ) / 2.0
	  a_err = ( a_high - a_low ) / 2.0

          if not haveY:
	      print "looking at, %s,   %s,   %f to %f   " %(sample, pt_cal, a_low, a_high)
	      hist_name = pt_cal + "/" + sample + "_" + pt_cal + "_%s_%8.4f_to_%8.4f" %( nameX, a_low, a_high)
	      hist_name = hist_name.replace(' ','').replace("-","m").replace(".","p")
	      print hist_name
	      mass_hist = in_file.Get(hist_name).Clone()
          else:
              print "looking at, %s,   %s,   %f to %f   " %(sample, pt_cal, a_low, a_high)
              sum_bins = binningY
              if plotVar == nameY: sum_bins = binningX
              mass_hist = False
              for j in range(sum_bins.GetSize()-1):
                b_low  = sum_bins.GetAt(j)
                b_high = sum_bins.GetAt(j+1)
                b_val = ( b_high + b_low ) / 2.0
                b_err = ( b_high - b_low ) / 2.0
            
                hist_name = pt_cal + "/" + sample + "_" + pt_cal + "_%s_%8.4f_to_%8.4f_%s_%8.4f_to_%8.4f" %(nameX, a_low, a_high, nameY, b_low, b_high)           #if a for X and b for Y
                if plotVar == nameY: hist_name = pt_cal + "/" + sample + "_" + pt_cal + "_%s_%8.4f_to_%8.4f_%s_%8.4f_to_%8.4f" %(nameX, b_low, b_high, nameY, a_low, a_high) #if b for X and a for Y
                hist_name = hist_name.replace(' ','').replace("-","m").replace(".","p")
                if mass_hist == False: mass_hist = in_file.Get(hist_name).Clone()
                else: mass_hist.Add(in_file.Get(hist_name).Clone())

                if sample == "ZJets_AMC" and YEAR == '2017': # two samples used in 2017
                  DY_2_name = pt_cal + "/" + "ZJets_AMC_2" + "_" + pt_cal + "_%s_%8.4f_to_%8.4f_%s_%8.4f_to_%8.4f" %(nameX, a_low, a_high, nameY, b_low, b_high)           #if a for X and b for Y
                  if plotVar == nameY: DY_2_name = pt_cal + "/" + "ZJets_AMC_2" + "_" + pt_cal + "_%s_%8.4f_to_%8.4f_%s_%8.4f_to_%8.4f" %(nameX, b_low, b_high, nameY, a_low, a_high) #if b for X and a for Y
                  if in_file.GetListOfKeys().Contains(DY_2_name):
                    mass_hist.Add(in_file.Get(DY_2_name).Clone()) 
                if sample == "ZJets_AMC" and YEAR == 'Run2': # merge 2018, but not use ZJets_AMC_2 of 2017, so the xsec norm is correct
                  DY_2_name = pt_cal + "/" + "ZJets_MG_1" + "_" + pt_cal + "_%s_%8.4f_to_%8.4f_%s_%8.4f_to_%8.4f" %(nameX, a_low, a_high, nameY, b_low, b_high)           #if a for X and b for Y
                  if plotVar == nameY: DY_2_name = pt_cal + "/" + "ZJets_MG_1" + "_" + pt_cal + "_%s_%8.4f_to_%8.4f_%s_%8.4f_to_%8.4f" %(nameX, b_low, b_high, nameY, a_low, a_high) #if b for X and a for Y
                  if in_file.GetListOfKeys().Contains(DY_2_name):
                    mass_hist.Add(in_file.Get(DY_2_name).Clone()) 

          
	  mean_val, mean_err, reso_val, reso_err = FitVoigtian(mass_hist, IsGen = (pt_cal == 'gen'), exp = True)
          if "d0" in plotVar:  # for d0, convert centimeter to micrometer
            a_val = a_val * 10000
            a_err = a_err * 10000
	  mean_plots[sample+pt_cal].SetPoint(i, a_val, mean_val)
	  mean_plots[sample+pt_cal].SetPointError(i, a_err, mean_err)

	  reso_plots[sample+pt_cal].SetPoint(i, a_val, reso_val)
          reso_plots[sample+pt_cal].SetPointError(i, a_err, reso_err)

	  mean_plots[sample+pt_cal].SetLineColor( GetColor(sample, pt_cal) )
	  mean_plots[sample+pt_cal].SetLineWidth(2)
 	  reso_plots[sample+pt_cal].SetLineColor( GetColor(sample, pt_cal) )
          reso_plots[sample+pt_cal].SetLineWidth(2)

	  fit_dir.cd()
          mass_hist.Write()

	cal_dir.cd()
	mean_plots[sample+pt_cal].Write()
	reso_plots[sample+pt_cal].Write()

    setTDRStyle()

    ovl_dir.cd()
    WriteOverlay(mean_plots, "mean", samples, pt_cals)
    WriteOverlay(reso_plots, "reso", samples, pt_cals)

    sum_dir.cd()
    WriteSummary(mean_plots, "mean", plotVar, YEAR, samples, pt_cals, file_dir)
    WriteSummary(reso_plots, "reso", plotVar, YEAR, samples, pt_cals, file_dir)








main()
