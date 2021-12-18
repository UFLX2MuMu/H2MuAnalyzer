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
if USER == 'xzuo':     PLOT_DIR = '/afs/cern.ch/work/x/xzuo/h2mm_944/src/H2MuAnalyzer/TrainMVA/output'

#BDT_WITH_MASS    = '2017_WH_ele_high_pt_dimu_lepMVA04_train_with_mass.root'
#BDT_WITHOUT_MASS = '2017_WH_ele_high_pt_dimu_lepMVA04_train_without_mass.root'
BDT_WITH_MASS = '2017_WH_mu_against_inclu_lepMVA04.root'
BDT_WITHOUT_MASS = '2017_WH_mu_against_inclu_lepMVA04.root'


if USER == 'eyigitba':
    OUT_DIR = '/afs/cern.ch/work/e/eyigitba/public/H2Mu/Run2/Histograms'
if USER == 'xzuo':     
    OUT_DIR = '/afs/cern.ch/work/x/xzuo/public/H2Mu/Run2/Histograms'
#    OUT_DIR = OUT_DIR + '/VH_selection_2019april/pt10_iso04/ZH_ele_massBDT'
    OUT_DIR = OUT_DIR + '/ZH_lep_2019_08_25_medLep_veto12'
    #MASS_FILE = 'mass_hists_cut_lepMVAp4.root'
#    LEP = 'ele'

def Template_By_Channel():
    out_name = "BDT_channels_lepMVAn04_with_mass_min110_40bins.root"
    out_file = TFile( OUT_DIR + "/plots/" + out_name , "RECREATE")
    
    file_chain = TChain("tree","chain");
    file_chain.Add( OUT_DIR + "/" + "all_samples.root")

    cfg = PC.Plot_Config("ZH_4l")

#    terms = ["BDT_mass_old", "BDT_mass_new"]
    terms = ["dimu_mass", "BDT_noMass_v3", "BDT_mass_more", "BDT_mass_min", "BDT_and_mass"]

    histos = {}
    stack_bkg = {}
    stack_sig = {}
    out_dir   = {}

    for term in terms:
	out_dir[term] 	= out_file.mkdir(term)
    	stack_bkg[term] = THStack("bkg_stack_"+term, "bkg_"+term) 
    	stack_sig[term] = THStack("sig_stack_"+term, "sig_"+term)
	histos[term]    = {}
	if term == "dimu_mass":
	    for sample in cfg.signals + cfg.bkgs:
		histos["dimu_mass"][sample]                 = TH1F("dimu_mass" + "_Net_" + sample, "dimu_mass" + "_Net_" + sample,                      50,110,160)
	    histos["dimu_mass"]["Data"]                 = TH1F("dimu_mass" + "_Net_" + "Data", "dimu_mass" + "_Net_" + "Data",                  50,110,160)
	else:
    	    for sample in cfg.signals + cfg.bkgs:
	        histos[term][sample] = TH1F(term + "_Net_" + sample, term + "_Net_" + sample, 40,-1,1) # should in general from -1 to 1
	    histos[term]["Data"] = TH1F(term + "_Net_" + "Data", term + "_Net_" + "Data", 40,-1,1)

    print file_chain.GetEntries()
    for iEvt in range( file_chain.GetEntries() ):
        file_chain.GetEvent(iEvt)
        if (iEvt % 10000 == 1):
            print "looking at event %d" %iEvt 

	if file_chain.dilep_mass < 70 or file_chain.dilep_mass > 110: continue

	MVA_cut = 0.4	
	if file_chain.mu1_lepMVA < MVA_cut or file_chain.mu2_lepMVA < MVA_cut or file_chain.lep1_lepMVA < MVA_cut or file_chain.lep2_lepMVA < MVA_cut:
            continue
