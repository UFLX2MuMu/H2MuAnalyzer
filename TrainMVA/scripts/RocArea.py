#! /usr/bin/env python 

import ROOT as R
from array import array

#IN_FILE = 'output/Run2_ZH_lep_against_inclu_no_mass_setxsec_trimMHT_20200311_right_RESwgt.root'
IN_FILE = 'output/Run2_WH_lep_all_vs_all_2020_03_23_v2.root'

#Trains = ['ZH_Run2_lep_against_inclu_sel_angle_without_mass_all_sig_all_bkg_ge0j',
#          'ZH_Run2_lep_against_inclu_skim_without_mass_all_sig_all_bkg_ge0j',
#	  'ZH_Run2_lep_against_inclu_skim101_without_mass_all_sig_all_bkg_ge0j',
#          'ZH_Run2_lep_against_inclu_skim102_without_mass_all_sig_all_bkg_ge0j',
#          'ZH_Run2_lep_against_inclu_skim103_without_mass_all_sig_all_bkg_ge0j',
#          'ZH_Run2_lep_against_inclu_skim104_without_mass_all_sig_all_bkg_ge0j']
#          'ZH_Run2_lep_against_inclu_skim2_without_mass_all_sig_all_bkg_ge0j',
#          'ZH_Run2_lep_against_inclu_skim3_without_mass_all_sig_all_bkg_ge0j',
#          'ZH_Run2_lep_against_inclu_skim4_without_mass_all_sig_all_bkg_ge0j',
#          'ZH_Run2_lep_against_inclu_skim5_without_mass_all_sig_all_bkg_ge0j',
#          'ZH_Run2_lep_against_inclu_skim6_without_mass_all_sig_all_bkg_ge0j',
#          'ZH_Run2_lep_against_inclu_skim7_without_mass_all_sig_all_bkg_ge0j']

#Trains = ['f_Opt_AWB_noMass_v3_resWgt_all_sig_all_bkg_resWgt',
#          'f_Opt_AWB_noMass_v3_resWgt_trimMET_all_sig_all_bkg_resWgt',
#          'f_Opt_AWB_noMass_v3_resWgt_trimMHT_all_sig_all_bkg_resWgt']

#Trains = ['f_Opt_AWB_noMass_v3_resWgt_all_sig_all_bkg_resWgt',
#          'f_Opt_AWB_noMass_v3_resWgt_trimMET_all_sig_all_bkg_resWgt',
#          'f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairM_all_sig_all_bkg_resWgt',
#          'f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairM_pt_all_sig_all_bkg_resWgt',
#          'f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairM_pt_dR_all_sig_all_bkg_resWgt',
#          'f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairM_pt_dR_dEta_all_sig_all_bkg_resWgt']

#Trains = ['f_Opt_AWB_noMass_v3_resWgt_all_sig_all_bkg_resWgt',
#          'f_Opt_AWB_noMass_v3_resWgt_trimMET_all_sig_all_bkg_resWgt',
#          'f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairM_all_sig_all_bkg_resWgt',
#          'f_Opt_AWB_noMass_v3_resWgt_trimMET_OS_pt_dR_dEta_all_sig_all_bkg_resWgt',
#          'f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairM_pt_dR_dEta_all_sig_all_bkg_resWgt',
#          'f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairAll_muSSMhtDphi_all_sig_all_bkg_resWgt',
#          'f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairAll_muSSMhtDphi_MHT_all_sig_all_bkg_resWgt',
#          'f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairAll_muSSMhtDphi_MHT_muSSpt_all_sig_all_bkg_resWgt']

#Trains = ['f_Opt_AWB_noMass_v3_resWgt_all_sig_all_bkg_resWgt',
#          'f_Opt_AWB_noMass_v3_resWgt_trimMET_all_sig_all_bkg_resWgt',
#          'f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairM_all_sig_all_bkg_resWgt',
#          'f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairAll_muSSMhtDphi_MHT_muSSpt_all_sig_all_bkg_resWgt',
#          'f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairAll_var1_lepMuSSdR_all_sig_all_bkg_resWgt',
#          'f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairAll_var1_lepMuSSdR_lepMuOScos_all_sig_all_bkg_resWgt',
#          'f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairAll_var1_lMuSSdR_lMuOScos_lMuOSdR_all_sig_all_bkg_resWgt',
#          'f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairAll_var1_lMuSSdR_lMuOScos_lMuOSdR_muSSOScos_all_sig_all_bkg_resWgt',
#          'f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairAll_var1_lMuSSdR_lMuOScos_lMuOSdR_muSSOScos_lMuOSdEta_all_sig_all_bkg_resWgt']

