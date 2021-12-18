#! /usr/bin/env python 

from ROOT import *
from array import array
from CMS_style import setTDRStyle

#IN_FILE = 'output/Run2_ZH_lep_against_inclu_no_mass_setxsec_trimMHT_20200311_right_RESwgt.root'
IN_FILE = 'output/Run2_WH_lep_all_vs_all_2020_03_23_v2.root'

#Train = 'ZH_Run2_lep_against_inclu_skim104_without_mass_all_sig_all_bkg_ge0j'
Train = 'f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairAll_var1_var2_muSSOSdEta_lepCharge_all_sig_all_bkg_resWgt'

#Version = 'BDTG_UF_v2'
Version = 'BDTG_UF_v1a'

def var_title(var):
  title = var
  if   var == 'H_pair_pt':       title = 'p_{T}(#mu#mu) (GeV)'
  elif var == 'muH1_eta_abs':    title = '|#eta(#mu_{leading})|'
  elif var == 'muH2_eta_abs':    title = '|#eta(#mu_{trailing})|'
  elif var == 'muSS_muOS_dR':    title = '#Delta^{}R(#mu_{OS},#mu_{SS})'
  elif var == 'lep_pt':          title = 'p_{T}(lep) (GeV)'
  elif var == 'nEles':           title = 'N_{electron}'
  elif var == 'lep_H_pair_dR':   title = '#Delta^{}R(lep, #mu#mu_{H})'
  elif var == 'lep_H_pair_dEta': title = '#Delta#eta(lep, #mu#mu_{H})'
  elif var == 'lep_muSS_dEta':   title = '#Delta#eta(lep, #mu_{SS})'
  elif var == 'lep_muSS_cosThStar': title = 'cos#theta^{*}(lep, #mu_{SS})'
  elif var == 'lep_muOS_dR':        title = '#Delta^{}R(lep, #mu_{OS})'
  elif var == 'lep_muOS_dEta':      title = '#Delta#eta(lep, #mu_{OS})'
  elif var == 'lep_muOS_cosThStar': title = 'cos#theta^{*}(lep, #mu_{OS})'
  elif var == 'muSS_MHT_MT':        title = 'M_{T}(#mu_{SS},MHT) (GeV)'
  elif var == 'lep_MHT_MT':         title = 'M_{T}(lep,MHT) (GeV)'
  elif var == 'lep_MHT_dPhi_abs':   title = '|#Delta#phi(lep, MHT)|'

  elif var == 'dimu_pt':        title = 'p_{T}(#mu#mu) (GeV)'
  elif var == 'dimu_abs_eta':   title = '|#eta(#mu#mu)|'
  elif var == 'dimu_abs_dPhi':  title = '|#Delta#phi(#mu#mu)|'
  elif var == 'dilep_mass':     title = 'm_{dilep} (GeV)'
  elif var == 'dilep_pt':       title = 'p_{T}(dilep) (GeV)'
  elif var == 'dilep_abs_eta':  title = '|#eta(dilep)|'
  elif var == 'dilep_dR':       title = '#Delta^{}R(dilep)'
  elif var == 'lep_ID':         title = 'lepton flavor'
  elif var == 'cts_dipair_H':   title = 'cos#theta*(#mu#mu,dilep)'
  elif var == 'dipair_dEta_H':  title = '#Delta#eta(#mu#mu,dilep)'

  return title

def main():

    train_chain = TChain(Train + '/TrainTree')
    test_chain  = TChain(Train + '/TestTree')
    train_chain.Add(IN_FILE)
    test_chain. Add(IN_FILE)

    wgt_str = 'weight'

    var_list = ["H_pair_pt", "muH1_eta_abs", "muH2_eta_abs", "muSS_muOS_dR", 
                "lep_pt", "nEles", "lep_H_pair_dR", "lep_H_pair_dEta", 
                "lep_muSS_dEta", "lep_muSS_cosThStar", "lep_muOS_dR", "lep_muOS_dEta", "lep_muOS_cosThStar",
                "muSS_MHT_MT", "lep_MHT_MT", "lep_MHT_dPhi_abs"] 

