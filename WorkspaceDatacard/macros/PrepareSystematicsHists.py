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
#LABEL  = 'ZH_lep_2019_08_25_medLep_veto12_sys'
#LABEL = 'WH_lep_final_ntuple_ele_loose_FSRGeo_2020_02_17'
#LABEL = 'WH_lep_final_BDT_cats_for_bias_2020_03_30'
#LABEL = 'ZH_lep_final_ntuple_ele_loose_FSRGeo_2020_02_17'
#LABEL = 'ZH_lep_SR_CR_final_BDT_2020_03_11'
#LABEL = 'WH_lep_sysJEC_2020_04_29'
#LABEL = 'ZH_lep_sysJEC_2020_05_04'
#LABEL = 'ZH_lep_fullsys_addRoch_2020_05_30'
#LABEL = 'WH_lep_fullsys_add_prefire_2020_05_26_Rochsys'
#LABEL = 'WH_lep_extra_lepMVA_uncert_for_2018_only_2020_07_16'
LABEL = 'ZH_lep_extra_lepMVA_uncert_for_2018_only_2020_07_16'

#CATE   = 'noZ10_noBtag'
#CATE   = 'PreselRun2_3lep_hiPt_lep20_medLepMVA_noZ10_noBtag'
CATE   = 'PreselRun2_3lep_lep20_MissHits1_medLepMVA_SR_Bveto'

#SYS_NAMES = ['noSys', 'JES_up', 'JES_down', 'PU_wgt_up', 'PU_wgt_down', 'IsoMu_SF_up', 'IsoMu_SF_down', 'LepMVA_SF_up', 'LepMVA_SF_down', 'B_SF_up', 'B_SF_down']
SYS_NAMES = ['noSys']
SYS_NAMES += ['PU_wgt_up', 'PU_wgt_down', 'prefiring_2016_up', 'prefiring_2016_down', 'prefiring_2017_up', 'prefiring_2017_down', 'IsoMu_SF_up', 'IsoMu_SF_down',
               'LepMVA_SF_mu_up', 'LepMVA_SF_mu_down', 'LepMVA_SF_ele_up', 'LepMVA_SF_ele_down', 'B_SF_up', 'B_SF_down', 'Roch_up', 'Roch_down']
SYS_NAMES += ['Absolute_up',   'BBEC1_up',   'EC2_up',   'FlavorQCD_up',   'HF_up',   'RelativeBal_up', 
              'Absolute_down', 'BBEC1_down', 'EC2_down', 'FlavorQCD_down', 'HF_down', 'RelativeBal_down',
              'Absolute_2016_up',   'BBEC1_2016_up',   'EC2_2016_up',   'HF_2016_up',   'RelativeSample_2016_up',
              'Absolute_2017_up',   'BBEC1_2017_up',   'EC2_2017_up',   'HF_2017_up',   'RelativeSample_2017_up',
              'Absolute_2018_up',   'BBEC1_2018_up',   'EC2_2018_up',   'HF_2018_up',   'RelativeSample_2018_up',
              'Absolute_2016_down', 'BBEC1_2016_down', 'EC2_2016_down', 'HF_2016_down', 'RelativeSample_2016_down',
              'Absolute_2017_down', 'BBEC1_2017_down', 'EC2_2017_down', 'HF_2017_down', 'RelativeSample_2017_down',
              'Absolute_2018_down', 'BBEC1_2018_down', 'EC2_2018_down', 'HF_2018_down', 'RelativeSample_2018_down']
YEARS = ['2016', '2017', '2018']

MERGE_YEAR = True
MERGE_WH   = True