#	mu1_SF = GetSF("muon", file_chain.mu1_pt,  file_chain.mu1_abs_eta, MVA_cut)
#        mu2_SF = GetSF("muon", file_chain.mu2_pt,  file_chain.mu2_abs_eta, MVA_cut)
#        lep1_SF = GetSF(LEP,   file_chain.lep1_pt, file_chain.lep1_abs_eta, MVA_cut)
#	lep2_SF = GetSF(LEP,   file_chain.lep2_pt, file_chain.lep2_abs_eta, MVA_cut)
#        MVA_SF = mu1_SF * mu2_SF * lep1_SF * lep2_SF
	MVA_SF = file_chain.all_lepMVA_SF

	FillHistTerm(histos, "dimu_mass",     cfg, file_chain.dimu_mass,     file_chain.Sample_ID, file_chain.xsec_norm * file_chain.event_wgt * MVA_SF)
	FillHistTerm(histos, "BDT_noMass_v3", cfg, file_chain.BDT_noMass_v3, file_chain.Sample_ID, file_chain.xsec_norm * file_chain.event_wgt * MVA_SF)
	FillHistTerm(histos, "BDT_mass_more", cfg, file_chain.BDT_mass_more, file_chain.Sample_ID, file_chain.xsec_norm * file_chain.event_wgt * MVA_SF)
	FillHistTerm(histos, "BDT_mass_min",  cfg, file_chain.BDT_mass_min,  file_chain.Sample_ID, file_chain.xsec_norm * file_chain.event_wgt * MVA_SF)
	FillHistTerm(histos, "BDT_and_mass",  cfg, file_chain.BDT_and_mass,  file_chain.Sample_ID, file_chain.xsec_norm * file_chain.event_wgt * MVA_SF)

    for term in terms:
	for sample in cfg.signals:
	    stack_sig[term].Add(histos[term][sample])
	histos[term]["sig"] = stack_sig[term].GetStack().Last().Clone()
	histos[term]["sig"].SetNameTitle( term + "_Net_Sig", term + "_Net_Sig" )
	for sample in cfg.bkgs:
	    stack_bkg[term].Add(histos[term][sample])
	histos[term]["bkg"] = stack_bkg[term].GetStack().Last().Clone()
        histos[term]["bkg"].SetNameTitle( term + "_Net_Bkg", term + "_Net_Bkg" )

	out_dir[term].cd()
	stack_sig[term].Write()
	stack_bkg[term].Write()
	for sample in ["sig", "bkg", "Data"]:
	    histos[term][sample].Write()
    out_file.Close()
## End of function Template_By_Channel()

def Mass_BDT_Likelihood_Template():
    out_name = "BDT_channels_lepMVAn04_with_mass_min110_40bins.root"
    out_file = TFile( OUT_DIR + "/plots/" + out_name , "UPDATE")
    
    file_chain = TChain("tree","chain");
    file_chain.Add( OUT_DIR + "/" + "all_samples.root")

    cfg = PC.Plot_Config("ZH_4l")

    terms = ["mass_BDT_likelihood_100", "mass_BDT_likelihood_140", "mass_BDT_likelihood_300", "mass_BDT_likelihood_500",
	     "mass_BDT_likelihood_2_100", "mass_BDT_likelihood_2_140", "mass_BDT_likelihood_2_300", "mass_BDT_likelihood_2_500"]

    Mass_Sig = out_file.Get("dimu_mass/dimu_mass_Net_Sig").Clone()
    Mass_Bkg = out_file.Get("dimu_mass/dimu_mass_Net_Bkg").Clone()
    BDT_Sig  = out_file.Get("BDT_noMass_v3/BDT_noMass_v3_Net_Sig").Clone()
    BDT_Bkg  = out_file.Get("BDT_noMass_v3/BDT_noMass_v3_Net_Bkg").Clone()

    print Mass_Sig.Integral()
    print Mass_Bkg.Integral()
    print BDT_Sig. Integral()   
    print BDT_Bkg. Integral()

    histos = {}
    stack_all = {}
    stack_bkg = {}
    stack_sig = {}
    legend = {}
    out_dir   = {}

    for term in terms:
	out_dir[term] 	= out_file.mkdir(term)
	stack_all[term] = THStack("h_stack_"+term, term)
    	stack_bkg[term] = THStack("bkg_stack_"+term, "bkg_"+term) 
    	stack_sig[term] = THStack("sig_stack_"+term, "sig_"+term)
	histos[term]    = {}
    	for sample in cfg.signals + cfg.bkgs:
	    histos[term][sample] = TH1F(term + "_Net_" + sample, term + "_Net_" + sample, 20,0,1) # should in general from -1 to 1
	histos[term]["Data"] = TH1F(term + "_Net_" + "Data", term + "_Net_" + "Data", 20,0,1)

    print file_chain.GetEntries()
    for iEvt in range( file_chain.GetEntries() ):
        file_chain.GetEvent(iEvt)
        if (iEvt % 10000 == 1):
            print "looking at event %d" %iEvt 

	if file_chain.dilep_mass < 70 or file_chain.dilep_mass > 110: continue

	MVA_cut = 0.4	
	if file_chain.mu1_lepMVA < MVA_cut or file_chain.mu2_lepMVA < MVA_cut or file_chain.lep1_lepMVA < MVA_cut or file_chain.lep2_lepMVA < MVA_cut:
            continue
