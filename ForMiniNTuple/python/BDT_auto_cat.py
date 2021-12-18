####################################################
###    make stack and ratio from miniNTuple      ###
####################################################

## Basic python includes for manipulating files
import os
import sys
from array import array

sys.path.insert(0, '%s/lib' % os.getcwd())
from ROOT import *
from MNT_Helper import LinearStack, RatioPlot, FillHistTerm, GetSF
from Plot_Helper import LoadNtuples, MakeStack, CreateCanvas, DrawOnCanv, SaveCanvPic, MakeLumiLabel, MakeCMSPrelimLabel, ScaleSignal, MakeRatioPlot, MakeLegend
import Plot_Configs as PC
from CMS_style import setTDRStyle

gROOT.SetBatch(True)

## Configure the script user
if 'abrinke1' in os.getcwd(): USER = 'abrinke1'
if 'bortigno' in os.getcwd(): USER = 'bortigno'
if 'eyigitba' in os.getcwd(): USER = 'eyigitba'
if 'xzuo'     in os.getcwd(): USER = 'xzuo'

## Directory for input histograms and output plots
if USER == 'abrinke1': PLOT_DIR = '/afs/cern.ch/work/a/abrinke1/public/H2Mu/2017/Histograms'
if USER == 'eyigitba': PLOT_DIR = '/afs/cern.ch/work/e/eyigitba/public/H2Mu/2017/Histograms'
if USER == 'xzuo':     PLOT_DIR = '/afs/cern.ch/work/x/xzuo/public/H2Mu/Run2/Histograms'

do_WH = False

#LABEL = 'miniNtuple_WH_2016_v5'  ## Sub-folder within PLOT_DIR containing histograms
#LABEL = 'VH_selection_2019april/pt10_iso04/ZH_mu_massBDT'
#LABEL = 'ZH_lep_2019_08_25_medLep_veto12'
#LABEL = 'ZH_Val_lep_2019_08_25_medLep_veto12'
#LABEL = 'ZZ_lep_final_ntuple_old_selection_2020_02_11'
#LABEL = 'ZH_lep_final_ntuple_ele_loose_FSRGeo_2020_02_17'
#LABEL = 'ZH_lep_SR_CR_standard_ID_jet25_2020_03_08'
LABEL = 'ZH_lep_SR_CR_final_BDT_2020_03_11'
#LABEL = 'leftover_of_ttH_and_ZH_2019_09_08'
#LEP = "muon"

if do_WH: LABEL = 'WH_lep_final_BDT_for_making_cats_2020_03_25'


VERBOSE = True
LOW_YIELD = 16.0


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



def Calc_sense(file_chain, bound, MVA_str, wgt_str):
    low_stats = False
    sense = 0.0
    sense_errsqr = 0.0
    for i in range( len(bound)-1 ):
      if low_stats: break
      if do_WH:
        file_chain.Draw("dimu_mass >> sig%d(800,110,150)" %i, "(samp_ID > -10 && samp_ID < 0 && BDTG_UF_v1a >=%f && BDTG_UF_v1a < %f && %s ) * %s * 0.5" %(bound[i], bound[i+1], MVA_str, wgt_str))
        file_chain.Draw("dimu_mass >> bkg%d(800,110,150)" %i, "(samp_ID > 0 && BDTG_UF_v1a >=%f && BDTG_UF_v1a < %f && %s ) * %s * 0.5" %(bound[i], bound[i+1], MVA_str, wgt_str))
      else:
        file_chain.Draw("dimu_mass >> sig%d(800,110,150)" %i, "(in_SR ==1 && Sample_ID == 2325 && BDT_final >= %f && BDT_final < %f && %s ) * %s" %(bound[i], bound[i+1], MVA_str, wgt_str))
        file_chain.Draw("dimu_mass >> bkg%d(800,110,150)" %i, "(in_SR ==1 &&Sample_ID<0 && Sample_ID!=-999 && BDT_final >= %f && BDT_final < %f && %s ) * %s" %(bound[i], bound[i+1], MVA_str, wgt_str))
      sig =  gDirectory.Get("sig%d"%i)
      bkg =  gDirectory.Get("bkg%d"%i)

      if bkg.Integral() < LOW_YIELD:
        low_stats = True
        if VERBOSE: print 'low yield, bkg = %.2f' %bkg.Integral()
        break
      low_bin, high_bin = findFWHM(sig)
      s_err   = array('d', [0])
      b_err   = array('d', [0])
      s_yield = sig.IntegralAndError(low_bin, high_bin, s_err)
      b_yield = bkg.IntegralAndError(low_bin, high_bin, b_err)
      sense += s_yield**2.0 / (s_yield + b_yield)
      sense_errsqr += ( s_err[0]**2.0 * (2.0*s_yield*b_yield + s_yield**2.0)**2.0/(s_yield + b_yield) ** 4.0 ) + (b_err[0]**2.0 * s_yield**4.0/(s_yield + b_yield)**4.0)
      if VERBOSE: print 's_yield = %.4f, b_yield = %.4f' %(s_yield, b_yield)

    if low_stats: return [0.0, 0.0]
    else:         return [sense ** 0.5, sense_errsqr ** 0.5 / (2.0 * sense ** 0.5)]