def GetWHOneFile(file_name, year, sys_name, hists):
    in_file = R.TFile.Open(file_name, 'READ')

    for hist in in_file.GetListOfKeys():
	hist_name = hist.GetName()
	if '_120' in hist_name or '_130' in hist_name: 	continue
	elif '_H_pair_mass_' not in hist_name: 		continue
	elif 'zoomH' not in hist_name: 			continue
        elif 'BDT_' in hist_name and 'BDT_final' not in hist_name:                           continue
        elif 'H2Mu_WH_' not in hist_name and 'H2Mu_ZH_' not in hist_name and 'H2Mu_ttH_' not in hist_name and 'H2Mu_ggZH_' not in hist_name and 'H2Mu_THQ_' not in hist_name and 'H2Mu_THW_' not in hist_name:  continue

	else:
	  if in_file.Get(hist_name).GetEntries() < 50: continue
          h_temp =  in_file.Get(hist_name).Clone()
          h_temp.SetDirectory(0)
          h_name = hist_name.replace('_H_pair_mass_','_%s_%s_H_pair_mass_' %(year, sys_name)).replace('_125', '').replace('H2Mu_ZH_', 'H2Mu_qqZH_').replace('H2Mu_','')
          h_temp.SetName( h_name )
          if MERGE_YEAR:
              if MERGE_WH: h_name = h_name.replace('_pos', '').replace('_neg', '')
              ## decorrelate PU_wgt by different years
              if sys_name == 'noSys':
                  ## put noSys hists for PU shift in other years
                  for dup_sys in ['PU_wgt_2016_up', 'PU_wgt_2016_down', 'PU_wgt_2017_up', 'PU_wgt_2017_down', 'PU_wgt_2018_up', 'PU_wgt_2018_down']:
                    if year in dup_sys: continue
                    dup_h_name = h_name.replace(year+'_noSys', dup_sys)
                    if dup_h_name in hists.keys():
                      hists[dup_h_name].Add(h_temp)
                    else:
                      hists[dup_h_name] = h_temp.Clone(dup_h_name)
                      hists[dup_h_name].SetDirectory(0)
              if 'PU_wgt_' in sys_name:  
                h_name = h_name.replace('%s_PU_wgt'%year, 'PU_wgt_%s'%year)
              ## decorrelate PU_wgt by different years block end
              h_name = h_name.replace('_%s_%s' %(year, sys_name), '_%s'%sys_name)  # be careful when removing year, since year string may also appear in sys_name
              if h_name in hists.keys():
                  hists[h_name].Add(h_temp)
              else:
                  hists[h_name] = h_temp
                  hists[h_name].SetNameTitle(h_name, h_name)
          else:
              hists[h_name] = h_temp

    in_file.Close()
## End of function GetWHOneFile(file_name, year, sys_name, hists):

