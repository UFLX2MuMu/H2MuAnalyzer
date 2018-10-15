
#include "H2MuAnalyzer/MakeHistos/interface/HistoHelper.h"

// Book a 1D histogram (defaults to TH1D)
TH1 * BookHisto(TString h_name, int nBins, float min, float max, TString opt1) {
  // std::cout << "  * Inside BookHisto(" << h_name << ", " << nBins << ", " << min << ", " << max << ", " << opt1 << ")" << std::endl;
  if      (opt1.CompareTo("TH1D") == 0) return new TH1D(h_name, h_name, nBins, min, max);
  else if (opt1.CompareTo("TH1F") == 0) return new TH1F(h_name, h_name, nBins, min, max);
  else { std::cout << "BookHisto option " << opt1 << " is neither TH1D nor TH1F - exiting" << std::endl; return 0; }
  
}
// Book a 2D histogram (defaults to TH2D)
TH2 * BookHisto(TString h_name, int nBinsX, float minX, float maxX, int nBinsY, float minY, float maxY, TString opt1) {
  if      (opt1.CompareTo("TH2D") == 0) return new TH2D(h_name, h_name, nBinsX, minX, maxX, nBinsY, minY, maxY);
  else if (opt1.CompareTo("TH2F") == 0) return new TH2F(h_name, h_name, nBinsX, minX, maxX, nBinsY, minY, maxY);
  else { std::cout << "BookHisto option " << opt1 << " is neither TH2D nor TH2F - exiting" << std::endl; return 0; }

}

// Fill a 1D histogram
void FillHisto(TH1 * hist, double val, float weight, bool overflow) {
  // std::cout << "    * Filling " << hist->GetName() << " with value " << val << std::endl;

  if (overflow) {
    // Set value of "val" inside histogram x-axis range
    int bin_lo = 1;
    int bin_hi = hist->GetNbinsX() + 1;
    if (val < hist->GetXaxis()->GetBinLowEdge(bin_lo)) val = hist->GetXaxis()->GetBinLowEdge(bin_lo) + 0.1 * hist->GetBinWidth(bin_lo);
    if (val > hist->GetXaxis()->GetBinLowEdge(bin_hi)) val = hist->GetXaxis()->GetBinLowEdge(bin_hi) - 0.1 * hist->GetBinWidth(bin_hi);
  }

  hist->Fill(val, weight);
}
// Fill a 2D histogram
void FillHisto(TH2 * hist, double valX, double valY, float weight, bool overflow) { 
  // std::cout << "    * Filling " << hist->GetName() << " with values " << valX << ", " << valY << std::endl;

  if (overflow) {
    // Set values of "valX" and "valY" inside histogram x-axis and y-axis ranges
    int binX_lo = 1;
    int binX_hi = hist->GetNbinsX() + 1;
    if (valX < hist->GetXaxis()->GetBinLowEdge(binX_lo)) valX = hist->GetXaxis()->GetBinLowEdge(binX_lo) + 0.1 * hist->GetBinWidth(binX_lo);
    if (valX > hist->GetXaxis()->GetBinLowEdge(binX_hi)) valX = hist->GetXaxis()->GetBinLowEdge(binX_hi) - 0.1 * hist->GetBinWidth(binX_hi);
    int binY_lo = 1;
    int binY_hi = hist->GetNbinsY() + 1;
    if (valY < hist->GetYaxis()->GetBinLowEdge(binY_lo)) valY = hist->GetYaxis()->GetBinLowEdge(binY_lo) + 0.1 * hist->GetBinWidth(binY_lo);
    if (valY > hist->GetYaxis()->GetBinLowEdge(binY_hi)) valY = hist->GetYaxis()->GetBinLowEdge(binY_hi) - 0.1 * hist->GetBinWidth(binY_hi);
  }

  hist->Fill(valX, valY, weight);
}

// Book a 1D histogram (if it does not already exist), then fill it
void BookAndFill( std::map<TString, TH1*> & h1_map, TString h_name,
		  int nBins, float min, float max,
		  double val, float weight, bool overflow ) {
  // std::cout << "\nInside BookAndFill(" << h_name << ", " << nBins << ", " << min << ", " << max << ", " << val << ")" << std::endl;
  if (h1_map.find(h_name) == h1_map.end()) {
    // std::cout << "  * Did not find histogram in map, now booking ... " << std::endl;
    TH1 * hist = BookHisto(h_name, nBins, min, max);
    h1_map[h_name] = hist;
    // std::cout << "  * ... booked!" << std::endl;
  }
  FillHisto(h1_map.find(h_name)->second, val, weight, overflow);
  // for (const auto it : h_map_1D) std::cout << "Inside HistoHelper, map at " << it.first << " = " << it.second->Integral() << std::endl;
}
// Book a 2D histogram (if it does not already exist), then fill it
void BookAndFill( std::map<TString, TH2*> & h2_map, TString h_name,
		  int nBinsX, float minX, float maxX,
		  int nBinsY, float minY, float maxY,
		  double valX, double valY, float weight, bool overflow ) {
  if (h2_map.find(h_name) == h2_map.end()) {
    TH2 * hist = BookHisto(h_name, nBinsX, minX, maxX, nBinsY, minY, maxY);
    h2_map[h_name] = hist;
  }
  FillHisto(h2_map.find(h_name)->second, valX, valY, weight, overflow);
}