#	mu1_SF = GetSF("muon", file_chain.mu1_pt,  file_chain.mu1_abs_eta, MVA_cut)
#        mu2_SF = GetSF("muon", file_chain.mu2_pt,  file_chain.mu2_abs_eta, MVA_cut)
#        lep1_SF = GetSF(LEP,   file_chain.lep1_pt, file_chain.lep1_abs_eta, MVA_cut)
#	lep2_SF = GetSF(LEP,   file_chain.lep2_pt, file_chain.lep2_abs_eta, MVA_cut)
#        MVA_SF = mu1_SF * mu2_SF * lep1_SF * lep2_SF
	MVA_SF = file_chain.all_lepMVA_SF

	## Calculate mass and BDT combined likelihood
	mass_bin = Mass_Sig.FindBin(file_chain.dimu_mass)
	BDT_bin  = BDT_Sig. FindBin(file_chain.BDT_noMass_v3)

	mass_s = Mass_Sig.GetBinContent(mass_bin)
	mass_b = Mass_Bkg.GetBinContent(mass_bin)
        bdt_s  = BDT_Sig. GetBinContent(BDT_bin)
	bdt_b  = BDT_Bkg. GetBinContent(BDT_bin)

	likelihood_100  = 100.0*mass_s / (100.0*mass_s + mass_b) * 100.0*bdt_s / (100.0*bdt_s + bdt_b)
	likelihood_140  = 140.0*mass_s / (140.0*mass_s + mass_b) * 140.0*bdt_s / (140.0*bdt_s + bdt_b)
	likelihood_300  = 300.0*mass_s / (300.0*mass_s + mass_b) * 300.0*bdt_s / (300.0*bdt_s + bdt_b)
	likelihood_500  = 500.0*mass_s / (500.0*mass_s + mass_b) * 500.0*bdt_s / (500.0*bdt_s + bdt_b)

	likelihood_2_100 = 100.0*mass_s * 100.0*bdt_s / (100.0*mass_s *100.0*bdt_s + mass_b*bdt_b)
	likelihood_2_140 = 140.0*mass_s * 140.0*bdt_s / (140.0*mass_s *140.0*bdt_s + mass_b*bdt_b)
	likelihood_2_300 = 300.0*mass_s * 300.0*bdt_s / (300.0*mass_s *300.0*bdt_s + mass_b*bdt_b)
	likelihood_2_500 = 500.0*mass_s * 500.0*bdt_s / (500.0*mass_s *500.0*bdt_s + mass_b*bdt_b)

	if likelihood_2_100 > 0.5 and file_chain.Sample_ID == 0 : continue # blind

	FillHistTerm(histos, "mass_BDT_likelihood_100", cfg, likelihood_100, file_chain.Sample_ID, file_chain.xsec_norm * file_chain.event_wgt * MVA_SF)
	FillHistTerm(histos, "mass_BDT_likelihood_140", cfg, likelihood_140, file_chain.Sample_ID, file_chain.xsec_norm * file_chain.event_wgt * MVA_SF)
	FillHistTerm(histos, "mass_BDT_likelihood_300", cfg, likelihood_300, file_chain.Sample_ID, file_chain.xsec_norm * file_chain.event_wgt * MVA_SF)
	FillHistTerm(histos, "mass_BDT_likelihood_500", cfg, likelihood_500, file_chain.Sample_ID, file_chain.xsec_norm * file_chain.event_wgt * MVA_SF)
	FillHistTerm(histos, "mass_BDT_likelihood_2_100", cfg, likelihood_2_100, file_chain.Sample_ID, file_chain.xsec_norm * file_chain.event_wgt * MVA_SF)
        FillHistTerm(histos, "mass_BDT_likelihood_2_140", cfg, likelihood_2_140, file_chain.Sample_ID, file_chain.xsec_norm * file_chain.event_wgt * MVA_SF)
        FillHistTerm(histos, "mass_BDT_likelihood_2_300", cfg, likelihood_2_300, file_chain.Sample_ID, file_chain.xsec_norm * file_chain.event_wgt * MVA_SF)
        FillHistTerm(histos, "mass_BDT_likelihood_2_500", cfg, likelihood_2_500, file_chain.Sample_ID, file_chain.xsec_norm * file_chain.event_wgt * MVA_SF)



    scaled_signal = {}
    for term in terms:
	histos[term]["Data"].SetMarkerStyle(20)
	for sample in cfg.signals:
	    stack_sig[term].Add(histos[term][sample])
	histos[term]["sig"] = stack_sig[term].GetStack().Last().Clone()
	histos[term]["sig"].SetNameTitle( term + "_Net_Sig", term + "_Net_Sig" )

	histos[term]["signal"] = stack_sig[term].GetStack().Last()
        stack_all[term].Add(histos[term]["signal"])
        histos[term]["signal"].SetFillColor( kGray )
        histos[term]["signal"].SetLineWidth(0)

	for sample in cfg.bkgs:
	    histos[term][sample].SetFillColor(cfg.colors[sample])
	    stack_bkg[term].Add(histos[term][sample])
	    stack_all[term].Add(histos[term][sample])
	histos[term]["bkg"] = stack_bkg[term].GetStack().Last().Clone()
        histos[term]["bkg"].SetNameTitle( term + "_Net_Bkg", term + "_Net_Bkg" )

	out_dir[term].cd()
	stack_sig[term].Write()
	stack_bkg[term].Write()
	for sample in ["sig", "bkg", "Data"]:
	    histos[term][sample].Write()

	scaled_signal[term] =  histos[term]["signal"].Clone()
        scaled_signal[term].Scale(20)
        scaled_signal[term].SetLineColor(kRed)
        scaled_signal[term].SetLineWidth(2)
        scaled_signal[term].SetFillStyle(0)

        legend[term] = TLegend(0.6,0.6,0.9,0.9)
        legend[term].AddEntry(histos[term]["Data"], "Data")
        legend[term].AddEntry(histos[term]["signal"], "signal sum")
        for sample in cfg.bkgs:
            legend[term].AddEntry(histos[term][sample], sample )
        legend[term].AddEntry(scaled_signal[term], "signal X20")
        LinearStack( term, stack_all[term], scaled_signal[term], histos[term]["Data"], legend[term], OUT_DIR+"/plots")

    out_file.Close()