def GetZHOneFile(file_name, year, sys_name, hists):
    print file_name
    file_chain = R.TChain("tree","chain")
    file_chain.Add(file_name)

    BDT_names = ['dimu_mass', 'dimu_mass_BDT_n10_n01', 'dimu_mass_BDT_n01_p10']
    BDT_str = {}
    BDT_str['dimu_mass'] = 'BDT_final >= -1'
    BDT_str['dimu_mass_BDT_n10_n01'] = 'BDT_final < -0.1'
    BDT_str['dimu_mass_BDT_n01_p10'] = 'BDT_final >= -0.1'

    ZH_ID   = "Sample_ID == 2325"
    ggZH_ID = "Sample_ID == 23250000"

    sel_str = "in_SR == 1 && lep1_missHit<2 && lep2_missHit<2 && dilep_mass > 81 && dilep_mass < 101"
    MVA_str = "mu1_lepMVA > 0.4 && mu2_lepMVA > 0.4 && lep1_lepMVA > 0.4 && lep2_lepMVA > 0.4"
    wgt_str = "(event_wgt * xsec_norm * lepMVA_SF_mu * lepMVA_SF_ele * Btag_wgt)"

    for BDT_name in BDT_names:
      file_chain.Draw("dimu_mass >>   ZH(800, 110, 150)", "(%s && %s && %s && %s) * %s" %(ZH_ID,   BDT_str[BDT_name], MVA_str, sel_str, wgt_str) )
      file_chain.Draw("dimu_mass >> ggZH(800, 110, 150)", "(%s && %s && %s && %s) * %s" %(ggZH_ID, BDT_str[BDT_name], MVA_str, sel_str, wgt_str) )

      ZH_temp   = R.gDirectory.Get("ZH")
      ggZH_temp = R.gDirectory.Get("ggZH")
      h_name = sys_name+"_"+BDT_name
      ZH_temp.   SetNameTitle("qqZH_"+year+"_"+h_name, "qqZH_"+year+"_"+h_name)
      ggZH_temp. SetNameTitle("ggZH_"+year+"_"+h_name, "ggZH_"+year+"_"+h_name)
      ZH_temp.  SetDirectory(0)
      ggZH_temp.SetDirectory(0)

      if MERGE_YEAR:
         ## decorrelate PU_wgt by different years
         if sys_name == 'noSys':
             ## put noSys hists for PU shift in other years
             for dup_sys in ['PU_wgt_2016_up', 'PU_wgt_2016_down', 'PU_wgt_2017_up', 'PU_wgt_2017_down', 'PU_wgt_2018_up', 'PU_wgt_2018_down']:
               if year in dup_sys: continue
               dup_h_name = h_name.replace('noSys', dup_sys)
               if "qqZH_"+dup_h_name in hists.keys():
                 hists["qqZH_"+dup_h_name].Add(ZH_temp)
               else:
                 hists["qqZH_"+dup_h_name] = ZH_temp.Clone("qqZH_"+dup_h_name)
                 hists["qqZH_"+dup_h_name].SetDirectory(0)
               if "ggZH_"+dup_h_name in hists.keys():
                 hists["ggZH_"+dup_h_name].Add(ggZH_temp)
               else:
                 hists["ggZH_"+dup_h_name] = ggZH_temp.Clone("ggZH_"+dup_h_name)
                 hists["ggZH_"+dup_h_name].SetDirectory(0)
         if 'PU_wgt_' in sys_name:
           h_name = h_name.replace('PU_wgt_', 'PU_wgt_%s_'%year)
         ## decorrelate PU_wgt by different years block end
         if "qqZH_"+h_name in hists.keys():   
             hists["qqZH_"+h_name].  Add(ZH_temp)
         else:                              
             hists["qqZH_"+h_name]   =   ZH_temp
             hists["qqZH_"+h_name].SetNameTitle("qqZH_" +h_name, "qqZH_" +h_name)
         if "ggZH_"+h_name in hists.keys():   
             hists["ggZH_"+h_name].Add(ggZH_temp)
         else:                              
             hists["ggZH_"+h_name] =   ggZH_temp
             hists["ggZH_"+h_name].SetNameTitle("ggZH_" +h_name, "ggZH_" +h_name)

      else:
          hists["qqZH_"  +year+"_"+h_name] = ZH_temp
          hists["ggZH_"+year+"_"+h_name] = ggZH_temp


## End of function GetZHOneFile(file_name, year, sys_name, hists):

def main():

    hists = {}
    print len(hists)
    for year in YEARS:
      for sys_name in SYS_NAMES:
	if 'WH' in LABEL:
          label = LABEL
          if year != '2018' or 'LepMVA' not in sys_name: label = 'WH_lep_fullsys_add_prefire_2020_05_26_Rochsys'
	  in_file_name = IN_DIR + year + '/Histograms/' + label + '/files/HADD/' + 'histos_' + CATE + '_' + sys_name + '.root'
	  GetWHOneFile(in_file_name, year, sys_name, hists)
	elif 'ZH' in LABEL:
          label = LABEL
          if year != '2018' or 'LepMVA' not in sys_name: label = 'ZH_lep_fullsys_addRoch_2020_05_30'
	  in_file_name = IN_DIR + year + '/Histograms/' + label + '/files/HADD/' + 'all_' +  sys_name + '.root'
	  GetZHOneFile(in_file_name, year, sys_name, hists)

    merge_str = ''
    if MERGE_YEAR: merge_str = '_merged'
    out_file_name = IN_DIR + 'Run2' + '/Histograms/' + LABEL + '/' + 'Systematics_Run2'+merge_str+'.root'
    out_file = R.TFile.Open(out_file_name, 'RECREATE')

    out_file.cd()
    print len(hists)
    for key in hists.keys():
	print key
	hists[key].Write()



main()

