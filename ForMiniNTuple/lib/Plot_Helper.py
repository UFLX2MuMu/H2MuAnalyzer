
import os
import Plot_Configs as PC
#import Analyzer_Configs as AC
from ROOT import *

#####################################################################

def LoadNtuples(ana_cfg):
    ntuples = {}
    for sample in ana_cfg.samp_names:
        ntuples[sample] = TChain("tree","chain_" + sample) 
	ntuples[sample]. Add(ana_cfg.sample_loc + '/ntuple_%s.root' %sample)
    return ntuples


def MakeStack(histos, ana_cfg, var_name):
    stacks = {}
    stacks['all']  = THStack("h_stack_"+var_name, var_name)
    stacks['sig']  = THStack("h_stack_"+var_name, var_name)
    stacks['bkg']  = THStack("h_stack_"+var_name, var_name)

    for sample in ana_cfg.sig_names:
        stacks['sig'].Add(histos[sample])
	stacks['all'].Add(histos[sample])
    for sample in ana_cfg.bkg_names:
        stacks['bkg'].Add(histos[sample])
        stacks['all'].Add(histos[sample])
    return stacks


def CreateCanvas(canv_name):
    canv = TCanvas(canv_name, canv_name, 600,600)
    return canv


def MakeLumiLabel(lumi):
    tex = TLatex()
    tex.SetTextSize(0.035)
    tex.SetTextAlign(31)
    tex.DrawLatexNDC(0.90, 0.91, '%s fb^{-1} (13 TeV)' %lumi)
    return tex


def MakeCMSPrelimLabel():
    tex = TLatex()
    tex.SetTextSize(0.04)
    tex.DrawLatexNDC(0.15, 0.85, '#scale[1.5]{CMS} preliminary')
    return tex


def ScaleSignal(sig_scale, stack_sig, var_name):
    sig_hist = stack_sig.GetStack().Last()
    sig_hist.Scale(sig_scale)
    sig_hist.SetLineColor(kRed)
    sig_hist.SetLineWidth(2)
    sig_hist.SetFillStyle(0)

    sig_hist.GetXaxis().SetTitle(var_name)
    sig_hist.GetXaxis().SetTitleSize(0.20)
    sig_hist.GetYaxis().SetTitle('Events / %.2f' %sig_hist.GetBinWidth(1))
    sig_hist.GetYaxis().SetTitleSize(0.20)
    return sig_hist


def MakeRatioPlot(h_data, h_MC, var_name):
    ratio_plot = TGraphAsymmErrors()
    ratio_plot.Divide(h_data, h_MC, "pois")
    ratio_plot.SetName("ratiograph_" + var_name)
    ratio_plot.SetMinimum(0.2)
    ratio_plot.SetMaximum(2.0)
    ratio_plot.SetMarkerStyle(20)

    ratio_plot.GetXaxis().SetRangeUser( h_data.GetXaxis().GetXmin(), h_data.GetXaxis().GetXmax() )
    ratio_plot.GetXaxis().SetLabelSize(0.15)
    if var_name == 'dimu_mass':
	ratio_plot.GetXaxis().SetTitle('M(#mu#mu) (GeV)')
    elif var_name == 'dimu_pt':
	ratio_plot.GetXaxis().SetTitle('p_{T}(#mu#mu) (GeV)')
    elif var_name == 'mu1_pt':
        ratio_plot.GetXaxis().SetTitle('p_{T}(#mu_{leading}) (GeV)')
    elif var_name == 'mu1_abs_eta':
        ratio_plot.GetXaxis().SetTitle('|#eta(#mu_{leading})|')
    elif var_name == 'dilep_mass':
        ratio_plot.GetXaxis().SetTitle('M(dilep) (GeV)')
    elif var_name == 'dilep_pt':
        ratio_plot.GetXaxis().SetTitle('p_{T}(dilep) (GeV)')
    elif var_name == 'lep1_pt':
        ratio_plot.GetXaxis().SetTitle('p_{T}(#lep_{leading}) (GeV)')
    elif var_name == 'lep1_abs_eta':
        ratio_plot.GetXaxis().SetTitle('|#eta(#lep_{leading})|')
    elif 'BDT' in var_name:
        ratio_plot.GetXaxis().SetTitle('BDT output')
    else:
        ratio_plot.GetXaxis().SetTitle(var_name)
    ratio_plot.GetXaxis().SetTitleSize(0.15)
    ratio_plot.GetXaxis().SetTitleOffset(0.8)

    ratio_plot.GetYaxis().SetNdivisions(505)
    ratio_plot.GetYaxis().SetLabelSize(0.15)
    ratio_plot.GetYaxis().SetTitle("data/MC")
    ratio_plot.GetYaxis().SetTitleSize(0.15)
    ratio_plot.GetYaxis().SetTitleOffset(0.3)

    return ratio_plot


