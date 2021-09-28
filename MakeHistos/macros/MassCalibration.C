
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

#include "H2MuAnalyzer/MakeHistos/interface/LoadNTupleBranches.h" // List of branches in the NTuple tree
#include "H2MuAnalyzer/MakeHistos/interface/HistoHelper.h"        // Use to book and fill output histograms
#include "H2MuAnalyzer/MakeHistos/interface/MassCalPlots.h"  	  // class and config for mass calibration study
#include "H2MuAnalyzer/MakeHistos/interface/ObjectSelection.h"    // Common object selections
#include "H2MuAnalyzer/MakeHistos/interface/EventSelection.h"     // Common event selections
#include "H2MuAnalyzer/MakeHistos/interface/EventWeight.h"        // Common event weights
#include "H2MuAnalyzer/MakeHistos/interface/CategoryCuts.h"       // Common category definitions
#include "H2MuAnalyzer/MakeHistos/interface/MiniNTupleHelper.h"   // "PlantTree" and "BookBranch" functions
#include "H2MuAnalyzer/MakeHistos/interface/ReadMVA.h"            // Read and evaluate XMLs for MVA

#include "H2MuAnalyzer/MakeHistos/src/CustomCorrections.C"
//#include "/afs/cern.ch/user/b/bortigno/public/hmm/hmmNamespace.C"
#include "/afs/cern.ch/work/x/xzuo/public/H2Mu/Run2/pt_correction/GeoFit.C"

// #include "H2MuAnalyzer/MakeHistos/interface/SampleDatabase2016.h" // Input data and MC samples

// Load the library of the local, compiled H2MuAnalyzer/MakeHistos directory
R__LOAD_LIBRARY(../../../tmp/slc6_amd64_gcc630/src/H2MuAnalyzer/MakeHistos/src/H2MuAnalyzerMakeHistos/libH2MuAnalyzerMakeHistos.so)

// Hard-coded options for running locally / manually
// Options passed in as arguments to ReadNTupleChain when running in batch mode
const int MIN_FILE = 1;     // Minimum index of input files to process
const int MAX_FILE = 3;     // Maximum index of input files to process
const int MAX_EVT  = 100000; // Maximum number of events to process
const int PRT_EVT  = 1000;  // Print every N events
const float SAMP_WGT = 1.0;
// const float LUMI = 36814; // pb-1
const bool verbose = false; // Print extra information


//const TString IN_DIR   = "/eos/cms/store/group/phys_higgs/HiggsExo/H2Mu/UF/ntuples/2018/102X/prod-v18-pre-tag/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/ZJets_MG_1/190521_174140/0000";
//const TString SAMPLE   = "ZJets_MG_1";
//const TString IN_DIR   = "/eos/cms/store/group/phys_higgs/HiggsExo/H2Mu/UF/ntuples/2016/94X_v3/prod-v16.0.7/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/ZJets_AMC/190714_182527/0000/";
//const TString SAMPLE   = "ZJets_AMC";

//const TString IN_DIR   = "/eos/cms/store/group/phys_higgs/HiggsExo/H2Mu/UF/ntuples/2016/94X_v3/prod-v16.0.7/DYJetsToLL_M-105To160_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8/ZJets_hiM_MG/190714_181926/0000/";
//const TString SAMPLE   = "ZJets_hiM_MG";
//const TString IN_DIR   = "/eos/cms/store/group/phys_higgs/HiggsExo/H2Mu/UF/ntuples/2016/94X_v3/prod-v16.0.7/TTTo2L2Nu_TuneCP5_PSweights_13TeV-powheg-pythia8/tt_ll_POW/190714_182301/0000/";
//const TString SAMPLE   = "tt_ll_POW";
//const TString IN_DIR   = "/eos/cms/store/group/phys_higgs/HiggsExo/H2Mu/UF/ntuples/2016/94X_v3/prod-v16.0.7/SingleMuon/SingleMu_2016B/190714_182320/0000/";
//const TString IN_DIR   = "/eos/cms/store/group/phys_higgs/HiggsExo/H2Mu/UF/ntuples/2016/94X_v3/prod-v16.0.7/SingleMuon/SingleMu_2016B/190714_182320/0000/";
//const TString SAMPLE   = "SingleMu_2016B";

//const TString IN_DIR   = "/eos/cms/store/group/phys_higgs/HiggsExo/H2Mu/UF/ntuples/2016/94X_v3/prod-v16.0.7/GluGluHToMuMu_M125_TuneCP5_PSweights_13TeV_amcatnloFXFX_pythia8/H2Mu_gg/190714_181756/0000";
//const TString SAMPLE   = "H2Mu_gg";

//const TString IN_DIR   = "/eos/cms/store/group/phys_higgs/HiggsExo/H2Mu/UF/ntuples/2016/94X_v3/prod-v16.0.7/WplusH_HToMuMu_WToAll_M125_TuneCP5_PSweights_13TeV_powheg_pythia8/H2Mu_WH_pos_125/190714_182055/0000";
//const TString SAMPLE   = "H2Mu_WH_pos_125";

//const TString IN_DIR    = "/eos/cms/store/group/phys_higgs/HiggsExo/H2Mu/UF/ntuples/data_2017_and_mc_fall17/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/ZJets_AMC/181019_172147/0000";
//const TString SAMPLE    = "ZJets_AMC";

const TString IN_DIR = "/eos/cms/store/group/phys_higgs/HiggsExo/H2Mu/UF/ntuples/data_2017_and_mc_fall17/WPlusH_HToMuMu_M125_13TeV_powheg_pythia8/H2Mu_WH_pos/180802_164326/0000";
const TString SAMPLE = "H2Mu_WH_pos";

const std::string YEAR  = "2018";
const std::string SLIM  = "notSlim";  // "Slim" or "notSlim" - original 2016 NTuples were in "Slim" format, some 2017 NTuples are "Slim"
				      // 2017 2l production 181019_* are not slim
const TString OUT_DIR   = "plots";
const TString HIST_TREE = "HistTree"; // "Hist", "Tree", or "HistTree" to output histograms, trees, or both

std::string GEN_INT = "GENint";

const std::vector<std::string> SEL_CUTS = {"PreselRun2"}; // Cuts which every event must pass
const std::vector<std::string> OPT_CUTS = {"NONE"}; // Multiple selection cuts, applied independently in parallel
//const std::vector<std::string> CAT_CUTS = {"NONE", "inclusive_01jet", "inclusive_2jets", "BB", "BE", "EE", "nonBB"}; // Event selection categories, also applied in parallel
//const std::vector<std::string> CAT_CUTS = {"NONE", "BB", "nonBB", "XX", "EE", "posBarrel", "posOverlap", "posEndcap"};
const std::vector<std::string> CAT_CUTS = {"NONE"};

