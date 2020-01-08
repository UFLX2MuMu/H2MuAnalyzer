###################################################################
###    make 2D mass calibration plots from mass histograms      ###
###################################################################

## Basic python includes for manipulating files
import os
import sys
import array

sys.path.insert(0, '%s/lib' % os.getcwd())
from ROOT import *
from MassCal_Helper import GetHistInfo, FitVoigtian, FitGaussian, FitCrystalBall, FitDSCB, GetColor, WriteOverlay, WriteSummary

gROOT.SetBatch(True)

## Configure the script user
if 'abrinke1' in os.getcwd(): USER = 'abrinke1'
if 'bortigno' in os.getcwd(): USER = 'bortigno'
if 'xzuo'     in os.getcwd(): USER = 'xzuo'

## Directory for input histograms and output plots
if USER == 'abrinke1': PLOT_DIR = '/afs/cern.ch/work/a/abrinke1/public/H2Mu/2016/Histograms'
if USER == 'xzuo':     PLOT_DIR = '/afs/cern.ch/work/x/xzuo/public/H2Mu/2016/Histograms'

LABEL = 'MassCal_KinRoch_approx/GeoBSRoch_sig_2D_muP_d0_rebin_muN_d0_rebin'
nameX = "muP_d0_rebin"
nameY = "muN_d0_rebin"
CAT   = "NONE_NONE"
#CAT   = "NONE_inclusive_01jet"
PEAK  = "Z"
if 'sig' in LABEL: PEAK = "H"
if 'val' in LABEL: PEAK = "off_set"
SUM   = "d0"

def main():
    file_dir = PLOT_DIR+"/"+LABEL
    out_file = TFile( file_dir + "/mass_cal_plots" + ".root", "RECREATE")
    in_file  = TFile.Open( file_dir + "/all_samples" + ".root", "READ")

    gen_dir  = out_file.mkdir("pull_gen")
    cor_dir  = out_file.mkdir("data_over_MC")
    ovl_dir  = out_file.mkdir("overlay")
    cal_dir  = out_file.mkdir("individual_cal_plots")
    fit_dir  = out_file.mkdir("fits_specifics")
    sum_dir  = out_file.mkdir("sum_mass_peaks")

#    if   PEAK == "Z":        samples = ["ZJets_AMC", "data"]
    if   PEAK == "Z":	     samples = ["ZJets_MG_1", "data"] # for 2018
#    elif PEAK == "H":        samples = ["H2Mu_gg", "H2Mu_VBF", "H2Mu_ttH", "sig_sum"] # for 2017
    elif PEAK == "H": 	     samples = ["H2Mu_gg", "H2Mu_VBF", "H2Mu_WH_pos_125", "H2Mu_WH_neg_125", "H2Mu_ttH_125", "H2Mu_ZH_125", "sig_sum"]
    elif PEAK == "off_set":  samples = ["ZJets_hiM_AMC", "ZJets_hiM_MG", "tt_ll_POW", "tt_ll_MG"]