def MakeLegend(sig_names, bkg_names, histos, scaled_signal, sig_scale):
    legend = TLegend(0.5,0.5,0.9,0.9)
    legend.SetNColumns(2)

    legend.AddEntry(histos["Data"], "data")
#    for sample in plt_cfg.ana_cfg.sig_names:
    for sample in sig_names:
	legend.AddEntry(histos[sample], sample )
#    legend.AddEntry(scaled_signal, "signal X%d" %plt_cfg.sig_scale)
    legend.AddEntry(scaled_signal, "signal X%d" %sig_scale)
#    for sample in plt_cfg.ana_cfg.bkg_names:
    for sample in bkg_names:
        legend.AddEntry(histos[sample], sample )
    return legend


def DrawOnCanv(canv, var_name, logY, stack, histos, scaled_sig, ratio_plot, legend, lumi_label, cms_label):
    canv.cd()
    upper_pad = TPad("upperpad_"+var_name, "upperpad_"+var_name, 0,0.25, 1,1)
    upper_pad.SetBottomMargin(0.04)
    upper_pad.Draw()
    upper_pad.cd()
#    if plt_cfg.logY:
    if logY:
	upper_pad.SetLogy()
	stack.SetMinimum(1e-1)
        stack.SetMaximum(1e8)
    else:
	stack.SetMaximum(histos['Data'].GetMaximum() * 1.5)
    stack.Draw('HIST')
    stack.SetTitle('')
    stack.GetXaxis().SetLabelSize(0)
    stack.GetYaxis().SetLabelSize(0.05)
    stack.GetYaxis().SetTitle('Events / Bin')
    stack.GetYaxis().SetTitleSize(0.05)
    stack.GetYaxis().SetTitleOffset(1.0)
    histos['Data'].SetMarkerStyle(20)
    histos['Data'].Draw('SAMEPE')
    scaled_sig.Draw('HISTSAME')

    legend.Draw()
    cms_label.DrawLatexNDC(0.15, 0.85, '#scale[1.5]{CMS} #bf{#it{Preliminary}}')
    cms_label.Draw('same')
#    lumi_label.DrawLatexNDC(0.90, 0.91, '%s fb^{-1} (13 TeV)' %plt_cfg.lumi)
    lumi_label.DrawLatexNDC(0.90, 0.91, '#bf{137.1 fb^{-1} (13 TeV)}' )
    lumi_label.Draw('same')

    canv.cd()
    lower_pad = TPad("lowerpad_"+var_name, "lowerpad_"+var_name, 0, 0, 1,0.25)
    lower_pad.SetTopMargin(0.04)
    lower_pad.SetBottomMargin(0.3)
    lower_pad.SetGridy()
    lower_pad.Draw()
    lower_pad.cd()
    ratio_plot.Draw('AP')


def SaveCanvPic(canv, save_dir, save_name):
    canv.cd()
    canv.SaveAs(save_dir + '/' + save_name + '.pdf')
    #canv.SaveAs(save_dir + '/' + save_name + '.png')





