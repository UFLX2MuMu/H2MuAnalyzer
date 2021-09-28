#! /usr/bin/env python

import os,sys
from array import array
import string
from ROOT import *
from CMS_style import setTDRStyle


CATS = ['ZH-Cat0', 'ZH-Cat1', 
        'WH-Cat0', 'WH-Cat1', 'WH-Cat2',
        'ZH-Comb', 'WH-Comb', 'VH-Comb'] 


OBS = [0.65, 1.81,
       0.89, 0.79, 0.64,
       1.92, 1.15, 1.86]

EXPS = [0.05, 0.14,
        0.08, 0.22, 0.33,
        0.15, 0.40, 0.43]


#FILES = {}
#FILES['ZH-Cat0'] = 'higgsCombineZHCat0_BWZ.AsymptoticLimits.mH125.root'
#FILES['ZH-Cat1'] = 'higgsCombineZHCat1_BWZ.AsymptoticLimits.mH125.root'
#FILES['WH-Cat0'] = 'higgsCombineWHCat0_BWZGamma.AsymptoticLimits.mH125.root'
#FILES['WH-Cat1'] = 'higgsCombineWHCat1_BWZ.AsymptoticLimits.mH125.root'
#FILES['WH-Cat2'] = 'higgsCombineWHCat2_BWZ.AsymptoticLimits.mH125.root'
#FILES['ZH-Comb'] = 'higgsCombineZH2cats_0603.AsymptoticLimits.mH125.root'
#FILES['WH-Comb'] = 'higgsCombineWH3cats_0603.AsymptoticLimits.mH125.root'
#FILES['VH-Comb'] = 'higgsCombineVHcats_0603.AsymptoticLimits.mH125.root'


PAVE_WIDTH = 0.34


#def GetLimitFromTree(FileName):
#
#  infile = TFile.Open(FileName)
#  if infile == None:  
#    print "**** ERROR ****"
#    print "file does not exist"
#    print "***************"
#
#  limitTree = infile.Get("limit")
#  if limitTree == None:  
#    print "**** ERROR ****"
#    print "limit tree does not exist"
#    print "***************"
#
#  obs = 0
#  med = 0
#  up1 = 0
#  up2 = 0
#  dn1 = 0
#  dn2 = 0
#
#  for iEntry in range(0, limitTree.GetEntries()):
#    limitTree.GetEntry(iEntry)
#    lim = limitTree.limit
#    quant = limitTree.quantileExpected
#
#    if abs(quant - 0.5) < 1e-5:
#      med = lim
#    if abs(quant - 0.025) < 1e-5:
#      dn2 = lim
#    if abs(quant - 0.16) < 1e-5:
#      dn1 = lim
#    if abs(quant - 0.84) < 1e-5:
#      up1 = lim
#    if abs(quant - 0.975) < 1e-5:
#      up2 = lim
#    if abs(quant + 1) < 1e-5:
#      obs = lim
#
#  return obs, med, up1, up2, dn1, dn2



def main():
  
  canv = TCanvas("canv", "canv", 600, 600)
  setTDRStyle()
  canv.SetLeftMargin(0.15)
  canv.SetTopMargin(0.07)
  canv.SetTicks(1, 1)

  nCats = len(CATS)

  xmin = 0
  xmax = 4.5
  frame = canv.DrawFrame(xmin, 0, xmax, nCats + 2)
  frame.GetYaxis().Set(nCats+2, 0, nCats+2)

  for iCat in range(nCats):
    frame.GetYaxis().SetBinLabel( nCats-iCat, CATS[iCat])

  frame.GetXaxis().SetTitle('Significance')
  frame.GetXaxis().SetLabelSize(0.03)
  frame.GetXaxis().SetTitleSize(0.035)
  frame.GetXaxis().SetTitleOffset(1.1)
  frame.Draw()

  
  marker = {}
  medline = {}
  pave = {}
  numb = {}
  width = PAVE_WIDTH

  for iCat, cat in enumerate(CATS, start=1):
#    obs[cat], med[cat], up1[cat], up2[cat], dn1[cat], dn2[cat] = GetLimitFromTree(FILES[cat])
#    print obs[cat], med[cat], up1[cat], up2[cat], dn1[cat], dn2[cat]

    ybin = nCats - iCat + 1
    marker[cat] = TGraphErrors()
    marker[cat].SetName('mark_'+cat)
    marker[cat].SetPoint(0, OBS[iCat-1], ybin-0.5)
    marker[cat].SetMarkerStyle(21)
    marker[cat].SetMarkerColor(kBlack)

#    medline[cat] = TGraphErrors()
#    medline[cat].SetName('line_'+cat)
#    medline[cat].SetPoint(0, med[cat], ybin-0.5)
#    medline[cat].SetPointError(0, 0, width)
#    medline[cat].SetMarkerStyle(0)
#    medline[cat].SetLineWidth(2)
#    medline[cat].SetLineStyle(2)
#    medline[cat].SetLineColor(kBlack)    


    pave[cat] = TPave(0, ybin-0.5-width, EXPS[iCat-1], ybin-0.5+width)
    pave[cat].SetFillColor(kAzure+6)

    xtext = 2.2
    numb[cat] = TPaveText(xtext, ybin-0.5-0.2, xtext+0.6, ybin-0.5+0.2)
    numb[cat].AddText( "{:.3} ({:.3})".format(OBS[iCat-1], EXPS[iCat-1]) )
    numb[cat].SetTextColor(kBlack)
    numb[cat].SetTextFont(43)
    numb[cat].SetTextSize(15)
    numb[cat].SetFillStyle(0)
    numb[cat].SetBorderSize(0)

    pave[cat].Draw("same")
    marker[cat].Draw("P same")
    numb[cat].Draw("same")

  # End for for iCat, cat in enumerate(CATS, start=1):

  lumi = TLatex()
  lumi.SetTextSize(0.03)
  lumi.SetTextAlign(31)
  lumi.DrawLatexNDC(0.90, 0.95, '#bf{137 fb^{-1} (13 TeV)}')
  lumi.Draw("same")

  cms_label = TLatex()
  cms_label.SetTextSize(0.035)
  cms_label.DrawLatexNDC(0.18, 0.85, '#scale[1.4]{CMS #bf{#it{Preliminary}}}')
  cms_label.Draw("same") 

  sepline = TLine(0, 3, 2.8, 3)
  sepline.SetLineColor(kBlack)
  sepline.Draw("Lsame")


  # make legend
  xstart = 0.7
  yhigh = 0.76
  textGap = 0.03
  linewid = 0.06
 
  leg = TLatex()
  leg.SetNDC()
  leg.SetTextSize(0.03)
  leg.SetTextAlign(12) # centered
  datapoint = TMarker(xstart, yhigh, 21)
  datapoint.SetMarkerColor(kBlack)
  datapoint.SetNDC()
  datapoint.Draw("same")
  leg.DrawLatexNDC( xstart + textGap , yhigh, "#bf{Observed}")

  yhigh = yhigh - linewid
  oneSD = TPave( xstart-0.015, yhigh - 0.02, xstart+0.015, yhigh + 0.02, 0, "NDC")
  oneSD.SetFillColor(kAzure+6)
  oneSD.Draw("same")
  leg.DrawLatexNDC(xstart + textGap , yhigh, "#bf{Expected}")


  canv.Update()
  canv.SaveAs('signif_plot.pdf')



main()