def Plot_cmsLabels(canv):
    canv.cd()
    gStyle.SetOptStat(0000)
    l_lumi = TLatex()
    l_lumi.SetTextSize(0.025)
    l_lumi.SetTextAlign(31)
    l_lumi.DrawLatexNDC(0.93, 0.95, '#font[42]{#scale[1.4]{(13 TeV)}}')
    l_lumi.Draw('SAME')

    l_cms  = TLatex()
    l_cms.SetTextSize(0.03)
    l_cms.DrawLatexNDC(0.15, 0.85, '#scale[1.5]{CMS #bf{#it{Preliminary Simulation}}}')
    l_cms.Draw('SAME')
    canv.Write()
    if canv.GetName() == 'mass_bkg':
      canv.SaveAs(PLOT_DIR+"/"+LABEL+"/plots/c_mass.pdf")


def Plot_nBkgs(c_mass, h_mass, quantiles):
    gStyle.SetOptStat(0000)
    gStyle.SetLegendBorderSize(0)
    gStyle.SetLegendFont(42)
    gStyle.SetLegendTextSize(0.025)
    c_mass.cd()
    l_mass = TLegend(0.66, 0.6, 0.93,0.9)

    colors = [kBlack, kBlue, kRed, kOrange, kGreen+2, kViolet]
    print len(h_mass)
    for i in range( len(h_mass) ):
      h_mass[i].Scale(1.0/h_mass[i].Integral())
      h_mass[i].SetLineColor(colors[i])
      h_mass[i].SetLineWidth(2)
      h_mass[i].SetMaximum(0.16)
      h_mass[i].SetMinimum(0.0)
      l_mass.AddEntry(h_mass[i], quantiles[i], 'L')
      h_mass[i].SetTitle('')
      h_mass[i].GetXaxis().SetTitle('m_{#mu#mu} (GeV)')
      h_mass[i].GetXaxis().SetTitleSize(0.04)
      h_mass[i].GetXaxis().SetTitleOffset(1.15)
      h_mass[i].GetYaxis().SetTitle('a.u.')
      h_mass[i].GetYaxis().SetTitleOffset(1.2)
      h_mass[i].Draw('HISTSAME')
    l_mass.Draw()
    Plot_cmsLabels(c_mass)
#    c_mass.Write()