## End of function Mass_BDT_Likelihood_Template()


def Template_From_TestSamp():
    out_name = "BDT_lepMVA04_with_mass.root"
    out_file = TFile( OUT_DIR + "/" + out_name , "RECREATE")

    in_file_BDT  = TFile.Open( PLOT_DIR + "/" + BDT_WITH_MASS, "READ")
    in_file_norm = TFile.Open( OUT_DIR + "/" + MASS_FILE, "READ")

    #bdt_name = "2017_WH_ele_with_mass_all_sig_all_bkg_ge0j"
    bdt_name = "2017_WH_ele_against_inclu_trimvar_with_mass_all_sig_all_bkg_ge0j"
    method_name = "BDTG_UF_v2"
    sig_hist_name = bdt_name + "/" + "Method_" + method_name + "/" + method_name + "/" + "MVA_" + method_name + "_S"
    bkg_hist_name = bdt_name + "/" + "Method_" + method_name + "/" + method_name + "/" + "MVA_" + method_name + "_B"
    BDT_sig = in_file_BDT.Get(sig_hist_name).Clone()
    BDT_sig.SetNameTitle("BDT_Net_Sig", "BDT_Net_Sig")
    BDT_bkg = in_file_BDT.Get(bkg_hist_name).Clone()
    BDT_bkg.SetNameTitle("BDT_Net_Bkg", "BDT_Net_Bkg")

    BDT_data = TH1D("BDT_Net_Data", "BDT_Net_Data", 100,-1,1) #placeholder empty data plot

    sig_norm = in_file_norm.Get("dimu_mass_Net_Sig").Integral()
    bkg_norm = in_file_norm.Get("dimu_mass_Net_Bkg").Integral()
    print "signal norm = %f" %sig_norm
    print "bkg    norm = %f" %bkg_norm

    print "signal BDT Integral = %f" %BDT_sig.Integral()
    print "bkg    BDT Integral = %f" %BDT_bkg.Integral()
    BDT_sig.Scale( 1/BDT_sig.Integral() )
    BDT_sig.Scale( sig_norm )
    BDT_bkg.Scale( 1/BDT_bkg.Integral() )
    BDT_bkg.Scale( bkg_norm )


    out_file.cd()
    BDT_sig.Write()
    BDT_bkg.Write()
    BDT_data.Write()
    out_file.Close()