// Command-line options for running in batch.  Running "root -b -l -q macros/ReadNTupleChain.C" will use hard-coded options above.
void MassCalibration( TString sample = "", TString in_dir = "", TString out_dir = "",
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
      SetBranchAddresses(*in_chain, br, {YEAR, SLIM}, "noSys", false); // Options in {} include "JES", "Flags", and "SFs"
    else
      SetBranchAddresses(*in_chain, br, {YEAR, SLIM, GEN_INT, "GEN", "Wgts"}, "noSys", false); // Options in {} include "JES", "Flags", and "SFs"
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

  evt_sel.muPair_mass_min = 12; // Allow masses down to 12 GeV (instead of 60 GeV) for background studies

  if (verbose) obj_sel.Print();
  if (verbose) evt_sel.Print();
  if (verbose) evt_wgt.Print();

  std::string PTC = obj_sel.mu_pt_corr; // Store muon pT correction in a shorter string; not changed later


  /////////////////////////////////////////
  ////   Config for mass calibration   ////
  /////////////////////////////////////////
  MassCalConfig 	mc_cfg;
//  ConfigureMassCal( mc_cfg, "Z", "muN_d0");
//  ConfigureMassCal( mc_cfg, "Z", "muP_d0_rebin", "muP_phi");
//  ConfigureMassCal( mc_cfg, "H", "muP_d0_rebin", "muN_d0_rebin");
  ConfigureMassCal( mc_cfg, "Z", "muP_phi", "muN_phi");

  std::map<TString, MassCalPlots*> mc_map_gen;
  std::map<TString, MassCalPlots*> mc_map_genBS;

  std::map<TString, MassCalPlots*> mc_map_PF;
  std::map<TString, MassCalPlots*> mc_map_Roch;
  std::map<TString, MassCalPlots*> mc_map_RochBS;
  std::map<TString, MassCalPlots*> mc_map_Kinfit;
  std::map<TString, MassCalPlots*> mc_map_KinRoch;
//  std::map<TString, MassCalPlots*> mc_map_KaMu;
//  std::map<TString, MassCalPlots*> mc_map_KinKaMu;

//  std::map<TString, MassCalPlots*> mc_map_GeoRoch;
  std::map<TString, MassCalPlots*> mc_map_GeoBSRoch;

//  std::map<TString, MassCalPlots*> mc_map_PtWPF;
//  std::map<TString, MassCalPlots*> mc_map_PtWKaMu;
//  std::map<TString, MassCalPlots*> mc_map_PtNPF;
//  std::map<TString, MassCalPlots*> mc_map_PtNKaMu;
//
//  std::map<TString, MassCalPlots*> mc_map_PtSeg20KaMu;
//  std::map<TString, MassCalPlots*> mc_map_PtSeg25KaMu;
//  std::map<TString, MassCalPlots*> mc_map_PtSeg30KaMu;
//  std::map<TString, MassCalPlots*> mc_map_PtShift20KaMu;
//  std::map<TString, MassCalPlots*> mc_map_PtShift25KaMu;
//  std::map<TString, MassCalPlots*> mc_map_PtShift30KaMu;
//  std::map<TString, MassCalPlots*> mc_map_PtGeoKaMu;
//  std::map<TString, MassCalPlots*> mc_map_PtGeoShiftKaMu;

//  std::map<TString, MassCalPlots*> mc_map_AdHoc0;
//  std::map<TString, MassCalPlots*> mc_map_AdHoc_re;
//  std::map<TString, MassCalPlots*> mc_map_AdHoc_eta;
//  std::map<TString, MassCalPlots*> mc_map_AdHocKaMu;

//  std::map<TString, MassCalPlots*> mc_map_Kingood;
//  std::map<TString, MassCalPlots*> mc_map_Kind0;
//
//  std::map<TString, MassCalPlots*> mc_map_Kind0_BB;
//  std::map<TString, MassCalPlots*> mc_map_Kind0_BE;
//  std::map<TString, MassCalPlots*> mc_map_Kind0_EE;

//  std::map<TString, MassCalPlots*> mc_map_Kind0_N50_N15;
//  std::map<TString, MassCalPlots*> mc_map_Kind0_N15_N05;
//  std::map<TString, MassCalPlots*> mc_map_Kind0_P05_P15;
//  std::map<TString, MassCalPlots*> mc_map_Kind0_P15_P50;

  for (int iOpt = 0; iOpt < OPT_CUTS.size(); iOpt++) {
    for (int iCat = 0; iCat < CAT_CUTS.size(); iCat++) {
	TString optCatStr = OPT_CUTS.at(iOpt)+"_"+CAT_CUTS.at(iCat);
	mc_map_gen    [ optCatStr ]   = new MassCalPlots( (sample.Contains("SingleMu") ? "data" :  sample) + "_" + optCatStr + "_gen", mc_cfg );
	mc_map_genBS  [ optCatStr ]   = new MassCalPlots( (sample.Contains("SingleMu") ? "data" :  sample) + "_" + optCatStr + "_genBS", mc_cfg );

	mc_map_PF     [ optCatStr ]   = new MassCalPlots( (sample.Contains("SingleMu") ? "data" :  sample) + "_" + optCatStr + "_PF", mc_cfg );
	mc_map_Roch   [ optCatStr ]   = new MassCalPlots( (sample.Contains("SingleMu") ? "data" :  sample) + "_" + optCatStr + "_Roch", mc_cfg );
	mc_map_RochBS [ optCatStr ]   = new MassCalPlots( (sample.Contains("SingleMu") ? "data" :  sample) + "_" + optCatStr + "_RochBS", mc_cfg );
	mc_map_Kinfit [ optCatStr ]   = new MassCalPlots( (sample.Contains("SingleMu") ? "data" :  sample) + "_" + optCatStr + "_Kinfit", mc_cfg );
        mc_map_KinRoch[ optCatStr ]   = new MassCalPlots( (sample.Contains("SingleMu") ? "data" :  sample) + "_" + optCatStr + "_KinRoch", mc_cfg );
//        mc_map_KaMu   [ optCatStr ]   = new MassCalPlots( (sample.Contains("SingleMu") ? "data" :  sample) + "_" + optCatStr + "_KaMu", mc_cfg );
//	mc_map_KinKaMu[ optCatStr ]   = new MassCalPlots( (sample.Contains("SingleMu") ? "data" :  sample) + "_" + optCatStr + "_KinKaMu", mc_cfg );

//        mc_map_GeoRoch[ optCatStr ]     = new MassCalPlots( (sample.Contains("SingleMu") ? "data" :  sample) + "_" + optCatStr + "_GeoRoch", mc_cfg );
	mc_map_GeoBSRoch[ optCatStr ]   = new MassCalPlots( (sample.Contains("SingleMu") ? "data" :  sample) + "_" + optCatStr + "_GeoBSRoch", mc_cfg );

//    	mc_map_PtWPF  [ optCatStr ]   = new MassCalPlots( (sample.Contains("SingleMu") ? "data" :  sample) + "_" + optCatStr + "_PtWPF", mc_cfg );
//	mc_map_PtWKaMu[ optCatStr ]   = new MassCalPlots( (sample.Contains("SingleMu") ? "data" :  sample) + "_" + optCatStr + "_PtWKaMu", mc_cfg );
//	mc_map_PtNPF  [ optCatStr ]   = new MassCalPlots( (sample.Contains("SingleMu") ? "data" :  sample) + "_" + optCatStr + "_PtNPF", mc_cfg );
//	mc_map_PtNKaMu[ optCatStr ]   = new MassCalPlots( (sample.Contains("SingleMu") ? "data" :  sample) + "_" + optCatStr + "_PtNKaMu", mc_cfg );
//
//	mc_map_PtSeg20KaMu[ optCatStr ]    = new MassCalPlots( (sample.Contains("SingleMu") ? "data" :  sample) + "_" + optCatStr + "_PtSeg20KaMu", mc_cfg );
//	mc_map_PtSeg25KaMu[ optCatStr ]    = new MassCalPlots( (sample.Contains("SingleMu") ? "data" :  sample) + "_" + optCatStr + "_PtSeg25KaMu", mc_cfg );
//	mc_map_PtSeg30KaMu[ optCatStr ]    = new MassCalPlots( (sample.Contains("SingleMu") ? "data" :  sample) + "_" + optCatStr + "_PtSeg30KaMu", mc_cfg );
//	mc_map_PtShift20KaMu[ optCatStr ]  = new MassCalPlots( (sample.Contains("SingleMu") ? "data" :  sample) + "_" + optCatStr + "_PtShift20KaMu", mc_cfg );
//	mc_map_PtShift25KaMu[ optCatStr ]  = new MassCalPlots( (sample.Contains("SingleMu") ? "data" :  sample) + "_" + optCatStr + "_PtShift25KaMu", mc_cfg );
//	mc_map_PtShift30KaMu[ optCatStr ]  = new MassCalPlots( (sample.Contains("SingleMu") ? "data" :  sample) + "_" + optCatStr + "_PtShift30KaMu", mc_cfg );
//	mc_map_PtGeoKaMu      [ optCatStr ]  = new MassCalPlots( (sample.Contains("SingleMu") ? "data" :  sample) + "_" + optCatStr + "_PtGeoKaMu", mc_cfg );
//	mc_map_PtGeoShiftKaMu [ optCatStr ]  = new MassCalPlots( (sample.Contains("SingleMu") ? "data" :  sample) + "_" + optCatStr + "_PtGeoShiftKaMu", mc_cfg );

//	mc_map_AdHoc0 [ optCatStr ]   = new MassCalPlots( (sample.Contains("SingleMu") ? "data" :  sample) + "_" + optCatStr + "_AdHoc0", mc_cfg );
//	mc_map_AdHoc_re  [ optCatStr ]   = new MassCalPlots( (sample.Contains("SingleMu") ? "data" :  sample) + "_" + optCatStr + "_AdHoc_re", mc_cfg );
//	mc_map_AdHoc_eta [ optCatStr ]   = new MassCalPlots( (sample.Contains("SingleMu") ? "data" :  sample) + "_" + optCatStr + "_AdHoc_eta", mc_cfg );
//	mc_map_AdHocKaMu [ optCatStr ]   = new MassCalPlots( (sample.Contains("SingleMu") ? "data" :  sample) + "_" + optCatStr + "_AdHocKaMu", mc_cfg );

//	mc_map_Kingood[ optCatStr ]   = new MassCalPlots( (sample.Contains("SingleMu") ? "data" :  sample) + "_" + optCatStr + "_good_Kinfit", mc_cfg );
//	mc_map_Kind0  [ optCatStr ]   = new MassCalPlots( (sample.Contains("SingleMu") ? "data" :  sample) + "_" + optCatStr + "_Kin_vs_d0kin", mc_cfg );
//
//	mc_map_Kind0_BB[ optCatStr ]  = new MassCalPlots( (sample.Contains("SingleMu") ? "data" :  sample) + "_" + optCatStr + "_Kin_vs_d0kin_BB", mc_cfg );
//	mc_map_Kind0_BE[ optCatStr ]  = new MassCalPlots( (sample.Contains("SingleMu") ? "data" :  sample) + "_" + optCatStr + "_Kin_vs_d0kin_BE", mc_cfg );
//	mc_map_Kind0_EE[ optCatStr ]  = new MassCalPlots( (sample.Contains("SingleMu") ? "data" :  sample) + "_" + optCatStr + "_Kin_vs_d0kin_EE", mc_cfg );

//	mc_map_Kind0_N50_N15[ optCatStr ]  = new MassCalPlots( (sample.Contains("SingleMu") ? "data" :  sample) + "_" + optCatStr + "_Kin_vs_d0kin_d0PV_N50_N15", mc_cfg );
//        mc_map_Kind0_N15_N05[ optCatStr ]  = new MassCalPlots( (sample.Contains("SingleMu") ? "data" :  sample) + "_" + optCatStr + "_Kin_vs_d0kin_d0PV_N15_N05", mc_cfg );
//        mc_map_Kind0_P05_P15[ optCatStr ]  = new MassCalPlots( (sample.Contains("SingleMu") ? "data" :  sample) + "_" + optCatStr + "_Kin_vs_d0kin_d0PV_P05_P15", mc_cfg );
//	mc_map_Kind0_P15_P50[ optCatStr ]  = new MassCalPlots( (sample.Contains("SingleMu") ? "data" :  sample) + "_" + optCatStr + "_Kin_vs_d0kin_d0PV_P15_P50", mc_cfg );

    }
  }

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
    // check if GEN_wgt is float or int, if the float value is 0 (not properly read), reload the int one
    if (GEN_INT == "GENint") br.GEN_wgt = (float) br.GEN_wgt_int;  
 
     // process muon collection and load new ones with Roch pt with systematic shifts 
//    MuonInfos muons_tmp;
//    if ( not sample.Contains("SingleMu") and SYS.find("Roch_") != std::string::npos ) {
//      muons_tmp = ReloadMuonRoch(Roch_Cor, *br.muons_orig, *br.genMuons, SYS);
//      br.muons = &muons_tmp;
//    }
//  no need to Roch sys in this macro
    br.muons = br.muons_orig;
 
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
//    std::cout<<"at line 318" <<std::endl; //debugging

    for (int iOpt = 0; iOpt < OPT_CUTS.size(); iOpt++) {
      std::string OPT_CUT = OPT_CUTS.at(iOpt);
      
      for (int iCat = 0; iCat < CAT_CUTS.size(); iCat++) {
	TString optCatStr = OPT_CUTS.at(iOpt)+"_"+CAT_CUTS.at(iCat);
	if ( not InCategory(obj_sel, br, "noSys", CAT_CUTS.at(iCat), verbose) ) continue;

	MuPairInfo    dimu;
	if ( mc_cfg.peak == "Z" or sample.Contains("gg") or sample.Contains("VBF") ) {
	// Z samples, or 2-muon signal samples
	  if ( SelectedMuPairs(obj_sel, br).size()!=1 or SelectedMuons(obj_sel, br).size()!=2 ) continue;
	  dimu = SelectedCandPair(obj_sel, br);
	}
	else if ( sample.Contains("WH") or sample.Contains("ZH") or sample.Contains("ttH") ) {
	// multi-muon signal samples
	  for (const auto & muPair: SelectedMuPairs(obj_sel, br)) {
	    if (muPair.mass > 117 and muPair.mass < 133) {  // allow 3 GeV margin between PF and corrected mass
		dimu = muPair;
		break;
	    }
	  }
	  if (dimu.mass == -999) continue; // no good dimuon found
	}
	else if (mc_cfg.peak == "off_set") {
	// Z samples, or ttbar samples, not mass fit but mass offset fit
	  if ( SelectedMuPairs(obj_sel, br).size()!=1 or SelectedMuons(obj_sel, br).size()!=2 ) continue;
          dimu = SelectedCandPair(obj_sel, br);
	  if (dimu.mass < 110 or dimu.mass > 160) continue;
	  //if (dimu.mass < 75 or dimu.mass > 105) continue;
 	}
	else continue;
	
	MuonInfo mu1, mu2, muP, muN;
	mu1 = br.muons->at(dimu.iMu1);
	mu2 = br.muons->at(dimu.iMu2);

	TLorentzVector mu_vec1 = FourVec( mu1, "PF" );
	TLorentzVector mu_vec2 = FourVec( mu2, "PF" );

//        std::cout<<"at line 359"<<std::endl; //debugging
	// for MC samples, find gen match
	GenParentInfo    gen_dimu;
	if (not isData) {
	  for (const auto & genPa : *br.genParents) {
	    int PaID = 23; // default peak Z
	    if (mc_cfg.peak == "H") PaID = 25; // peak H
	    if (genPa.ID != PaID or genPa.daughter_1_ID + genPa.daughter_2_ID != 0) continue;  // legit parent particle
	    if (genPa.daughter_1_ID != 13 and genPa.daughter_1_ID != -13) continue;         // decays to dimuon
	    if (genPa.daughter_1_idx < 0 or genPa.daughter_2_idx < 0) continue;             // present in genMuons collection
	    TLorentzVector gen_vec1 = FourVec( br.genMuons->at(genPa.daughter_1_idx) );
            TLorentzVector gen_vec2 = FourVec( br.genMuons->at(genPa.daughter_2_idx) );
	    if ( mu_vec1.DeltaR(gen_vec1)<0.05 and mu_vec2.DeltaR(gen_vec2)<0.05 ) gen_dimu = genPa;
            if ( mu_vec1.DeltaR(gen_vec2)<0.05 and mu_vec2.DeltaR(gen_vec1)<0.05 ) gen_dimu = genPa;
	  }
	}
	if ( mc_cfg.peak != "off_set" and gen_dimu.mass == -999 and not isData ) continue;
	// gen match found

//        std::cout<<"gen_dimu_mass="<<gen_dimu.mass<<std::endl; //debugging
	GenMuonInfo gen_muP, gen_muN;
	if (gen_dimu.mass != -999 and not isData and (br.genMuons->at(gen_dimu.daughter_1_idx)).charge == 1) {
          gen_muP = br.genMuons->at(gen_dimu.daughter_1_idx);
          gen_muN = br.genMuons->at(gen_dimu.daughter_2_idx);
        }
	else if (gen_dimu.mass != -999 and not isData) {	
          gen_muP = br.genMuons->at(gen_dimu.daughter_2_idx);
          gen_muN = br.genMuons->at(gen_dimu.daughter_1_idx);
        }     

	// getting offset for "off_set" option
	float off_set = 0.0;
	if (mc_cfg.peak == "off_set") {
	  GenMuonInfo gen_mu1, gen_mu2;
	  for (const auto & genMu : *br.genMuons) {
	    TLorentzVector genMu_vec = FourVec(genMu);
	    if ( mu_vec1.DeltaR(genMu_vec)<0.05 and abs(genMu.mother_ID) != 13) gen_mu1 = genMu;
	    if ( mu_vec2.DeltaR(genMu_vec)<0.05 and abs(genMu.mother_ID) != 13) gen_mu2 = genMu; 
	    // use highest pt match
	    if ( gen_mu1.pt != -999 and gen_mu2.pt != -999) continue;
	  }
	  // if either of the muon is not matched to gen
	  if ( gen_mu1.pt == -999 and gen_mu2.pt == -999) continue;
	  TLorentzVector genMu1_vec  = FourVec( gen_mu1 );
	  TLorentzVector genMu2_vec  = FourVec( gen_mu2 );
	  TLorentzVector gendimu_vec = genMu1_vec + genMu2_vec;
	  off_set = gendimu_vec.M();
	}

//        std::cout<<"dimu_mass_Roch="<<dimu.mass_Roch<<std::endl; //debugging
	if (mu1.charge == 1)  muP = mu1, muN = mu2;
        else muP = mu2, muN = mu1;
	float dimu_mass_KinRoch = dimu.mass_kinfit * dimu.mass_Roch / dimu.mass;
	float dimu_pt_KinRoch	= dimu.pt_kinfit * dimu.pt_Roch / dimu.pt;
 	float mu1_pt_KinRoch	= mu1.pt_kinfit * mu1.pt_Roch / mu1.pt;
	float mu2_pt_KinRoch    = mu2.pt_kinfit * mu2.pt_Roch / mu2.pt;
	float muP_pt_KinRoch    = muP.pt_kinfit * muP.pt_Roch / muP.pt;

//	float dimu_mass_KinKaMu = dimu.mass_kinfit * dimu.mass_KaMu / dimu.mass;
//        float dimu_pt_KinKaMu   = dimu.pt_kinfit * dimu.pt_KaMu / dimu.pt;
//        float mu1_pt_KinKaMu    = mu1.pt_kinfit * mu1.pt_KaMu / mu1.pt;
//        float mu2_pt_KinKaMu    = mu2.pt_kinfit * mu2.pt_KaMu / mu2.pt;
//	float muP_pt_KinKaMu    = muP.pt_kinfit * muP.pt_KaMu / muP.pt;

//	float dimu_mass_AdHoc0     = dimu.mass / ( 1.0 + 0.01 * hmm::massCor       (muP.d0_PV, muN.d0_PV, std::stoi(YEAR)) );
//	float dimu_mass_AdHoc_re   = dimu.mass / ( 1.0 + 0.01 * AdHocMassCor::massCor_PF_all(muP.d0_PV, muN.d0_PV, std::stoi(YEAR)) );
//	float dimu_mass_AdHoc_eta  = dimu.mass / ( 1.0 + 0.01 * AdHocMassCor::massCor_PF_CC(muP.d0_PV, muN.d0_PV, std::stoi(YEAR)) );
//	float dimu_mass_AdHocKaMu  = dimu.mass_KaMu / ( 1.0 + 0.01 * AdHocMassCor::massCor_KaMu_all(muP.d0_PV, muN.d0_PV, std::stoi(YEAR)) );
//	if (abs(muP.eta)>=1.7 or abs(muN.eta)>=1.7) {
//	      dimu_mass_AdHoc_eta  = dimu.mass / ( 1.0 + 0.01 * AdHocMassCor::massCor_PF_FX(muP.d0_PV, muN.d0_PV, std::stoi(YEAR)) );
//	      dimu_mass_AdHocKaMu  = dimu.mass_KaMu / ( 1.0 + 0.01 * AdHocMassCor::massCor_KaMu_FX(muP.d0_PV, muN.d0_PV, std::stoi(YEAR)) );
//	}

	TLorentzVector muP_vec, muN_vec, sum_vec;

//	muP_vec.SetPtEtaPhiM( PtGeoCor::PtGeoFit_mod(muP.d0_PV * muP.charge, muP.pt_Roch, muP.eta, std::stoi(YEAR)), muP.eta, muP.phi, 0.105658367 );
//	muN_vec.SetPtEtaPhiM( PtGeoCor::PtGeoFit_mod(muN.d0_PV * muN.charge, muN.pt_Roch, muN.eta, std::stoi(YEAR)), muN.eta, muN.phi, 0.105658367 );
//	sum_vec = muP_vec + muN_vec;
//	float dimu_mass_GeoRoch = sum_vec.M();
//        float dimu_pt_GeoRoch = sum_vec.Pt();
//        float dimu_eta_GeoRoch = sum_vec.Eta();

	muP_vec.SetPtEtaPhiM( PtGeoCor_UF::PtGeo_BS_Roch(muP.d0_BS * muP.charge, muP.pt_Roch, muP.eta, std::stoi(YEAR)), muP.eta, muP.phi, 0.105658367 );
        muN_vec.SetPtEtaPhiM( PtGeoCor_UF::PtGeo_BS_Roch(muN.d0_BS * muN.charge, muN.pt_Roch, muN.eta, std::stoi(YEAR)), muN.eta, muN.phi, 0.105658367 );
        sum_vec = muP_vec + muN_vec;
        float dimu_mass_GeoBSRoch = sum_vec.M();
        float dimu_pt_GeoBSRoch = sum_vec.Pt();
        float dimu_eta_GeoBSRoch = sum_vec.Eta();

//        std::cout<<"dimu_mass_GeoBSRoch="<<dimu_mass_GeoBSRoch<<std::endl; //debugging

//	muP_vec.SetPtEtaPhiM(muP.pt - PtGeoCor::PtGeo_Wide_PF(muP.d0_PV * muP.charge, muP.pt, muP.eta, std::stoi(YEAR)), muP.eta, muP.phi, 0.105658367);
//	muN_vec.SetPtEtaPhiM(muN.pt - PtGeoCor::PtGeo_Wide_PF(muN.d0_PV * muN.charge, muN.pt, muN.eta, std::stoi(YEAR)), muN.eta, muN.phi, 0.105658367);
//	sum_vec = muP_vec + muN_vec;
//	float dimu_mass_PtWPF = sum_vec.M();
//
//	muP_vec.SetPtEtaPhiM(muP.pt_KaMu - PtGeoCor::PtGeo_Wide_KaMu(muP.d0_PV * muP.charge, muP.pt_KaMu, muP.eta, std::stoi(YEAR)), muP.eta, muP.phi, 0.105658367);
//        muN_vec.SetPtEtaPhiM(muN.pt_KaMu - PtGeoCor::PtGeo_Wide_KaMu(muN.d0_PV * muN.charge, muN.pt_KaMu, muN.eta, std::stoi(YEAR)), muN.eta, muN.phi, 0.105658367);
//        sum_vec = muP_vec + muN_vec;
//        float dimu_mass_PtWKaMu = sum_vec.M();
//
//	muP_vec.SetPtEtaPhiM(muP.pt - PtGeoCor::PtGeo_Narrow_PF(muP.d0_PV * muP.charge, muP.pt, muP.eta, std::stoi(YEAR)), muP.eta, muP.phi, 0.105658367);
//        muN_vec.SetPtEtaPhiM(muN.pt - PtGeoCor::PtGeo_Narrow_PF(muN.d0_PV * muN.charge, muN.pt, muN.eta, std::stoi(YEAR)), muN.eta, muN.phi, 0.105658367);
//        sum_vec = muP_vec + muN_vec;
//        float dimu_mass_PtNPF = sum_vec.M();
//
//	muP_vec.SetPtEtaPhiM(muP.pt_KaMu - PtGeoCor::PtGeo_Narrow_KaMu(muP.d0_PV * muP.charge, muP.pt_KaMu, muP.eta, std::stoi(YEAR)), muP.eta, muP.phi, 0.105658367);
//        muN_vec.SetPtEtaPhiM(muN.pt_KaMu - PtGeoCor::PtGeo_Narrow_KaMu(muN.d0_PV * muN.charge, muN.pt_KaMu, muN.eta, std::stoi(YEAR)), muN.eta, muN.phi, 0.105658367);
//        sum_vec = muP_vec + muN_vec;
//        float dimu_mass_PtNKaMu = sum_vec.M();
//
//	muP_vec.SetPtEtaPhiM(muP.pt_KaMu - PtGeoCor::PtGeo_Piece20_KaMu(muP.d0_PV * muP.charge, muP.pt_KaMu, muP.eta, std::stoi(YEAR)), muP.eta, muP.phi, 0.105658367);
//        muN_vec.SetPtEtaPhiM(muN.pt_KaMu - PtGeoCor::PtGeo_Piece20_KaMu(muN.d0_PV * muN.charge, muN.pt_KaMu, muN.eta, std::stoi(YEAR)), muN.eta, muN.phi, 0.105658367);
//        sum_vec = muP_vec + muN_vec;
//        float dimu_mass_PtSeg20KaMu = sum_vec.M();
//
//	muP_vec.SetPtEtaPhiM(muP.pt_KaMu - PtGeoCor::PtGeo_Piece25_KaMu(muP.d0_PV * muP.charge, muP.pt_KaMu, muP.eta, std::stoi(YEAR)), muP.eta, muP.phi, 0.105658367);
//        muN_vec.SetPtEtaPhiM(muN.pt_KaMu - PtGeoCor::PtGeo_Piece25_KaMu(muN.d0_PV * muN.charge, muN.pt_KaMu, muN.eta, std::stoi(YEAR)), muN.eta, muN.phi, 0.105658367);
//        sum_vec = muP_vec + muN_vec;
//        float dimu_mass_PtSeg25KaMu = sum_vec.M();
//
//	muP_vec.SetPtEtaPhiM(muP.pt_KaMu - PtGeoCor::PtGeo_Piece30_KaMu(muP.d0_PV * muP.charge, muP.pt_KaMu, muP.eta, std::stoi(YEAR)), muP.eta, muP.phi, 0.105658367);
//        muN_vec.SetPtEtaPhiM(muN.pt_KaMu - PtGeoCor::PtGeo_Piece30_KaMu(muN.d0_PV * muN.charge, muN.pt_KaMu, muN.eta, std::stoi(YEAR)), muN.eta, muN.phi, 0.105658367);
//        sum_vec = muP_vec + muN_vec;
//        float dimu_mass_PtSeg30KaMu = sum_vec.M();
//
//	muP_vec.SetPtEtaPhiM(muP.pt_KaMu - PtGeoCor::PtGeo_Alter20_KaMu(muP.d0_PV * muP.charge, muP.pt_KaMu, muP.eta, std::stoi(YEAR)), muP.eta, muP.phi, 0.105658367);
//        muN_vec.SetPtEtaPhiM(muN.pt_KaMu - PtGeoCor::PtGeo_Alter20_KaMu(muN.d0_PV * muN.charge, muN.pt_KaMu, muN.eta, std::stoi(YEAR)), muN.eta, muN.phi, 0.105658367);
//        sum_vec = muP_vec + muN_vec;
//        float dimu_mass_PtShift20KaMu = sum_vec.M();
//
//	muP_vec.SetPtEtaPhiM(muP.pt_KaMu - PtGeoCor::PtGeo_Alter25_KaMu(muP.d0_PV * muP.charge, muP.pt_KaMu, muP.eta, std::stoi(YEAR)), muP.eta, muP.phi, 0.105658367);
//        muN_vec.SetPtEtaPhiM(muN.pt_KaMu - PtGeoCor::PtGeo_Alter25_KaMu(muN.d0_PV * muN.charge, muN.pt_KaMu, muN.eta, std::stoi(YEAR)), muN.eta, muN.phi, 0.105658367);
//        sum_vec = muP_vec + muN_vec;
//        float dimu_mass_PtShift25KaMu = sum_vec.M();
//
//	muP_vec.SetPtEtaPhiM(muP.pt_KaMu - PtGeoCor::PtGeo_Alter30_KaMu(muP.d0_PV * muP.charge, muP.pt_KaMu, muP.eta, std::stoi(YEAR)), muP.eta, muP.phi, 0.105658367);
//        muN_vec.SetPtEtaPhiM(muN.pt_KaMu - PtGeoCor::PtGeo_Alter30_KaMu(muN.d0_PV * muN.charge, muN.pt_KaMu, muN.eta, std::stoi(YEAR)), muN.eta, muN.phi, 0.105658367);
//        sum_vec = muP_vec + muN_vec;
//        float dimu_mass_PtShift30KaMu = sum_vec.M();


//	float dimu_mass_PtGeoKaMu = 0.0;
//	float dimu_mass_PtGeoShiftKaMu = 0.0;
//	if (YEAR == "2016") {
//	  muP_vec.SetPtEtaPhiM(muP.pt_KaMu - PtGeoCor::PtGeo_tanh_KaMu(muP.d0_PV * muP.charge, muP.pt_KaMu, muP.eta, std::stoi(YEAR)), muP.eta, muP.phi, 0.105658367);
//          muN_vec.SetPtEtaPhiM(muN.pt_KaMu - PtGeoCor::PtGeo_tanh_KaMu(muN.d0_PV * muN.charge, muN.pt_KaMu, muN.eta, std::stoi(YEAR)), muN.eta, muN.phi, 0.105658367);
//          sum_vec = muP_vec + muN_vec;
//          dimu_mass_PtGeoKaMu = sum_vec.M();
//
//	  muP_vec.SetPtEtaPhiM(muP.pt_KaMu - PtGeoCor::PtGeo_tanhShift_KaMu(muP.d0_PV * muP.charge, muP.pt_KaMu, muP.eta, std::stoi(YEAR)), muP.eta, muP.phi, 0.105658367);
//          muN_vec.SetPtEtaPhiM(muN.pt_KaMu - PtGeoCor::PtGeo_tanhShift_KaMu(muN.d0_PV * muN.charge, muN.pt_KaMu, muN.eta, std::stoi(YEAR)), muN.eta, muN.phi, 0.105658367);
//          sum_vec = muP_vec + muN_vec;
//          dimu_mass_PtGeoShiftKaMu = sum_vec.M();
//	}
//	else if (YEAR == "2017" or YEAR == "2018") {
//	  muP_vec.SetPtEtaPhiM(muP.pt_Roch - PtGeoCor::PtGeo_tanh_KaMu(muP.d0_PV * muP.charge, muP.pt_Roch, muP.eta, std::stoi(YEAR)), muP.eta, muP.phi, 0.105658367);
//          muN_vec.SetPtEtaPhiM(muN.pt_Roch - PtGeoCor::PtGeo_tanh_KaMu(muN.d0_PV * muN.charge, muN.pt_Roch, muN.eta, std::stoi(YEAR)), muN.eta, muN.phi, 0.105658367);
//          sum_vec = muP_vec + muN_vec;
//          dimu_mass_PtGeoKaMu = sum_vec.M();
//
//          muP_vec.SetPtEtaPhiM(muP.pt_Roch - PtGeoCor::PtGeo_tanhShift_KaMu(muP.d0_PV * muP.charge, muP.pt_Roch, muP.eta, std::stoi(YEAR)), muP.eta, muP.phi, 0.105658367);
//          muN_vec.SetPtEtaPhiM(muN.pt_Roch - PtGeoCor::PtGeo_tanhShift_KaMu(muN.d0_PV * muN.charge, muN.pt_Roch, muN.eta, std::stoi(YEAR)), muN.eta, muN.phi, 0.105658367);
//          sum_vec = muP_vec + muN_vec;
//          dimu_mass_PtGeoShiftKaMu = sum_vec.M();
//	}

 	float d0_diff = (muP.d0_PV - muN.d0_PV) / 2.0;
	float d0_mean = (mu1.d0_PV + mu2.d0_PV) / 2.0;

	// some customized selections
	if ( CAT_CUTS.at(iCat) == "BB"    and ( abs(muP.eta)>=1.7 or  abs(muN.eta)>=1.7 ) ) continue;
	if ( CAT_CUTS.at(iCat) == "BE"    and ( abs(muP.eta)>=1.7 and abs(muN.eta)>=1.7 ) ) continue;
	if ( CAT_CUTS.at(iCat) == "BE"    and ( abs(muP.eta)< 1.7 and abs(muN.eta)< 1.7 ) ) continue;
	if ( CAT_CUTS.at(iCat) == "EE"    and ( abs(muP.eta)< 1.7 or  abs(muN.eta)< 1.7 ) ) continue;
	if ( CAT_CUTS.at(iCat) == "XX"    and ( abs(muP.eta)< 0.9 or  abs(muN.eta)< 0.9 ) ) continue;
	if ( CAT_CUTS.at(iCat) == "nonBB" and ( abs(muP.eta)< 1.7 and abs(muN.eta)< 1.7 ) ) continue;

	if ( CAT_CUTS.at(iCat) == "posBarrel"  and  abs(muN.eta) > 0.9 ) continue;
	if ( CAT_CUTS.at(iCat) == "posOverlap" and  ( abs(muN.eta) < 0.9 or abs(muN.eta) > 1.7 ) ) continue;
	if ( CAT_CUTS.at(iCat) == "posEndcap"  and  abs(muN.eta) < 1.7 ) continue;

//        std::cout<<"offset = " << off_set << "muP.d0_BS = " << muP.d0_BS << "muN.d0_BS = " << muN.d0_BS << std::endl; //debugging
//        std::cout<<"event_wgt = " << event_wgt << std::endl; //debugging
//        std::cout<<"pref_wgt = " <<br.l1pref_wgt << "GEN_wgt = " << br.GEN_wgt<<std::endl; //debugging

	mc_map_gen    [optCatStr]->FillEvent( (sample.Contains("SingleMu") ? "data" : sample) + "_" + optCatStr + "_gen",     gen_dimu.mass - off_set,     gen_muP.phi, gen_muN.phi, event_wgt, false);
	mc_map_genBS  [optCatStr]->FillEvent( (sample.Contains("SingleMu") ? "data" : sample) + "_" + optCatStr + "_genBS",   gen_dimu.mass - off_set,     gen_muP.phi, gen_muN.phi, event_wgt, false);
	mc_map_PF     [optCatStr]->FillEvent( (sample.Contains("SingleMu") ? "data" : sample) + "_" + optCatStr + "_PF",      dimu.mass - off_set, 	   muP.phi,     muN.phi, event_wgt, false);
	mc_map_Roch   [optCatStr]->FillEvent( (sample.Contains("SingleMu") ? "data" : sample) + "_" + optCatStr + "_Roch",    dimu.mass_Roch - off_set,    muP.phi, muN.phi, event_wgt, false);
	mc_map_RochBS [optCatStr]->FillEvent( (sample.Contains("SingleMu") ? "data" : sample) + "_" + optCatStr + "_RochBS",  dimu.mass_Roch - off_set,    muP.phi, muN.phi, event_wgt, false);
  	mc_map_Kinfit [optCatStr]->FillEvent( (sample.Contains("SingleMu") ? "data" : sample) + "_" + optCatStr + "_Kinfit",  dimu.mass_kinfit - off_set,  muP.phi, muN.phi, event_wgt, false);
	mc_map_KinRoch[optCatStr]->FillEvent( (sample.Contains("SingleMu") ? "data" : sample) + "_" + optCatStr + "_KinRoch", dimu_mass_KinRoch - off_set, muP.phi, muN.phi, event_wgt, false);
//	mc_map_KaMu   [optCatStr]->FillEvent( (sample.Contains("SingleMu") ? "data" : sample) + "_" + optCatStr + "_KaMu",    dimu.mass_KaMu - off_set,    muP.d0_PV, muP.phi, event_wgt, false);
//	mc_map_KinKaMu[optCatStr]->FillEvent( (sample.Contains("SingleMu") ? "data" : sample) + "_" + optCatStr + "_KinKaMu", dimu_mass_KinKaMu - off_set, muP.d0_PV, muP.phi, event_wgt, false);

//	mc_map_GeoRoch  [optCatStr]->FillEvent( (sample.Contains("SingleMu") ? "data" : sample) + "_" + optCatStr + "_GeoRoch",   dimu_mass_GeoRoch - off_set,   muP.d0_PV, muN.d0_PV, event_wgt, false);
	mc_map_GeoBSRoch[optCatStr]->FillEvent( (sample.Contains("SingleMu") ? "data" : sample) + "_" + optCatStr + "_GeoBSRoch", dimu_mass_GeoBSRoch - off_set, muP_vec.Phi(), muN.phi, event_wgt, false);

//        std::cout<<"at line 543"<<std::endl; //debugging
//	mc_map_PtWPF  [optCatStr]->FillEvent( (sample.Contains("SingleMu") ? "data" : sample) + "_" + optCatStr + "_PtWPF",   dimu_mass_PtWPF   - off_set, muP.d0_PV, muN.d0_PV, event_wgt, false);
//	mc_map_PtWKaMu[optCatStr]->FillEvent( (sample.Contains("SingleMu") ? "data" : sample) + "_" + optCatStr + "_PtWKaMu", dimu_mass_PtWKaMu - off_set, muP.d0_PV, muN.d0_PV, event_wgt, false);
// 	mc_map_PtNPF  [optCatStr]->FillEvent( (sample.Contains("SingleMu") ? "data" : sample) + "_" + optCatStr + "_PtNPF",   dimu_mass_PtNPF   - off_set, muP.d0_PV, muN.d0_PV, event_wgt, false);
//        mc_map_PtNKaMu[optCatStr]->FillEvent( (sample.Contains("SingleMu") ? "data" : sample) + "_" + optCatStr + "_PtNKaMu", dimu_mass_PtNKaMu - off_set, muP.d0_PV, muN.d0_PV, event_wgt, false);
//
//	mc_map_PtSeg20KaMu[optCatStr]->FillEvent( (sample.Contains("SingleMu") ? "data" : sample) + "_" + optCatStr + "_PtSeg20KaMu", dimu_mass_PtSeg20KaMu - off_set, muP.d0_PV, muN.d0_PV, event_wgt, false);
//	mc_map_PtSeg25KaMu[optCatStr]->FillEvent( (sample.Contains("SingleMu") ? "data" : sample) + "_" + optCatStr + "_PtSeg25KaMu", dimu_mass_PtSeg25KaMu - off_set, muP.d0_PV, muN.d0_PV, event_wgt, false);
//	mc_map_PtSeg30KaMu[optCatStr]->FillEvent( (sample.Contains("SingleMu") ? "data" : sample) + "_" + optCatStr + "_PtSeg30KaMu", dimu_mass_PtSeg30KaMu - off_set, muP.d0_PV, muN.d0_PV, event_wgt, false);
//	mc_map_PtShift20KaMu[optCatStr]->FillEvent( (sample.Contains("SingleMu") ? "data" : sample) + "_" + optCatStr + "_PtShift20KaMu", dimu_mass_PtShift20KaMu - off_set, muP.d0_PV, muN.d0_PV, event_wgt, false);
//        mc_map_PtShift25KaMu[optCatStr]->FillEvent( (sample.Contains("SingleMu") ? "data" : sample) + "_" + optCatStr + "_PtShift25KaMu", dimu_mass_PtShift25KaMu - off_set, muP.d0_PV, muN.d0_PV, event_wgt, false);
//        mc_map_PtShift30KaMu[optCatStr]->FillEvent( (sample.Contains("SingleMu") ? "data" : sample) + "_" + optCatStr + "_PtShift30KaMu", dimu_mass_PtShift30KaMu - off_set, muP.d0_PV, muN.d0_PV, event_wgt, false);
//	mc_map_PtGeoKaMu     [optCatStr]->FillEvent( (sample.Contains("SingleMu") ? "data" : sample) + "_" + optCatStr + "_PtGeoKaMu",      dimu_mass_PtGeoKaMu      - off_set, muP.d0_PV, muN.d0_PV, event_wgt, false);
//	mc_map_PtGeoShiftKaMu[optCatStr]->FillEvent( (sample.Contains("SingleMu") ? "data" : sample) + "_" + optCatStr + "_PtGeoShiftKaMu", dimu_mass_PtGeoShiftKaMu - off_set, muP.d0_PV, muN.d0_PV, event_wgt, false);

//	mc_map_AdHoc0    [optCatStr]->FillEvent( (sample.Contains("SingleMu") ? "data" : sample) + "_" + optCatStr + "_AdHoc0",    dimu_mass_AdHoc0 - off_set,    muP.d0_PV, muN.d0_PV, event_wgt, false);
//	mc_map_AdHoc_re  [optCatStr]->FillEvent( (sample.Contains("SingleMu") ? "data" : sample) + "_" + optCatStr + "_AdHoc_re",  dimu_mass_AdHoc_re - off_set,  muP.d0_PV, muN.d0_PV, event_wgt, false);
//	mc_map_AdHoc_eta [optCatStr]->FillEvent( (sample.Contains("SingleMu") ? "data" : sample) + "_" + optCatStr + "_AdHoc_eta", dimu_mass_AdHoc_eta - off_set, muP.d0_PV, muN.d0_PV, event_wgt, false);
//	mc_map_AdHocKaMu [optCatStr]->FillEvent( (sample.Contains("SingleMu") ? "data" : sample) + "_" + optCatStr + "_AdHocKaMu", dimu_mass_AdHocKaMu - off_set, muP.d0_PV, muN.d0_PV, event_wgt, false);


//	if (mu1.d0_PV_kinfit > -1 and mu2.d0_PV_kinfit > -1) {
//	  mc_map_Kingood[optCatStr]->FillEvent( (sample.Contains("SingleMu") ? "data" : sample) + "_" + optCatStr + "_good_Kinfit", dimu.mass_kinfit, d0_diff, event_wgt, false);
//	  float d0kin_diff = (muP.d0_PV_kinfit - muN.d0_PV_kinfit) / 2.0;
//	  float d0kin_mean = (muP.d0_PV_kinfit + muN.d0_PV_kinfit) / 2.0;
//	  mc_map_Kind0[optCatStr]->FillEvent( (sample.Contains("SingleMu") ? "data" : sample) + "_" + optCatStr + "_Kin_vs_d0kin", dimu.mass_kinfit, d0kin_diff, event_wgt, false);
//
//	  if (abs(mu1.eta) < 0.9 and abs(mu2.eta) < 0.9) mc_map_Kind0_BB[optCatStr]->FillEvent( (sample.Contains("SingleMu") ? "data" : sample) + "_" + optCatStr + "_Kin_vs_d0kin_BB", dimu_mass_KinRoch, d0kin_diff, event_wgt, false);
//	  if (abs(mu1.eta) > 1.2 and abs(mu2.eta) > 1.2) mc_map_Kind0_EE[optCatStr]->FillEvent( (sample.Contains("SingleMu") ? "data" : sample) + "_" + optCatStr + "_Kin_vs_d0kin_EE", dimu_mass_KinRoch, d0kin_diff, event_wgt, false);
//	  if (abs(mu1.eta) < 0.9 and abs(mu2.eta) > 1.2) mc_map_Kind0_BE[optCatStr]->FillEvent( (sample.Contains("SingleMu") ? "data" : sample) + "_" + optCatStr + "_Kin_vs_d0kin_BE", dimu_mass_KinRoch, d0kin_diff, event_wgt, false);
//	  if (abs(mu1.eta) > 1.2 and abs(mu2.eta) < 0.9) mc_map_Kind0_BE[optCatStr]->FillEvent( (sample.Contains("SingleMu") ? "data" : sample) + "_" + optCatStr + "_Kin_vs_d0kin_BE", dimu_mass_KinRoch, d0kin_diff, event_wgt, false);

//	  if ( d0_diff > -0.005 and d0_diff < -0.0015 )       mc_map_Kind0_N50_N15[optCatStr]->FillEvent( (sample.Contains("SingleMu") ? "data" : sample) + "_" + optCatStr + "_Kin_vs_d0kin_d0PV_N50_N15", dimu.mass_kinfit, d0kin_diff, event_wgt, false);
//	  else if ( d0_diff > -0.0015 and d0_diff < -0.0005 ) mc_map_Kind0_N15_N05[optCatStr]->FillEvent( (sample.Contains("SingleMu") ? "data" : sample) + "_" + optCatStr + "_Kin_vs_d0kin_d0PV_N15_N05", dimu.mass_kinfit, d0kin_diff, event_wgt, false);
//	  else if ( d0_diff > 0.0005 and d0_diff < 0.0015 )   mc_map_Kind0_P05_P15[optCatStr]->FillEvent( (sample.Contains("SingleMu") ? "data" : sample) + "_" + optCatStr + "_Kin_vs_d0kin_d0PV_P05_P15", dimu.mass_kinfit, d0kin_diff, event_wgt, false);
//	  else if ( d0_diff > 0.0015 and d0_diff < 0.005 )    mc_map_Kind0_P15_P50[optCatStr]->FillEvent( (sample.Contains("SingleMu") ? "data" : sample) + "_" + optCatStr + "_Kin_vs_d0kin_d0PV_P15_P50", dimu.mass_kinfit, d0kin_diff, event_wgt, false);
//	}

      } // End loop: for (int iCat = 0; iCat < CAT_CUTS.size(); iCat++)
    } // End loop: for (int iOpt = 0; iOpt < OPT_CUTS.size(); iOpt++)

  } // End loop: for (int iEvt = 0; iEvt < in_chain->GetEntries(); iEvt++)
  std::cout << "\n******* Leaving the event loop *******" << std::endl;


  // Place histograms made with separate selection and cateogry cuts into separate files
  for (int iOpt = 0; iOpt < OPT_CUTS.size(); iOpt++) {
    for (int iCat = 0; iCat < CAT_CUTS.size(); iCat++) {
      TString optCatStr = OPT_CUTS.at(iOpt)+"_"+CAT_CUTS.at(iCat);
      
      // Create output file
      TString out_file_name;
      if (out_file_str.Length() > 0) out_file_name.Form( "%s/MassCal_%s_%s_%s.root",    out_dir.Data(), sample.Data(), optCatStr.Data(), out_file_str.Data() );
      else                           out_file_name.Form( "%s/MassCal_%s_%s_%d_%d.root", out_dir.Data(), sample.Data(), optCatStr.Data(), MIN_FILE, MAX_FILE );
      std::cout << "\nCreating output file " << out_file_name.Data() << std::endl;
      TFile* out_file = TFile::Open( out_file_name, "RECREATE" );
      
      // Write output file
      if (verbose) std::cout << "\nWriting output file " << out_file_name.Data() << std::endl;
      out_file->cd();

      TDirectory* gen_dir = out_file->mkdir("gen");
      TDirectory* genBS_dir = out_file->mkdir("genBS");
      TDirectory* PF_dir = out_file->mkdir("PF");
      TDirectory* Roch_dir = out_file->mkdir("Roch");
      TDirectory* RochBS_dir = out_file->mkdir("RochBS");
      TDirectory* Kinfit_dir = out_file->mkdir("Kinfit");
      TDirectory* KinRoch_dir = out_file->mkdir("KinRoch");
//      TDirectory* KaMu_dir = out_file->mkdir("KaMu");
//      TDirectory* KinKaMu_dir = out_file->mkdir("KinKaMu");

//      TDirectory* GeoRoch_dir = out_file->mkdir("GeoRoch");
      TDirectory* GeoBSRoch_dir = out_file->mkdir("GeoBSRoch");

//      TDirectory* PtWPF_dir   = out_file->mkdir("PtWPF");
//      TDirectory* PtWKaMu_dir = out_file->mkdir("PtWKaMu");
//      TDirectory* PtNPF_dir   = out_file->mkdir("PtNPF");
//      TDirectory* PtNKaMu_dir = out_file->mkdir("PtNKaMu");
//
//      TDirectory* PtSeg20KaMu_dir = out_file->mkdir("PtSeg20KaMu");
//      TDirectory* PtSeg25KaMu_dir = out_file->mkdir("PtSeg25KaMu");
//      TDirectory* PtSeg30KaMu_dir = out_file->mkdir("PtSeg30KaMu");
//      TDirectory* PtShift20KaMu_dir = out_file->mkdir("PtShift20KaMu");
//      TDirectory* PtShift25KaMu_dir = out_file->mkdir("PtShift25KaMu");
//      TDirectory* PtShift30KaMu_dir = out_file->mkdir("PtShift30KaMu");
//      TDirectory* PtGeoKaMu_dir      = out_file->mkdir("PtGeoKaMu");
//      TDirectory* PtGeoShiftKaMu_dir = out_file->mkdir("PtGeoShiftKaMu"); 

//      TDirectory* AdHoc0_dir = out_file->mkdir("AdHoc0");
//      TDirectory* AdHoc_re_dir  = out_file->mkdir("AdHoc_re");
//      TDirectory* AdHoc_eta_dir = out_file->mkdir("AdHoc_eta");
//      TDirectory* AdHocKaMu_dir = out_file->mkdir("AdHocKaMu");

//      TDirectory* Kingood_dir = out_file->mkdir("good_Kinfit");    
//      TDirectory* Kind0_dir = out_file->mkdir("Kin_vs_d0kin");
//
//      TDirectory* Kind0_BB_dir = out_file->mkdir("Kin_vs_d0kin_BB");
//      TDirectory* Kind0_BE_dir = out_file->mkdir("Kin_vs_d0kin_BE");
//      TDirectory* Kind0_EE_dir = out_file->mkdir("Kin_vs_d0kin_EE");

//      TDirectory* Kind0_N50_N15_dir  = out_file->mkdir("Kin_vs_d0kin_d0PV_N50_N15");
//      TDirectory* Kind0_N15_N05_dir  = out_file->mkdir("Kin_vs_d0kin_d0PV_N15_N05");
//      TDirectory* Kind0_P05_P15_dir  = out_file->mkdir("Kin_vs_d0kin_d0PV_P05_P15");
//      TDirectory* Kind0_P15_P50_dir  = out_file->mkdir("Kin_vs_d0kin_d0PV_P15_P50");

      MassCalPlots* mc_gen       = mc_map_gen[optCatStr]; 
      MassCalPlots* mc_genBS     = mc_map_genBS[optCatStr];
      MassCalPlots* mc_PF	 = mc_map_PF[optCatStr];
      MassCalPlots* mc_Roch	 = mc_map_Roch[optCatStr];
      MassCalPlots* mc_RochBS    = mc_map_RochBS[optCatStr];
      MassCalPlots* mc_Kinfit	 = mc_map_Kinfit[optCatStr];
      MassCalPlots* mc_KinRoch   = mc_map_KinRoch[optCatStr]; 
//      MassCalPlots* mc_KaMu      = mc_map_KaMu[optCatStr];
//      MassCalPlots* mc_KinKaMu   = mc_map_KinKaMu[optCatStr];

//      MassCalPlots* mc_GeoRoch     = mc_map_GeoRoch[optCatStr];
      MassCalPlots* mc_GeoBSRoch   = mc_map_GeoBSRoch[optCatStr];


//      MassCalPlots* mc_PtWPF     = mc_map_PtWPF[optCatStr];
//      MassCalPlots* mc_PtWKaMu   = mc_map_PtWKaMu[optCatStr];
//      MassCalPlots* mc_PtNPF     = mc_map_PtNPF[optCatStr];
//      MassCalPlots* mc_PtNKaMu   = mc_map_PtNKaMu[optCatStr];
//      
//      MassCalPlots* mc_PtSeg20KaMu   = mc_map_PtSeg20KaMu[optCatStr];
//      MassCalPlots* mc_PtSeg25KaMu   = mc_map_PtSeg25KaMu[optCatStr];
//      MassCalPlots* mc_PtSeg30KaMu   = mc_map_PtSeg30KaMu[optCatStr];
//      MassCalPlots* mc_PtShift20KaMu   = mc_map_PtShift20KaMu[optCatStr];
//      MassCalPlots* mc_PtShift25KaMu   = mc_map_PtShift25KaMu[optCatStr];
//      MassCalPlots* mc_PtShift30KaMu   = mc_map_PtShift30KaMu[optCatStr];
//      MassCalPlots* mc_PtGeoKaMu       = mc_map_PtGeoKaMu[optCatStr];
//      MassCalPlots* mc_PtGeoShiftKaMu  = mc_map_PtGeoShiftKaMu[optCatStr];

//      MassCalPlots* mc_AdHoc0     = mc_map_AdHoc0[optCatStr];
//      MassCalPlots* mc_AdHoc_re   = mc_map_AdHoc_re[optCatStr];
//      MassCalPlots* mc_AdHoc_eta  = mc_map_AdHoc_eta[optCatStr];
//      MassCalPlots* mc_AdHocKaMu  = mc_map_AdHocKaMu[optCatStr];

//      MassCalPlots* mc_Kingood   = mc_map_Kingood[optCatStr]; 
//      MassCalPlots* mc_Kind0     = mc_map_Kind0[optCatStr];
//
//      MassCalPlots* mc_Kind0_BB     = mc_map_Kind0_BB[optCatStr];
//      MassCalPlots* mc_Kind0_BE     = mc_map_Kind0_BE[optCatStr];
//      MassCalPlots* mc_Kind0_EE     = mc_map_Kind0_EE[optCatStr];

//      MassCalPlots* mc_Kind0_N50_N15    = mc_map_Kind0_N50_N15[optCatStr];
//      MassCalPlots* mc_Kind0_N15_N05    = mc_map_Kind0_N15_N05[optCatStr];
//      MassCalPlots* mc_Kind0_P05_P15    = mc_map_Kind0_P05_P15[optCatStr];
//      MassCalPlots* mc_Kind0_P15_P50    = mc_map_Kind0_P15_P50[optCatStr];

      gen_dir->cd();
      mc_gen->summary_plot_2D->Write();
      for (std::map<TString, TH1*>::iterator it = mc_gen->mass_plots.begin(); it != mc_gen->mass_plots.end(); ++it) {
        std::string h_name = it->second->GetName();
        h_name.erase( h_name.find(optCatStr+"_"), optCatStr.Length() + 1 );
        it->second->SetName(h_name.c_str());
        it->second->SetTitle(h_name.c_str());
        it->second->Write();
      }

      genBS_dir->cd();
      mc_genBS->summary_plot_2D->Write();
      for (std::map<TString, TH1*>::iterator it = mc_genBS->mass_plots.begin(); it != mc_genBS->mass_plots.end(); ++it) {
        std::string h_name = it->second->GetName();
        h_name.erase( h_name.find(optCatStr+"_"), optCatStr.Length() + 1 );
        it->second->SetName(h_name.c_str());
        it->second->SetTitle(h_name.c_str());
        it->second->Write();
      }

      PF_dir->cd();
      mc_PF->summary_plot_2D->Write();
      for (std::map<TString, TH1*>::iterator it = mc_PF->mass_plots.begin(); it != mc_PF->mass_plots.end(); ++it) {
	std::string h_name = it->second->GetName();
	h_name.erase( h_name.find(optCatStr+"_"), optCatStr.Length() + 1 );
	it->second->SetName(h_name.c_str());
	it->second->SetTitle(h_name.c_str());
  	it->second->Write();
      }

      Roch_dir->cd();
      mc_Roch->summary_plot_2D->Write();
      for (std::map<TString, TH1*>::iterator it = mc_Roch->mass_plots.begin(); it != mc_Roch->mass_plots.end(); ++it) {
	std::string h_name = it->second->GetName();
        h_name.erase( h_name.find(optCatStr+"_"), optCatStr.Length() + 1 );
        it->second->SetName(h_name.c_str());
        it->second->SetTitle(h_name.c_str());
  	it->second->Write(); 
      }

      RochBS_dir->cd();
      mc_RochBS->summary_plot_2D->Write();
      for (std::map<TString, TH1*>::iterator it = mc_RochBS->mass_plots.begin(); it != mc_RochBS->mass_plots.end(); ++it) {
        std::string h_name = it->second->GetName();
        h_name.erase( h_name.find(optCatStr+"_"), optCatStr.Length() + 1 );
        it->second->SetName(h_name.c_str());
        it->second->SetTitle(h_name.c_str());
        it->second->Write();
      }

      Kinfit_dir->cd();
      mc_Kinfit->summary_plot_2D->Write();
      for (std::map<TString, TH1*>::iterator it = mc_Kinfit->mass_plots.begin(); it != mc_Kinfit->mass_plots.end(); ++it) {
	std::string h_name = it->second->GetName();
        h_name.erase( h_name.find(optCatStr+"_"), optCatStr.Length() + 1 );
        it->second->SetName(h_name.c_str());
        it->second->SetTitle(h_name.c_str());
  	it->second->Write();
      }

      KinRoch_dir->cd();
      mc_KinRoch->summary_plot_2D->Write();
      for (std::map<TString, TH1*>::iterator it = mc_KinRoch->mass_plots.begin(); it != mc_KinRoch->mass_plots.end(); ++it) {
	std::string h_name = it->second->GetName();
        h_name.erase( h_name.find(optCatStr+"_"), optCatStr.Length() + 1 );
        it->second->SetName(h_name.c_str());
        it->second->SetTitle(h_name.c_str());
   	it->second->Write();
      }

//      KaMu_dir->cd();
//      mc_KaMu->summary_plot_2D->Write();
//      for (std::map<TString, TH1*>::iterator it = mc_KaMu->mass_plots.begin(); it != mc_KaMu->mass_plots.end(); ++it) {
//        std::string h_name = it->second->GetName();
//        h_name.erase( h_name.find(optCatStr+"_"), optCatStr.Length() + 1 );
//        it->second->SetName(h_name.c_str());
//        it->second->SetTitle(h_name.c_str());
//        it->second->Write();
//      }
//
//      KinKaMu_dir->cd();
//      mc_KinKaMu->summary_plot_2D->Write();
//      for (std::map<TString, TH1*>::iterator it = mc_KinKaMu->mass_plots.begin(); it != mc_KinKaMu->mass_plots.end(); ++it) {
//        std::string h_name = it->second->GetName();
//        h_name.erase( h_name.find(optCatStr+"_"), optCatStr.Length() + 1 );
//        it->second->SetName(h_name.c_str());
//        it->second->SetTitle(h_name.c_str());
//        it->second->Write();
//      }

//      GeoRoch_dir->cd();
//      mc_GeoRoch->summary_plot_2D->Write();
//      for (std::map<TString, TH1*>::iterator it = mc_GeoRoch->mass_plots.begin(); it != mc_GeoRoch->mass_plots.end(); ++it) {
//        std::string h_name = it->second->GetName();
//        h_name.erase( h_name.find(optCatStr+"_"), optCatStr.Length() + 1 );
//        it->second->SetName(h_name.c_str());
//        it->second->SetTitle(h_name.c_str());
//        it->second->Write();
//      }

      GeoBSRoch_dir->cd();
      mc_GeoBSRoch->summary_plot_2D->Write();
      for (std::map<TString, TH1*>::iterator it = mc_GeoBSRoch->mass_plots.begin(); it != mc_GeoBSRoch->mass_plots.end(); ++it) {
        std::string h_name = it->second->GetName();
        h_name.erase( h_name.find(optCatStr+"_"), optCatStr.Length() + 1 );
        it->second->SetName(h_name.c_str());
        it->second->SetTitle(h_name.c_str());
        it->second->Write();
      }


//      PtWPF_dir->cd();
//      mc_PtWPF->summary_plot_2D->Write();
//      for (std::map<TString, TH1*>::iterator it = mc_PtWPF->mass_plots.begin(); it != mc_PtWPF->mass_plots.end(); ++it) {
//        std::string h_name = it->second->GetName();
//        h_name.erase( h_name.find(optCatStr+"_"), optCatStr.Length() + 1 );
//        it->second->SetName(h_name.c_str());
//        it->second->SetTitle(h_name.c_str());
//        it->second->Write();
//      }
//
//      PtWKaMu_dir->cd();
//      mc_PtWKaMu->summary_plot_2D->Write();
//      for (std::map<TString, TH1*>::iterator it = mc_PtWKaMu->mass_plots.begin(); it != mc_PtWKaMu->mass_plots.end(); ++it) {
//        std::string h_name = it->second->GetName();
//        h_name.erase( h_name.find(optCatStr+"_"), optCatStr.Length() + 1 );
//        it->second->SetName(h_name.c_str());
//        it->second->SetTitle(h_name.c_str());
//        it->second->Write();
//      }
//
//      PtNPF_dir->cd();
//      mc_PtNPF->summary_plot_2D->Write();
//      for (std::map<TString, TH1*>::iterator it = mc_PtNPF->mass_plots.begin(); it != mc_PtNPF->mass_plots.end(); ++it) {
//        std::string h_name = it->second->GetName();
//        h_name.erase( h_name.find(optCatStr+"_"), optCatStr.Length() + 1 );
//        it->second->SetName(h_name.c_str());
//        it->second->SetTitle(h_name.c_str());
//        it->second->Write();
//      }
//
//      PtNKaMu_dir->cd();
//      mc_PtNKaMu->summary_plot_2D->Write();
//      for (std::map<TString, TH1*>::iterator it = mc_PtNKaMu->mass_plots.begin(); it != mc_PtNKaMu->mass_plots.end(); ++it) {
//        std::string h_name = it->second->GetName();
//        h_name.erase( h_name.find(optCatStr+"_"), optCatStr.Length() + 1 );
//        it->second->SetName(h_name.c_str());
//        it->second->SetTitle(h_name.c_str());
//        it->second->Write();
//      }
//
//      PtSeg20KaMu_dir->cd();
//      mc_PtSeg20KaMu->summary_plot_2D->Write();
//      for (std::map<TString, TH1*>::iterator it = mc_PtSeg20KaMu->mass_plots.begin(); it != mc_PtSeg20KaMu->mass_plots.end(); ++it) {
//        std::string h_name = it->second->GetName();
//        h_name.erase( h_name.find(optCatStr+"_"), optCatStr.Length() + 1 );
//        it->second->SetName(h_name.c_str());
//        it->second->SetTitle(h_name.c_str());
//        it->second->Write();
//      }
//
//     PtSeg25KaMu_dir->cd();
//      mc_PtSeg25KaMu->summary_plot_2D->Write();
//      for (std::map<TString, TH1*>::iterator it = mc_PtSeg25KaMu->mass_plots.begin(); it != mc_PtSeg25KaMu->mass_plots.end(); ++it) {
//        std::string h_name = it->second->GetName();
//        h_name.erase( h_name.find(optCatStr+"_"), optCatStr.Length() + 1 );
//        it->second->SetName(h_name.c_str());
//        it->second->SetTitle(h_name.c_str());
//        it->second->Write();
//      }
//
//      PtSeg30KaMu_dir->cd();
//      mc_PtSeg30KaMu->summary_plot_2D->Write();
//      for (std::map<TString, TH1*>::iterator it = mc_PtSeg30KaMu->mass_plots.begin(); it != mc_PtSeg30KaMu->mass_plots.end(); ++it) {
//        std::string h_name = it->second->GetName();
//        h_name.erase( h_name.find(optCatStr+"_"), optCatStr.Length() + 1 );
//        it->second->SetName(h_name.c_str());
//        it->second->SetTitle(h_name.c_str());
//        it->second->Write();
//      }
//
//      PtShift20KaMu_dir->cd();
//      mc_PtShift20KaMu->summary_plot_2D->Write();
//      for (std::map<TString, TH1*>::iterator it = mc_PtShift20KaMu->mass_plots.begin(); it != mc_PtShift20KaMu->mass_plots.end(); ++it) {
//        std::string h_name = it->second->GetName();
//        h_name.erase( h_name.find(optCatStr+"_"), optCatStr.Length() + 1 );
//        it->second->SetName(h_name.c_str());
//        it->second->SetTitle(h_name.c_str());
//        it->second->Write();
//      }
//
//     PtShift25KaMu_dir->cd();
//      mc_PtShift25KaMu->summary_plot_2D->Write();
//      for (std::map<TString, TH1*>::iterator it = mc_PtShift25KaMu->mass_plots.begin(); it != mc_PtShift25KaMu->mass_plots.end(); ++it) {
//        std::string h_name = it->second->GetName();
//        h_name.erase( h_name.find(optCatStr+"_"), optCatStr.Length() + 1 );
//        it->second->SetName(h_name.c_str());
//        it->second->SetTitle(h_name.c_str());
//        it->second->Write();
//      }
//
//      PtShift30KaMu_dir->cd();
//      mc_PtShift30KaMu->summary_plot_2D->Write();
//      for (std::map<TString, TH1*>::iterator it = mc_PtShift30KaMu->mass_plots.begin(); it != mc_PtShift30KaMu->mass_plots.end(); ++it) {
//        std::string h_name = it->second->GetName();
//        h_name.erase( h_name.find(optCatStr+"_"), optCatStr.Length() + 1 );
//        it->second->SetName(h_name.c_str());
//        it->second->SetTitle(h_name.c_str());
//        it->second->Write();
//      }

//      PtGeoKaMu_dir->cd();
//      mc_PtGeoKaMu->summary_plot_2D->Write();
//      for (std::map<TString, TH1*>::iterator it = mc_PtGeoKaMu->mass_plots.begin(); it != mc_PtGeoKaMu->mass_plots.end(); ++it) {
//        std::string h_name = it->second->GetName();
//        h_name.erase( h_name.find(optCatStr+"_"), optCatStr.Length() + 1 );
//        it->second->SetName(h_name.c_str());
//        it->second->SetTitle(h_name.c_str());
//        it->second->Write();
//      }
//
//      PtGeoShiftKaMu_dir->cd();
//      mc_PtGeoShiftKaMu->summary_plot_2D->Write();
//      for (std::map<TString, TH1*>::iterator it = mc_PtGeoShiftKaMu->mass_plots.begin(); it != mc_PtGeoShiftKaMu->mass_plots.end(); ++it) {
//        std::string h_name = it->second->GetName();
//        h_name.erase( h_name.find(optCatStr+"_"), optCatStr.Length() + 1 );
//        it->second->SetName(h_name.c_str());
//        it->second->SetTitle(h_name.c_str());
//        it->second->Write();
//      }


//      AdHoc0_dir->cd();
//      mc_AdHoc0->summary_plot_2D->Write();
//      for (std::map<TString, TH1*>::iterator it = mc_AdHoc0->mass_plots.begin(); it != mc_AdHoc0->mass_plots.end(); ++it) {
//        std::string h_name = it->second->GetName();
//        h_name.erase( h_name.find(optCatStr+"_"), optCatStr.Length() + 1 );
//        it->second->SetName(h_name.c_str());
//        it->second->SetTitle(h_name.c_str());
//        it->second->Write();
//      }
//
//      AdHoc_re_dir->cd();
//      mc_AdHoc_re->summary_plot_2D->Write();
//      for (std::map<TString, TH1*>::iterator it = mc_AdHoc_re->mass_plots.begin(); it != mc_AdHoc_re->mass_plots.end(); ++it) {
//        std::string h_name = it->second->GetName();
//        h_name.erase( h_name.find(optCatStr+"_"), optCatStr.Length() + 1 );
//        it->second->SetName(h_name.c_str());
//        it->second->SetTitle(h_name.c_str());
//        it->second->Write();
//      }
//  
//      AdHoc_eta_dir->cd();
//      mc_AdHoc_eta->summary_plot_2D->Write();
//      for (std::map<TString, TH1*>::iterator it = mc_AdHoc_eta->mass_plots.begin(); it != mc_AdHoc_eta->mass_plots.end(); ++it) {
//        std::string h_name = it->second->GetName();
//        h_name.erase( h_name.find(optCatStr+"_"), optCatStr.Length() + 1 );
//        it->second->SetName(h_name.c_str());
//        it->second->SetTitle(h_name.c_str());
//        it->second->Write();
//      }

//      AdHocKaMu_dir->cd();
//      mc_AdHocKaMu->summary_plot_2D->Write();
//      for (std::map<TString, TH1*>::iterator it = mc_AdHocKaMu->mass_plots.begin(); it != mc_AdHocKaMu->mass_plots.end(); ++it) {
//        std::string h_name = it->second->GetName();
//        h_name.erase( h_name.find(optCatStr+"_"), optCatStr.Length() + 1 );
//        it->second->SetName(h_name.c_str());
//        it->second->SetTitle(h_name.c_str());
//        it->second->Write();
//      }



//      Kingood_dir->cd();
//      mc_Kingood->summary_plot_1D->Write();
//      for (std::map<TString, TH1*>::iterator it = mc_Kingood->mass_plots.begin(); it != mc_Kingood->mass_plots.end(); ++it) {
//        std::string h_name = it->second->GetName();
//        h_name.erase( h_name.find(optCatStr+"_"), optCatStr.Length() + 1 );
//        it->second->SetName(h_name.c_str());
//        it->second->SetTitle(h_name.c_str());
//        it->second->Write();
//      }
//
//      Kind0_dir->cd();
//      mc_Kind0->summary_plot_1D->Write();
//      for (std::map<TString, TH1*>::iterator it = mc_Kind0->mass_plots.begin(); it != mc_Kind0->mass_plots.end(); ++it) {
//        std::string h_name = it->second->GetName();
//        h_name.erase( h_name.find(optCatStr+"_"), optCatStr.Length() + 1 );
//        it->second->SetName(h_name.c_str());
//        it->second->SetTitle(h_name.c_str());
//        it->second->Write();
//      }
//
//
//
//      Kind0_BB_dir->cd();
//      mc_Kind0_BB->summary_plot_1D->Write();
//      for (std::map<TString, TH1*>::iterator it = mc_Kind0_BB->mass_plots.begin(); it != mc_Kind0_BB->mass_plots.end(); ++it) {
//        std::string h_name = it->second->GetName();
//        h_name.erase( h_name.find(optCatStr+"_"), optCatStr.Length() + 1 );
//        it->second->SetName(h_name.c_str());
//        it->second->SetTitle(h_name.c_str());
//        it->second->Write();
//      }
//
//      Kind0_BE_dir->cd();
//      mc_Kind0_BE->summary_plot_1D->Write();
//      for (std::map<TString, TH1*>::iterator it = mc_Kind0_BE->mass_plots.begin(); it != mc_Kind0_BE->mass_plots.end(); ++it) {
//        std::string h_name = it->second->GetName();
//        h_name.erase( h_name.find(optCatStr+"_"), optCatStr.Length() + 1 );
//        it->second->SetName(h_name.c_str());
//        it->second->SetTitle(h_name.c_str());
//        it->second->Write();
//      }
//
//      Kind0_EE_dir->cd();
//      mc_Kind0_EE->summary_plot_1D->Write();
//      for (std::map<TString, TH1*>::iterator it = mc_Kind0_EE->mass_plots.begin(); it != mc_Kind0_EE->mass_plots.end(); ++it) {
//        std::string h_name = it->second->GetName();
//        h_name.erase( h_name.find(optCatStr+"_"), optCatStr.Length() + 1 );
//        it->second->SetName(h_name.c_str());
//        it->second->SetTitle(h_name.c_str());
//        it->second->Write();
//      }

      
//      Kind0_N50_N15_dir->cd();
//      mc_Kind0_N50_N15->summary_plot_1D->Write();
//      for (std::map<TString, TH1*>::iterator it = mc_Kind0_N50_N15->mass_plots.begin(); it != mc_Kind0_N50_N15->mass_plots.end(); ++it) {
//        std::string h_name = it->second->GetName();
//        h_name.erase( h_name.find(optCatStr+"_"), optCatStr.Length() + 1 );
//        it->second->SetName(h_name.c_str());
//        it->second->SetTitle(h_name.c_str());
//        it->second->Write();
//      }
//
//      Kind0_N15_N05_dir->cd();
//      mc_Kind0_N15_N05->summary_plot_1D->Write();
//      for (std::map<TString, TH1*>::iterator it = mc_Kind0_N15_N05->mass_plots.begin(); it != mc_Kind0_N15_N05->mass_plots.end(); ++it) {
//        std::string h_name = it->second->GetName();
//        h_name.erase( h_name.find(optCatStr+"_"), optCatStr.Length() + 1 );
//        it->second->SetName(h_name.c_str());
//        it->second->SetTitle(h_name.c_str());
//        it->second->Write();
//      }
//
//      Kind0_P05_P15_dir->cd();
//      mc_Kind0_P05_P15->summary_plot_1D->Write();
//      for (std::map<TString, TH1*>::iterator it = mc_Kind0_P05_P15->mass_plots.begin(); it != mc_Kind0_P05_P15->mass_plots.end(); ++it) {
//        std::string h_name = it->second->GetName();
//        h_name.erase( h_name.find(optCatStr+"_"), optCatStr.Length() + 1 );
//        it->second->SetName(h_name.c_str());
//        it->second->SetTitle(h_name.c_str());
//        it->second->Write();
//      }
//
//      Kind0_P15_P50_dir->cd();
//      mc_Kind0_P15_P50->summary_plot_1D->Write();
//      for (std::map<TString, TH1*>::iterator it = mc_Kind0_P15_P50->mass_plots.begin(); it != mc_Kind0_P15_P50->mass_plots.end(); ++it) {
//        std::string h_name = it->second->GetName();
//        h_name.erase( h_name.find(optCatStr+"_"), optCatStr.Length() + 1 );
//        it->second->SetName(h_name.c_str());
//        it->second->SetTitle(h_name.c_str());
//        it->second->Write();
//      }


      out_file->Write();
      out_file->Close();
      std::cout << "Wrote output file " << out_file_name.Data() << std::endl;
      
    } // End loop: for (int iCat = 0; iCat < CAT_CUTS.size(); iCat++)
  } // End loop: for (int iOpt = 0; iOpt < OPT_CUTS.size(); iOpt++)


  std::cout << "\nExiting MassCalibration()\n";
  
} // End void MassCalibration()
