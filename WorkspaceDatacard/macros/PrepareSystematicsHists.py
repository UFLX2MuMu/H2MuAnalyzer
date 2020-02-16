#! /usr/bin/env python

########################################################################
##                    Workspace and Datacard Maker                    ##
########################################################################
##  Makes .root file and datacard needed for shape limit setting.     ##
##  Output .root and .txt files to be used with Higgs Combine.        ##
##  Modeled after Viktor Khristenko's code in:                        ##
##  UFDimuAnalysis/python/limit_setting/WorkspaceAndDatacardMaker.py  ##
########################################################################

#============================================
# Imports
#============================================

import os
import string

import ROOT as R



#============================================
# User-defined settings
#============================================

## Configure the script user
if 'abrinke1' in os.getcwd(): USER = 'abrinke1'
if 'bortigno' in os.getcwd(): USER = 'bortigno'
if 'eyigitba' in os.getcwd(): USER = 'eyigitba'
if 'xzuo'     in os.getcwd(): USER = 'xzuo'

if USER =='eyigitba': IN_DIR = '/afs/cern.ch/work/e/eyigitba/public/H2Mu/'
if USER =='xzuo':     IN_DIR = '/afs/cern.ch/work/x/xzuo/public/H2Mu/'
#LABEL  = 'WH_lep_AWB_2019_07_23_signal_sys_v1'
#LABEL  = 'WH_lep_AWB_2019_10_20_BtagSF_sys'
LABEL  = 'ZH_lep_2019_08_25_medLep_veto12_sys'

#CATE   = 'noZ10_noBtag'
CATE   = 'PreselRun2_3lep_hiPt_lep20_medLepMVA_noZ10_noBtag'

SYS_NAMES = ['noSys', 'JES_up', 'JES_down', 'PU_wgt_up', 'PU_wgt_down', 'IsoMu_SF_up', 'IsoMu_SF_down', 'LepMVA_SF_up', 'LepMVA_SF_down', 'B_SF_up', 'B_SF_down']
YEARS = ['2016', '2017', '2018']



def GetWHOneFile(file_name, year, sys_name, hists):
    in_file = R.TFile.Open(file_name, 'READ')

    for hist in in_file.GetListOfKeys():
	hist_name = hist.GetName()
	if '_120' in hist_name or '_130' in hist_name: 	continue
	elif '_H_pair_mass_' not in hist_name: 		continue
	elif 'zoomH' not in hist_name: 			continue

	else:
	  hists.append( in_file.Get(hist_name).Clone() )
	  hists[-1].SetName( hist_name.replace('_H_pair_mass_','_%s_%s_H_pair_mass_' %(year, sys_name)).replace('_125', '').replace('H2Mu_gg_', 'ggH_').replace('H2Mu_','') )
	  hists[-1].SetDirectory(0)
	  print hists[-1].GetName()

    in_file.Close()
## End of function GetWHOneFile(file_name, year, sys_name, hists):

def GetZHOneFile(file_name, year, sys_name, hists):
    print file_name
    file_chain = R.TChain("tree","chain")
    file_chain.Add(file_name)

    BDT_names = ['dimu_mass', 'dimu_mass_BDT_n10_n01', 'dimu_mass_BDT_n01_p10']
    BDT_str = {}
    BDT_str['dimu_mass'] = 'BDT_noMass_v3 >= -1'
    BDT_str['dimu_mass_BDT_n10_n01'] = 'BDT_noMass_v3 < -0.1'
    BDT_str['dimu_mass_BDT_n01_p10'] = 'BDT_noMass_v3 >= -0.1'

    ZH_ID  = "Sample_ID == 2325"
    ttH_ID = "Sample_ID == 60625"

    MVA_str = "mu1_lepMVA > 0.4 && mu2_lepMVA > 0.4 && lep1_lepMVA > 0.4 && lep2_lepMVA > 0.4"
    wgt_str = "(event_wgt * xsec_norm * all_lepMVA_SF * Btag_wgt)"

    for BDT_name in BDT_names:
      file_chain.Draw("dimu_mass >>  ZH(80, 110, 150)", "(%s && %s && %s) * %s" %(ZH_ID,  BDT_str[BDT_name], MVA_str, wgt_str) )
      file_chain.Draw("dimu_mass >> ttH(80, 110, 150)", "(%s && %s && %s) * %s" %(ttH_ID, BDT_str[BDT_name], MVA_str, wgt_str) )

      ZH_temp  = R.gDirectory.Get("ZH")
      ttH_temp = R.gDirectory.Get("ttH")
      ZH_temp. SetNameTitle("ZH_" +year+"_"+sys_name+"_"+BDT_name, "ZH_" +year+"_"+sys_name+"_"+BDT_name)
      ttH_temp.SetNameTitle("ttH_"+year+"_"+sys_name+"_"+BDT_name, "ttH_"+year+"_"+sys_name+"_"+BDT_name)

      hists.append(ZH_temp)
      hists[-1].SetDirectory(0)
      hists.append(ttH_temp)
      hists[-1].SetDirectory(0)


## End of function GetZHOneFile(file_name, year, sys_name, hists):

def main():

    hists = []
    print len(hists)
    for year in YEARS:
      for sys_name in SYS_NAMES:
	if 'WH' in LABEL:
	  in_file_name = IN_DIR + year + '/Histograms/' + LABEL + '/files/HADD/' + 'histos_' + CATE + '_' + sys_name + '.root'
	  GetWHOneFile(in_file_name, year, sys_name, hists)
	elif 'ZH' in LABEL:
	  in_file_name = IN_DIR + year + '/Histograms/' + LABEL + '/files/HADD/' + 'all_' +  sys_name + '.root'
	  GetZHOneFile(in_file_name, year, sys_name, hists)

    out_file_name = IN_DIR + 'Run2' + '/Histograms/' + LABEL + '/' + 'Systematics_Run2.root'
    out_file = R.TFile.Open(out_file_name, 'RECREATE')

    out_file.cd()
    print len(hists)
    for hist in hists:
	print hist.GetName()
	hist.Write()



main()

