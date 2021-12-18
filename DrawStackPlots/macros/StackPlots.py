#! /usr/bin/env python

####################################################
###    Make stack and ratio from added histos    ###
####################################################

## N.B. For some reason frequently segfaults after plotting ~20 stacks - AWB 21.01.2019
## Can run as ./macros/StackPlots.py i j to plots stacks with indices [i, j]
## Script prints index of each stack so you can see the last one which succeded

## Basic python includes
import os
import sys
import math
import copy
from shutil import copyfile

## ROOT includes
import ROOT as R
R.gROOT.SetBatch(True)  ## Don't display canvases to screen while running
R.gStyle.SetOptStat(0)

## Info about data and MC NTuples from H2MuAnalyzer/MakeHistos/python/SampleDatabase.py
sys.path.insert(0, '%s/../MakeHistos/python' % os.getcwd())
from SampleDatabase import GetSamples
## Configuration for stack plots from H2MuAnalyzer/DrawStackPlots/python/StackPlotConfig.py
sys.path.insert(0, '%s/python' % os.getcwd())
from StackPlotConfig import ConfigStackPlot

## Configure the script user
if 'abrinke1' in os.getcwd(): USER = 'abrinke1'
if 'bortigno' in os.getcwd(): USER = 'bortigno'
if 'eyigitba' in os.getcwd(): USER = 'eyigitba'
if 'xzuo'     in os.getcwd(): USER = 'xzuo'

## Settings for this stack-drawing job
if USER == 'abrinke1':
    YEAR     = 'Run2'  ## Dataset year (2016, 2017, 2018, or Run2)
    PLOT_DIR = '/afs/cern.ch/work/a/abrinke1/public/H2Mu/%s/Histograms' % YEAR

    # CONFIG   = 'WH_lep'   ## Pre-defined stack configuration from python/StackPlotConfig.py
    # LABEL    = 'WH_lep_AWB_2019_07_08_signal_v1'  ## Sub-folder within PLOT_DIR containing histograms
    # CATEGORY = '3lep_medLepMVA_noZ10_noBtag'  ## Category for which to draw plots

    CONFIG   = 'ttH_3l'   ## Pre-defined stack configuration from python/StackPlotConfig.py
    LABEL    = 'ttH_3l_AWB_2019_07_12_signal_v1'  ## Sub-folder within PLOT_DIR containing histograms
    CATEGORY = '3lep_medLepMVA_noZ10_ge2j_btag'  ## Category for which to draw plots

    IN_FILE  = 'histos_PreselRun2_%s_merged.root' % CATEGORY  ## File with input histograms

    SCALE     = 'lin' ## 'log' or 'lin' scaling of y-axis
    RATIO_MIN = 0.0   ## Minimum value in ratio plot
    RATIO_MAX = 2.0   ## Maximum value in ratio plot

elif USER == 'eyigitba':
    YEAR = '2018'
#    YEAR = 'Run2'
    PLOT_DIR = '/afs/cern.ch/work/e/eyigitba/public/H2Mu/%s/Histograms' %YEAR

#    CONFIG   = '2l'
#    LABEL    = 'data_MC_2019_11_06_GeoFitBS'
#    CATEGORY = 'NONE'

    CONFIG   = 'WH_lep'
    CONFIG   = 'WH_lep_allMass'
    LABEL    = 'WH_lep_AWB_2019_10_20_BtagSF'
    CATEGORY = '3lep_hiPt_lep20_medLepMVA_noZ10_noBtag_noSys'

    IN_FILE  = 'histos_PreselRun2_%s_merged.root' % CATEGORY
#    IN_FILE  = 'all_NONE_%s.root' %CATEGORY
    SCALE    = 'lin' ## 'log' or 'lin' scaling of y-axis
    RATIO_MIN = 0.0   ## Minimum value in ratio plot
    RATIO_MAX = 2.0   ## Maximum value in ratio plot


elif USER == 'xzuo':
    YEAR = '2016'
#    YEAR = 'Run2'
    PLOT_DIR = '/afs/cern.ch/work/x/xzuo/public/H2Mu/%s/Histograms' %YEAR

#    CONFIG   = '2l'
#    LABEL    = 'data_MC_2019_11_06_GeoFitBS'
#    CATEGORY = 'NONE'

    CONFIG   = 'WH_lep'
#    CONFIG   = 'WH_lep_allMass'
#    LABEL    = 'WH_lep_final_BDT_cats_for_bias_2020_03_30'
#    LABEL    = 'WH_lep_preApp_final_SR_CR_missHit_2020_05_09'
    LABEL    = 'WH_lep_add_MHT_JES_shift_for_ARC_2020_07_04'
#    CATEGORY = 'e2mu_lep20_ConVetoAll_medLepMVA_SR_Bveto_noSys'
    CATEGORY = '3lep_lep20_MissHits1_medLepMVA_SR_Bveto_noSys'

    IN_FILE  = 'histos_PreselRun2_%s_merged.root' % CATEGORY
