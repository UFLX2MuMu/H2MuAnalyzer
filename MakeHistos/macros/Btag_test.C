
/////////////////////////////////////////////////////////////
///   Macro to study backgrounds in WH, W-->lv category   ///
///          (WZ, ttW, ttZ, Z+fake, ttbar+fake)           ///
///            Andrew Brinkerhoff  03.09.2018             ///
/////////////////////////////////////////////////////////////

// Basic ROOT includes to read and write files
#include "TFile.h"
#include "TSystem.h"
#include "TChain.h"
#include "TTree.h"
#include "TBranch.h"

#include <memory>

#include "H2MuAnalyzer/MakeHistos/interface/LoadNTupleBranches.h" // List of branches in the NTuple tree
#include "H2MuAnalyzer/MakeHistos/interface/HistoHelper.h"        // Use to book and fill output histograms
#include "H2MuAnalyzer/MakeHistos/interface/ObjectSelection.h"    // Common object selections
#include "H2MuAnalyzer/MakeHistos/interface/EventSelection.h"     // Common event selections
#include "H2MuAnalyzer/MakeHistos/interface/EventWeight.h"        // Common event weights
#include "H2MuAnalyzer/MakeHistos/interface/CategoryCuts.h"       // Common category definitions
#include "H2MuAnalyzer/MakeHistos/interface/MiniNTupleHelper.h"   // "PlantTree" and "BookBranch" functions
#include "H2MuAnalyzer/MakeHistos/interface/ReadMVA.h"            // Read and evaluate XMLs for MVA
#include "H2MuAnalyzer/MakeHistos/interface/BtagSFHelper.h"      // Read Btag Efficiency

#include "CondFormats/BTauObjects/interface/BTagCalibration.h"
#include "CondTools/BTau/interface/BTagCalibrationReader.h"

#include "H2MuAnalyzer/MakeHistos/interface/JetCorrectorParameters_x.h"
#include "H2MuAnalyzer/MakeHistos/interface/JetCorrectionUncertainty_x.h"
//#include "H2MuAnalyzer/MakeHistos/interface/SimpleJetCorrectionUncertainty_x.h "

// #include "H2MuAnalyzer/MakeHistos/interface/SampleDatabase2016.h" // Input data and MC samples

// Load the library of the local, compiled H2MuAnalyzer/MakeHistos directory
R__LOAD_LIBRARY(../../../tmp/slc6_amd64_gcc630/src/H2MuAnalyzer/MakeHistos/src/H2MuAnalyzerMakeHistos/libH2MuAnalyzerMakeHistos.so)

// Hard-coded options for running locally / manually
// Options passed in as arguments to ReadNTupleChain when running in batch mode
const int MIN_FILE = 1;     // Minimum index of input files to process
const int MAX_FILE = 1;     // Maximum index of input files to process
const int MAX_EVT  = 1000; // Maximum number of events to process
const int PRT_EVT  = 1000;  // Print every N events
const float SAMP_WGT = 1.0;
// const float LUMI = 36814; // pb-1
const bool verbose = false; // Print extra information


//const TString IN_DIR   = "/eos/cms/store/group/phys_higgs/HiggsExo/H2Mu/UF/ntuples/2018/102X/prod-v18-pre-tag/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/ZJets_MG_1/190521_174140/0000";
//const TString SAMPLE   = "ZJets_MG_1";

const TString IN_DIR   = "/eos/cms/store/group/phys_higgs/HiggsExo/H2Mu/UF/ntuples/2018/102X/prod-v18.1.6.skim3l/ttHToMuMu_M125_TuneCP5_PSweights_13TeV-powheg-pythia8/H2Mu_ttH_125/190528_111755/0000";
const TString SAMPLE   = "H2Mu_ttH_125";

const std::string YEAR  = "2018";
const std::string SLIM  = "notSlim";  // "Slim" or "notSlim" - original 2016 NTuples were in "Slim" format, some 2017 NTuples are "Slim"
const TString OUT_DIR   = "plots";
const TString HIST_TREE = "HistTree"; // "Hist", "Tree", or "HistTree" to output histograms, trees, or both
const std::string SYS_SHIFT = "noSys";

