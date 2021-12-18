#! /usr/bin/env python 

from ROOT import *
from array import array

IN_FILE = 'output/Run2_WH_lep_all_vs_all_2020_03_23_v2.root'

Train = 'f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairAll_var1_var2_muSSOSdEta_lepCharge_all_sig_all_bkg_resWgt'

Version = 'BDTG_UF_v1a'
Var = 'muSS_MHT_MT'
hbin = '(40, -0.8, 0.9)'

def main():

    train_chain = TChain(Train + '/TrainTree')
    test_chain  = TChain(Train + '/TestTree')
    train_chain.Add(IN_FILE)
    test_chain. Add(IN_FILE)

    wgt_str = 'weight'
    lowMT_str  = 'muSS_MHT_MT<40'
    medMT_str  = 'muSS_MHT_MT>40 && muSS_MHT_MT<80'
    highMT_str = 'muSS_MHT_MT>80'

    test_chain.Draw("%s>>BDT_sig_lowMT %s" %(Version, hbin), "(classID == 0 && nEles == 1 && %s) * %s" %(lowMT_str, wgt_str), "goff")
    test_chain.Draw("%s>>BDT_sig_medMT %s" %(Version, hbin), "(classID == 0 && nEles == 1 && %s) * %s" %(medMT_str, wgt_str), "goff")
    test_chain.Draw("%s>>BDT_sig_highMT%s" %(Version, hbin), "(classID == 0 && nEles == 1 && %s) * %s" %(highMT_str, wgt_str), "goff")

    test_chain.Draw("%s>>BDT_bkg_lowMT %s" %(Version, hbin), "(classID == 1 && samp_ID < 10 && nEles == 1 && %s) * %s" %(lowMT_str, wgt_str), "goff")
    test_chain.Draw("%s>>BDT_bkg_medMT %s" %(Version, hbin), "(classID == 1 && samp_ID < 10 && nEles == 1 && %s) * %s" %(medMT_str, wgt_str), "goff")
    test_chain.Draw("%s>>BDT_bkg_highMT%s" %(Version, hbin), "(classID == 1 && samp_ID < 10 && nEles == 1 && %s) * %s" %(highMT_str, wgt_str), "goff")

    sig_lowMT  = gDirectory.Get("BDT_sig_lowMT")
    sig_medMT  = gDirectory.Get("BDT_sig_medMT")
    sig_highMT = gDirectory.Get("BDT_sig_highMT")

    bkg_lowMT  = gDirectory.Get("BDT_bkg_lowMT")
    bkg_medMT  = gDirectory.Get("BDT_bkg_medMT")
    bkg_highMT = gDirectory.Get("BDT_bkg_highMT")

    sig_lowMT. Scale( 1/sig_lowMT. Integral() ) 
    sig_medMT. Scale( 1/sig_medMT. Integral() )
    sig_highMT.Scale( 1/sig_highMT.Integral() )

    bkg_lowMT. Scale( 1/bkg_lowMT. Integral() )
    bkg_medMT. Scale( 1/bkg_medMT. Integral() )
    bkg_highMT.Scale( 1/bkg_highMT.Integral() )


    canv = TCanvas("canvas", "canvas", 600, 600)
    canv.cd()
    gStyle.SetOptStat(0000)

    sig_lowMT.SetTitle('')
    sig_medMT.SetTitle('')
    sig_highMT.SetTitle('')
    bkg_lowMT.SetTitle('')
    bkg_medMT.SetTitle('')
    bkg_highMT.SetTitle('')

    sig_lowMT.GetXaxis().SetTitle('BDT score')
    sig_lowMT.GetYaxis().SetTitle("a.u.")
    sig_lowMT.GetYaxis().SetTitleOffset(1.15)

    sig_lowMT. SetLineColor(kOrange+2)
    sig_lowMT. SetLineWidth(2)
    sig_medMT. SetLineColor(kRed)
    sig_medMT. SetLineWidth(2)
    sig_highMT.SetLineColor(kViolet)
    sig_highMT.SetLineWidth(2)

    bkg_lowMT. SetLineColor(kBlue)
    bkg_lowMT. SetLineWidth(2)
    bkg_lowMT. SetFillColorAlpha(kBlue,0.1)
    bkg_lowMT. SetFillStyle(3345)
    bkg_medMT. SetLineColor(kCyan)
    bkg_medMT. SetLineWidth(2)
    bkg_medMT. SetFillColorAlpha(kCyan,0.1)
    bkg_medMT. SetFillStyle(3345)
    bkg_highMT.SetLineColor(kGreen)
    bkg_highMT.SetLineWidth(2)
    bkg_highMT.SetFillColorAlpha(kGreen,0.1)
    bkg_highMT.SetFillStyle(3345)

    maximum = max(sig_lowMT.GetMaximum(), sig_medMT.GetMaximum(), sig_highMT.GetMaximum()) * 1.7
    sig_lowMT.GetYaxis().SetRangeUser(0.0,maximum)

    sig_lowMT. Draw("hist")
    sig_medMT. Draw("histsame")
    sig_highMT.Draw("histsame")
    bkg_lowMT. Draw("histsame")
    bkg_medMT. Draw("histsame")
    bkg_highMT.Draw("histsame")

    leg = TLegend(0.15,0.65,0.9,0.9)
    leg.SetNColumns(3)
    leg.SetFillColor(0)
    leg.SetFillStyle(0)
    leg.SetBorderSize(0)
    leg.AddEntry(sig_lowMT,  "Sig, muSS_MHT_MT < 40", "L")
    leg.AddEntry(sig_medMT,  "Sig, 40 < muSS_MHT_MT < 80", "L")
    leg.AddEntry(sig_highMT, "Sig, 80 < muSS_MHT_MT", "L")
    leg.AddEntry(bkg_lowMT,  "Bkg, muSS_MHT_MT < 40", "L")
    leg.AddEntry(bkg_medMT,  "Bkg, 40 < muSS_MHT_MT < 80", "L")
    leg.AddEntry(bkg_highMT, "Bkg, 80 < muSS_MHT_MT", "L")
    leg.Draw("same")

#      l_lumi = TLatex()
#      l_lumi.SetTextSize(0.025)
#      l_lumi.SetTextAlign(31)
#      l_lumi.DrawLatexNDC(0.90, 0.91, '137.1 fb^{-1} (13 TeV)')
#      l_lumi.Draw('SAME')
#      l_cms  = TLatex()
#      l_cms.SetTextSize(0.03)
#      l_cms.DrawLatexNDC(0.15, 0.85, '#scale[1.5]{CMS} simulation')
#      l_cms.Draw('SAME')

    canv.SaveAs('scripts/check_MT_plot_ele_only_nonprompt_bkg' + '.png')
    canv.SaveAs('scripts/check_MT_plot_ele_only_nonprompt_bkg' + '.pdf')


main()	