#    IN_FILE  = 'all_NONE_%s.root' %CATEGORY
    SCALE    = 'lin' ## 'log' or 'lin' scaling of y-axis
    RATIO_MIN = 0.0   ## Minimum value in ratio plot
    RATIO_MAX = 2.0   ## Maximum value in ratio plot

elif USER == 'bortigno':
    PLOT_DIR = 'NONE'
else: print 'Invalid USER = %s' % USER

APPLY_KFactor = True
DRAW_JES_sys = False
if LABEL == '': DRAW_JES_sys = True

LUMI = 137.2
if YEAR == '2016': LUMI = 35.9
if YEAR == '2017': LUMI = 41.5
if YEAR == '2018': LUMI = 59.7

## Configure year, category, label, and config from command line
## ./macros/StackPlots.py i j YEAR CATEGORY LABEL CONFIG
if len(sys.argv) > 3:
    OLD_YEAR = YEAR
    YEAR = str(sys.argv[3])
    print '\nYEAR changed from %s to %s' % (OLD_YEAR, YEAR)
    PLOT_DIR = PLOT_DIR.replace(OLD_YEAR, YEAR)
if len(sys.argv) > 4:
    OLD_CATEGORY = CATEGORY
    CATEGORY = str(sys.argv[4])
    print '\nCATEGORY changed from %s to %s' % (OLD_CATEGORY, CATEGORY)
    IN_FILE = IN_FILE.replace(OLD_CATEGORY, CATEGORY)
if len(sys.argv) > 5:
    OLD_LABEL = LABEL
    LABEL = str(sys.argv[5])
    print '\nLABEL changed from %s to %s' % (OLD_LABEL, LABEL)
if len(sys.argv) > 6:
    OLD_CONFIG = CONFIG
    CONFIG = str(sys.argv[6])
    print '\nCONFIG changed from %s to %s' % (OLD_CONFIG, CONFIG)



def GetDistSysRatio(in_file, dist, groups, all_stack):

  if dist == 'MHT': 
    dist_up = 'MHT_JES_up'
    dist_dn = 'MHT_JES_down'
  elif dist == 'muSS_MHT_MT':
    dist_up = 'muSS_MHT_JES_up_MT'
    dist_dn = 'muSS_MHT_JES_down_MT'
  elif dist == 'lep_MHT_MT':
    dist_up = 'lep_MHT_JES_up_MT'
    dist_dn = 'lep_MHT_JES_down_MT'

  elif dist == 'MET':
    dist_up = 'MET_JES_up'
    dist_dn = 'MET_JES_down'
  elif dist == 'muSS_MET_MT':
    dist_up = 'muSS_MET_JES_up_MT'
    dist_dn = 'muSS_MET_JES_down_MT'
  elif dist == 'lep_MET_MT':
    dist_up = 'lep_MET_JES_up_MT'
    dist_dn = 'lep_MET_JES_down_MT'


  cfg = ConfigStackPlot(CONFIG, YEAR)

  stack_up = R.THStack('stack_up_'+dist, dist+'_up background')
  stack_dn = R.THStack('stack_dn_'+dist, dist+'_dn background')

  for group, samps in groups['Bkg'].items():
    for samp in samps:
        try:
            hist_up = in_file.Get(samp+'_'+dist_up).Clone('tmp_up')
            hist_dn = in_file.Get(samp+'_'+dist_dn).Clone('tmp_dn')

## no need for empty bin uncertainty here
#            ## Set uncertainty for empty bins to Integral/(nEffectuveEntries*sqrt(nEmptyBins))
#            nEmptyBins = 0
#            for iBin in range(1, hist.GetNbinsX()+1):
#                if hist.GetBinContent(iBin) == 0: nEmptyBins += 1
#            if nEmptyBins > 0:
#                empty_uncert = abs(hist.Integral() / (hist.GetEffectiveEntries() * math.sqrt(nEmptyBins)))
#                for jBin in range(1, hist.GetNbinsX()+1):
#                    hist.SetBinError(jBin, max(abs(hist.GetBinError(jBin)), empty_uncert))

            ## Scale samples for which there is more than on MC dataset
            if samp in cfg.weight.keys():
                print 'Scaling sample %s by %.2f' % (samp, cfg.weight[samp])
                hist_up.Scale(cfg.weight[samp])
                hist_dn.Scale(cfg.weight[samp])

            ## apply k-factor to WZ_3l sample
            if samp == 'WZ_3l' and APPLY_KFactor:
                if CONFIG  == 'WH_lep_allMass': 
                  hist_up.Scale(1.127)
                  hist_dn.Scale(1.127)
                else:                           
                  hist_up.Scale(1.102)
                  hist_dn.Scale(1.102)

            stack_up.Add(hist_up.Clone('hist_'+dist_up+'_'+samp))
            stack_dn.Add(hist_dn.Clone('hist_'+dist_dn+'_'+samp))

            del hist_up
            del hist_dn
        except:
            print '  - Getting sys hists. Could not find histogram '+samp+'_'+dist+'up/down'


  ratio_up = stack_up.GetStack().Last()
  ratio_dn = stack_dn.GetStack().Last()
  ratio_up.SetNameTitle('ratio_'+dist_up, '')
  ratio_dn.SetNameTitle('ratio_'+dist_dn, '')
  ratio_up.Divide(all_stack.GetStack().Last())
  ratio_dn.Divide(all_stack.GetStack().Last())

  h_stat_err_band = all_stack.GetStack().Last().Clone('stat_err_band_'+dist)
  h_stat_err_band.Divide(all_stack.GetStack().Last())
  stat_err_band = R.TGraphAsymmErrors(h_stat_err_band)      ## might have problem if there are empty bins in the histo 
  sys_err_band = stat_err_band.Clone('sys_err_band_'+dist)
  jec_err_band = stat_err_band.Clone('jec_err_band_'+dist)
