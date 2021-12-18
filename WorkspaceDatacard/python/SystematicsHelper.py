#
# grabbing systematics. 
# SysConfig contains general systematics from yellow report
# getSys get files 
#
# general recommandations for datacard conventions can be found at
# https://twiki.cern.ch/twiki/bin/view/CMS/HiggsWG/HiggsCombinationConventions


import os
import sys

import ROOT as R
import ROOT.RooFit as RF

sys.path.insert(1, '%s/../FitBackground/python' % os.getcwd())



# function to convert names from UF convention to HiggsCombine convention
def SysNameConversion(sys_name): 
    if   sys_name == 'JES': 	      return 'CMS_scale_j'
    elif 'PU_wgt' in sys_name:        return sys_name.replace('PU_wgt', 'CMS_pileup')
    elif 'prefiring' in sys_name:     return sys_name
    elif sys_name == 'Roch':          return 'CMS_scale_m'
    elif sys_name == 'IsoMu_SF':      return 'CMS_trig_m'
    elif sys_name == 'LepMVA_SF_mu':  return 'CMS_lepmva_eff_m'
    elif sys_name == 'LepMVA_SF_ele': return 'CMS_lepmva_eff_e'
    elif sys_name == 'B_SF':          return 'CMS_eff_btag'
    else:
        if   sys_name == 'FlavorQCD':      return 'CMS_scale_j_' + 'flavorQCD'
        elif sys_name == 'RelativeBal':    return 'CMS_scale_j_' + 'relativeBal'
        elif 'RelativeSample' in sys_name: return 'CMS_scale_j_' + sys_name.replace('RelativeSample', 'relativeSample')
        else:	                           return 'CMS_scale_j_' + sys_name.lower()  # remove else: 'invalid', assume all others are JEC uncerts
## End function SysNameConversion(sys_name):




