
import os
import sys
import array

from ROOT import *

gROOT.SetBatch(True)

COMPARE_YEAR = True

## ---- hardcode for id comparisons
#YEAR_ERA = '2016BCDEF'
YEAR_ERA = '2016GH'
#YEAR_ERA = '2017'
#YEAR_ERA = '2018'

YEAR_tZq = '2016'
if YEAR_ERA == '2017' or YEAR_ERA == '2018': YEAR_tZq = '2017'

POG_ISO = 'mu_2016_legacy_rereco_RunBCDEF_SF_ISO_syst.root'
POG_ID  = 'mu_2016_legacy_rereco_RunBCDEF_SF_ID_syst.root'
LEP_MVA = 'scaleFactors_%s_mu.root' %YEAR_tZq

HIST_ISO = 'NUM_LooseRelIso_DEN_MediumID_eta_pt'
HIST_ID  = 'NUM_MediumID_DEN_genTracks_eta_pt'
POG_PtAxis = 'Y'

if YEAR_ERA == '2016GH':
  POG_ISO = 'mu_2016_legacy_rereco_RunGH_SF_ISO_syst.root'
  POG_ID  = 'mu_2016_legacy_rereco_RunGH_SF_ID_syst.root'

elif YEAR_ERA == '2017': 
  POG_ISO  = 'mu_2017_RunBCDEF_SF_ISO_syst.root'
  POG_ID   = 'mu_2017_RunBCDEF_SF_ID_syst.root'
  HIST_ISO = 'NUM_LooseRelIso_DEN_MediumID_pt_abseta'
  HIST_ID  = 'NUM_MediumID_DEN_genTracks_pt_abseta'
  POG_PtAxis = 'X'

elif YEAR_ERA == '2018':
  POG_ISO  = 'mu_2018_RunABCD_SF_ISO_syst.root'
  POG_ID   = 'mu_2018_RunABCD_SF_ID_syst.root'
  HIST_ISO = 'NUM_LooseRelIso_DEN_MediumID_pt_abseta'
  HIST_ID  = 'NUM_MediumID_DEN_TrackerMuons_pt_abseta'
  POG_PtAxis = 'X'


## ---- hardcode for year comparisons
YEARS_MU = ['2016BCDEF', '2016GH', '2017', '2018']
YEARS_ELE = ['2016', '2017', '2018']
MU_ID = {}
MU_ISO = {}
ELE_ID = {}
MU_H_ID = {}
MU_H_ISO = {}
ELE_H_ID = {}
MU_Pt_Axis = {}
ELE_Pt_Axis = {}

MU_ID['2016BCDEF'] = 'mu_2016_legacy_rereco_RunBCDEF_SF_ID_syst.root'
MU_ID['2016GH']    = 'mu_2016_legacy_rereco_RunGH_SF_ID_syst.root'
MU_ID['2017']      = 'mu_2017_RunBCDEF_SF_ID_syst.root'
MU_ID['2018']      = 'mu_2018_RunABCD_SF_ID_syst.root'

MU_H_ID['2016BCDEF'] = 'NUM_MediumID_DEN_genTracks_eta_pt'
MU_H_ID['2016GH']    = 'NUM_MediumID_DEN_genTracks_eta_pt'
MU_H_ID['2017']      = 'NUM_MediumID_DEN_genTracks_pt_abseta'
MU_H_ID['2018']      = 'NUM_MediumID_DEN_TrackerMuons_pt_abseta'

MU_ISO['2016BCDEF'] = 'mu_2016_legacy_rereco_RunBCDEF_SF_ISO_syst.root'
MU_ISO['2016GH']    = 'mu_2016_legacy_rereco_RunGH_SF_ISO_syst.root'
MU_ISO['2017']      = 'mu_2017_RunBCDEF_SF_ISO_syst.root'
MU_ISO['2018']      = 'mu_2018_RunABCD_SF_ISO_syst.root'

MU_H_ISO['2016BCDEF'] = 'NUM_LooseRelIso_DEN_MediumID_eta_pt'
MU_H_ISO['2016GH']    = 'NUM_LooseRelIso_DEN_MediumID_eta_pt'
MU_H_ISO['2017']      = 'NUM_LooseRelIso_DEN_MediumID_pt_abseta'
MU_H_ISO['2018']      = 'NUM_LooseRelIso_DEN_MediumID_pt_abseta'