#  stat_err_band.SetDirectory(0)
#  sys_err_band.SetDirectory(0)

  for ibin in range(1, h_stat_err_band.GetNbinsX()+1):
    stat_err = h_stat_err_band.GetBinError(ibin)
    sys_up   = max( ratio_up.GetBinContent(ibin) - 1, ratio_dn.GetBinContent(ibin) - 1, 0)
    sys_dn   = max( 1 - ratio_up.GetBinContent(ibin), 1 - ratio_dn.GetBinContent(ibin), 0)

    err_up = math.sqrt(stat_err**2.0 + sys_up**2.0)
    err_dn = math.sqrt(stat_err**2.0 + sys_dn**2.0)

    sys_err_band.SetPointEYhigh(ibin, err_up)
    sys_err_band.SetPointEYlow (ibin, err_dn)

    jec_err_band.SetPointEYhigh(ibin, sys_up)
    jec_err_band.SetPointEYlow (ibin, sys_dn)

  del stack_up, stack_dn

  return stat_err_band, sys_err_band, jec_err_band
  ## delete objects created by GetDistSysRatio()

## End function GetDistSysRatio()


## Function to draw each stack plot and ratio plot on the same canvas
## Add in_file as input to enable MC sys band in ratio plot 
def DrawOneStack( in_file, dist, sig_stack, all_stack, groups, group_hist, legend, out_file_name ):   # Do not use TRatioPlot! It is a devil! - XWZ 19.09.2018
    h_data = group_hist['Dat']

    ## Create a new TCanvas
    canv = R.TCanvas('can_'+dist, 'can_'+dist, 600, 600)
    canv.Clear()
    canv.cd()
    
    ## Draw the upper pad, with the stack histogram
    upper_pad = R.TPad('upperPad_'+dist, 'upperPad_'+dist, 0, 0.25, 1, 1)
    upper_pad.SetBottomMargin(0.04);
    #upper_pad.SetRightMargin(0.20)
    upper_pad.Draw()
    upper_pad.cd()

    ## Draw signal + background stack
    if SCALE == 'log':
        upper_pad.SetLogy()
        all_stack.SetMinimum( 0.01 )
        if h_data: all_stack.SetMaximum( 10.0*max(h_data.GetMaximum(), all_stack.GetMaximum()) )
        else:      all_stack.SetMaximum( 10.0*all_stack.GetMaximum() )
    elif SCALE == 'lin':
        all_stack.SetMinimum(0)
        if h_data: all_stack.SetMaximum( 1.5*max(h_data.GetMaximum(), all_stack.GetMaximum()) )
        else:      all_stack.SetMaximum( 1.5*all_stack.GetMaximum() )
#	if dist == 'H_pair_mass_zoomZ': all_stack.SetMaximum(250)
#        if dist == 'lep_pt': all_stack.SetMaximum(100)
    else: print 'Invalid SCALE = %s. Exiting.' % SCALE, sys.exit()
        
    all_stack.Draw('HIST')
    all_stack.SetTitle('')
    all_stack.GetXaxis().SetLabelSize(0)
    all_stack.GetYaxis().SetLabelSize(0.05)
    all_stack.GetYaxis().SetTitle('Events / Bin')
    all_stack.GetYaxis().SetTitleSize(0.05)
    all_stack.GetYaxis().SetTitleOffset(1.0)

    ## Draw the data histogram
    if h_data:
        ## Blind 120 - 130 GeV range of mass plots
#        if 'H_pair_mass_window' in h_data.GetName():
#            for i in range(1, h_data.GetNbinsX()+1):
#                if h_data.GetXaxis().GetBinCenter(i) > 120 and h_data.GetXaxis().GetBinCenter(i) < 130:
#                    h_data.SetBinContent(i, 0)
#                    h_data.SetBinError(i, 0)
        ## Blind high-score part of BDT with mass
        if 'BDT' in h_data.GetName() and ('withMass' in h_data.GetName() or 'retrain' in h_data.GetName() or 'combo' in h_data.GetName()):
            for i in range(1, h_data.GetNbinsX()+1):
                if h_data.GetXaxis().GetBinCenter(i) > 0.4:
                    h_data.SetBinContent(i, 0)
                    h_data.SetBinError(i, 0)
        h_data.SetMarkerStyle(20)
        h_data.Draw('SAME')

    ## Draw the signal histogram, normalized to total MC area
    h_sig = False
    if sig_stack.GetNhists() != 0:
        h_sig = sig_stack.GetStack().Last().Clone('tmp')
        h_sig.SetLineColor(R.kRed)