#################################################
##  class to store values for all systematics  ##
#################################################
class SystematicsConfig:
    def __init__(self, _signals, _exp_sys, _hists_sys):

	self.signals    = _signals
	self.sys_names  = [] # complete list of relavant systematics, it is a superset of what is in the xml config files
	self.sys_values = {}

	## check whether the signal is split by year
        self.sig_split = False
        if '_201' in _signals[-1]: self.sig_split = True

	if _exp_sys[0] == 'Norminal':  self.sys_names = ['Norminal']
	else:  
	  self.LoadSystematics(_exp_sys, _hists_sys)

    def LoadSystematics(self, exp_sys, hists_sys):

	################################
	##  load all signal channels  ##
	################################	
        if self.signals == []:
	  for year in ['2016', '2017', '2018']:
	    self.signals.append('ggH_'     + year)
	    self.signals.append('VBF_'     + year)
	    self.signals.append('WH_pos_'  + year)
	    self.signals.append('WH_neg_'  + year)
	    self.signals.append('ZH_'      + year)
	    self.signals.append('ttH_'     + year)


	##########################################
	## get values for different systematics ##
	##########################################

	#-----------------------------------
	# luminosity systematics
	# from Lumi POG https://twiki.cern.ch/twiki/bin/view/CMS/TWikiLUM#LumiComb
	#-----------------------------------

        if self.sig_split:
   	    # luminosity sum of the uncorrelated parts for 2016
   	    self.sys_names.append('lumi_2016_uncor')
   	    self.sys_values['lumi_2016_uncor'] = {}
   	    for signal in self.signals:
   	        if '2016' in signal: self.sys_values['lumi_2016_uncor'][signal] = '1.022'
   	        else:		 self.sys_values['lumi_2016_uncor'][signal] = '-'

	    # luminosity sum of the uncorrelated parts for 2017	
	    self.sys_names.append('lumi_2017_uncor')
            self.sys_values['lumi_2017_uncor'] = {}
            for signal in self.signals:
                if '2017' in signal: self.sys_values['lumi_2017_uncor'][signal] = '1.020'
	        else:		 self.sys_values['lumi_2017_uncor'][signal] = '-'

	    # luminosity sum of the uncorrelated parts for 2018      
            self.sys_names.append('lumi_2018_uncor')
            self.sys_values['lumi_2018_uncor'] = {}
            for signal in self.signals:
                if '2018' in signal: self.sys_values['lumi_2018_uncor'][signal] = '1.015'
	        else:		 self.sys_values['lumi_2018_uncor'][signal] = '-'

	    # X-Y factorization      
            self.sys_names.append('lumi_XY')
            self.sys_values['lumi_XY'] = {}
            for signal in self.signals:
                if   '2016' in signal: self.sys_values['lumi_XY'][signal] = '1.009'
	        elif '2017' in signal: self.sys_values['lumi_XY'][signal] = '1.008'
	        elif '2018' in signal: self.sys_values['lumi_XY'][signal] = '1.020'
	        else:		   self.sys_values['lumi_XY'][signal] = '-'

	    # length scale 17-18      
            self.sys_names.append('lumi_LengScale')
            self.sys_values['lumi_LengScale'] = {}
            for signal in self.signals:
	        if   '2017' in signal: self.sys_values['lumi_LengScale'][signal] = '1.003'
	        elif '2018' in signal: self.sys_values['lumi_LengScale'][signal] = '1.002'
	        else:		   self.sys_values['lumi_LengScale'][signal] = '-'

	    # Beam-beam deflection 15-17     
            self.sys_names.append('lumi_BeamDef')
            self.sys_values['lumi_BeamDef'] = {}
            for signal in self.signals:
	        if   '2016' in signal: self.sys_values['lumi_BeamDef'][signal] = '1.004'
	        elif '2017' in signal: self.sys_values['lumi_BeamDef'][signal] = '1.004'
	        else: 		   self.sys_values['lumi_BeamDef'][signal] = '-'           

	    # dynamic beta 15-17      
            self.sys_names.append('lumi_DynaBeta')
            self.sys_values['lumi_DynaBeta'] = {}
            for signal in self.signals:
                if   '2016' in signal: self.sys_values['lumi_DynaBeta'][signal] = '1.005'
	        elif '2017' in signal: self.sys_values['lumi_DynaBeta'][signal] = '1.005'
	        else:		   self.sys_values['lumi_DynaBeta'][signal] = '-'

 	    # Beam current calibration 17-18      
            self.sys_names.append('lumi_BeamCal')
            self.sys_values['lumi_BeamCal'] = {}
            for signal in self.signals:
                if   '2017' in signal: self.sys_values['lumi_BeamCal'][signal] = '1.003'
	        elif '2018' in signal: self.sys_values['lumi_BeamCal'][signal] = '1.002'
	        else:		   self.sys_values['lumi_BeamCal'][signal] = '-'

	    # Ghosts and satellites 15-17      
            self.sys_names.append('lumi_GhostSat')
            self.sys_values['lumi_GhostSat'] = {}
            for signal in self.signals:
                if   '2016' in signal: self.sys_values['lumi_GhostSat'][signal] = '1.004'
	        elif '2017' in signal: self.sys_values['lumi_GhostSat'][signal] = '1.001'
	        else:		   self.sys_values['lumi_GhostSat'][signal] = '-'

        else:  # signal not split by year
            # lumi 2016
            self.sys_names.append('lumi_13TeV_2016')
            self.sys_values['lumi_13TeV_2016'] = {}
            for signal in self.signals:
                self.sys_values['lumi_13TeV_2016'][signal] = '1.007'
            
            # lumi 2017
            self.sys_names.append('lumi_13TeV_2017')
            self.sys_values['lumi_13TeV_2017'] = {}
            for signal in self.signals:
                self.sys_values['lumi_13TeV_2017'][signal] = '1.007'
      
            # lumi 2018
            self.sys_names.append('lumi_13TeV_2018')
            self.sys_values['lumi_13TeV_2018'] = {}
            for signal in self.signals:
                self.sys_values['lumi_13TeV_2018'][signal] = '1.011'

	#---------------------------------
	# theoretical and phenomenological systematics
	# from CERN Yellow Report CERN-2017-002-M
	#---------------------------------

	# Branching Rato
	self.sys_names.append('BR_hmm')
	self.sys_values['BR_hmm'] = {}
	for signal in self.signals:
	    self.sys_values['BR_hmm'][signal] = '1.012'
	
	# cross sections
	self.sys_names.append('QCDscale_ggH')
	self.sys_values['QCDscale_ggH'] = {}
	for signal in self.signals:
	    if 'ggH' in signal: self.sys_values['QCDscale_ggH'][signal] = '1.039'
	    else:		self.sys_values['QCDscale_ggH'][signal] = '-'

	#
	self.sys_names.append('QCDscale_VBF')
        self.sys_values['QCDscale_VBF'] = {}
        for signal in self.signals:
            if 'VBF' in signal: self.sys_values['QCDscale_VBF'][signal] = '0.997/1.004'
            else:               self.sys_values['QCDscale_VBF'][signal] = '-'

	#
	self.sys_names.append('QCDscale_WH')
        self.sys_values['QCDscale_WH'] = {}
        for signal in self.signals:
            if 'WH' in signal: self.sys_values['QCDscale_WH'][signal] = '0.993/1.005'  # correlated for WH_pos and WH_neg
            else:              self.sys_values['QCDscale_WH'][signal] = '-'

	#
	self.sys_names.append('QCDscale_qqZH')
        self.sys_values['QCDscale_qqZH'] = {}
        for signal in self.signals:
            if 'qqZH' in signal: self.sys_values['QCDscale_qqZH'][signal] = '0.994/1.005'
            else:                self.sys_values['QCDscale_qqZH'][signal] = '-'

        #
        self.sys_names.append('QCDscale_ggZH')
        self.sys_values['QCDscale_ggZH'] = {}
        for signal in self.signals:
            if 'ggZH' in signal: self.sys_values['QCDscale_ggZH'][signal] = '0.811/1.251'
            else:                self.sys_values['QCDscale_ggZH'][signal] = '-'

	#
	self.sys_names.append('QCDscale_ttH')
        self.sys_values['QCDscale_ttH'] = {}
        for signal in self.signals:
            if 'ttH' in signal: self.sys_values['QCDscale_ttH'][signal] = '0.908/1.058'
            else:               self.sys_values['QCDscale_ttH'][signal] = '-'

        #
        self.sys_names.append('QCDscale_tHq')
        self.sys_values['QCDscale_tHq'] = {}
        for signal in self.signals:
            if 'THQ' in signal: self.sys_values['QCDscale_tHq'][signal] = '0.851/1.065'
            else:               self.sys_values['QCDscale_tHq'][signal] = '-'

        #
        self.sys_names.append('QCDscale_tHW')
        self.sys_values['QCDscale_tHW'] = {}
        for signal in self.signals:
            if 'THW' in signal: self.sys_values['QCDscale_tHW'][signal] = '0.933/1.049'
            else:               self.sys_values['QCDscale_tHW'][signal] = '-'

	#
	self.sys_names.append('pdf_Higgs_gg')
        self.sys_values['pdf_Higgs_gg'] = {}
        for signal in self.signals:
            if 'ggH'  in signal: self.sys_values['pdf_Higgs_gg'][signal] = '1.032'
            if 'ggZH' in signal: self.sys_values['pdf_Higgs_gg'][signal] = '1.024'
            else:                self.sys_values['pdf_Higgs_gg'][signal] = '-'

	#
	self.sys_names.append('pdf_Higgs_qqbar')
        self.sys_values['pdf_Higgs_qqbar'] = {}
        for signal in self.signals:
            if   'VBF'  in signal: self.sys_values['pdf_Higgs_qqbar'][signal] = '0.979/1.021'
	    elif 'WH'   in signal: self.sys_values['pdf_Higgs_qqbar'][signal] = '1.019'
	    elif 'qqZH' in signal: self.sys_values['pdf_Higgs_qqbar'][signal] = '1.019'
            else:                  self.sys_values['pdf_Higgs_qqbar'][signal] = '-'

	#
	self.sys_names.append('pdf_Higgs_ttH')
        self.sys_values['pdf_Higgs_ttH'] = {}
        for signal in self.signals:
            if   'ttH' in signal: self.sys_values['pdf_Higgs_ttH'][signal] = '1.036'
            else:                 self.sys_values['pdf_Higgs_ttH'][signal] = '-'

        #
        self.sys_names.append('pdf_Higgs_tHq')
        self.sys_values['pdf_Higgs_tHq'] = {}
        for signal in self.signals:
            if 'THQ' in signal:   self.sys_values['pdf_Higgs_tHq'][signal] = '1.037'
            else:                 self.sys_values['pdf_Higgs_tHq'][signal] = '-'

        #
        self.sys_names.append('pdf_Higgs_tHW')
        self.sys_values['pdf_Higgs_tHW'] = {}
        for signal in self.signals:
            if 'THW' in signal:   self.sys_values['pdf_Higgs_tHW'][signal] = '1.063'
            else:                 self.sys_values['pdf_Higgs_tHW'][signal] = '-'


        #---------------------------------
        # experimental systematics
        # from up/down shifted histograms
        #---------------------------------
	for sys_name in exp_sys:
	    self.getSystematicShifts(sys_name, hists_sys)

    ## End function: LoadSystematics(self):



    ###############################################
    ##  function to calculate systematic values  ##
    ##  from up/down histogram yields            ##
    ###############################################

    def getSystematicShifts(self, sys_name, hists_sys): # hists_sys should be a dictionary, intended to be the same as in DataLoader
	HC_sys_name = SysNameConversion(sys_name)
	if HC_sys_name == 'invalid':
	    print 'Not valid systematic name: %s'  %sys_name
	    return

	hist_keys = hists_sys.keys()
	print hist_keys
	print '\n\n'

        self.sys_names.append(HC_sys_name)
        self.sys_values[HC_sys_name] = {}
        for signal in self.signals:
        # loop through the given list regardless of whether they are merged or split
        # correlate same systematics in different years, even if signals are split by year

            if (signal + '_noSys') not in hist_keys or (signal + '_' + sys_name + '_up') not in hist_keys or (signal + '_' + sys_name + '_down') not in hist_keys:
            # some hists do not exist in the given collection    
                print 'cannot find %s hists\n' %signal
                self.sys_values[HC_sys_name][signal] = '-'

            else: # all shifted hists found for this signal
                noSys_intg = hists_sys[signal + '_noSys'].Integral()
                up_intg    = hists_sys[signal + '_' + sys_name + '_up'].Integral()
                down_intg  = hists_sys[signal + '_' + sys_name + '_down'].Integral()

                if hists_sys[signal + '_noSys'].GetEntries() < 100:   # hist do not have enough events to make a reasonable signal modeling
                    print 'hist %s only have %d entries' %(signal, hists_sys[signal + '_noSys'].GetEntries())
                    self.sys_values[HC_sys_name][signal] = '-'
                else:     # found proper values to set
                    self.sys_values[HC_sys_name][signal] = '%.3f/%.3f' %( down_intg/noSys_intg, up_intg/noSys_intg )