#Trains = ['f_Opt_AWB_noMass_v3_resWgt_all_sig_all_bkg_resWgt',
#          'f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairM_all_sig_all_bkg_resWgt',
#          'f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairAll_var1_lepMuSSdR_all_sig_all_bkg_resWgt',
#          'f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairAll_var1_lepMuSSdR_muSSOScos_all_sig_all_bkg_resWgt']

#Trains = ['f_Opt_AWB_noMass_v3_resWgt_all_sig_all_bkg_resWgt',
#          'f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairM_all_sig_all_bkg_resWgt',
#          'f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairAll_var1_var2_all_sig_all_bkg_resWgt',
#          'f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairAll_var1_var2_lmuOScos_all_sig_all_bkg_resWgt',
#          'f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairAll_var1_var2_lmuOSdR_all_sig_all_bkg_resWgt',
#          'f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairAll_var1_var2_lmuOSdEta_all_sig_all_bkg_resWgt',
#          'f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairAll_var1_var2_lmuSSdEta_all_sig_all_bkg_resWgt',
#          'f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairAll_var1_var2_muSSOSdEta_all_sig_all_bkg_resWgt',
#          'f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairAll_var1_var2_lHdEta_all_sig_all_bkg_resWgt',
#          'f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairAll_var1_var2_lepCharge_all_sig_all_bkg_resWgt']

#Trains = ['f_Opt_AWB_noMass_v3_resWgt_all_sig_all_bkg_resWgt',
#          'f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairM_all_sig_all_bkg_resWgt',
#          'f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairAll_var1_var2_all_sig_all_bkg_resWgt',
#          'f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairAll_var1_var2_muSSOSdEta_lepCharge_all_sig_all_bkg_resWgt',
#          'f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairAll_var1_var2_muSSOSdEta_lepCharge_lmuOSdEta_lHdEta_all_sig_all_bkg_resWgt']

Trains = ['f_Opt_AWB_noMass_v3_resWgt_all_sig_all_bkg_resWgt',
          'f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairAll_var1_var2_muSSOSdEta_lepCharge_all_sig_all_bkg_resWgt',
          'f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairAllbutM_var1_var2_muSSOSdEta_lepCharge_all_sig_all_bkg_resWgt']

#Versions = ['BDTG_UF_v1', 'BDTG_UF_v2', 'BDTG_UF_v3']
Versions = ['BDTG_UF_v1', 'BDTG_UF_v1a','BDTG_UF_v1b', 'BDTG_UF_v1c', 'BDTG_UF_v1d']

ROC_name = 'rejBvsS'
train_ROC_name = 'trainingRejBvsS'

colors = {}
colors['BDTG_UF_v1']  = R.kBlack
colors['BDTG_UF_v2']  = R.kRed
colors['BDTG_UF_v3']  = R.kBlue
colors['BDTG_UF_v1a'] = R.kRed
colors['BDTG_UF_v1b'] = R.kBlue
colors['BDTG_UF_v1c'] = R.kGreen
colors['BDTG_UF_v1d'] = R.kOrange

colors['ZH_Run2_lep_against_inclu_sel_angle_without_mass_all_sig_all_bkg_ge0j'] = R.kBlack
colors['ZH_Run2_lep_against_inclu_skim104_without_mass_all_sig_all_bkg_ge0j'] = R.kRed
colors['f_Opt_AWB_noMass_v3_resWgt_all_sig_all_bkg_resWgt'] = R.kBlack
colors['f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairAll_var1_var2_muSSOSdEta_lepCharge_all_sig_all_bkg_resWgt'] = R.kRed
colors['f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairAllbutM_var1_var2_muSSOSdEta_lepCharge_all_sig_all_bkg_resWgt'] = R.kBlue

