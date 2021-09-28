#! /usr/bin/env python 

import ROOT as R
from array import array

VERBOSE = True

IN_FILE = '/afs/cern.ch/work/x/xzuo/public/H2Mu/Run2/Histograms/ZH_lep_SR_CR_kfactor_2020_05_07/plots/mass_hists_lepMVAp04.root'
#IN_FILE = '/afs/cern.ch/work/x/xzuo/public/H2Mu/Run2/Histograms/WH_lep_preApp_final_SR_CR_missHit_2020_05_09/plots/3lep_lep20_MissHits1_medLepMVA_SR_Bveto_noSys/StackPlots.root'

def findFWHM(hist):
    xmax = 0
    ymax = 0
    for i in range(4,hist.GetNbinsX()-4):
      if ymax < hist.Integral(i-2, i+2):
        ymax = hist.Integral(i-2, i+2)
        xmax = i
    if ymax == 0:
      print 'weird case: no max found in hist'
      return (0,0)

    lo_bin = hist.GetNbinsX()
    hi_bin = 0
    for i in range(4,hist.GetNbinsX()-4):
      if i < lo_bin and hist.Integral(i-2, i+2) > ymax/2.0: lo_bin = i
      if i > hi_bin and hist.Integral(i-2, i+2) > ymax/2.0: hi_bin = i

    if VERBOSE: print "FWHM is %.2f, %.2f" %(hist.GetBinCenter(lo_bin), hist.GetBinCenter(hi_bin))
    return (lo_bin, hi_bin)

def main():

    in_file  = R.TFile.Open(IN_FILE, "READ")
    out_file = R.TFile.Open('scripts/BDT_plots_ZH.root','RECREATE')

    samples = ['Data', 'Bkg', 'Sig', 'ggZH', 'ZH', 'DY', 'ZZ', 'ggZZ', 'others']
#    samples = ['WZ', 'ZZ+ggZZ', 'ZJets', 'top', 'Other']

    out_file.cd()
#    in_file.Get("BDT_final_Net_Data").Clone("BDT_final_Net_Data").Rebin(3).Write()
#    in_file.Get("BDT_final_Net_Sig").Clone("BDT_final_Net_Sig").Rebin(3).Write()
#    in_file.Get("BDT_final_Net_Bkg").Clone("BDT_final_Net_Bkg").Rebin(3).Write()
    for sample in samples:
      in_file.Get("BDT_final_Net_"+sample).Clone("BDT_final_Net_"+sample).Rebin(2).Write()
#    in_file.Get("BDT_final_zoom_WH").Clone("BDT_final_Net_WH").Rebin(5).Write()
#    h_ZH = in_file.Get("BDT_final_zoom_ZH").Clone("BDT_final_Net_ZH_ggZH").Rebin(5)
#    h_ZH.Add( in_file.Get("BDT_final_zoom_ggZH").Clone("BDT_final_Net_ggZH").Rebin(5) )
#    h_ttH = in_file.Get("BDT_final_zoom_ttH").Clone("BDT_final_Net_ttH_tH").Rebin(5)
#    h_ttH.Add( in_file.Get("BDT_final_zoom_THQ").Clone("BDT_final_Net_tHq").Rebin(5) )
#    h_ttH.Add( in_file.Get("BDT_final_zoom_THW").Clone("BDT_final_Net_tHW").Rebin(5) )
#    h_ZH.Write()
#    h_ttH.Write()


    ## ZH
    h_bin  = array('d',[-1.0, -0.1, 1.0])
    h_sens = R.TH1F ("significance", "significance", len(h_bin)-1 , h_bin)

    sig0 = in_file.Get("dimu_mass_BDT_n10_n01_Net_Sig").Clone("Sig_cat0")
    bkg0 = in_file.Get("dimu_mass_BDT_n10_n01_Net_Bkg").Clone("Bkg_cat0")

    low_bin, high_bin = findFWHM(sig0)
    s0 = sig0.Integral(low_bin, high_bin)
    b0 = bkg0.Integral(low_bin, high_bin)

    sig1 = in_file.Get("dimu_mass_BDT_n01_p10_Net_Sig").Clone("Sig_cat1")
    bkg1 = in_file.Get("dimu_mass_BDT_n01_p10_Net_Bkg").Clone("Bkg_cat1")

    low_bin, high_bin = findFWHM(sig1)
    s1 = sig1.Integral(low_bin, high_bin)
    b1 = bkg1.Integral(low_bin, high_bin)

    h_sens.SetBinContent(1, s0 / pow(b0,0.5))
    h_sens.SetBinContent(2, s1 / pow(b1,0.5))

    sig0.Write()
    bkg0.Write()
    sig1.Write()
    bkg1.Write()
    h_sens.Write()
    ## ZH

    ## WH
#    h_bin  = array('d',[-1.0, -0.1, 0.3, 1.0])
#    h_sens = R.TH1F ("significance", "significance", len(h_bin)-1 , h_bin)
#
#    sig0 = in_file.Get("H_pair_mass_BDT_final_n10_n01_zoomH_Net_Sig").Clone("Sig_cat0")
#    bkg0 = in_file.Get("H_pair_mass_BDT_final_n10_n01_zoomH_Net_Bkg").Clone("Bkg_cat0")
#
#    low_bin, high_bin = findFWHM(sig0)
#    s0 = sig0.Integral(low_bin, high_bin)
#    b0 = bkg0.Integral(low_bin, high_bin)
#
#    sig1 = in_file.Get("H_pair_mass_BDT_final_n01_p03_zoomH_Net_Sig").Clone("Sig_cat1")
#    bkg1 = in_file.Get("H_pair_mass_BDT_final_n01_p03_zoomH_Net_Bkg").Clone("Bkg_cat1")
#
#    low_bin, high_bin = findFWHM(sig1)
#    s1 = sig1.Integral(low_bin, high_bin)
#    b1 = bkg1.Integral(low_bin, high_bin)
#
#    sig2 = in_file.Get("H_pair_mass_BDT_final_p03_p10_zoomH_Net_Sig").Clone("Sig_cat2")
#    bkg2 = in_file.Get("H_pair_mass_BDT_final_p03_p10_zoomH_Net_Bkg").Clone("Bkg_cat2")
#
#    low_bin, high_bin = findFWHM(sig2)
#    s2 = sig1.Integral(low_bin, high_bin)
#    b2 = bkg1.Integral(low_bin, high_bin)
#
#    h_sens.SetBinContent(1, s0 / pow(b0,0.5))
#    h_sens.SetBinContent(2, s1 / pow(b1,0.5))
#    h_sens.SetBinContent(3, s2 / pow(b2,0.5))
#
#    sig0.Write()
#    bkg0.Write()
#    sig1.Write()
#    bkg1.Write()
#    sig2.Write()
#    bkg2.Write()
#    h_sens.Write()
    ## WH


    out_file.Close()
    in_file.Close()

main()	