#        if h_sig.Integral() > 0: h_sig.Scale( all_stack.GetStack().Last().Integral() / h_sig.Integral() )
	if h_sig.Integral() > 0: h_sig.Scale(50)
        h_sig.Draw('HISTSAME')

    for group in groups['Sig'].keys():
        group_hist[group].Scale(50)
        group_hist[group].Draw('HISTSAME')

    ## Save the net signal, net background, and net data histograms
    h_net_bkg  = all_stack.GetStack().Last().Clone(dist+'_Net_Bkg')
    if h_sig:
        h_net_sig  = sig_stack.GetStack().Last().Clone(dist+'_Net_Sig')
	h_net_bkg.Add(h_net_sig, -1)
    if h_data: h_net_data = h_data.Clone(dist+'_Net_Data')

    ## Draw the legend
    legend.Draw()

    cms_label = R.TLatex()
    cms_label.SetTextSize(0.04)    
    cms_label.DrawLatexNDC(0.15, 0.85, '#scale[1.5]{CMS} #bf{#it{Preliminary}}')
    cms_label.Draw('same')
    lumi_label = R.TLatex()
    lumi_label.SetTextSize(0.035)
    lumi_label.SetTextAlign(31)
    lumi_label.DrawLatexNDC(0.90, 0.91, '#bf{%s fb^{-1} (13 TeV)}' %LUMI )
    lumi_label.Draw('same')

    ## Draw the lower pad, with the ratio histogram
    canv.cd()

    lower_pad = R.TPad('lowerPad_'+dist, 'lowerPad_'+dist, 0, 0, 1, 0.25)
    lower_pad.SetTopMargin(0.04)
    #lower_pad.SetRightMargin(0.20)
    lower_pad.SetBottomMargin(0.3)
    lower_pad.SetGridy()
    lower_pad.Draw()
    lower_pad.cd()

    ## Create the ratio histogram of data / MC
    if h_data:
        ratio_hist = h_data.Clone('ratioHist')
        ratio_hist.Divide(all_stack.GetStack().Last())
        ratio_hist.SetTitle('')
        ratio_hist.SetMinimum(RATIO_MIN)
        ratio_hist.SetMaximum(RATIO_MAX)
        ratio_hist.GetYaxis().SetNdivisions(505)  ## For some reason causes segfault after a dozen or so plots - AWB 09.10.2018
        ratio_hist.SetMarkerStyle(20)
        ratio_hist.GetXaxis().SetLabelSize(0.15)
	if dist == 'H_pair_mass_window':
	    ratio_hist.GetXaxis().SetTitle("M(#mu#mu) (GeV)")
	elif dist == 'H_pair_pt':
	    ratio_hist.GetXaxis().SetTitle("p_{T}(#mu#mu) (GeV)")
	elif dist == 'muH1_pt':
            ratio_hist.GetXaxis().SetTitle("p_{T}(#mu_{leading}) (GeV)")
	elif dist == 'muH1_eta':
            ratio_hist.GetXaxis().SetTitle("#eta(#mu_{leading})")
        elif dist == 'muH2_pt':
            ratio_hist.GetXaxis().SetTitle("p_{T}(#mu_{trailing}) (GeV)")
        elif dist == 'muH2_eta':
            ratio_hist.GetXaxis().SetTitle("#eta(#mu_{traiking})")
	elif dist == 'lep_pt':
            ratio_hist.GetXaxis().SetTitle("p_{T}(lep) (GeV)")
	elif dist == 'lep_eta':
            ratio_hist.GetXaxis().SetTitle("#eta(lep)")
        elif dist == 'muSS_muOS_dR':
            ratio_hist.GetXaxis().SetTitle("#Delta R(#mu_{OS},#mu_{SS})")
        elif dist == 'lep_H_pair_dR':
            ratio_hist.GetXaxis().SetTitle("#Delta R(lep, #mu#mu_{H})")
        elif dist == 'lep_H_pair_dEta':
            ratio_hist.GetXaxis().SetTitle("#Delta#eta(lep, #mu#mu_{H})")
        elif dist == 'lep_muSS_dEta':
            ratio_hist.GetXaxis().SetTitle("#Delta#eta(lep, #mu_{SS})")
        elif dist == 'lep_muSS_cosThStar':
            ratio_hist.GetXaxis().SetTitle("cos#theta^{*}(lep, #mu_{SS})")
        elif dist == 'lep_muOS_dR':
            ratio_hist.GetXaxis().SetTitle("#Delta R(lep, #mu_{OS})")
        elif dist == 'lep_muOS_dEta':
            ratio_hist.GetXaxis().SetTitle("#Delta#eta(lep, #mu_{OS})")
        elif dist == 'lep_muOS_cosThStar':
            ratio_hist.GetXaxis().SetTitle("cos#theta^{*}(lep, #mu_{OS})")
        elif dist == 'muSS_MHT_MT':
            ratio_hist.GetXaxis().SetTitle("M_{T}(#mu_{SS},MHT) (GeV)")
        elif dist == 'lep_MHT_MT':
            ratio_hist.GetXaxis().SetTitle("M_{T}(lep,MHT) (GeV)")
        elif dist == 'lep_MHT_dPhi_abs':
            ratio_hist.GetXaxis().SetTitle("|#Delta#phi(lep, MHT)|")
	elif dist == 'BDT_final':
	    ratio_hist.GetXaxis().SetTitle("BDT output")
	elif dist == 'H_pair_mass_zoomZ':
	    ratio_hist.GetXaxis().SetTitle("M(#mu#mu) (GeV)")
	else:
	    ratio_hist.GetXaxis().SetTitle(dist)
	ratio_hist.GetXaxis().SetTitleSize(0.15)
        ratio_hist.GetXaxis().SetTitleOffset(0.8)
	ratio_hist.GetYaxis().SetLabelSize(0.15)
	ratio_hist.GetYaxis().SetTitle("data/MC")
        ratio_hist.GetYaxis().SetTitleSize(0.15)
        ratio_hist.GetYaxis().SetTitleOffset(0.3)
	ratio_hist.Draw()
        if dist == 'MHT' or dist == 'muSS_MHT_MT' or dist == 'lep_MHT_MT' or dist == 'MET' or dist == 'muSS_MET_MT' or dist == 'lep_MET_MT':
            stat_err_band, sys_err_band, jec_err_band = GetDistSysRatio(in_file, dist, groups, all_stack)
            stat_err_band.SetLineColor(0)
            stat_err_band.SetMarkerColor(0)
            stat_err_band.SetMarkerSize(0)
            stat_err_band.SetFillColor(R.kGray)
            sys_err_band.SetLineColor(0)
            sys_err_band.SetMarkerColor(0)
            sys_err_band.SetMarkerSize(0)
            sys_err_band.SetFillColor(R.kOrange)
            jec_err_band.SetLineColor(0)
            jec_err_band.SetMarkerColor(0)
            jec_err_band.SetMarkerSize(0)
            jec_err_band.SetFillColor(R.kBlue)
            sys_err_band.Draw("E2 same")
            stat_err_band.Draw("E2 same")
            jec_err_band.Draw("E2 same")
            ratio_hist.Draw("same")

    canv.Update()
    if not ('BDT' in dist and 'zoom' in dist):
        canv.SaveAs(PLOT_DIR+'/'+LABEL+'/plots/'+CATEGORY+'/'+dist+'.png')
        canv.SaveAs(PLOT_DIR+'/'+LABEL+'/plots/'+CATEGORY+'/'+dist+'.pdf')

    ## Open output root file and save canvas and net histograms
    out_file_loc = R.TFile.Open(out_file_name, 'UPDATE')
    out_file_loc.cd()
    canv.Write()
    if h_sig: h_net_sig.Write()
    h_net_bkg.Write()
    if h_data: h_net_data.Write()
    out_file_loc.Close()

    ## Delete objects created in DrawOneStack()
    del canv, upper_pad, lower_pad, h_net_bkg, out_file_loc
    if h_sig:  del h_sig, h_net_sig
    if h_data: del ratio_hist, h_net_data