const std::vector<std::string> SEL_CUTS = {"PreselRun2"}; // Cuts which every event must pass
const std::vector<std::string> OPT_CUTS = {"None"}; // Multiple selection cuts, applied independently in parallel
const std::vector<std::string> CAT_CUTS = {"None"}; // Event selection categories, also applied in parallel


// Command-line options for running in batch.  Running "root -b -l -q macros/ReadNTupleChain.C" will use hard-coded options above.
void Btag_test( TString sample = "", TString in_dir = "", TString out_dir = "",
	     std::vector<TString> in_files = {}, TString out_file_str = "",
	     int max_evt = 0, int prt_evt = 0, float samp_weight = 1.0,
	     TString hist_tree = "", std::string SYS = "" ) {
  
  // Set variables to hard-coded values if they are not initialized
  if (sample.Length()    == 0) sample  	   = SAMPLE;
  if (in_dir.Length()    == 0) in_dir  	   = IN_DIR;
  if (out_dir.Length()   == 0) out_dir 	   = OUT_DIR;
  if (max_evt            == 0) max_evt 	   = MAX_EVT;
  if (prt_evt            == 0) prt_evt 	   = PRT_EVT;
  if (samp_weight        == 0) samp_weight = SAMP_WGT;
  if (hist_tree.Length() == 0) hist_tree   = HIST_TREE;
  if (SYS.length()       == 0) SYS         = SYS_SHIFT;

  // Initialize empty file to access each file in the list
  TFile *file_tmp(0);

  // List of input files
  std::vector<TString> in_file_names;
  TString in_file_name;
  for (int i = 0; i < in_files.size(); i++) {
    in_file_name.Form("%s/%s", in_dir.Data(), in_files.at(i).Data());
    std::cout << "Adding file " << in_file_name.Data() << std::endl;
    in_file_names.push_back(in_file_name);
  }
  if (in_files.size() == 0) {
    for (int i = MIN_FILE; i <= MAX_FILE; i++) {
      in_file_name.Form("%s/tuple_%d.root", in_dir.Data(), i);
      std::cout << "Adding file " << in_file_name.Data() << std::endl;
      in_file_names.push_back(in_file_name.Data());
    }
  }

  // Open all input files
  for (int i = 0; i < in_file_names.size(); i++) {
    if ( !gSystem->AccessPathName(in_file_names.at(i)) )
      file_tmp = TFile::Open( in_file_names.at(i) ); // Check if file exists
    if (!file_tmp) {
      std::cout << "ERROR: could not open data file " << in_file_names.at(i) << std::endl;
      return;
    }
  }

  // Initialize set of pointers to all branches in tree
  NTupleBranches br;

  // Add trees from the input files to the TChain
  TChain * in_chain = new TChain("dimuons/tree");
  for (int i = 0; i < in_file_names.size(); i++) {
    in_chain->Add( in_file_names.at(i) );
  // for (int i = 0; i < samp->filenames.size(); i++) {
  //   in_chain->Add( samp->filenames.at(i) );
    // Set branch addresses, from interface/LoadNTupleBranches.h
    if (sample.Contains("SingleMu"))
      SetBranchAddresses(*in_chain, br, {YEAR, SLIM}); // Options in {} include "JES", "Flags", and "SFs"
    else
      SetBranchAddresses(*in_chain, br, {YEAR, SLIM, "GEN", "Wgts", "JES"}, (SYS.find("JES_") != std::string::npos ? SYS : "noSys"), false); // Options in {} include "JES", "Flags", and "SFs"
  }

  // Initialize empty map of histogram names to output histograms
  std::map<TString, TH1*> h_map_1D;
  std::map<TString, TH2*> h_map_2D;

  gROOT->cd(); // Navigate to "local" memory, so all histograms are not saved in out_tuple

  // Configuration for object selection, event selection, and object weighting
  ObjectSelectionConfig obj_sel;
  EventSelectionConfig  evt_sel;
  EventWeightConfig     evt_wgt;
  ConfigureObjectSelection(obj_sel, YEAR);
  ConfigureEventSelection (evt_sel, YEAR);
  ConfigureEventWeight    (evt_wgt, YEAR);

  evt_wgt.muon_ID  = false;
  evt_wgt.muon_Iso = false;
  evt_sel.muPair_mass_min = 12; // Allow masses down to 12 GeV (instead of 60 GeV) for background studies

  if (verbose) obj_sel.Print();
  if (verbose) evt_sel.Print();
  if (verbose) evt_wgt.Print();

  std::string PTC = obj_sel.mu_pt_corr; // Store muon pT correction in a shorter string; not changed later






  BTagCalibration calib("csvv1", "data/deepCSV/DeepCSV_102XSF_V1.csv");
  BTagCalibrationReader reader(BTagEntry::OP_LOOSE,  // operating point
                             "central",             // central sys type
                             {"up", "down"});      // other sys types
  reader.load(calib,                // calibration instance
            BTagEntry::FLAV_B,    // btag flavour
            "comb");               // measurement type

  reader.load(calib, BTagEntry::FLAV_C, "comb");
  reader.load(calib, BTagEntry::FLAV_UDSG, "incl");

  double test_jet_SF = reader.eval_auto_bounds("central", BTagEntry::FLAV_B, 1.0, 50);
  std::cout << "jet_SF for eta 1.0, pt 50 is   " << test_jet_SF << std::endl;



  std::unique_ptr<JetCorrectorParameters_x> JECpar;
  std::cout << "got pointer" << std::endl;
//  JetCorrectorParameters_x *JECpar = new JetCorrectorParameters_x("data/JEC/Autumn18_V19_MC_UncertaintySources_AK4PF.txt");
  JECpar.reset( new JetCorrectorParameters_x("data/JEC/Regrouped_Autumn18_V19_MC_UncertaintySources_AK4PFchs.txt", "Absolute_2018") );
  std::cout << "got txt file" << std::endl;
  JetCorrectionUncertainty_x *JECunc = new JetCorrectionUncertainty_x(*JECpar);
  std::cout << "set uncert" << std::endl;

  JECunc -> setJetPt(50);
  JECunc -> setJetEta(1.5);
  std::cout << "set jet pt eta" << std::endl;
  float sup = JECunc->getUncertainty(true);
  JECunc -> setJetPt(50);
  JECunc -> setJetEta(1.5);
  std::cout << "set jet pt eta again" << std::endl;
  float sdw = JECunc->getUncertainty(false);
  
  std::cout << "sup = " << sup << ",  sdw = " << sdw << std::endl;

//  JetCorrector->setJetEta(1.5);
//  JetCorrector->setJetPt(30);
//  double test_JEC = JetCorrector->getCorrection();
//  std::cout << "jet JEC for eta 1.5, pt 30 is " << test_JEC << std::endl;

 
  std::fstream b_eff_file("data/deepCSV/Btag_efficiency/Btag_efficiency_3l_2016.csv");
  std::string line_temp, word_temp;
  std::getline(b_eff_file, line_temp);
  std::getline(b_eff_file, line_temp);
  std::cout << line_temp << "\n" <<std::endl;

  std::stringstream s_temp(line_temp);
  while(std::getline(s_temp, word_temp, ',')) {
    std::cout << word_temp << std::endl;
  }
  std::cout << std::stof(word_temp) << std::endl;



  BtagEffEntry BTE;
  GetBtagEff(BTE, YEAR, (std::string) sample);
  BTE.Print();



  std::cout << "\n******* About to enter the loop over " << in_chain->GetEntries() << " events *******" << std::endl;
  for (int iEvt = 0; iEvt < in_chain->GetEntries(); iEvt++) {
    
    if (iEvt > max_evt && max_evt > 0) break;
    if ( (iEvt % prt_evt) == 0 ) {
      std::cout << "\n*********************" << std::endl;
      std::cout << "Looking at event " << iEvt <<  std::endl;
      std::cout << "*********************" << std::endl;
    }
    
    if (verbose) std::cout << "Before running GetEntry, event = " << br.event;
    
    in_chain->GetEntry(iEvt);
   
    // process muon collection and load new ones with Roch pt with systematic shifts 
    MuonInfos muons_tmp;
    if ( not sample.Contains("SingleMu") and SYS.find("Roch_") != std::string::npos ) {
      muons_tmp = ReloadMuonRoch(Roch_Cor, *br.muons_orig, *br.genMuons, SYS);
      br.muons = &muons_tmp;
    }
    else br.muons = br.muons_orig;
 
    if (verbose) std::cout << "... after, event = " << br.event << std::endl;

    // For original 2016 and some 2017 NTuples, convert "SlimJets" collection into regular jets
    JetInfos jets_tmp;
    if (SLIM == "Slim") {
      jets_tmp = ConvertSlimJets(*(br.slimJets));
      br.jets  = &jets_tmp;
    }

    ///////////////////////////////////////////
    ///  Apply selection and category cuts  ///
    ///////////////////////////////////////////

    // Check if event passes basic selection cuts defined in src/SelectionCuts.cc
    bool pass_sel_cuts = true;
    for (int i = 0; i < SEL_CUTS.size(); i++) {
      if (not PassSelection(br, evt_sel, obj_sel, SEL_CUTS.at(i), verbose)) pass_sel_cuts = false;
    }
    if (not pass_sel_cuts) continue;

    // Get event weight for MC, defined in src/EventWeight.cc
    bool isData = sample.Contains("SingleMu");
    float event_wgt = ( isData ? 1.0 : EventWeight(br, evt_wgt, verbose) );

      /////////////////////////////////////////////////////////////////////////////////////////
      ///  Loop through alternate, optional selection cuts defined in src/SelectionCuts.cc  ///
      /////////////////////////////////////////////////////////////////////////////////////////

    for (int iOpt = 0; iOpt < OPT_CUTS.size(); iOpt++) {
      std::string OPT_CUT = OPT_CUTS.at(iOpt);
      
      for (int iCat = 0; iCat < CAT_CUTS.size(); iCat++) {
	std::string h_pre = (std::string)sample + "_"+OPT_CUTS.at(iOpt)+"_"+CAT_CUTS.at(iCat)+"_";
	MuPairInfo    dimu;
	dimu = SelectedCandPair(obj_sel, br);

	float B_SF = 1.0;
	JetInfos  jets  = SelectedJets(obj_sel, br);
	for (const auto & jet : jets) {
	  float jet_SF = 1.0;
	  if      (abs(jet.partonID) == 5) jet_SF *= reader.eval_auto_bounds("central", BTagEntry::FLAV_B, jet.eta, jet.pt);
	  else if (abs(jet.partonID) == 4) jet_SF *= reader.eval_auto_bounds("central", BTagEntry::FLAV_C, jet.eta, jet.pt);
	  else				   jet_SF *= reader.eval_auto_bounds("central", BTagEntry::FLAV_UDSG, jet.eta, jet.pt);
	  BookAndFill(h_map_1D, h_pre + "jet_SF",  100,   0,   2, jet_SF, event_wgt, false);
	  if ( abs(jet.partonID) != 5 )  BookAndFill(h_map_1D, h_pre + "Ljet_SF",  200,   -2,   2, jet_SF, event_wgt, false);
	  if ( abs(jet.partonID) == 5 )  BookAndFill(h_map_1D, h_pre + "Bjet_SF",  200,   -2,   2, jet_SF, event_wgt, false);
	  B_SF *= jet_SF;
	}

	BookAndFill(h_map_1D, h_pre + "dimu_mass", 50, 110, 160, dimu.mass, event_wgt, false);  // overflow = false because we do not want overflow bins to mess up the mass fit.
	BookAndFill(h_map_1D, h_pre + "Btag_SF",  200,   -2,   2, B_SF, event_wgt, false);
	BookAndFill(h_map_1D, h_pre + "nJets",      10,   0,   10, jets.size(), event_wgt, false);

      } // End loop: for (int iCat = 0; iCat < CAT_CUTS.size(); iCat++)
    } // End loop: for (int iOpt = 0; iOpt < OPT_CUTS.size(); iOpt++)

  } // End loop: for (int iEvt = 0; iEvt < in_chain->GetEntries(); iEvt++)
  std::cout << "\n******* Leaving the event loop *******" << std::endl;


  std::cout << "\n******* normalizing histos, weight =  " << samp_weight << " *******" << std::endl;
  if (h_map_1D.empty()) std::cout << "h_map_1D is empty" << std::endl;
  else {
    for (std::map<TString, TH1*>::iterator it_term = h_map_1D.begin() ; it_term != h_map_1D.end() ; it_term++) {
        it_term->second->Scale(samp_weight);
    }
  }
  if (h_map_2D.empty()) std::cout << "h_map_2D is empty" << std::endl;
  else {
    for (std::map<TString, TH2*>::iterator it_term = h_map_2D.begin() ; it_term != h_map_2D.end() ; it_term++) {
        it_term->second->Scale(samp_weight);
    }
  }

  // Place histograms made with separate selection and cateogry cuts into separate files
  for (int iOpt = 0; iOpt < OPT_CUTS.size(); iOpt++) {
    for (int iCat = 0; iCat < CAT_CUTS.size(); iCat++) {
      std::string optCatStr = OPT_CUTS.at(iOpt)+"_"+CAT_CUTS.at(iCat);
      
      // Create output file
      TString out_file_name;
      if (out_file_str.Length() > 0) out_file_name.Form( "%s/histos_%s_%s_%s.root",    out_dir.Data(), sample.Data(), optCatStr.c_str(), out_file_str.Data() );
      else                           out_file_name.Form( "%s/histos_%s_%s_%d_%d.root", out_dir.Data(), sample.Data(), optCatStr.c_str(), MIN_FILE, MAX_FILE );
      std::cout << "\nCreating output file " << out_file_name.Data() << std::endl;
      TFile* out_file = TFile::Open( out_file_name, "RECREATE" );
      
      // Write output file
      if (verbose) std::cout << "\nWriting output file " << out_file_name.Data() << std::endl;
      out_file->cd();

      // Write out 1D histograms
      for (std::map<TString, TH1*>::iterator it = h_map_1D.begin(); it != h_map_1D.end(); ++it) {
	std::string h_name = it->second->GetName();
	if (h_name.find(optCatStr+"_") != std::string::npos) {
	  // Remove optional selection and category cuts from histogram names
	  h_name.erase( h_name.find(optCatStr+"_"), optCatStr.length() + 1 );
	  it->second->SetName(h_name.c_str());
	  it->second->SetTitle(h_name.c_str());
	  // std::cout << "  * Writing 1D histogram " << it->second->GetName() << std::endl;
	  it->second->Write();
	}
      }
      // Write out 2D histograms
      for (std::map<TString, TH2*>::iterator it = h_map_2D.begin(); it != h_map_2D.end(); ++it) {
	std::string h_name = it->second->GetName();
	if (h_name.find(optCatStr+"_") != std::string::npos) {
	  // Remove optional selection and category cuts from histogram names
	  h_name.erase( h_name.find(optCatStr+"_"), optCatStr.length() + 1 );
	  it->second->SetName(h_name.c_str());
	  it->second->SetTitle(h_name.c_str());
	  // std::cout << "  * Writing 2D histogram " << it->second->GetName() << std::endl;
	  it->second->Write();
	}
      }
  
      out_file->Write();
      out_file->Close();
      std::cout << "Wrote output file " << out_file_name.Data() << std::endl;
      
    } // End loop: for (int iCat = 0; iCat < CAT_CUTS.size(); iCat++)
  } // End loop: for (int iOpt = 0; iOpt < OPT_CUTS.size(); iOpt++)


  std::cout << "\nExiting Btag_test()\n";
  
} // End void Btag_test()