MU_Pt_Axis['2016BCDEF'] = 'Y'
MU_Pt_Axis['2016GH']    = 'Y'
MU_Pt_Axis['2017']      = 'X'
MU_Pt_Axis['2018']      = 'X'


ELE_ID['2016'] = '2016LegacyReReco_ElectronMVA90_Fall17V2.root'
ELE_ID['2017'] = '2017_ElectronMVA90.root '
ELE_ID['2018'] = '2018_ElectronMVA90.root '

ELE_H_ID['2016'] = 'EGamma_SF2D'
ELE_H_ID['2017'] = 'EGamma_SF2D'
ELE_H_ID['2018'] = 'EGamma_SF2D'

ELE_Pt_Axis['2016'] = 'Y'
ELE_Pt_Axis['2017'] = 'Y'
ELE_Pt_Axis['2018'] = 'Y'





def MultiplySFs(pog_id, pog_iso, eta_bin):
  pog_sf = pog_id.Clone('pog_'+eta_bin)
  print pog_sf.GetNbinsX()
  for i in range(1, pog_sf.GetNbinsX()+1):
    pog_sf.SetBinContent( i, pog_id.GetBinContent(i) * pog_iso.GetBinContent(i) )
    pog_sf.SetBinError  ( i, pog_id.GetBinError(i)   + pog_iso.GetBinError(i)   )

  return pog_sf

def MergeEtaBins(sf_hist, sf_type, pt_axis, eta_min, eta_max):
  hist_merge = None
  n_etabin   = 0
  if pt_axis == 'X':
    hist_merge = sf_hist.ProjectionX(sf_type + ('_%.3f_%.3f'%(eta_min, eta_max)).replace('.','p'), 1,1, 'e')
    n_etabin = sf_hist.GetNbinsY()
  elif pt_axis == 'Y':
    hist_merge = sf_hist.ProjectionY(sf_type + ('_%.3f_%.3f'%(eta_min, eta_max)).replace('.','p'), 1,1, 'e')
    n_etabin = sf_hist.GetNbinsX()
  else: 
    print 'wrong option of pt_axis. Exiting'
    exit()

  print '\n\nhave %d eta bins' %n_etabin
  #loop through pt bins
  print eta_min, eta_max
  print hist_merge.GetNbinsX()
  for i in range(1, hist_merge.GetNbinsX()+1):
    sf_values  = []
    sf_weights = []
    #loop through eta bins
    for j in range(1, n_etabin+1):
      if pt_axis == 'X':
        bin_low  = sf_hist.GetYaxis().GetBinLowEdge(j)
        bin_high = sf_hist.GetYaxis().GetBinUpEdge(j)
        if bin_low == -1.566 and bin_high == -1.444: continue
        if bin_low ==  1.444 and bin_high ==  1.566: continue
        if abs(bin_low) >= eta_max and abs(bin_high) >= eta_max: continue
        if abs(bin_low) <= eta_min and abs(bin_high) <= eta_min: continue
        sf_values. append( sf_hist.GetBinContent(i, j) )
        sf_weights.append( 1.0 / sf_hist.GetBinError(i, j)**2.0 )
      elif pt_axis == 'Y':
        bin_low  = sf_hist.GetXaxis().GetBinLowEdge(j)
        bin_high = sf_hist.GetXaxis().GetBinUpEdge(j)
        if bin_low == -1.566 and bin_high == -1.444: continue
        if bin_low ==  1.444 and bin_high ==  1.566: continue
        if abs(bin_low) >= eta_max and abs(bin_high) >= eta_max: continue
        if abs(bin_low) <= eta_min and abs(bin_high) <= eta_min: continue
        sf_values. append( sf_hist.GetBinContent(j, i) )
        sf_weights.append( 1.0 / sf_hist.GetBinError(j, i)**2.0 )

    print 'merging %d eta bins' %len(sf_values)
    sum_value = 0
    sum_weight = 0
    for k in range( len(sf_values) ):
      sum_value  += sf_values[k] * sf_weights[k]
      sum_weight += sf_weights[k]
    hist_merge.SetBinContent( i, sum_value / sum_weight )
    hist_merge.SetBinError  ( i, 1.0 / sum_weight**0.5 )

  return hist_merge
## End of MergeEtaBins()


