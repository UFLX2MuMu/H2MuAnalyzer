####################################################
###    make stack and ratio from miniNTuple      ###
####################################################

## Basic python includes for manipulating files
import os
import sys

sys.path.insert(0, '%s/lib' % os.getcwd() )
from ROOT import *
from MNT_Helper import LinearStack, RatioPlot, FillHistTerm, GetSF
import Plot_Configs as PC
#R.gROOT.SetBatch(True)

## Configure the script user
if 'abrinke1' in os.getcwd(): USER = 'abrinke1'
if 'bortigno' in os.getcwd(): USER = 'bortigno'
if 'eyigitba' in os.getcwd(): USER = 'eyigitba'
if 'xzuo'     in os.getcwd(): USER = 'xzuo'

## Directory for input histograms and output plots
if USER == 'abrinke1': PLOT_DIR = '/afs/cern.ch/work/a/abrinke1/public/H2Mu/2017/Histograms'
if USER == 'eyigitba': PLOT_DIR = '/afs/cern.ch/work/e/eyigitba/public/H2Mu/2017/Histograms'
if USER == 'xzuo':     PLOT_DIR = '/afs/cern.ch/work/x/xzuo/public/H2Mu/Run2/Histograms'

#LABEL = 'miniNtuple_WH_2016_v5'  ## Sub-folder within PLOT_DIR containing histograms
#LABEL = 'WH_ele_loose_ID_loose_iso_loose_mu_iso_v1'
#LABEL = 'WH_mu_med_ID_loose_iso_v1'

LABEL = 'ZH_lep_2019_08_25_medLep_veto12'
#LABEL = 'VH_selection_2019april/pt10_iso04/ZH_ele_massBDT'
#LABEL = 'leftover_of_ttH_and_ZH_2019_09_08'
#LEP = 'ele'

def InitHists(histos, terms, signals, bkgs):
    for sample in signals + bkgs:
	histos["dimu_mass"][sample] 		    = TH1F("dimu_mass" + "_Net_" + sample, "dimu_mass" + "_Net_" + sample, 			50,110,160)
	histos["dimu_mass_BDT_n10_n01"][sample]     = TH1F("dimu_mass_BDT_n10_n01" + "_Net_" + sample, "dimu_mass_BDT_n10_n01" + "_Net_" + sample,                  50,110,160)
	histos["dimu_mass_BDT_n01_p10"][sample]     = TH1F("dimu_mass_BDT_n01_p10" + "_Net_" + sample, "dimu_mass_BDT_n01_p10" + "_Net_" + sample,                  50,110,160)
    histos["dimu_mass"]["Data"]     		= TH1F("dimu_mass" + "_Net_" + "Data", "dimu_mass" + "_Net_" + "Data",                  50,110,160)
    histos["dimu_mass_BDT_n10_n01"]["Data"]     = TH1F("dimu_mass_BDT_n10_n01" + "_Net_" + "Data", "dimu_mass_BDT_n10_n01" + "_Net_" + "Data",                  50,110,160)
    histos["dimu_mass_BDT_n01_p10"]["Data"]     = TH1F("dimu_mass_BDT_n01_p10" + "_Net_" + "Data", "dimu_mass_BDT_n01_p10" + "_Net_" + "Data",                  50,110,160)

def main():
    out_name = "mass_hists_lepMVAp04.root"

    file_dir = PLOT_DIR+"/"+LABEL+"/"
    out_file = TFile( file_dir + "plots/" + out_name , "RECREATE")
    file_chain = TChain("tree","chain");
    file_chain.Add(file_dir + "all_samples.root")

    terms = ["dimu_mass", "dimu_mass_BDT_n10_n01", "dimu_mass_BDT_n01_p10"]

    cfg = PC.Plot_Config("ZH_4l")

    histos = {}
    stack_all = {}
    stack_bkg = {}
    stack_sig = {}
    stack_data = {}
    ratios = {}
    legend = {}

    for term in terms:
        histos[term] = {}
        stack_all[term] = THStack("h_stack_"+term, term)
	stack_bkg[term] = THStack("bkg_stack_"+term, "bkg_"+term)
        stack_sig[term] = THStack("sig_stack_"+term, "sig_"+term)
        stack_data[term] = THStack("data_stack"+term, "data_"+term)
	

    InitHists(histos, terms, cfg.signals, cfg.bkgs)

    print file_chain.GetEntries()
    for iEvt in range( file_chain.GetEntries() ):
	file_chain.GetEvent(iEvt)
	if iEvt % 10000 == 0:
	    print "looking at event %d" %iEvt

	if file_chain.dilep_mass < 70 or file_chain.dilep_mass > 110: continue

	MVA_cut = 0.4
	if file_chain.mu1_lepMVA < MVA_cut or file_chain.mu2_lepMVA < MVA_cut or file_chain.lep1_lepMVA < MVA_cut or file_chain.lep2_lepMVA < MVA_cut:
	    continue

