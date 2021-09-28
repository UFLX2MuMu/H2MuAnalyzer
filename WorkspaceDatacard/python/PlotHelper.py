
## Functions to print analytic fits to data and MC

import os
import sys

import ROOT as R
import ROOT.RooFit as RF

sys.path.insert(1, '%s/../FitBackground/python' % os.getcwd())
import FitFunctions as FF  ## From FitBackground/python/FitFunctions.py


def DrawFits(sig_fits, bkg_fit, data_fit, cat, out_dir):

    #-------------------------------------------------------------------
    ## Plot data and fit into a frame

    R.gStyle.SetPadBottomMargin(0.13)
    R.gStyle.SetPadLeftMargin(0.12)

    c_sigs = []
    c_bkg  = R.TCanvas('c_%s_bkg'  % cat, 'c_%s_bkg'  % cat, 600, 600)
    c_data = R.TCanvas('c_%s_data' % cat, 'c_%s_data' % cat, 600, 600)

    cms_latex = R.TLatex()
    cms_latex.SetTextAlign(11)
    cms_latex.SetTextSize(0.022)

    ## Signal
    for sig_fit in sig_fits:
	c_sigs.append(  R.TCanvas('c_%s_%s'  %(cat, sig_fit.name) , 'c_%s_%s'  %(cat, sig_fit.name), 600, 600)  )
        c_sigs[-1].cd()
        fra_sig = sig_fit.var.frame()
        sig_fit.dat  .plotOn (fra_sig)
        sig_fit.model.plotOn (fra_sig, RF.LineColor(R.kBlue), RF.Range('FULL'))
        ## Plot sub-components of signal model
        if len(sig_fit.arg_sets) > 1:
            sig_fit.model.plotOn (fra_sig, RF.Components(sig_fit.arg_sets[0]), RF.LineStyle(R.kDashed), RF.LineColor(R.kGreen),  RF.Range('FULL'))
            sig_fit.model.plotOn (fra_sig, RF.Components(sig_fit.arg_sets[1]), RF.LineStyle(R.kDashed), RF.LineColor(R.kRed),    RF.Range('FULL'))
        if len(sig_fit.arg_sets) > 2:
            sig_fit.model.plotOn (fra_sig, RF.Components(sig_fit.arg_sets[2]), RF.LineStyle(R.kDashed), RF.LineColor(R.kViolet), RF.Range('FULL'))
        sig_fit.model.paramOn(fra_sig, RF.Layout(0.5, 0.90, 0.80))

        fra_sig.SetMaximum(fra_sig.GetMaximum() * 1.2)
        fra_sig.getAttText().SetTextSize(0.023)
        fra_sig.GetXaxis().SetTitle('m_{#mu#mu} (GeV)')
        fra_sig.GetYaxis().SetTitle('Events / 0.5 GeV')
        fra_sig.GetYaxis().SetTitleOffset(1.7)
        fra_sig.SetTitle('')
        fra_sig.Draw()
        sig_chi = R.TLatex(0.65, 0.23, "#chi^{2} = %.2f" % fra_sig.chiSquare())
        sig_chi.SetNDC(R.kTRUE)
        sig_chi.Draw()
        cms_latex.DrawLatexNDC(0.16, 0.82, '#scale[2.0]{CMS #bf{#it{Preliminary Simulation}}}')
        cms_latex.DrawLatexNDC(0.67, 0.92,'#font[42]{#scale[1.5]{137 fb^{-1} (13 TeV)}}')
        c_sigs[-1].SaveAs(out_dir+'/plot/%s_fit_%s_%d.png' % (sig_fit.name, sig_fit.fit_type, sig_fit.order))

    ## Background
    c_bkg.cd()
    fra_bkg = bkg_fit.var.frame()
    bkg_fit.dat  .plotOn(fra_bkg)
    bkg_fit.model.plotOn(fra_bkg, RF.LineColor(R.kBlue), RF.Range('FULL'))
    ## Plot sub-components of background model
    if len(bkg_fit.arg_sets) > 1:
        bkg_fit.model.plotOn(fra_bkg, RF.Components(bkg_fit.arg_sets[0]), RF.LineStyle(R.kDashed), RF.LineColor(R.kGreen),  RF.Range('FULL'))
        bkg_fit.model.plotOn(fra_bkg, RF.Components(bkg_fit.arg_sets[1]), RF.LineStyle(R.kDashed), RF.LineColor(R.kRed),    RF.Range('FULL'))
    if len(bkg_fit.arg_sets) > 2:
        bkg_fit.model.plotOn(fra_bkg, RF.Components(bkg_fit.arg_sets[2]), RF.LineStyle(R.kDashed), RF.LineColor(R.kViolet), RF.Range('FULL'))
    bkg_fit.model.paramOn(fra_bkg, RF.Layout(0.55, 0.90, 0.80))
 
    fra_bkg.SetMaximum(fra_bkg.GetMaximum() * 1.2) 
    fra_bkg.getAttText().SetTextSize(0.023)
    fra_bkg.GetXaxis().SetTitle('m_{#mu#mu} (GeV)')
    fra_bkg.GetYaxis().SetTitle('Events / 0.5 GeV')
    fra_bkg.GetYaxis().SetTitleOffset(1.7)
    fra_bkg.SetTitle('')
    fra_bkg.Draw()
    bkg_chi = R.TLatex(0.65, 0.5, "#chi^{2} = %.2f" % fra_bkg.chiSquare())
    bkg_chi.SetNDC(R.kTRUE)
    bkg_chi.Draw()
    cms_latex.DrawLatexNDC(0.16, 0.82, '#scale[2.0]{CMS #bf{#it{Preliminary Simulation}}}')
    cms_latex.DrawLatexNDC(0.67, 0.92,'#font[42]{#scale[1.5]{137 fb^{-1} (13 TeV)}}')
    c_bkg.SaveAs(out_dir+'/plot/%s_fit_%s_%d.png' % (bkg_fit.name, bkg_fit.fit_type, bkg_fit.order))

    ## Data
    c_data.cd()
    fra_data = data_fit.var.frame()
    hist_temp = data_fit.hist_orig.Clone("h_temp" + data_fit.name)
    nBins = hist_temp.GetNbinsX()
    # blind data in plots