## End function: DrawOneStack()



def main():

    print '\nInside StackPlots.py, main()'

    #######################################
    ###  Set up input and output files  ###
    #######################################

    if USER == 'abrinke1':   in_file_dir = PLOT_DIR+'/'+LABEL+'/files/HADD'
    elif USER == 'eyigitba': in_file_dir = PLOT_DIR+'/'+LABEL+'/files/HADD'
    elif USER == 'xzuo':     in_file_dir = PLOT_DIR+'/'+LABEL+'/files/HADD'
    else:                    in_file_dir = PLOT_DIR+'/'+LABEL+'/files/sum'

    in_file_name  = in_file_dir+'/'+IN_FILE
    out_dir       = PLOT_DIR+'/'+LABEL+'/plots/'+CATEGORY
    out_file_name = out_dir+'/StackPlots.root'
    if not os.path.exists(out_dir): os.makedirs(out_dir)
    
    ## If this is the first time running StackPlots.py, create new output file
    if ( len(sys.argv) == 1 or int(sys.argv[1]) <= 1 ):
        print 'Creating output file %s' % out_file_name
        out_file = R.TFile.Open(out_file_name, 'RECREATE')
        out_file.mkdir('groups')
        ## Post plots to webpage: https://abrinke1.web.cern.ch/abrinke1/H2Mu/
        if USER == 'abrinke1':
            web_dir = '/afs/cern.ch/user/%s/%s/www/H2Mu/%s/Histograms/%s/plots' % (USER[0], USER, YEAR, LABEL)
            if not os.path.exists(web_dir): os.makedirs(web_dir)
            if not os.path.exists(web_dir+'/'+CATEGORY):
                os.symlink(PLOT_DIR+'/'+LABEL+'/plots/'+CATEGORY+'/', web_dir+'/'+CATEGORY)
            copyfile('/afs/cern.ch/user/%s/%s/www/index.php' % (USER[0], USER), web_dir+'/'+CATEGORY+'/index.php')

    else:  ## If StackPlots.py crashed and you are finishing the plots, update existing file
        print 'Re-opening output file %s' % out_file_name
        out_file = R.TFile.Open(out_file_name, 'UPDATE')
    out_file.Close()  ## Re-open later when saving stack
    del out_file
    print 'Opening input file %s' % in_file_name
    in_file = R.TFile.Open(in_file_name, 'READ')


    #########################################
    ###  Configure stack plot properties  ###
    #########################################

    cfg = ConfigStackPlot(CONFIG, YEAR)

    mass       = cfg.sig_mass
    loc        = cfg.ntuple_loc
    groups     = cfg.groups
    excl_samps = cfg.excl_samps
    colors     = cfg.colors


    ###################################################
    ###  Set up groups of samples to plot together  ###
    ###################################################

    ## List of expected samples from SampleDatabase.py
    if (YEAR == 'Run2'):
        DB_samps = GetSamples(loc, '2016') + GetSamples(loc, '2017') + GetSamples(loc, '2018')
    else:
        DB_samps = GetSamples(loc, YEAR)

    ## Exclude samples with incorrect signal mass
    for samp in [S for S in DB_samps if S.evt_type == 'Sig']:
        if    mass == '125' and ('120' in samp.name or '130' in samp.name): excl_samps.append(samp.name)
        elif (mass == '120' or mass == '130') and not mass in samp.name:    excl_samps.append(samp.name)
        elif mass != '120' and mass != '125' and mass != '130': print 'Invalid signal mass choice %s - quitting' % mass, sys.exit()
    # ## Exclude extra Higgs signals
    # for samp in [B for B in DB_samps if B.evt_type == 'Bkg']:
    #     if ( samp.name.startswith('H2W_') or samp.name.startswith('H2Tau') or samp.name.startswith('H2Z_') ): excl_samps.append(samp.name)


    ##################################################################
    ###  Find all the samples and distributions in the input file  ###
    ##################################################################

    dists       = []  ## All distributions contained in the file
    found_samps = []  ## All samples found in the file
    ## Loop over all the histograms in the file
    ## Histogram name composed of sample+'_'+distribution
    for key in R.gDirectory.GetListOfKeys():
        ## See if histogram name starts with a known sample name
        for samp in [S for S in DB_samps if key.GetName().startswith(S.name+'_')]:
            ## Make sure sample name is not a sub-string of another sample
            unique_samp = True
            for samp2 in [S for S in DB_samps if key.GetName().startswith(S.name+'_')]:
                if samp2.name != samp.name and samp.name in samp2.name: unique_samp = False
            if not unique_samp: continue

            dist = key.GetName().replace(samp.name+'_', '')  ## Remove sample name to get distribution name
            if not dist in dists: dists.append(dist)         ## Append the distribution to the list of distributions

    print '\nList of found distributions:'
    print ', '.join(dists)

    ## Clean out excluded samples from the Database list
    DB_samps = [S for S in DB_samps if not S.name in excl_samps]

    ## Check to see if there are any samples not known to SampleDatabase.py
    for key in R.gDirectory.GetListOfKeys():
        for dist in [D for D in dists if key.GetName().endswith('_'+D)]:
            ## Make sure distribution name is not a sub-string of another distribution
            unique_dist = True
            for dist2 in dists:
                if dist2 != dist and dist in dist2: unique_dist = False
            if not unique_dist: continue

            samp = key.GetName().replace('_'+dist, '')            ## Remove distribution name to get sample name
            if samp in excl_samps: continue                       ## Don't consider explicitly excluded samples
            if not samp in found_samps: found_samps.append(samp)  ## Append the sample to the list of found samples
            if not any(samp == S.name for S in DB_samps):
                print 'Found histogram %s with distribution %s and sample %s, not found in SampleDatabase.py or excl_samps:'
                print excl_samps
                print 'Check that %s is in SampleDatabase.py or excl_samps, and that GetSamples(%s, %s) is configured properly' % (samp, loc, YEAR)
                sys.exit()

    print '\nList of found samples:'
    print ', '.join(found_samps)

    ## Check to see if any samples in SampleDatabase.py do not appear in the found list
    not_found_samps = []
    for samp in [S for S in DB_samps if not S.name in found_samps]: not_found_samps.append(samp.name)
    if len(not_found_samps) > 0:
        print '\nSURPRISING SITUATION!!!  The following samples not found in the input file:'
        print ', '.join(not_found_samps)

    ## Clean out not found samples from the Database list
    DB_samps = [S for S in DB_samps if not S.name in not_found_samps]
                

    ##################################
    ###  Assign samples to groups  ###
    ##################################

    ## If not otherwise assigned, put sample in 'Data', 'Signal', or 'Other' category
    for samp in DB_samps:
        if not any(samp.name in samps for group, samps in groups[samp.evt_type].items()):
            if samp.evt_type  == 'Data': groups['Data']['Data']  .append(samp.name)
            elif samp.evt_type == 'Sig': groups['Sig'] ['Signal'].append(samp.name)
            elif samp.evt_type == 'Bkg': groups['Bkg'] ['Other'] .append(samp.name)

    ## Sanity check that found_samps and DB_samps contain the same list of samples
    for samp in found_samps:
        if not any(samp == S.name for S in DB_samps): print '\nSample %s in found_samps not in SampleDatabase.py. Exiting', sys.exit()
    for samp in DB_samps:
        if not samp.name in found_samps: print '\nSample %s in SampleDatabase.py not in found_samps. Exiting', sys.exit()

    ## Check to see if we're missing any samples we were expecting
    for evt_type in groups.keys():
        for group, samps in groups[evt_type].items():
            for samp in [S for S in samps if not S in found_samps and not S in not_found_samps]:
                print '\nSURPRISING SITUATION!!!  Sample %s from %s group %s not found in final list of samples:' % (samp, evt_type, group)
                print ', '.join(found_samps)
            ## Drop samples from group if they do not appear in found_samps
            groups[evt_type][group] = [S for S in samps if S in found_samps]

    print '\nFinal list of data samples:'
    for group in groups['Data'].keys(): print group+': '+', '.join(groups['Data'][group])
    print '\nFinal list of signal samples:'
    for group in groups['Sig'].keys(): print group+': '+', '.join(groups['Sig'][group])
    print '\nFinal list of background samples:'
    for group in groups['Bkg'].keys(): print group+': '+', '.join(groups['Bkg'][group])
    print '\nFinal list of excluded samples (signal, usually wrong-mass):'
    for excl_samp in excl_samps:
        if 'H2Mu' in excl_samp: print (excl_samp+', '),
    print '\nFinal list of excluded samples (background):'
    for excl_samp in excl_samps:
        if not 'H2Mu' in excl_samp: print (excl_samp+', '),
    print '\n'


    ## Drop empty groups
    for kind in ['Data', 'Bkg']:  
        for group in groups[kind]:
            if len(groups[kind][group]) == 0:
                print '\nRemoving group %s from %s!!!  Found no input samples' % (group, kind)
                del groups[kind][group]


    ##########################################
    ###  Histograms, stacks, and canvases  ###
    ##########################################

    ## Save a copy of the original set of groups
    orig_groups = copy.deepcopy(groups)

    print '\nAbout to start creating and filling the plots'
    print 'Looping over samples and distributions to get histograms'
    iDist = 0
    for dist in dists:
        iDist += 1
        ## Only plot stacks in certain index range, if speficied by the user
        if ( len(sys.argv) > 1 and (iDist < int(sys.argv[1]) or iDist > int(sys.argv[2])) ): continue
        print '  * Looking at distribution %s (#%d)' % (dist, iDist)

        ## Reset "group" to the original set of groups
        groups = copy.deepcopy(orig_groups)

	group_hist = {}  ## Summed sample histograms by group
	stack_all  = R.THStack('all_stack_'+dist, dist+' signal + background')  # now stack_all is only used for backgrounds
	stack_sig  = R.THStack('sig_stack_'+dist, dist+' signal')
	stack_dat  = R.THStack('dat_stack_'+dist, dist+' data')

        ## Construct histogram for each group from samples in group
        for evt_type in groups.keys():
            for group, samps in groups[evt_type].items():
                for samp in samps:
                    try:
                        hist = in_file.Get(samp+'_'+dist).Clone('tmp')

                        ## Set uncertainty for empty bins to Integral/(nEffectuveEntries*sqrt(nEmptyBins))
                        if not evt_type is 'Data':
                            nEmptyBins = 0
                            for iBin in range(1, hist.GetNbinsX()+1):
                                if hist.GetBinContent(iBin) == 0: nEmptyBins += 1
                            if nEmptyBins > 0:
                                empty_uncert = abs(hist.Integral() / (hist.GetEffectiveEntries() * math.sqrt(nEmptyBins)))
                                for jBin in range(1, hist.GetNbinsX()+1):
                                    hist.SetBinError(jBin, max(abs(hist.GetBinError(jBin)), empty_uncert))

                        ## Scale samples for which there is more than on MC dataset
                        if samp in cfg.weight.keys():
                            print 'Scaling sample %s by %.2f' % (samp, cfg.weight[samp])
                            hist.Scale(cfg.weight[samp])

                        ## apply k-factor to WZ_3l sample
                        if samp == 'WZ_3l' and APPLY_KFactor:
                          if CONFIG  == 'WH_lep_allMass': hist.Scale(1.127)
                          else:                           hist.Scale(1.102)

                        if not group in group_hist.keys():
                            group_hist[group] = hist.Clone('hist_'+dist+'_'+group)
                        else:
                            group_hist[group].Add(hist)
                        del hist
                    except:
                        print '  - Could not find histogram '+samp+'_'+dist

            ## Remove any groups that have no histograms
            for key in groups[evt_type].keys():
                if not key in group_hist.keys():
                    del groups[evt_type][key]

        if ( len(groups['Sig'].keys()) + len(groups['Bkg'].keys()) == 0 ):
            print '\n\n******* Super-bizzare situation: no signal or background histograms for %s!!!' % dist
            print 'There are %d histograms in data.  Moving on to the next plot,\n' % len(groups['Data'].keys())
            continue

        ## Fill the signal stack
	for group in groups['Sig'].keys():
            group_hist[group].SetLineColor(colors[group])
            group_hist[group].SetLineWidth(2)
	    stack_sig.Add(group_hist[group])
	if stack_sig.GetNhists() != 0: group_hist['Sig'] = stack_sig.GetStack().Last()
        ## Fill the signal + background stack, saving individual stack components
        out_file_loc = R.TFile.Open(out_file_name, 'UPDATE')
        out_file_loc.cd('groups')
        for group in (groups['Bkg'].keys()):  # removed signal from stack_all.
            group_hist[group].SetFillColor(colors[group])
            stack_all.Add(group_hist[group])
            ## Write out the histogram into 'groups/', stripping 'hist_' from the name
            group_hist[group].Write(group_hist[group].GetName().replace('hist_',''))
        for group in (groups['Sig'].keys()): # write signal to 'groups/'
            group_hist[group].Write(group_hist[group].GetName().replace('hist_',''))
	group_hist['MC'] = stack_all.GetStack().Last()
        out_file_loc.Close()  ## Re-open later when saving stack
        ## Fill the data stack
        MC_only = True  ## If we don't find any data histograms, fill 'data' as sum of MC
        for group in groups['Data'].keys():
            if group in group_hist.keys():
                stack_dat.Add(group_hist[group])
                MC_only = False
        if not MC_only:
            group_hist['Dat'] = stack_dat.GetStack().Last()
        else:
            print '\nFor distribution %s, could not find any data.  Filling with sum of MC.\n' % dist
            group_hist['Dat'] = 0

        ## Set aliases for final histograms
	h_sig = False
        if 'Sig' in group_hist.keys(): h_sig = group_hist['Sig']
        h_bkg = group_hist['MC']
        h_dat = group_hist['Dat']
        nSig  = 0 if not h_sig else h_sig.Integral()
        nBkg  = h_bkg.Integral()
        nDat  = 0 if MC_only else h_dat.Integral()

        ## Compute B/S and S/sqrt(B) for this distribution
        B_to_S = (nBkg/nSig) if (nSig > 0) else 0
        sigSq  = 0
	if h_sig:
          for i in range(1, h_sig.GetNbinsX()+1):
            numB = h_bkg.GetBinContent(i)
            errB = h_bkg.GetBinError(i)
            ## Don't consider empty bins (statistically limited)
            if (numB <= 0): continue
            sigSq += ( pow(h_sig.GetBinContent(i), 2) / (numB + pow(errB, 2)) )

        ## Create TLegend
        legend = R.TLegend(0.6, 0.6, 0.9, 0.9)
        for evt_type in groups.keys():
            for group in groups[evt_type].keys():
                if MC_only and group == 'Data': continue