#    var_list = ["dimu_pt", "dimu_abs_eta", "dimu_abs_dPhi", 
#                "dilep_mass", "dilep_pt", "dilep_abs_eta", "dilep_dR",
#                "lep_ID", "cts_dipair_H", "dipair_dEta_H"]

    setTDRStyle()

    for var in var_list:
      print "plotting " + var

      if   var == "H_pair_pt":           h_bin = "(60, 0, 600)"
      elif var == "muH1_eta_abs":        h_bin = "(60, 0, 2.4)"
      elif var == "muH2_eta_abs":        h_bin = "(60, 0, 2.4)"
      elif var == "muSS_muOS_dR":        h_bin = "(50, 0, 4.5)"
      elif var == "lep_pt":              h_bin = "(50, 20, 320)"
      elif var == "nEles":               h_bin = "(2,  0, 2)"
      elif var == "lep_H_pair_dR":       h_bin = "(50, 0, 8)"
      elif var == "lep_H_pair_dEta":     h_bin = "(60, -5, 7)"
      elif var == "lep_muSS_dEta":       h_bin = "(50, -5, 5)"
      elif var == "lep_muSS_cosThStar":  h_bin = "(50, -1, 1)"
      elif var == "lep_muOS_dR":         h_bin = "(60, 0,  6)"
      elif var == "lep_muOS_dEta":       h_bin = "(50, -5, 5)"
      elif var == "lep_muOS_cosThStar":  h_bin = "(50, -1, 1)"
      elif var == "muSS_MHT_MT":         h_bin = "(60, 0, 600)"
      elif var == "lep_MHT_MT":          h_bin = "(60, 0, 300)"
      elif var == "lep_MHT_dPhi_abs":    h_bin = "(50, 0, 3.14)"

      elif var == "dimu_pt":        h_bin = "(60, 0, 600)"
      elif var == "dimu_abs_eta":   h_bin = "(50, 0, 7)"
      elif var == "dimu_abs_dPhi":  h_bin = "(50, 0, 3.14)"
      elif var == "dilep_mass":     h_bin = "(50, 81, 101)"
      elif var == "dilep_pt":       h_bin = "(60, 0, 600)"
      elif var == "dilep_abs_eta":  h_bin = "(50, 0, 7)"
      elif var == "dilep_dR":       h_bin = "(50, 0, 5)"
      elif var == "lep_ID":         h_bin = "(3,  11, 14)"
      elif var == "cts_dipair_H":   h_bin = "(50, -1, 1)"
      elif var == "dipair_dEta_H":  h_bin = "(60, -9, 9)"



      ## in ZH training, classID == 1 is signal
      ## in WH training, classID == 0 is signal
      ## probably depends on whether the first event is a sig or bkg

      train_chain.Draw("%s >> %s_sig%s" %(var, var, h_bin), "(classID == 0) * %s" %wgt_str, "goff")
      train_chain.Draw("%s >> %s_bkg%s" %(var, var, h_bin), "(classID == 1) * %s" %wgt_str, "goff")
      h_sig = gDirectory.Get("%s_sig"%var)
      h_bkg = gDirectory.Get("%s_bkg"%var)
 
      test_chain.Draw("%s >> %s_sig_t%s" %(var, var, h_bin), "(classID == 0) * %s" %wgt_str, "goff")
      test_chain.Draw("%s >> %s_bkg_t%s" %(var, var, h_bin), "(classID == 1) * %s" %wgt_str, "goff")
      h_sig .Add( gDirectory.Get("%s_sig_t"%var) )
      h_bkg .Add( gDirectory.Get("%s_bkg_t"%var) )
 
      h_sig.Scale( 1/h_sig.Integral() )
      h_bkg.Scale( 1/h_bkg.Integral() )

      canv = TCanvas("canvas", "canvas", 600, 600)
      canv.cd()
      gStyle.SetOptStat(0000)

      h_bkg.SetTitle('')
      h_bkg.GetXaxis().SetTitle(var_title(var))
      h_bkg.GetXaxis().SetTitleSize(0.04)
      h_bkg.GetXaxis().SetTitleOffset(1.15)
      h_bkg.GetYaxis().SetTitle("a.u.")
      h_bkg.GetYaxis().SetTitleOffset(1.15)
    
      h_sig.SetTitle('')
      h_sig.GetXaxis().SetTitle(var_title(var))
      h_sig.GetXaxis().SetTitleSize(0.04)
      h_sig.GetXaxis().SetTitleOffset(1.15)
      h_sig.GetYaxis().SetTitle("a.u.")
      h_sig.GetYaxis().SetTitleOffset(1.15)
    
      h_sig.SetLineColor(kBlue)
      h_sig.SetLineWidth(2)
      h_sig.SetFillColorAlpha(kBlue,0.1)
      h_sig.SetFillStyle(3345)   
 
      h_bkg.SetLineColor(kRed)
      h_bkg.SetLineWidth(2)
      h_bkg.SetFillColorAlpha(kRed,0.1)
      h_bkg.SetFillStyle(3354)

      maximum = max(h_sig.GetMaximum(), h_bkg.GetMaximum()) * 1.4
      h_sig.GetYaxis().SetRangeUser(0.0,maximum)
      h_bkg.GetYaxis().SetRangeUser(0.0,maximum)

      h_sig.Draw("hist")
      h_bkg.Draw("hist same")
      leg = TLegend(0.7,0.72,0.9,0.9)
      leg.SetFillColor(0)
      leg.SetFillStyle(0)
      leg.SetBorderSize(0)
      leg.AddEntry(h_sig,"Signal","F")
      leg.AddEntry(h_bkg,"Background","F")
      leg.Draw("same")

      l_lumi = TLatex()
      l_lumi.SetTextSize(0.025)
      l_lumi.SetTextAlign(31)
      l_lumi.DrawLatexNDC(0.93, 0.95, '#font[42]{#scale[1.3]{(13 TeV)}}')
      l_lumi.Draw('SAME')
      l_cms  = TLatex()
      l_cms.SetTextSize(0.03)
      l_cms.DrawLatexNDC(0.15, 0.85, '#scale[1.5]{CMS #bf{#it{Preliminary Simulation}}}')
      l_cms.Draw('SAME')

      canv.SaveAs('scripts/WH_inputs/BDT_' + var + '.png')
      canv.SaveAs('scripts/WH_inputs/BDT_' + var + '.pdf')


main()	
