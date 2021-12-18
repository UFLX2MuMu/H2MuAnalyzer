################################################################
###    make mass calibration plots from mass histograms      ###
################################################################

## Basic python includes for manipulating files
import os
import sys

from ROOT import *
sys.path.insert(0, '%s/../MakeHistos/python' % os.getcwd())
from SampleDatabase import GetSamples


gROOT.SetBatch(True)

## Configure the script user
if 'abrinke1' in os.getcwd(): USER = 'abrinke1'
if 'bortigno' in os.getcwd(): USER = 'bortigno'
if 'xzuo'     in os.getcwd(): USER = 'xzuo'

YEAR = '2016'
## Directory for input histograms and output plots
if USER == 'abrinke1': PLOT_DIR = '/afs/cern.ch/work/a/abrinke1/public/H2Mu/%s/Histograms' % YEAR
if USER == 'xzuo':     PLOT_DIR = '/afs/cern.ch/work/x/xzuo/public/H2Mu/%s/Histograms' % YEAR

LABEL = 'Btag_eff_cal_3l_2019_10_09'

def main():
    file_dir = PLOT_DIR+"/"+LABEL
    in_file  = TFile.Open( file_dir + "/all_samples" + ".root", "READ")
    out_file = open('Btag_efficiency_%s.csv' %YEAR, 'w')

    sample_info = GetSamples('CERN_3l', YEAR)
    samples = []
    for samp in sample_info:
	if samp.evt_type != 'Data': samples.append(samp.name)
    print samples

    jet_flavors = ['FLAV_B', 'FLAV_C', 'FLAV_UDSG']

    out_file.write('sample, JetFlavor, TotalNumberJets, LooseEff, MediumEff \n')
    for sample in samples:
      for JF in jet_flavors:
	print 'doing %s, %s' % (sample, JF)
	if not in_file.GetListOfKeys().Contains( sample + '_nJets_' + JF + '_all' ):
	  nJets_raw = 0
	  eff_tagL  = 0.0
	  eff_tagM  = 0.0
	else:
	  nJets_raw   = in_file.Get( sample + '_nJets_' + JF + '_all').GetEntries()
	  nJets_wgted = in_file.Get( sample + '_nJets_' + JF + '_all').Integral()
	  if not in_file.GetListOfKeys().Contains( sample + '_nJets_' + JF + '_tagL' ):
	    nJets_tagL = 0.0
	  else:
	    nJets_tagL  = in_file.Get( sample + '_nJets_' + JF + '_tagL').Integral()
	  if not in_file.GetListOfKeys().Contains( sample + '_nJets_' + JF + '_tagM' ):
	    nJets_tagM = 0.0
	  else:
	    nJets_tagM  = in_file.Get( sample + '_nJets_' + JF + '_tagM').Integral()

	  eff_tagL = nJets_tagL / nJets_wgted
	  eff_tagM = nJets_tagM / nJets_wgted

	out_file.write( '%s, %s, %d, %f, %f \n' %(sample, JF, nJets_raw, eff_tagL, eff_tagM) ) 

    out_file.close()




main()