#    for i in range(1, nBins+1):
#        if hist_temp.GetBinLowEdge(i) >= 120 and hist_temp.GetBinLowEdge(i) < 130:
#            hist_temp.SetBinContent(i, 0)
    dat_blind = R.RooDataHist('dat_blind'+data_fit.name, 'dat_blind'+data_fit.name, R.RooArgList(R.RooArgSet(data_fit.var)), hist_temp)
    dat_blind.plotOn(fra_data)
    data_fit.model.plotOn(fra_data, RF.LineColor(R.kBlue), RF.Range('FULL'))
    ## Plot sub-components of data model
    if len(bkg_fit.arg_sets) > 1:
        data_fit.model.plotOn(fra_data, RF.Components(data_fit.arg_sets[0]), RF.LineStyle(R.kDashed), RF.LineColor(R.kGreen),  RF.Range('FULL'))
        data_fit.model.plotOn(fra_data, RF.Components(data_fit.arg_sets[1]), RF.LineStyle(R.kDashed), RF.LineColor(R.kRed),    RF.Range('FULL'))
    if len(data_fit.arg_sets) > 2:
        data_fit.model.plotOn(fra_data, RF.Components(data_fit.arg_sets[2]), RF.LineStyle(R.kDashed), RF.LineColor(R.kViolet), RF.Range('FULL'))
    data_fit.model.paramOn(fra_data, RF.Layout(0.55, 0.90, 0.80))

    fra_data.SetMaximum(fra_data.GetMaximum() * 1.2)
    fra_data.getAttText().SetTextSize(0.023)
    fra_data.GetXaxis().SetTitle('m_{#mu#mu} (GeV)')
    fra_data.GetYaxis().SetTitle('Events / 0.5 GeV')
    fra_data.GetYaxis().SetTitleOffset(1.7)
    fra_data.SetTitle('')
    fra_data.Draw()
    data_chi = R.TLatex(0.65, 0.5, "#chi^{2} = %.2f" % fra_data.chiSquare())
    data_chi.SetNDC(R.kTRUE)
    data_chi.Draw()
    cms_latex.DrawLatexNDC(0.16, 0.82, '#scale[2.0]{CMS #bf{#it{Preliminary}}}')
    cms_latex.DrawLatexNDC(0.67, 0.92,'#font[42]{#scale[1.5]{137 fb^{-1} (13 TeV)}}')
    c_data.SaveAs(out_dir+'/plot/%s_fit_%s_%d.png' % (data_fit.name, data_fit.fit_type, data_fit.order))

## End function: def DrawFits(sig_fits, bkg_fit, data_fit, cat):