def Plot_3Sigs(c_BDT, h_125, h_120, h_130):
    c_BDT.cd()
    gStyle.SetOptStat(0000)
    gStyle.SetLegendBorderSize(0)
    gStyle.SetLegendFont(42)
    gStyle.SetLegendTextSize(0.04)

    upper_pad = TPad("upperpad", "upperpad", 0,0.25, 1,1)
    upper_pad.SetBottomMargin(0.02)
    upper_pad.Draw()
    upper_pad.cd()

    l_BDT = TLegend(0.66,0.63, 0.9,0.9)
    h_125.Scale(1.0/h_125.Integral())
    h_120.Scale(1.0/h_120.Integral())
    h_130.Scale(1.0/h_130.Integral())

    h_125.SetMarkerStyle(20)
    h_120.SetLineColor(kRed)
    h_130.SetLineColor(kBlue)

    l_BDT.AddEntry(h_125, 'm_{H} = 125 GeV', 'PE')
    l_BDT.AddEntry(h_120, 'm_{H} = 120 GeV', 'L')
    l_BDT.AddEntry(h_130, 'm_{H} = 130 GeV', 'L')
    h_125.SetTitle('')
    h_125.GetXaxis().SetLabelSize(0)
    h_125.GetYaxis().SetTitle('a.u.')
    h_125.GetYaxis().SetTitleSize(0.06)
    h_125.GetYaxis().SetTitleOffset(0.9)
    h_125.GetYaxis().SetRangeUser(0, 0.08)
    h_125.Draw('PE')
    h_120.Draw('HISTSAME')
    h_130.Draw('HISTSAME')
    l_BDT.Draw()

    l_lumi = TLatex()
    l_lumi.SetTextSize(0.035)
    l_lumi.SetTextAlign(31)
    l_lumi.DrawLatexNDC(0.93, 0.95, '#font[42]{#scale[1.3]{(13 TeV)}}')
    l_lumi.Draw('SAME')

    l_cms  = TLatex()
    l_cms.SetTextSize(0.04)
    l_cms.DrawLatexNDC(0.15, 0.85, '#scale[1.5]{CMS #bf{#it{Preliminary Simulation}}}')
    l_cms.Draw('SAME')

    c_BDT.cd()
    lower_pad = TPad("lowerpad", "lowerpad", 0, 0, 1,0.25)
    lower_pad.SetTopMargin(0.05)
    lower_pad.SetBottomMargin(0.3)
    lower_pad.SetGridy()
    lower_pad.Draw()
    lower_pad.cd()

    ratio_120 = h_120.Clone('ratio_120')
    ratio_130 = h_130.Clone('ratio_130')
    ratio_120.Divide(h_125)
    ratio_130.Divide(h_125)

    ratio_120.SetTitle('')
    ratio_120.GetXaxis().SetTitle('BDT output')
    ratio_120.GetXaxis().SetTitleSize(0.14)
    ratio_120.GetXaxis().SetLabelSize(0.14)
    ratio_120.GetYaxis().SetRangeUser(0, 2)
    ratio_120.GetYaxis().SetNdivisions(502)
    ratio_120.GetYaxis().SetTitle('Ratio')
    ratio_120.GetYaxis().SetTitleOffset(0.35)
    ratio_120.GetYaxis().SetTitleSize(0.14)
    ratio_120.GetYaxis().SetLabelSize(0.14)
    ratio_120.GetYaxis().CenterTitle(1)
    ratio_120.GetXaxis().SetTickLength(0.06)
    ratio_120.GetYaxis().SetTickLength(0.04)
    ratio_120.Draw('HIST')
    ratio_130.Draw('HISTSAME')
    c_BDT.Write()
    c_BDT.SaveAs(PLOT_DIR+"/"+LABEL+"/plots/c_BDT.pdf")



def main():
    out_name = "BDT_cat_optimization_err.root"
    if do_WH: out_name = "BDT_cat_optimization_err_3ndn0p1.root"

    file_dir = PLOT_DIR+"/"+LABEL+"/"
    out_file = TFile( file_dir + "plots/" + out_name , "RECREATE")
    file_chain = TChain("tree","chain");
    if do_WH: file_chain.Add(file_dir + "files/HADD/tuples_noSys.root")
    else:     file_chain.Add(file_dir + "all_samples.root")

    setTDRStyle()

    # this only for WH tree with TestTree
    if do_WH:
      file_chain = TChain("f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairAll_var1_var2_muSSOSdEta_lepCharge_all_sig_all_bkg_resWgt/TestTree")
      file_chain.Add('/afs/cern.ch/work/x/xzuo/h2mm_944/src/H2MuAnalyzer/TrainMVA/output/Run2_WH_lep_all_vs_all_2020_03_23_v2.root')

    points = range(1,200) ## scan 1 to 199
    sens = {}

    MVA_str = "mu1_lepMVA > 0.4 && mu2_lepMVA > 0.4 && lep1_lepMVA > 0.4 && lep2_lepMVA > 0.4"
    wgt_str = "event_wgt * xsec_norm * all_lepMVA_SF * Btag_wgt"

    if do_WH:
      MVA_str = '1'
