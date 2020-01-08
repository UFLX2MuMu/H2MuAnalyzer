import ROOT as R


SMALL_VALUE = 0.05  ## an arbitrary value as the threshold for small values in bins

## similar function should be applied to the StackPlot making for each MC sample.
def Hist_RobustUncert(hist_in, new_name):
    hist  = hist_in.Clone(new_name)
    nbins = hist.GetNbinsX()
    for ibin in range(1, nbins+1):
	if abs(hist.GetBinContent(ibin)) < SMALL_VALUE and abs(hist.GetBinError(ibin)) < SMALL_VALUE:
	    neighbor_yield = 0
	    neighbor_count = 0
	    for neighbor in range( max(1,ibin-2), min(ibin+2,nbins)+1 ):
		neighbor_yield += abs(hist.GetBinContent(neighbor))   ## Some bins have negative values. Take them as positive for a conservative estimation
		neighbor_count += 1
	    avg_yield = 1.0 * neighbor_yield / neighbor_count
	    hist.SetBinError(ibin, avg_yield)
    return hist


## A calculation used in the function Hist_SmoothShape(), to get the intrinsic chi^2 of a histogram 
def Chi_Square(hist):
    nbins = hist.GetNbinsX()
    chi2 = 0
    for ibin in range(1, nbins):
	val_diff = abs( hist.GetBinContent(ibin+1) - hist.GetBinContent(ibin) )
	val_err  = hist.GetBinError(ibin) + hist.GetBinError(ibin+1)
	if val_err == 0: continue
        chi2 += (val_diff ** 2.0) / (val_err ** 2.0)
    return chi2


def Hist_SmoothShape(hist):
    h_name = hist.GetName()
    rebin = 1
    hist_rebin = hist.Clone(h_name+"_rebin1")

    if Chi_Square(hist_rebin) > hist_rebin.GetNbinsX():
	rebin = 2
	hist_rebin = hist.Rebin(2,h_name + "_rebin2")
    if Chi_Square(hist_rebin) > hist_rebin.GetNbinsX():
	rebin = 5
        hist_rebin = hist.Rebin(5,h_name + "_rebin5")
    if Chi_Square(hist_rebin) > hist_rebin.GetNbinsX():
	rebin = 10
        hist_rebin = hist.Rebin(10,h_name + "_rebin10")
    if Chi_Square(hist_rebin) > hist_rebin.GetNbinsX():
	print "Histogram %s has large fluctuation and small error. Smoothing histogram failed. stopped at rebin(10)" %h_name

    return hist.Rebin(rebin)    



