
## Helper functions to modify existing fits

import os
import sys

import ROOT as R
import ROOT.RooFit as RF

sys.path.insert(1, '%s/../FitBackground/python' % os.getcwd())
import FitFunctions as FF  ## From FitBackground/python/FitFunctions.py


def FreezeParams(fit, to_freeze):

    ## Freeze fit coefficients
    if 'Coefs' in to_freeze:
        # print '\nFreezing coefficients:'
        for i in range(len(fit.coef_list)):
            # print fit.coef_list[i].GetName()
            fit.coef_list[i].setConstant(R.kTRUE)

    ## Freeze other shape parameters
    if 'Params' in to_freeze:
        # print '\nFreezing parameters:'
        for j in range(len(fit.params[0])):
            for i in range(len(fit.params)):
                # print fit.params[i][j].GetName()
                if hasattr(fit.params[i][j], 'setConstant'): fit.params[i][j].setConstant(R.kTRUE) ## Some are RooFormulaVar, no need to freeze
                

    ## allow shape systematics for signal
    ## NOTE: it should only be applied to DSCB signal model!
    if 'param_sys' in to_freeze:
	for j in range(len(fit.params[0])):
            for i in range(len(fit.params)):

                print "%s = %f\n" % ( fit.params[i][j].GetName(), fit.params[i][j].getValV() ) 		
		if '_mean' in fit.params[i][j].GetName(): 
                    fit.params[i][j].setConstant(R.kTRUE)
#		    rel_uncert = 0.0016  # ~ 0.2 GeV on mean
#		    var_value  = fit.params[i][j].getValV()
#		    fit.params[i][j].setRange( var_value * (1.0-rel_uncert), var_value * (1.0+rel_uncert) )
		elif '_sigma' in fit.params[i][j].GetName(): 
                    fit.params[i][j].setConstant(R.kTRUE)
#		    rel_uncert = 0.02  # 2% on width
#                    var_value  = fit.params[i][j].getValV()
#                    fit.params[i][j].setRange( var_value * (1.0-rel_uncert), var_value * (1.0+rel_uncert) )
		else:
		    fit.params[i][j].setConstant(R.kTRUE)


## End function: def FreezeParams(fit, mask):