BDT_names = {}
BDT_names['ZH_Run2_lep_against_inclu_sel_angle_without_mass_all_sig_all_bkg_ge0j'] = 'no trim(16 vars)'
BDT_names['ZH_Run2_lep_against_inclu_skim104_without_mass_all_sig_all_bkg_ge0j'] = '10 vars'
BDT_names['f_Opt_AWB_noMass_v3_resWgt_all_sig_all_bkg_resWgt'] = 'no_trim(34vars)'
BDT_names['f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairAll_var1_var2_muSSOSdEta_lepCharge_all_sig_all_bkg_resWgt'] = 'trimmed(16vars)'
BDT_names['f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairAllbutM_var1_var2_muSSOSdEta_lepCharge_all_sig_all_bkg_resWgt'] = '16vars+OS_mass'

def main():

    in_file  = R.TFile.Open(IN_FILE, "READ")
    out_file = R.TFile.Open('scripts/WH_ROC_out.root','RECREATE')

    for train in Trains:
      for ver in Versions:
	ROC = in_file.Get(train + '/Method_' + ver + '/' + ver + '/MVA_' + ver + '_' + ROC_name)
	print train + '   ' + ver
  	#n = 0.65/ROC.GetBinWidth(1)
	#print n
	for n in [50,60,70,80,90]:
	  print '%.2f, %.2f, %.2f' %(ROC.Integral(), ROC.GetBinCenter(n), ROC.GetBinContent(n))

    rocs = {}
    for train in Trains:
      canv = R.TCanvas('overlay_'+train, 'overlay_'+train, 800, 800)
      legend = R.TLegend(0.7,0.7,0.9,0.9)
      canv.cd()
      canv.SetGridx()
      canv.SetGridy()
      for ver in Versions:
        rocs[train+'_'+ver] = in_file.Get(train + '/Method_' + ver + '/' + ver + '/MVA_' + ver + '_' + ROC_name).Clone(train + '_' + ver)
	rocs[train+'_'+ver].SetLineWidth(2)
	rocs[train+'_'+ver].SetLineColor(colors[ver])
	rocs[train+'_'+ver].GetXaxis().SetNdivisions(510)
	rocs[train+'_'+ver].GetYaxis().SetNdivisions(510)
	legend.AddEntry( rocs[train+'_'+ver], ver)
	rocs[train+'_'+ver].Draw('HISTSAME')
      legend.Draw()
      canv.Write()


    for ver in Versions:
      if ver != 'BDTG_UF_v1a': continue
      canv = R.TCanvas('overlay_'+ver, 'overlay_'+ver, 800, 800)
      legend = R.TLegend(0.7,0.7,0.9,0.9)
      canv.cd()
      canv.SetGridx()
      canv.SetGridy()
      for train in Trains:
	if train == 'f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairAllbutM_var1_var2_muSSOSdEta_lepCharge_all_sig_all_bkg_resWgt': continue
        rocs[train+'_'+ver].SetLineColor(colors[train])
	legend.AddEntry( rocs[train+'_'+ver], BDT_names[train])
	rocs[train+'_'+ver].Draw('HISTSAME')
      legend.Draw()
      canv.Write()

    for train in Trains:
      canv = R.TCanvas('overtrain_'+train, 'overtrain_'+train, 800, 800)
      legend = R.TLegend(0.7,0.7,0.9,0.9)
      canv.cd()
      canv.SetGridx()
      canv.SetGridy()
      ver = 'BDTG_UF_v1a'