#	for year in ['2016', '2017', '2018']:
#          if HC_sys_name == 'CMS_eff_m_mva' and year == '2018': # do not have separate row for lepMVA 2018
#            continue
#	  self.sys_names.append(HC_sys_name + '_' + year)
#	  self.sys_values[HC_sys_name + '_' + year] = {}
#
#	  for signal in self.signals:
#	    if year not in signal:	
#	    # systematics in this year does not effect signals in other years
#		self.sys_values[HC_sys_name + '_' + year][signal] = '-'  
#
#	    elif (signal + '_noSys') not in hist_keys or (signal + '_' + sys_name + '_up') not in hist_keys or (signal + '_' + sys_name + '_down') not in hist_keys:
#	    # some hists do not exist in the given collection
#		print 'cannot find %s hists\n' %signal
#		self.sys_values[HC_sys_name + '_' + year][signal] = '-'
#		
#	    else:  # all shifted hists found for this signal
#	        noSys_intg = hists_sys[signal + '_noSys'].Integral()
#		up_intg	   = hists_sys[signal + '_' + sys_name + '_up'].Integral()
#		down_intg  = hists_sys[signal + '_' + sys_name + '_down'].Integral()
#
#		if hists_sys[signal + '_noSys'].GetEntries() < 100:   # hist do not have enough events to make a reasonable signal modeling
#		    print 'hist %s only have %d entries' %(signal, hists_sys[signal + '_noSys'].GetEntries())
#		    self.sys_values[HC_sys_name + '_' + year][signal] = '-'
#		else:     # found proper values to set
#		    self.sys_values[HC_sys_name + '_' + year][signal] = '%.3f/%.3f' %( down_intg/noSys_intg, up_intg/noSys_intg )
#
#	    # End of if, elif, else
#	  # End of for signal in self.signals:
#	# End of for year in ['2016', '2017', '2018']:
#        
#        if HC_sys_name == 'CMS_eff_m_mva': # correlate 2018 lepMVA SF with 2017
#          for signal in self.signals:
#            if '2018' not in signal: continue 
#            elif (signal + '_noSys') not in hist_keys or (signal + '_' + sys_name + '_up') not in hist_keys or (signal + '_' + sys_name + '_down') not in hist_keys:
#                print 'cannot find %s hists\n' %signal
#                self.sys_values[HC_sys_name + '_' + year][signal] = '-'
#            else:
#                noSys_intg = hists_sys[signal + '_noSys'].Integral()
#                up_intg    = hists_sys[signal + '_' + sys_name + '_up'].Integral()
#                down_intg  = hists_sys[signal + '_' + sys_name + '_down'].Integral()
#                if hists_sys[signal + '_noSys'].GetEntries() < 100:   # hist do not have enough events to make a reasonable signal modeling
#                    print 'hist %s only have %d entries' %(signal, hists_sys[signal + '_noSys'].GetEntries())
#                    self.sys_values[HC_sys_name + '_' + '2017'][signal] = '-'
#                else:     # found proper values to set
#                    self.sys_values[HC_sys_name + '_' + '2017'][signal] = '%.3f/%.3f' %( down_intg/noSys_intg, up_intg/noSys_intg )


    ## End function: getSystematicShifts(self, sys_name, hists_sys):