def DrawOverlayID(graph_pog, graph_base, graph_mva, eta_bin):
#  eta_bin = 'eta in '

  canv = TCanvas('overlay_'+eta_bin, 'overlay', 600, 600)
  graph_pog. SetTitle('') 
  graph_base.SetTitle('')
  graph_mva. SetTitle('')
  graph_pog. SetLineColor(kRed)
  graph_base.SetLineColor(kBlack)
  graph_mva. SetLineColor(kBlue)
  graph_pog. SetMarkerColor(kRed)
  graph_base.SetMarkerColor(kBlack)
  graph_mva. SetMarkerColor(kBlue)
  graph_pog. SetMarkerStyle(20)
  graph_base.SetMarkerStyle(20)
  graph_mva. SetMarkerStyle(20)

  legend = TLegend(0.3, 0.15, 0.85, 0.4)
  legend.SetHeader(eta_bin, 'C')
  legend.AddEntry(graph_pog,  YEAR_ERA + ' POG MediumID x LooseRelIso', 'LPE')
  legend.AddEntry(graph_base, YEAR_tZq + ' tZq baseline', 'LPE')
  legend.AddEntry(graph_mva,  YEAR_tZq + ' tZq lepMVA>0.4', 'LPE')

  canv.cd()
  graph_base.GetXaxis().SetRangeUser(20, 200)
  graph_base.GetXaxis().SetTitleSize(0.045)
  graph_base.GetXaxis().SetTitleOffset(1.1)
  graph_base.GetYaxis().SetRangeUser(0.9, 1.05)
  graph_base.SetYTitle('SF')
  graph_base.GetYaxis().SetTitleSize(0.045)
  graph_base.GetYaxis().SetTitleOffset(1.05)
  graph_base.Draw('PE][')
  graph_mva.Draw('samePE][')
  graph_pog.Draw('samePE][')
  legend.Draw('same')

  canv.Update()
  canv.Write()
  pic_name = YEAR_ERA + '_' + eta_bin.replace('.','p').replace('<','_').replace('|','').replace('#','')
  canv.SaveAs('out_pics/' + pic_name+'.png')
  canv.SaveAs('out_pics/' + pic_name+'.pdf')


def DrawOverlayYear(graph_16, graph_16gh, graph_17, graph_18, id_name, eta_bin):
#  eta_bin = 'eta in '

  canv = TCanvas('overlay_'+eta_bin, 'overlay', 600, 600)
  graph_16.SetTitle('') 
  graph_17.SetTitle('')
  graph_18.SetTitle('')
  graph_16.SetLineColor(kRed)
  graph_17.SetLineColor(kBlack)
  graph_18.SetLineColor(kBlue)
  graph_16.SetMarkerColor(kRed)
  graph_17.SetMarkerColor(kBlack)
  graph_18.SetMarkerColor(kBlue)
  graph_16.SetMarkerStyle(20)
  graph_17.SetMarkerStyle(20)
  graph_18.SetMarkerStyle(20)
  if graph_16gh != None:
    graph_16gh.SetTitle('')
    graph_16gh.SetLineColor(kOrange+2)
    graph_16gh.SetMarkerColor(kOrange+2)
    graph_16gh.SetMarkerStyle(20)

  legend = TLegend(0.3, 0.15, 0.85, 0.4)
  legend.SetHeader(eta_bin, 'C')
  if graph_16gh == None: legend.AddEntry(graph_16, '2016 ' + id_name, 'LPE')
  else:
    legend.AddEntry(graph_16, '2016BCDEF ' + id_name, 'LPE')
    legend.AddEntry(graph_16gh, '2016GH '  + id_name, 'LPE')
  legend.AddEntry(graph_17, '2017 ' + id_name, 'LPE')
  legend.AddEntry(graph_18, '2018 ' + id_name, 'LPE')

  canv.cd()
  graph_16.SetXTitle('P_{T}')
  graph_16.GetXaxis().SetRangeUser(20, 200)
  graph_16.GetXaxis().SetTitleSize(0.045)
  graph_16.GetXaxis().SetTitleOffset(1.1)
  graph_16.GetYaxis().SetRangeUser(0.9, 1.05)
  graph_16.SetYTitle('SF')
  graph_16.GetYaxis().SetTitleSize(0.045)
  graph_16.GetYaxis().SetTitleOffset(1.05)
  graph_16.Draw('PE][')
  if graph_16gh != None: graph_16gh.Draw('samePE][')
  graph_17.Draw('samePE][')
  graph_18.Draw('samePE][')
  legend.Draw('same')

  canv.Update()
  canv.Write()
  pic_name = id_name + '_' + eta_bin.replace('.','p').replace('<','_').replace('|','').replace('#','')
  canv.SaveAs('out_pics/' + pic_name+'.png')
  canv.SaveAs('out_pics/' + pic_name+'.pdf')