#    elif PEAK == "off_set":  samples = ["ZJets_AMC"]
    if   PEAK == "Z":        pt_cals = ["gen", "genBS", "PF", "Roch", "RochBS", "Kinfit", "KinRoch", "GeoRoch", "GeoBSRoch"]
    elif PEAK == "H":        pt_cals = ["PF", "Roch", "Kinfit", "KinRoch", "GeoRoch", "GeoBSRoch"]
    elif PEAK == "off_set":  pt_cals = ["PF", "Roch", "RochBS", "Kinfit", "KinRoch", "KaMu", "KinKaMu", "GeoRoch"]

    if   SUM == "d0": sum_names = ["all", "d0P_pos_d0N_pos", "d0P_neg_d0N_pos", "d0P_neg_d0N_neg", "d0P_pos_d0N_neg"]#, "d0P_In0004", "d0P_Out0004", "both_In0004", "both_Out0004"]

    summary_info = in_file.Get( pt_cals[0] + "/summary_" + samples[0] + "_" + CAT + "_" + pt_cals[0]).Clone()
    binningX = summary_info.GetXaxis().GetXbins()  ## return type is TArrayD
    binningY = summary_info.GetYaxis().GetXbins()  ## return type is TArrayD

    print binningX

    mean_plots = {}
    reso_plots = {}
    pull_plots = {}
    sum_plots  = {}
    for pt_cal in pt_cals:
      for sample in samples:
	mean_plots[sample+pt_cal] = summary_info.Clone("Mean_" + sample + "_" + pt_cal) 
	mean_plots[sample+pt_cal].SetTitle("Mean_" + sample + "_" + pt_cal) 
	reso_plots[sample+pt_cal] = summary_info.Clone("Reso_" + sample + "_" + pt_cal) 
	reso_plots[sample+pt_cal].SetTitle("Reso_" + sample + "_" + pt_cal)
	for i in range(binningX.GetSize()-1):
	  for j in range(binningY.GetSize()-1):
	    x_low  = round( binningX.GetAt(i),    5)
	    x_high = round( binningX.GetAt(i+1),  5)
	    y_low  = round( binningY.GetAt(j),    5)
            y_high = round( binningY.GetAt(j+1),  5)

	    print "looking at, %s,   %s,   %f to %f   " %(sample, pt_cal, x_low, x_high)
	    hist_name = pt_cal + "/" + sample + "_" + pt_cal + "_%s_%8.4f_to_%8.4f_%s_%8.4f_to_%8.4f" %( nameX, x_low, x_high, nameY, y_low, y_high)
	    hist_name = hist_name.replace(' ','').replace("-","m").replace(".","p")
	    print hist_name
	    if sample != "sig_sum": mass_hist = in_file.Get(hist_name).Clone()
	    else: 
		mass_hist = in_file.Get(hist_name.replace("sig_sum", "H2Mu_gg")).Clone(hist_name)
		for component in samples:
		    if component == "sig_sum" or component == "H2Mu_gg": continue
		    mass_hist.Add( in_file.Get(hist_name.replace("sig_sum",component)).Clone() )

	    if SUM == "d0":
	      for sum_name in sum_names:
		## check if the bin of mass_hist is in the sum_name range
		if sum_name == "d0P_pos_d0N_pos" and (x_low  < 0 or y_low  < 0): continue
		if sum_name == "d0P_neg_d0N_pos" and (x_high > 0 or y_low  < 0): continue
		if sum_name == "d0P_neg_d0N_neg" and (x_high > 0 or y_high > 0): continue
		if sum_name == "d0P_pos_d0N_neg" and (x_low  < 0 or y_high > 0): continue

		if sum_name == "d0P_Out0004"  and (x_low >= -0.004 and x_high <=  0.004): continue
		if sum_name == "d0P_In0004"   and (x_low >=  0.004 or  x_high <= -0.004): continue
		if sum_name == "both_Out0004" and (x_low >= -0.004 and x_high <=  0.004 and y_low >= -0.004 and y_high <= 0.004): continue
                if sum_name == "both_In0004"  and ( (x_low >= 0.004 or x_high <= -0.004) or (y_low >= 0.004 or y_high <= -0.004) ): continue

		if sample+pt_cal+sum_name not in sum_plots.keys():
		  sum_plots[sample+pt_cal+sum_name]= mass_hist.Clone(sample+pt_cal+sum_name)
		else:
		  sum_plots[sample+pt_cal+sum_name].Add( mass_hist )


	    if   PEAK == "Z":       mean_val, mean_err, reso_val, reso_err = FitVoigtian(mass_hist, exp = True)
	    elif PEAK == "H":       #mean_val, mean_err, reso_val, reso_err = FitCrystalBall(mass_hist)
		temp_tuple = FitDSCB(mass_hist)
		mean_val = temp_tuple[1]
		mean_err = temp_tuple[2]
		reso_val = temp_tuple[3]
		reso_err = temp_tuple[4]
	    elif PEAK == "off_set": mean_val, mean_err, reso_val, reso_err = FitCrystalBall(mass_hist)
	    if reso_err == 10:  # if not a good fit
		mean_plots[sample+pt_cal].SetBinContent(i+1,j+1, 0.0)
		mean_plots[sample+pt_cal].SetBinError  (i+1,j+1, mean_err)
	 	reso_plots[sample+pt_cal].SetBinContent(i+1,j+1, 0.0)
                reso_plots[sample+pt_cal].SetBinError  (i+1,j+1, reso_err)

	    else:
		mean_plots[sample+pt_cal].SetBinContent(i+1,j+1, mean_val)
                mean_plots[sample+pt_cal].SetBinError  (i+1,j+1, mean_err)
		reso_plots[sample+pt_cal].SetBinContent(i+1,j+1, reso_val)
                reso_plots[sample+pt_cal].SetBinError  (i+1,j+1, reso_err)

	    fit_dir.cd()
            mass_hist.Write()

	cal_dir.cd()
	gStyle.SetPaintTextFormat("4.2f%")
	if   PEAK == "Z":       mean_plots[sample+pt_cal].GetZaxis().SetRangeUser(89.5,92.5)
	elif PEAK == "H":       mean_plots[sample+pt_cal].GetZaxis().SetRangeUser(123,127)
	elif PEAK == "off_set": mean_plots[sample+pt_cal].GetZaxis().SetRangeUser(-2,2)
	mean_plots[sample+pt_cal].SetXTitle(nameX)
	mean_plots[sample+pt_cal].SetYTitle(nameY)
	mean_plots[sample+pt_cal].Write()
	reso_plots[sample+pt_cal].GetZaxis().SetRangeUser(1,2.5)
	reso_plots[sample+pt_cal].SetXTitle(nameX)
	reso_plots[sample+pt_cal].SetYTitle(nameY)
	reso_plots[sample+pt_cal].Write()

  	if SUM == "d0":
          sum_dir.cd()
          for sum_name in sum_names:
            if   PEAK == "Z":       FitVoigtian   (sum_plots[sample+pt_cal+sum_name], exp = True)
            elif PEAK == "H":	    #FitCrystalBall(sum_plots[sample+pt_cal+sum_name])       
		temp_tuple = FitDSCB(sum_plots[sample+pt_cal+sum_name])
		sum_plots[sample+pt_cal+sum_name] = temp_tuple[0]
		sum_plots[sample+pt_cal+sum_name].SetLineColor(kBlue)
		sum_plots[sample+pt_cal+sum_name].SetMarkerColor(kBlue)
	    elif PEAK == "off_set": FitCrystalBall(sum_plots[sample+pt_cal+sum_name])
            sum_plots[sample+pt_cal+sum_name].Write()


    if PEAK == "off_set": return

    for sample in samples:
      for pt_cal in pt_cals:
        if pt_cal == "gen" or pt_cal == "genBS": continue
        pull_plots[sample+pt_cal] = summary_info.Clone("pull_" + sample + "_" + pt_cal)
	pull_plots[sample+pt_cal].SetTitle("pull_" + sample + "_" + pt_cal)
   
        for i in range(binningX.GetSize()-1):
          for j in range(binningY.GetSize()-1):
  	    if   PEAK == "Z" and pt_cal != "GeoBSRoch" and pt_cal != "RochBS": mass_gen = mean_plots[sample+"gen"]. GetBinContent(i+1, j+1)
	    elif PEAK == "Z":				                       mass_gen = mean_plots[sample+"genBS"]. GetBinContent(i+1, j+1) 
	    elif PEAK == "H":       mass_gen = 125.0
  	    mass_cal = mean_plots[sample+pt_cal].GetBinContent(i+1, j+1)

	    if mass_gen == 0 or mass_cal == 0:
	      pull_plots[sample+pt_cal].SetBinContent(i+1,j+1, 0.0)
	    else:
	      pull_plots[sample+pt_cal].SetBinContent(i+1,j+1, 100.0 * (mass_cal - mass_gen)/mass_gen )

	gen_dir.cd()
	gStyle.SetPaintTextFormat("4.2f%%")
	pull_plots[sample+pt_cal].GetZaxis().SetRangeUser(-2.0,2.0)
	pull_plots[sample+pt_cal].SetXTitle(nameX)
	pull_plots[sample+pt_cal].SetYTitle(nameY)
	pull_plots[sample+pt_cal].Write()


    if PEAK == "H": return

    mean_dataMC = {}
    reso_dataMC = {}
    for pt_cal in pt_cals:
      for sample in samples:
	if pt_cal == "gen" or pt_cal == "genBS":  continue
	if sample == "data": continue
	mean_dataMC[sample+pt_cal] = summary_info.Clone("Mean_data_over_MC_" + sample + "_" + pt_cal)
	mean_dataMC[sample+pt_cal].SetTitle("Mean_data_over_MC_" + sample + "_" + pt_cal)
	reso_dataMC[sample+pt_cal] = summary_info.Clone("Reso_data_over_MC_" + sample + "_" + pt_cal)
	reso_dataMC[sample+pt_cal].SetTitle("Reso_data_over_MC_" + sample + "_" + pt_cal)
	
	for i in range(binningX.GetSize()-1):
          for j in range(binningY.GetSize()-1):
	    mass_data = mean_plots["data"+pt_cal].GetBinContent(i+1, j+1)
	    mass_MC   = mean_plots[sample+pt_cal].GetBinContent(i+1, j+1)

	    reso_data = reso_plots["data"+pt_cal].GetBinContent(i+1, j+1)
            reso_MC   = reso_plots[sample+pt_cal].GetBinContent(i+1, j+1)

	    if mass_data == 0 or mass_MC == 0:
		mean_dataMC[sample+pt_cal].SetBinContent(i+1,j+1, 0.0)
	    else:
		mean_dataMC[sample+pt_cal].SetBinContent(i+1,j+1, 100.0 * (mass_data - mass_MC)/mass_MC)

	    if reso_data == 0 or reso_MC == 0:
		reso_dataMC[sample+pt_cal].SetBinContent(i+1,j+1, 0.0)
            else:
                reso_dataMC[sample+pt_cal].SetBinContent(i+1,j+1, 100.0 * (reso_data - reso_MC)/reso_MC)

	cor_dir.cd()
	gStyle.SetPaintTextFormat("4.2f%%")
	mean_dataMC[sample+pt_cal].GetZaxis().SetRangeUser(-0.5,0.5)
	mean_dataMC[sample+pt_cal].SetXTitle(nameX)
	mean_dataMC[sample+pt_cal].SetYTitle(nameY)
	mean_dataMC[sample+pt_cal].Write()
	reso_dataMC[sample+pt_cal].GetZaxis().SetRangeUser(-10,10)
	reso_dataMC[sample+pt_cal].SetXTitle(nameX)
	reso_dataMC[sample+pt_cal].SetYTitle(nameY)
	reso_dataMC[sample+pt_cal].Write()	






#    ovl_dir.cd()
#    WriteOverlay(mean_plots, "mean", samples, pt_cals)
#    WriteOverlay(reso_plots, "reso", samples, pt_cals)
#
#    sum_dir.cd()
#    WriteSummary(mean_plots, "mean", nameX, samples, pt_cals, file_dir)
#    WriteSummary(reso_plots, "reso", nameX, samples, pt_cals, file_dir)
#







main()