//Initialize a default set of histos for data/MC comparison
//when using this function, be careful that the histograms filled later on has to have the same names and binnings as listed here
void BookForMCvsData(std::map<TString, TH1*> & h1_map, std::string sample, std::vector<std::string> OPT_CUTS, std::vector<std::string> CAT_CUTS) {
  if ( ! h1_map.empty() ) {
    std::cout << "map not empty, cannot initialize" << std::endl;
    return;
  }
  else {
    for (UInt_t iOpt = 0; iOpt < OPT_CUTS.size(); iOpt++) {
        for (UInt_t iCat = 0; iCat < CAT_CUTS.size(); iCat++) {
            std::string h_pre = sample + "_"+OPT_CUTS.at(iOpt)+"_"+CAT_CUTS.at(iCat)+"_";
	    h1_map[h_pre+"dimuon_mass"]         = BookHisto( h_pre+"dimuon_mass",         50, 110, 160	);
	    h1_map[h_pre+"dimuon_pt"]           = BookHisto( h_pre+"dimuon_pt",           50, 20, 520	);
	    h1_map[h_pre+"dimuon_eta"]          = BookHisto( h_pre+"dimuon_eta",          50, -5, 5	);
	    h1_map[h_pre+"dimuon_delta_eta"]    = BookHisto( h_pre+"dimuon_delta_eta",    50, -5, 5	);
	    h1_map[h_pre+"dimuon_delta_phi"]    = BookHisto( h_pre+"dimuon_delta_phi",    50, -4, 4	);
	    h1_map[h_pre+"dimuon_dR"]    	= BookHisto( h_pre+"dimuon_dR",    	  50, 0, 5      );
	    h1_map[h_pre+"dimuon_d0_diff"]      = BookHisto( h_pre+"dimuon_d0_diff",      50, -0.1, 0.1 );
	    h1_map[h_pre+"leading_muon_pt"]     = BookHisto( h_pre+"leading_muon_pt",     50, 20, 270	);
	    h1_map[h_pre+"leading_muon_eta"]    = BookHisto( h_pre+"leading_muon_eta",    50, -2.5, 2.5	);
	    h1_map[h_pre+"leading_muon_d0"]     = BookHisto( h_pre+"leading_muon_d0",     50, -0.1, 0.1 );
	    h1_map[h_pre+"subleading_muon_pt"]  = BookHisto( h_pre+"subleading_muon_pt",  50, 20, 270	);
	    h1_map[h_pre+"subleading_muon_eta"] = BookHisto( h_pre+"subleading_muon_eta", 50, -2.5, 2.5	);
	    h1_map[h_pre+"subleading_muon_d0"]  = BookHisto( h_pre+"subleading_muon_d0",  50, -0.1, 0.1 );
	    h1_map[h_pre+"dijet_mass_1000"]     = BookHisto( h_pre+"dijet_mass_1000",     50, 0, 1000	);
	    h1_map[h_pre+"dijet_mass_200"]      = BookHisto( h_pre+"dijet_mass_200",      50, 0, 200    ); 
	    h1_map[h_pre+"dijet_pt_800"]        = BookHisto( h_pre+"dijet_pt_800",        50, 0, 600	);
	    h1_map[h_pre+"dijet_pt_200"]        = BookHisto( h_pre+"dijet_pt_200",        50, 0, 200    );
	    h1_map[h_pre+"dijet_eta"]           = BookHisto( h_pre+"dijet_eta",           50, -10, 10	);
	    h1_map[h_pre+"dijet_delta_eta"]     = BookHisto( h_pre+"dijet_delta_eta",     50, -10, 10	);
	    h1_map[h_pre+"dijet_delta_phi"]     = BookHisto( h_pre+"dijet_delta_phi",     50, -4, 4	);
	    h1_map[h_pre+"dijet_dR"]     	= BookHisto( h_pre+"dijet_dR",     	  50, 0, 10     );
	    h1_map[h_pre+"leading_jet_pt"]      = BookHisto( h_pre+"leading_jet_pt",      50, 30, 530	);
	    h1_map[h_pre+"leading_jet_eta"]     = BookHisto( h_pre+"leading_jet_eta",     50, -5, 5	);
	    h1_map[h_pre+"subleading_jet_pt"]   = BookHisto( h_pre+"subleading_jet_pt",   50, 30, 530	);
	    h1_map[h_pre+"subleading_jet_eta"]  = BookHisto( h_pre+"subleading_jet_eta",  50, -5, 5	);
	    h1_map[h_pre+"MET"]                 = BookHisto( h_pre+"MET",                 50, 0, 300	);
	    h1_map[h_pre+"mht_pt"]              = BookHisto( h_pre+"mht_pt",              50, 0, 300    );
	    h1_map[h_pre+"nJets"]               = BookHisto( h_pre+"nJets",               6, -0.5, 5.5	);
	    h1_map[h_pre+"nBJets"]              = BookHisto( h_pre+"nBJets",              6, -0.5, 5.5	);
	    h1_map[h_pre+"nMuons"]              = BookHisto( h_pre+"nMuons",              5, 0, 5	);
	    h1_map[h_pre+"nElectrons"]          = BookHisto( h_pre+"nElectrons",          4, 0, 4	);
	    h1_map[h_pre+"nVertices"]           = BookHisto( h_pre+"nVertices",           50, 0, 100	);
	    h1_map[h_pre+"nPU"]                 = BookHisto( h_pre+"nPU",                 50, 0, 100	);
        }//end of Cat loop
    }//end of Opt loop
  }//end of else
}