def main_id():

    in_file_ISO  = TFile.Open( 'data/' + POG_ISO, "READ")
    in_file_ID   = TFile.Open( 'data/' + POG_ID,  "READ")
    in_file_LEP  = TFile.Open( 'data/' + LEP_MVA, "READ")

    out_file = TFile.Open('%s_out.root'%YEAR_ERA, 'RECREATE')

    gStyle.SetOptStat(0000)
    gStyle.SetLegendBorderSize(0)
    gStyle.SetPadTickX(1)
    gStyle.SetPadTickY(1) 

    SF2D_id       = in_file_ID. Get(HIST_ID)
    SF2D_iso      = in_file_ISO.Get(HIST_ISO)
    SF2D_tZq_base = in_file_LEP.Get('MuonToTTVLoose')
    SF2D_MVA_Med  = in_file_LEP.Get('MuonToTTVLeptonMvattZ3l')

    eta_binning = [0.0, 0.9, 1.2, 2.1, 2.4]

    for i in range(1, len(eta_binning)+1):
      if i < len(eta_binning):  # for individual eta bins
        eta_min = eta_binning[i-1]
        eta_max = eta_binning[i]
      else:        # for eta average
        eta_min = eta_binning[0]
        eta_max = eta_binning[i-1]
      eta_bin = '%.2f<|#eta|<%.2f' %(eta_min, eta_max)

      SF_id       = MergeEtaBins(SF2D_id,       'id',   POG_PtAxis, eta_min, eta_max)
      SF_iso      = MergeEtaBins(SF2D_iso,      'iso',  POG_PtAxis, eta_min, eta_max) 
      SF_tZq_base = MergeEtaBins(SF2D_tZq_base, 'base', 'X',        eta_min, eta_max)
      SF_MVA_Med  = MergeEtaBins(SF2D_MVA_Med,  'med',  'X',        eta_min, eta_max)

      SF_pog = MultiplySFs(SF_id, SF_iso, eta_bin)

      out_file.cd()
      SF_id.Write()
      SF_iso.Write()
      SF_tZq_base.Write()     
      SF_MVA_Med.Write()
      SF_pog.Write()

      DrawOverlayID(SF_pog, SF_tZq_base, SF_MVA_Med, eta_bin)

    out_file.Close()