#                if   evt_type == 'Data' and nBkg > 0.0:   ## Display data integral and data / MC ration in the legend
#                    legend.AddEntry( group_hist[group], '%s (%d, %.2f)'  % (group, nDat, nDat / nBkg) )
#                elif evt_type == 'Sig'  and nSig > 0.0:  ## Display signal integral and scale factor in the legend
#                    legend.AddEntry( group_hist[group], '%s (%.3f)' % (group, nSig) )
#                    legend.AddEntry( group_hist[group], 'B/S=%d, %.3f#sigma' % (B_to_S, math.sqrt(sigSq)) )
	 	if evt_type == 'Sig' and nSig > 0.0:
		    legend.AddEntry( group_hist[group], '%s X50' %group)
                else:
                    if group == 'Others': legend.AddEntry(group_hist[group], 'Other Bkg.')
                    else:                 legend.AddEntry(group_hist[group], group)


        ## Draw stack plot
	DrawOneStack( in_file, dist, stack_sig, stack_all, groups, group_hist, legend, out_file_name )
        ## Delete objects created in loop over dists
        del group_hist, stack_all, stack_sig, stack_dat, h_sig, h_bkg, h_dat, legend
        
    ## End loop: for dist in dists

    print '\nFinished looping over all distributions.  Done!'

main()  ## End of main function