#      wgt_str = 'event_wgt * samp_wgt * lepMVA_wgt_OPT_3lep_CAT_lep20_medLepMVA_SR_Bveto'
      wgt_str = 'total_wgt'


    ## get sensitivity before cut
    bound = [-1.0, 1.0]
    if do_WH: bound = [-1.0, -0.1, 0.3, 1.0]
    sens0 = Calc_sense(file_chain, bound, MVA_str, wgt_str)    

    ## for each cut point, get sensitivity
    for point in points:
      cut = point / 100.0 -1.0
      if VERBOSE: print 'new cut at %.2f ********' %cut
      bound = [-1.0, 1.0]
      if do_WH: bound = [-1.0, -0.1, 0.3, 1.0]
      bound.append(cut)
      bound.sort()
      if cut < -0.3 or cut > 0.1:  # to run faster
        sens['%d'%point] = sens0
        continue

      sens['%d'%point] = Calc_sense(file_chain, bound, MVA_str, wgt_str)
      if sens['%d'%point] == [0.0, 0.0]: sens['%d'%point] = sens0

    out_file.cd()
    cut_opt = TGraphErrors()
    cut_opt.SetName("BDT_cut_opt")
    for point in points:
      cut = point / 100.0 -1.0
      cut_opt.SetPoint     (point-1, cut,       sens['%d'%point][0])
      cut_opt.SetPointError(point-1, 1.0/200.0, sens['%d'%point][1])
    cut_opt.SetMarkerStyle(20)
    cut_opt.GetXaxis().SetTitle('BDT_value')
    cut_opt.GetYaxis().SetTitle('Significance')
    cut_opt.GetYaxis().SetTitleOffset(1.5)
 
    cut_opt.Write()

    canv = TCanvas('BDT_cut', 'BDT_cut', 600, 600)
    canv.cd()
    canv.SetGridx(1)
    canv.SetGridy(1)
    cut_opt.Draw('P')
    Plot_cmsLabels(canv)
#    canv.Write()