def main_year():
    LEP = 'ele'
    out_file = TFile.Open('%s_out.root'%LEP, 'RECREATE')

    gStyle.SetOptStat(0000)
    gStyle.SetLegendBorderSize(0)
    gStyle.SetPadTickX(1)
    gStyle.SetPadTickY(1)

    if LEP == 'mu':    
      id_name = 'muon MediumID x LooseRelIso'

      iso_16   = TFile.Open( 'data/' + MU_ISO['2016BCDEF'], "READ")
      iso_16gh = TFile.Open( 'data/' + MU_ISO['2016GH'], "READ")
      iso_17   = TFile.Open( 'data/' + MU_ISO['2017'], "READ")
      iso_18   = TFile.Open( 'data/' + MU_ISO['2018'], "READ")
      id_16   =  TFile.Open( 'data/' + MU_ID['2016BCDEF'], "READ")
      id_16gh =  TFile.Open( 'data/' + MU_ID['2016GH'], "READ")
      id_17   =  TFile.Open( 'data/' + MU_ID['2017'], "READ")
      id_18   =  TFile.Open( 'data/' + MU_ID['2018'], "READ")

      sf2d_iso_16   = iso_16.  Get(MU_H_ISO['2016BCDEF'])
      sf2d_iso_16gh = iso_16gh.Get(MU_H_ISO['2016GH'])
      sf2d_iso_17   = iso_17.  Get(MU_H_ISO['2017'])
      sf2d_iso_18   = iso_18.  Get(MU_H_ISO['2018'])
      sf2d_id_16    = id_16.   Get(MU_H_ID['2016BCDEF'])
      sf2d_id_16gh  = id_16gh. Get(MU_H_ID['2016GH'])
      sf2d_id_17    = id_17.   Get(MU_H_ID['2017'])
      sf2d_id_18    = id_18.   Get(MU_H_ID['2018'])

      eta_binning = [0.0, 0.9, 1.2, 2.1, 2.4]
      for i in range(1, len(eta_binning)+1):
        if i < len(eta_binning):  # for individual eta bins
          eta_min = eta_binning[i-1]
          eta_max = eta_binning[i]
        else:        # for eta average
          eta_min = eta_binning[0]
          eta_max = eta_binning[i-1]
        eta_bin = '%.2f<|#eta|<%.2f' %(eta_min, eta_max)

        sf_iso_16   = MergeEtaBins(sf2d_iso_16,   'iso_2016BCDEF', MU_Pt_Axis['2016BCDEF'], eta_min, eta_max)
        sf_iso_16gh = MergeEtaBins(sf2d_iso_16gh, 'iso_2016GH',    MU_Pt_Axis['2016GH'],    eta_min, eta_max)
        sf_iso_17   = MergeEtaBins(sf2d_iso_17,   'iso_2017',      MU_Pt_Axis['2017'],      eta_min, eta_max)
        sf_iso_18   = MergeEtaBins(sf2d_iso_18,   'iso_2018',      MU_Pt_Axis['2018'],      eta_min, eta_max)
        sf_id_16    = MergeEtaBins(sf2d_id_16,    'id_2016BCDEF',  MU_Pt_Axis['2016BCDEF'], eta_min, eta_max)
        sf_id_16gh  = MergeEtaBins(sf2d_id_16gh,  'id_2016GH',     MU_Pt_Axis['2016GH'],    eta_min, eta_max)
        sf_id_17    = MergeEtaBins(sf2d_id_17,    'id_2017',       MU_Pt_Axis['2017'],      eta_min, eta_max)
        sf_id_18    = MergeEtaBins(sf2d_id_18,    'id_2018',       MU_Pt_Axis['2018'],      eta_min, eta_max)

        sf_16 = MultiplySFs(sf_id_16, sf_iso_16, eta_bin)
        sf_16gh = MultiplySFs(sf_id_16gh, sf_iso_16gh, eta_bin)
        sf_17 = MultiplySFs(sf_id_17, sf_iso_17, eta_bin)
        sf_18 = MultiplySFs(sf_id_18, sf_iso_18, eta_bin)
         
        out_file.cd()
        DrawOverlayYear(sf_16, sf_16gh, sf_17, sf_18, id_name, eta_bin)

    elif LEP == 'ele': 
      id_name = 'electron MVA wp90'
      
      id_16   =  TFile.Open( 'data/' + ELE_ID['2016'], "READ")
      id_17   =  TFile.Open( 'data/' + ELE_ID['2017'], "READ")
      id_18   =  TFile.Open( 'data/' + ELE_ID['2018'], "READ")

      sf2d_id_16    = id_16.   Get(ELE_H_ID['2016'])
      sf2d_id_17    = id_17.   Get(ELE_H_ID['2017'])
      sf2d_id_18    = id_18.   Get(ELE_H_ID['2018'])

      eta_binning = [0.0, 0.8, 1.444, 1.566, 2.0, 2.5]
      for i in range(1, len(eta_binning)+1):
        if i < len(eta_binning):  # for individual eta bins
          eta_min = eta_binning[i-1]
          eta_max = eta_binning[i]
        else:        # for eta average
          eta_min = eta_binning[0]
          eta_max = eta_binning[i-1]
        eta_bin = '%.3f<|#eta|<%.3f' %(eta_min, eta_max)
        if eta_min == 1.444 and eta_max == 1.566: continue  #skip the gap      
 
        sf_id_16    = MergeEtaBins(sf2d_id_16,    'id_2016',  ELE_Pt_Axis['2016'], eta_min, eta_max)
        sf_id_17    = MergeEtaBins(sf2d_id_17,    'id_2017',  ELE_Pt_Axis['2017'], eta_min, eta_max)
        sf_id_18    = MergeEtaBins(sf2d_id_18,    'id_2018',  ELE_Pt_Axis['2018'], eta_min, eta_max)

        out_file.cd()
        DrawOverlayYear(sf_id_16, None, sf_id_17, sf_id_18, id_name, eta_bin)


    out_file.Close()


if COMPARE_YEAR: main_year()
else:            main_id()