#	mu1_SF = GetSF("muon", file_chain.mu1_pt,  file_chain.mu1_abs_eta, MVA_cut)
#	mu2_SF = GetSF("muon", file_chain.mu2_pt,  file_chain.mu2_abs_eta, MVA_cut)
#	lep1_SF = GetSF(LEP,   file_chain.lep1_pt, file_chain.lep1_abs_eta, MVA_cut)
# 	lep2_SF = GetSF(LEP,   file_chain.lep2_pt, file_chain.lep2_abs_eta, MVA_cut)
#	MVA_SF = mu1_SF * mu2_SF * lep1_SF * lep2_SF

#	MVA_SF = 1.0
	MVA_SF = file_chain.all_lepMVA_SF

	FillHistTerm(histos, "dimu_mass"   	, cfg, file_chain.dimu_mass   	, file_chain.Sample_ID  , file_chain.xsec_norm * file_chain.event_wgt * MVA_SF)
	if file_chain.BDT_noMass_v3 < -0.1:
	  FillHistTerm(histos, "dimu_mass_BDT_n10_n01"        , cfg, file_chain.dimu_mass     , file_chain.Sample_ID  , file_chain.xsec_norm * file_chain.event_wgt * MVA_SF)
	else:
	  FillHistTerm(histos, "dimu_mass_BDT_n01_p10"        , cfg, file_chain.dimu_mass     , file_chain.Sample_ID  , file_chain.xsec_norm * file_chain.event_wgt * MVA_SF)

    out_file.cd()
    scaled_signal = {}
    for term in terms:
	histos[term]["Data"].SetMarkerStyle(20)
        for sample in cfg.signals:
            histos[term][sample].SetLineColor(cfg.colors[sample])
            histos[term][sample].SetLineWidth(2)
            stack_sig[term].Add(histos[term][sample])
	histos[term]["signal"] = stack_sig[term].GetStack().Last()
	histos[term]["signal"].SetNameTitle(term + "_Net_Sig", term + "_Net_Sig")

        stack_all[term].Add(histos[term]["signal"])
        histos[term]["signal"].SetFillColor( kGray )
        histos[term]["signal"].SetLineWidth(0)
        for sample in cfg.bkgs:
            histos[term][sample].SetFillColor(cfg.colors[sample])
	    stack_bkg[term].Add(histos[term][sample])
            stack_all[term].Add(histos[term][sample])
	histos[term]["bkg"] = stack_bkg[term].GetStack().Last()
	histos[term]["bkg"].SetNameTitle(term + "_Net_Bkg", term + "_Net_Bkg")

	scaled_signal[term] =  histos[term]["signal"].Clone()
  	scaled_signal[term].Scale(20)
	scaled_signal[term].SetLineColor(kRed)
	scaled_signal[term].SetLineWidth(2)
	scaled_signal[term].SetFillStyle(0)


	legend[term] = TLegend(0.7,0.7,1,1)
	legend[term].AddEntry(histos[term]["Data"], "Data")
	legend[term].AddEntry(histos[term]["signal"], "signal sum")
	for sample in cfg.bkgs:
            legend[term].AddEntry(histos[term][sample], sample )
	legend[term].AddEntry(scaled_signal[term], "signal X20")
	LinearStack( term, stack_all[term], scaled_signal[term], histos[term]["Data"], legend[term], PLOT_DIR+"/"+LABEL+"/plots")

	for sample in ["Data", "signal", "bkg"] + cfg.signals + cfg.bkgs:
	    histos[term][sample].Write()


    out_file.Close()
main()











	