#    lumi_label = MakeLumiLabel(LUMI)
#    cms_label  = MakeCMSPrelimLabel()

    ## plot overlay of bkg mass shape in BDT quantiles
    bkg_mass = []
    if do_WH:
      file_chain.Draw("dimu_mass >> bkg_BDT1(20,110,150)", "(samp_ID > 0 && BDTG_UF_v1a > -1.0  && BDTG_UF_v1a < -0.35 && %s) * %s * 0.5" %(MVA_str, wgt_str))
      file_chain.Draw("dimu_mass >> bkg_BDT2(20,110,150)", "(samp_ID > 0 && BDTG_UF_v1a > -0.35 && BDTG_UF_v1a < -0.19 && %s) * %s * 0.5" %(MVA_str, wgt_str))
      file_chain.Draw("dimu_mass >> bkg_BDT3(20,110,150)", "(samp_ID > 0 && BDTG_UF_v1a > -0.19 && BDTG_UF_v1a < -0.04 && %s) * %s * 0.5" %(MVA_str, wgt_str))
      file_chain.Draw("dimu_mass >> bkg_BDT4(20,110,150)", "(samp_ID > 0 && BDTG_UF_v1a > -0.04 && BDTG_UF_v1a <  0.15 && %s) * %s * 0.5" %(MVA_str, wgt_str))
      file_chain.Draw("dimu_mass >> bkg_BDT5(20,110,150)", "(samp_ID > 0 && BDTG_UF_v1a >  0.15 && BDTG_UF_v1a <  1.0  && %s) * %s * 0.5" %(MVA_str, wgt_str))
    else:
      file_chain.Draw("dimu_mass >> bkg_BDT1(20,110,150)", "(in_SR ==1 && Sample_ID<0 && Sample_ID!=-999 && BDT_final > -1.0 && BDT_final < -0.42 && %s ) * %s" %(MVA_str, wgt_str))
      file_chain.Draw("dimu_mass >> bkg_BDT2(20,110,150)", "(in_SR ==1 && Sample_ID<0 && Sample_ID!=-999 && BDT_final >-0.42 && BDT_final < -0.21 && %s ) * %s" %(MVA_str, wgt_str))
      file_chain.Draw("dimu_mass >> bkg_BDT3(20,110,150)", "(in_SR ==1 && Sample_ID<0 && Sample_ID!=-999 && BDT_final >-0.21 && BDT_final < -0.01 && %s ) * %s" %(MVA_str, wgt_str))
      file_chain.Draw("dimu_mass >> bkg_BDT4(20,110,150)", "(in_SR ==1 && Sample_ID<0 && Sample_ID!=-999 && BDT_final >-0.01 && BDT_final <  0.21 && %s ) * %s" %(MVA_str, wgt_str))
      file_chain.Draw("dimu_mass >> bkg_BDT5(20,110,150)", "(in_SR ==1 && Sample_ID<0 && Sample_ID!=-999 && BDT_final > 0.21 && BDT_final <   1.0 && %s ) * %s" %(MVA_str, wgt_str))
    bkg_mass.append(gDirectory.Get("bkg_BDT1"))
    bkg_mass.append(gDirectory.Get("bkg_BDT2"))
    bkg_mass.append(gDirectory.Get("bkg_BDT3"))
    bkg_mass.append(gDirectory.Get("bkg_BDT4"))
    bkg_mass.append(gDirectory.Get("bkg_BDT5"))
    quantiles = ['-1.0 < BDT < -0.42', '-0.42 < BDT < -0.21', '-0.21 < BDT < -0.01', '-0.01 < BDT < 0.21', '0.21 < BDT < 1.0']
    if do_WH: quantiles = ['-1.0 < BDT < -0.35', '-0.35 < BDT < -0.19', '-0.19 < BDT < -0.04', '-0.04 < BDT < 0.15', '0.15 < BDT < 1.0']

    c_mass = TCanvas('mass_bkg', 'mass_bkg', 600, 600)
    Plot_nBkgs(c_mass, bkg_mass, quantiles)

    ## plot overlay of sig BDT shape with different m_Higgs
    if do_WH:
      file_chain.Draw("BDTG_UF_v1a >> sig_M125(50,-1,1)", "(samp_ID > -10 && samp_ID < 0   && dimu_mass>110 && dimu_mass<150 && %s ) * %s * 0.5" %(MVA_str, wgt_str))
      file_chain.Draw("BDTG_UF_v1a >> sig_M120(50,-1,1)", "(samp_ID > -20 && samp_ID < -10 && dimu_mass>110 && dimu_mass<150 && %s ) * %s * 0.5" %(MVA_str, wgt_str))
      file_chain.Draw("BDTG_UF_v1a >> sig_M130(50,-1,1)", "(samp_ID > -30 && samp_ID < -20 && dimu_mass>110 && dimu_mass<150 && %s ) * %s * 0.5" %(MVA_str, wgt_str))
    else:
      file_chain.Draw("BDT_final >> sig_M125(50,-1,1)", "(Sample_ID == 2325    && dimu_mass>110 && dimu_mass<150 && %s ) * %s" %(MVA_str, wgt_str))
      file_chain.Draw("BDT_final >> sig_M120(50,-1,1)", "(Sample_ID == 1202325 && dimu_mass>110 && dimu_mass<150 && %s ) * %s" %(MVA_str, wgt_str))
      file_chain.Draw("BDT_final >> sig_M130(50,-1,1)", "(Sample_ID == 1302325 && dimu_mass>110 && dimu_mass<150 && %s ) * %s" %(MVA_str, wgt_str))
    sig_BDT_125 = gDirectory.Get("sig_M125")
    sig_BDT_120 = gDirectory.Get("sig_M120")
    sig_BDT_130 = gDirectory.Get("sig_M130")

    c_BDT = TCanvas('BDT_sig', 'BDT_sig', 600, 600)
    Plot_3Sigs(c_BDT, sig_BDT_125, sig_BDT_120, sig_BDT_130)
    

    out_file.Close()
main()











	