#      rocs[train+'_'+ver+'_train'] = in_file.Get(train + '/Method_' + ver + '/' + ver + '/MVA_' + ver + '_' + train_ROC_name).Clone(train + '_' + ver + '_train')
#      rocs[train+'_'+ver+'_train'].SetLineWidth(2)
#      rocs[train+'_'+ver+'_train'].SetLineColor(R.kRed)
#      rocs[train+'_'+ver].SetLineColor(R.kBlue)
#      legend.AddEntry( rocs[train+'_'+ver+'_train'], 'training')
#      legend.AddEntry( rocs[train+'_'+ver],          'testing')
#      rocs[train+'_'+ver+'_train'].Draw('HISTSAME')
#      rocs[train+'_'+ver].Draw('HISTSAME')

      h_train = R.TGraphErrors() 
      h_test  = R.TGraphErrors()
      h_train.SetName('Roc_train'+train)
      h_test .SetName('Roc_test' +train)

      train_S = in_file.Get(train + '/Method_' + ver + '/' + ver + '/MVA_' + ver + '_Train_S').Clone(train + '_' + ver + '_train_S')
      train_B = in_file.Get(train + '/Method_' + ver + '/' + ver + '/MVA_' + ver + '_Train_B').Clone(train + '_' + ver + '_train_B')
      test_S  = in_file.Get(train + '/Method_' + ver + '/' + ver + '/MVA_' + ver + '_S').Clone(train + '_' + ver + '_S')
      test_B  = in_file.Get(train + '/Method_' + ver + '/' + ver + '/MVA_' + ver + '_B').Clone(train + '_' + ver + '_B')
      for i in range(1, train_S.GetNbinsX()+1):
        s_tr = 0.0    # s_tr means the number of signal above certain BDT score in training
        b_tr = 0.0
        s_tr_err = array('d', [0])
        b_tr_err = array('d', [0])
        s_te = 0.0
        b_te = 0.0
        s_te_err = array('d', [0])
        b_te_err = array('d', [0])
        ns_tr = 0.0    # ns_tr means the number of signal under certain BDT score in training
        nb_tr = 0.0
        ns_tr_err = array('d', [0])
        nb_tr_err = array('d', [0])
        ns_te = 0.0
        nb_te = 0.0
        ns_te_err = array('d', [0])
        nb_te_err = array('d', [0])

        s_tr = train_S.IntegralAndError(i, train_S.GetNbinsX(), s_tr_err)
        b_tr = train_B.IntegralAndError(i, train_S.GetNbinsX(), b_tr_err)
        s_te = test_S .IntegralAndError(i, train_S.GetNbinsX(), s_te_err)
        b_te = test_B .IntegralAndError(i, train_S.GetNbinsX(), b_te_err)
        ns_tr = train_S.IntegralAndError(0, i-1, ns_tr_err)
        nb_tr = train_B.IntegralAndError(0, i-1, nb_tr_err)
        ns_te = test_S .IntegralAndError(0, i-1, ns_te_err)
        nb_te = test_B .IntegralAndError(0, i-1, nb_te_err)

        h_train.SetPoint     (i-1, s_tr/train_S.Integral(),        1.0 - b_tr/train_B.Integral())
        h_train.SetPointError(i-1, ( ns_tr**2.0 * s_tr_err[0]**2.0 + s_tr**2.0 * ns_tr_err[0]**2.0 )**0.5 / train_S.Integral()**2.0, ( nb_tr**2.0 * b_tr_err[0]**2.0 + b_tr**2.0 * nb_tr_err[0]**2.0)**0.5 / train_B.Integral()**2.0)
        h_test .SetPoint     (i-1, s_te/test_S.Integral(),         1.0 - b_te/test_B.Integral())
        h_test .SetPointError(i-1, ( ns_te**2.0 * s_te_err[0]**2.0 + s_te**2.0 * ns_te_err[0]**2.0 )**0.5 / test_S.Integral()**2.0,  ( nb_te**2.0 * b_te_err[0]**2.0 + b_te**2.0 * nb_te_err[0]**2.0)**0.5 / test_B.Integral()**2.0)

      h_train.SetLineColor(R.kRed)
      h_train.SetMarkerColor(R.kRed)
      h_test. SetLineColor(R.kBlue)
      h_test. SetMarkerColor(R.kBlue)
      h_test.GetXaxis().SetTitle('Sig Eff')
      h_test.GetYaxis().SetTitle('1 - Bkg Eff')

      legend.AddEntry(h_train, 'training', 'LPE')
      legend.AddEntry(h_test,  'testing', 'LPE')
      canv.cd()
      h_test. Draw('ALP')
      h_train.Draw('SAME')

      legend.Draw()
      l_lumi = R.TLatex()
      l_lumi.SetTextSize(0.025)
      l_lumi.SetTextAlign(31)
      l_lumi.DrawLatexNDC(0.90, 0.91, '137.1 fb^{-1} (13 TeV)')
      l_lumi.Draw('SAME')
      l_cms  = R.TLatex()
      l_cms.SetTextSize(0.03)
      l_cms.DrawLatexNDC(0.15, 0.85, '#scale[1.5]{CMS} simulation')
      l_cms.Draw('SAME')

      h_train.Write()
      h_test. Write()
      canv.Write()
      canv.SaveAs('scripts/' + canv.GetName() + '.pdf')

    out_file.Close()
    in_file.Close()

main()	