def BDT_Cut_Mass_Template():
    out_name_neg = "mass_BDT_neg_lepMVA04.root"
    out_name_pos = "mass_BDT_pos_lepMVA04.root"
    out_file_neg = TFile( OUT_DIR + "/" + out_name_neg , "RECREATE")
    out_file_pos = TFile( OUT_DIR + "/" + out_name_pos , "RECREATE")

    in_file_BDT  = TFile.Open( PLOT_DIR + "/" + BDT_WITHOUT_MASS, "READ")

    TestTree = in_file_BDT.Get("2017_WH_ele_without_mass_all_sig_all_bkg_ge0j/TestTree")
    TestTree.Draw("dimu_mass>>bkg_neg(12,100,160)", "2 * event_wgt * xsec_norm * ( BDTG_UF_v2 < 0 && Sample_ID < 0)")
    bkg_neg = gDirectory.Get("bkg_neg")
    bkg_neg.SetName("dimu_mass_Net_Bkg")
    bkg_pos = TestTree.Draw("dimu_mass>>bkg_pos(12,100,160)", "2 * event_wgt * xsec_norm * ( BDTG_UF_v2 >= 0 && Sample_ID < 0)")
    bkg_pos = gDirectory.Get("bkg_pos")
    bkg_pos.SetName("dimu_mass_Net_Bkg")
    sig_neg = TestTree.Draw("dimu_mass>>sig_neg(12,100,160)", "2 * event_wgt * xsec_norm * ( BDTG_UF_v2 < 0 && Sample_ID > 0)")
    sig_neg = gDirectory.Get("sig_neg")
    sig_neg.SetName("dimu_mass_Net_Sig")
    sig_pos = TestTree.Draw("dimu_mass>>sig_pos(12,100,160)", "2 * event_wgt * xsec_norm * ( BDTG_UF_v2 >= 0 && Sample_ID > 0)")
    sig_pos = gDirectory.Get("sig_pos")
    sig_pos.SetName("dimu_mass_Net_Sig")

    data_neg = bkg_neg.Clone()   # place holder
    data_neg.SetName("dimu_mass_Net_Data")
    data_pos = bkg_pos.Clone()
    data_pos.SetName("dimu_mass_Net_Data")

    out_file_neg.cd()
    bkg_neg.Write()
    sig_neg.Write()
    data_neg.Write()
    out_file_neg.Close()

    out_file_pos.cd()
    bkg_pos.Write()
    sig_pos.Write()
    data_pos.Write()
    out_file_pos.Close()

def main():
    Template_By_Channel()
    Mass_BDT_Likelihood_Template()

    #Template_From_TestSamp()
    #BDT_Cut_Mass_Template()
main()











	
