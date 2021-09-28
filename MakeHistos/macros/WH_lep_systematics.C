
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
#include "H2MuAnalyzer/MakeHistos/interface/ObjectSelection.h"    // Common object selections
#include "H2MuAnalyzer/MakeHistos/interface/EventSelection.h"     // Common event selections
#include "H2MuAnalyzer/MakeHistos/interface/EventWeight.h"        // Common event weights
#include "H2MuAnalyzer/MakeHistos/interface/CategoryCuts.h"       // Common category definitions
#include "H2MuAnalyzer/MakeHistos/interface/MiniNTupleHelper.h"   // "PlantTree" and "BookBranch" functions
#include "H2MuAnalyzer/MakeHistos/interface/SampleID.h"
#include "H2MuAnalyzer/MakeHistos/interface/ReadMVA.h"            // Read and evaluate XMLs for MVA
#include "H2MuAnalyzer/MakeHistos/interface/BtagSFHelper.h"       // Common Btag utilities
#include "H2MuAnalyzer/MakeHistos/interface/RoccoR.h"

#include "CondFormats/BTauObjects/interface/BTagCalibration.h"
#include "CondTools/BTau/interface/BTagCalibrationReader.h"

#include "H2MuAnalyzer/MakeHistos/src/ZZKfactor.C"

//const std::string YEAR = "2017";
// // Load the library of the local, compiled H2MuAnalyzer/MakeHistos directory
// R__LOAD_LIBRARY(../../../tmp/slc6_amd64_gcc630/src/H2MuAnalyzer/MakeHistos/src/H2MuAnalyzerMakeHistos/libH2MuAnalyzerMakeHistos.so)

//const std::string YEAR = "2016";
const std::string YEAR = "2018";
// Load the library of the local, compiled H2MuAnalyzer/MakeHistos directory
R__LOAD_LIBRARY(../../../tmp/slc6_amd64_gcc630/src/H2MuAnalyzer/MakeHistos/src/H2MuAnalyzerMakeHistos/libH2MuAnalyzerMakeHistos.so)


// Hard-coded options for running locally / manually
// Options passed in as arguments to ReadNTupleChain when running in batch mode
const int MIN_FILE = 1;    // Minimum index of input files to process
const int MAX_FILE = 20;    // Maximum index of input files to process
const int MAX_EVT  = 1000; // Maximum number of events to process
const int PRT_EVT  = 100;  // Print every N events
const float SAMP_WGT = 1.0;
// const float LUMI = 36814; // pb-1
const bool verbose = false; // Print extra information

//const TString IN_DIR = "/eos/cms/store/group/phys_higgs/HiggsExo/H2Mu/UF/ntuples/2018/102X/prod-v18.2.0.skim3l/SingleMuon/SingleMu_2018A/191209_170001";
//const TString SAMPLE = "SingleMu_2018A";

//const TString IN_DIR = "/eos/cms/store/group/phys_higgs/HiggsExo/H2Mu/UF/ntuples/2016/94X_v4/prod-v16.2.1.skim3l/SingleMuon/SingleMu_2016B/200115_163937";
//const TString SAMPLE = "SingleMu_2016B";

//const TString IN_DIR = "/eos/cms/store/group/phys_higgs/HiggsExo/H2Mu/UF/ntuples/2016/94X_v4/prod-v16.2.1.skim3l_MG_wgt_fixed/THW_HToMuMu_TuneCP5_13TeV-madgraph-pythia/H2Mu_THW_125/200529_092801";
//const TString SAMPLE = "H2Mu_THW_125";

const TString IN_DIR = "/eos/cms/store/group/phys_higgs/HiggsExo/H2Mu/UF/ntuples/2018/102X/prod-v18.2.0.skim3l/WplusH_HToMuMu_WToAll_M125_TuneCP5_PSweights_13TeV_powheg_pythia8/H2Mu_WH_pos_125/191209_170335";
const TString SAMPLE = "H2Mu_WH_pos_125";

//const TString IN_DIR = "/eos/cms/store/group/phys_higgs/HiggsExo/H2Mu/UF/ntuples/2018/102X/prod-v18.2.0.skim3l/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8/WZ_3l/191209_172259";
//const TString SAMPLE = "WZ_3l";

//const TString IN_DIR = "/eos/cms/store/group/phys_higgs/HiggsExo/H2Mu/UF/ntuples/2016/94X_v4/prod-v16.2.1.skim3l/ttHToMuMu_M125_TuneCP5_PSweights_13TeV-powheg-pythia8/H2Mu_ttH_125/191209_161136";
//const TString SAMPLE = "H2Mu_ttH_125";

//const std::string SLIM  = (YEAR == "2017" ? "Slim" : "notSlim");  // "Slim" or "notSlim" - Some 2017 NTuples are "Slim"
const std::string SLIM = "notSlim";
const TString OUT_DIR   = "plots";
const TString HIST_TREE = "HistTree"; // "Hist", "Tree", or "HistTree" to output histograms, trees, or both

std::string GEN_INT = "GENint";

// Systematic shifts to apply
//const std::string SYS_SHIFT = "noSys";
//const std::string SYS_SHIFT = "Roch_down";
//const std::string SYS_SHIFT = "prefiring_2016_up";
//const std::string SYS_SHIFT = "B_SF_up";
const std::string SYS_SHIFT = "LepMVA_SF_mu_up";
// const std::string SYS_SHIFT = "IsoMu_SF_down";
//const std::string SYS_SHIFT = "HF_up";

// Cuts which every event must pass, applied in sequence
const std::vector<std::string> SEL_CUTS = {"PreselRun2"};
// Multiple selection cuts, applied independently in parallel
//const std::vector<std::string> OPT_CUTS = {"3lep"};
const std::vector<std::string> OPT_CUTS = {"3lep", "3mu", "e2mu"};
//const std::vector<std::string> OPT_CUTS = {"3lep_allMass", "3mu_allMass", "e2mu_allMass"};
// Category selection cuts, also applied in parallel
// *** IMPORTANT!!! No category name may start with a sub-string which is identical to another entire category name! ***
// const std::vector<std::string> CAT_CUTS = { "looseLepMVA_noZ5_noBtag",
// 					    "medLepMVA_noZ10_noBtag",
// 					    "hiPt_lepW20_medLepMVA_noZ10_noBtag" };
//                                             "hiPt_lepW20_medLepMVA_onZ10_noBtag" };

const std::vector<std::string> CAT_CUTS = { "lep20_MissHits1_medLepMVA_SR_Bveto"};
                                            //"lep20_MissHits1_medLepMVA_SR_Bpass" }; // for ttH test of TH MG_wgt
					    //"lep20_looseLepMVA_SR_Bveto" }; //for training
                                            //"lep20_medLepMVA_CR_Bveto" };
//const std::vector<std::string> CAT_CUTS = { "lep20_medLepMVA_WZCR_Bveto",
//                                            "lep20_medLepMVA_DYCR_Bveto",
//                                            "lep20_WZCR_SMPWZ",    // mimic selection in SMP-18-002 (WZ cross differential section measurement) 
//                                            "lep20_DYCR_SMPWZ" };  // mimic selection in SMP-18-002 (WZ cross differential section measurement)
//const std::vector<std::string> CAT_CUTS = { "lep20_medLepMVA_WZCR_Bveto",
//                                            "lep20_medLepMVA_SR_Bveto",
//                                            "lep20_MissHits0_medLepMVA_WZCR_Bveto",
//                                            "lep20_MissHits0_medLepMVA_SR_Bveto",
//                                            "lep20_MissHits1_medLepMVA_WZCR_Bveto", 
//                                            "lep20_MissHits1_medLepMVA_SR_Bveto"};  // some extra check on 2017


// Command-line options for running in batch.  Running "root -b -l -q macros/ReadNTupleChain.C" will use hard-coded options above.
void WH_lep_systematics( TString sample = "", TString in_dir = "", TString out_dir = "",
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
  if (SYS.length()	 == 0) SYS	   = SYS_SHIFT;

  // check whether GEN_wgt is saved as int or float in ntuple
  // GEN_wgt is not updated in 2016 ntuples.
  if (YEAR != "2016" and (sample.Contains("H2Mu_THQ") or sample.Contains("H2Mu_THW")) ) GEN_INT = "isGENfloat";

  std::cout << GEN_INT << std::endl;

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
    if (IN_DIR.Contains("0000")) {
      for (int i = MIN_FILE; i <= MAX_FILE; i++) {
	in_file_name.Form("%s/tuple_%d.root", in_dir.Data(), i);
	std::cout << "Adding file " << in_file_name.Data() << std::endl;
	in_file_names.push_back(in_file_name.Data());
      }
    } else {
      in_file_name.Form("%s/NTuple_0.root", in_dir.Data());
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

  // ///////////////////////////////////////////////////////
  // ///  Input samples: MC signal, MC background, data  ///
  // ///////////////////////////////////////////////////////
  
  // std::map<TString, Sample*> samples;
  // std::cout << "\nAbout to get the samples" << std::endl;

  // // Load samples into our map. "UF" if you are on the UF servers
  // // or "CERN" if you at CERN. "ALL" specifies that we want to load the Data
  // // and all of the MC samples. Can loop through and remove the ones you don't want
  // // to use if you desire or just grab the ones you care about from the map.
  // GetSamples2016(samples, "CERN_hiM", sample );
  // Sample * samp = samples[sample];

  // // Open all input files
  // for (int i = 0; i < samp->filenames.size(); i++) {
  //   if ( !gSystem->AccessPathName(samp->filenames.at(i)) )
  //     file_tmp = TFile::Open( samp->filenames.at(i) ); // Check if file exists
  //   if (!file_tmp) {
  //     std::cout << "ERROR: could not open data file " << samp->filenames.at(i) << std::endl;
  //     return;
  //   }
  // }


  // Initialize set of pointers to all branches in input tree
  NTupleBranches br;

  // Add trees from the input files to the TChain
  TChain * in_chain = new TChain("dimuons/tree");
  for (int i = 0; i < in_file_names.size(); i++) {
    in_chain->Add( in_file_names.at(i) );
  // for (int i = 0; i < samp->filenames.size(); i++) {
  //   in_chain->Add( samp->filenames.at(i) );
    // Set branch addresses, from interface/LoadNTupleBranches.h 
    if (sample.Contains("SingleMu"))
      SetBranchAddresses(*in_chain, br, {YEAR, SLIM}); // Options in {} include "JES", "Flags", and "Effs"
    else
      SetBranchAddresses(*in_chain, br, {YEAR, SLIM, GEN_INT, "GEN", "Wgts", "JES"}, (SYS.find("JES_") != std::string::npos ? SYS : "noSys"), false); // Options in {} include "JES", "Flags", and "Effs"
  }
  // float lumi_SF = samp->getLumiScaleFactor(LUMI);
  // std::cout << "For LUMI = " << LUMI << ", lumi_SF = " << lumi_SF << std::endl;


  // Concatenate basic selection cuts into string for output file name
  std::string selStr = "";
  for (int i = 0; i < SEL_CUTS.size(); i++) selStr = selStr+SEL_CUTS.at(i)+"_";
  selStr.pop_back();

  // Create output file for ntuple
  TString out_tuple_name;
  if (out_file_str.Length() > 0)   out_tuple_name.Form( "%s/tuple_%s_%s_%s_%s.root",    out_dir.Data(), sample.Data(), selStr.c_str(), SYS.c_str(), out_file_str.Data() );
  else                             out_tuple_name.Form( "%s/tuple_%s_%s_%s_%d_%d.root", out_dir.Data(), sample.Data(), selStr.c_str(), SYS.c_str(), MIN_FILE, MAX_FILE );
  TFile * out_tuple = TFile::Open( out_tuple_name, "RECREATE" );
  TTree * out_tree  = PlantTree("tree", "tree");

  // Initialize empty map of histogram names to output histograms
  std::map<TString, TH1*> h_map_1D;
  std::map<TString, TH2*> h_map_2D;

  // Initialize empty map of branch names to ouput branches
  std::map<TString, float>       b_map_flt;
  std::map<TString, int>         b_map_int;
  std::map<TString, std::string> b_map_str;
  // Track the number of elements in each map (should be the same in each event)
  int n_map_flt = -99;
  int n_map_int = -99;
  int n_map_str = -99;

  gROOT->cd(); // Navigate to "local" memory, so all histograms are not saved in out_tuple


  // Configuration for object selection, event selection, and object weighting
  ObjectSelectionConfig obj_sel;
  EventSelectionConfig  evt_sel;
  EventWeightConfig     evt_wgt;
  ConfigureObjectSelection(obj_sel, YEAR, "lepMVA");
  ConfigureEventSelection (evt_sel, YEAR);
  ConfigureEventWeight    (evt_wgt, YEAR, SYS);

//  obj_sel.mu_pt_min  = 10;
  evt_wgt.muon_ID  = false;
  evt_wgt.muon_Iso = false;
  evt_sel.muPair_mass_min = 12;  // Allow masses down to 12 GeV (instead of 60 GeV) for background studies

  obj_sel.Print();
  evt_sel.Print();
  evt_wgt.Print();

  ObjectSelectionConfig obj_sel_orig = obj_sel;  // Store original object selection (may change for different CAT_CUTS)

  std::string PTC = obj_sel.mu_pt_corr; // Store muon pT correction in a shorter string; not changed later

  // set up Rochester correction
  std::cout << "\n****** setting up Rochester correction file *****" << std::endl;
  std::string Roch_name = "";
  if (YEAR == "2016")      Roch_name = "data/roccor_Run2_v3/RoccoR2016.txt";
  else if (YEAR == "2017") Roch_name = "data/roccor_Run2_v3/RoccoR2017.txt";
  else if (YEAR == "2018") Roch_name = "data/roccor_Run2_v3/RoccoR2018.txt";
  RoccoR Roch_Cor(Roch_name);

  // set up Btag SF
  std::cout << "\n****** setting up Btag efficiency and SF tools ******" << std::endl;
  BtagEffEntry BTE;
  GetBtagEff(BTE, YEAR, (std::string) sample);

  std::string CSV_name = "";
  if (YEAR == "2016")      CSV_name = "data/deepCSV/DeepCSV_2016LegacySF_V1.csv";
  else if (YEAR == "2017") CSV_name = "data/deepCSV/DeepCSV_94XSF_V4_B_F.csv";
  else if (YEAR == "2018") CSV_name = "data/deepCSV/DeepCSV_102XSF_V1.csv";
  BTagCalibration Bcalib("DeepCSV", CSV_name);

  BTagCalibrationReader readerL(BTagEntry::OP_LOOSE,  // operating point
                             "central",             // central sys type
                             {"up", "down"});      // other sys types
  readerL.load(Bcalib,                // calibration instance
            BTagEntry::FLAV_B,    // btag flavour
            "comb");               // measurement type
  readerL.load(Bcalib, BTagEntry::FLAV_C, "comb");
  readerL.load(Bcalib, BTagEntry::FLAV_UDSG, "incl");

  BTagCalibrationReader readerM(BTagEntry::OP_MEDIUM, "central", {"up", "down"});
  readerM.load(Bcalib, BTagEntry::FLAV_B, "comb");
  readerM.load(Bcalib, BTagEntry::FLAV_C, "comb");
  readerM.load(Bcalib, BTagEntry::FLAV_UDSG, "incl");

  // set up lepMVA SF
  std::cout << "\n******* About to load 2D LepMVA efficiency scale factor histograms *******" << std::endl;
  std::map<std::string, TH2F *> lepSF;
  lepSF["mu_T"]  = LoadSFsLepMVA(YEAR,  "mu", "T");
  lepSF["mu_M"]  = LoadSFsLepMVA(YEAR,  "mu", "M");
  lepSF["mu_L"]  = LoadSFsLepMVA(YEAR,  "mu", "L");
  lepSF["ele_T"] = LoadSFsLepMVA(YEAR, "ele", "T");
  lepSF["ele_M"] = LoadSFsLepMVA(YEAR, "ele", "M");
  lepSF["ele_L"] = LoadSFsLepMVA(YEAR, "ele", "L");

  // set up ggZZ k-factor file
  std::cout << "\n******* About to load ggZZ k-factor file *******" << std::endl;
  TFile * ggZZ_file = TFile::Open( "data/Kfactor/Kfactor_Collected_ggHZZ_2l2l_NNLO_NNPDF_NarrowWidth_13TeV.root", "READ" );
  TSpline3 * ggZZ_kf = (TSpline3*)ggZZ_file->Get("sp_kfactor_Nominal");
  std::cout << "got spline " << ggZZ_kf->GetName() << std::endl;

  // set up BDT xml files
  std::cout << "\n******* About to load XML files for signal-background BDTs *******" << std::endl;
  MVA BDT_AWB_3_noMass( "data/XMLs/WH_3l/Andrew/2019_05_20/f_Opt_AWB_noMass_v3_resWgt_all_sig_all_bkg_resWgt/",
			"weights/f_Opt_AWB_noMass_v3_resWgt_all_sig_all_bkg_resWgt_BDTG_UF_v1.weights.xml",
			"BDTG_UF_v1" );
  MVA BDT_AWB_3_retrain( "data/XMLs/WH_3l/Andrew/2019_05_20/f_Opt_noMassBDT_mass_v3_all_sig_all_bkg_ge0j/",
  			 "weights/f_Opt_noMassBDT_mass_v3_all_sig_all_bkg_ge0j_BDTG_UF_v1.weights.xml",
  			 "BDTG_UF_v1" );
  MVA BDT_final( "data/XMLs/WH_3l/Xunwu/2020_03_25/f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairAll_var1_var2_muSSOSdEta_lepCharge_all_sig_all_bkg_resWgt/",
                 "weights/f_Opt_AWB_noMass_v3_resWgt_trimMET_OSpairAll_var1_var2_muSSOSdEta_lepCharge_all_sig_all_bkg_resWgt_BDTG_UF_v1a.weights.xml",
                 "BDTG_UF_v1a" );

  std::cout << "\n******* About to enter the loop over " << in_chain->GetEntries() << " events *******" << std::endl;
  for (int iEvt = 0; iEvt < in_chain->GetEntries(); iEvt++) {
    
    if (iEvt > max_evt && max_evt > 0) break;
    if ( (iEvt % prt_evt) == 0 ) {
      std::cout << "\n*********************" << std::endl;
      std::cout << "Looking at event " << iEvt <<  std::endl;
      std::cout << "*********************" << std::endl;
    }
    
    if (verbose) std::cout << "Before running GetEntry, event = " << br.event->event;
    
    in_chain->GetEntry(iEvt);
    // check if GEN_wgt is float or int, if the float value is 0 (not properly read), reload the int one
    if (GEN_INT == "GENint") br.GEN_wgt = (float) br.GEN_wgt_int;   

    if (verbose) std::cout << "... after, event = " << br.event->event << std::endl;

    // process muon collection and load new ones with Roch pt with systematic shifts
    MuonInfos muons_tmp;
    if ( not sample.Contains("SingleMu") and SYS.find("Roch_") != std::string::npos ) {
      muons_tmp = ReloadMuonRoch(Roch_Cor, *br.muons_orig, *br.genMuons, SYS);
      br.muons = &muons_tmp;
    }
    else if ( sample.Contains("SingleMu") ) {  // for data, do not care about Roch sys. But important to check if Roch is applied correctly
      muons_tmp = ReloadMuonRoch_data(Roch_Cor, *br.muons_orig, SYS);
      br.muons = br.muons_orig;
    }
    else br.muons = br.muons_orig;

    // For original 2016 and some 2017 and 2018 NTuples, convert "SlimJets" collection into regular jets
    JetInfos jets_tmp;
    JetInfos jets_JES_up_tmp;
    JetInfos jets_JES_down_tmp;
    if (SLIM == "Slim") {
      jets_tmp = ConvertSlimJets(*(br.slimJets));
      br.jets  = &jets_tmp;
      if (not sample.Contains("SingleMu")) {
        jets_JES_up_tmp   = ConvertSlimJets(*(br.slimJets_JES_up));
        jets_JES_down_tmp = ConvertSlimJets(*(br.slimJets_JES_down));
        br.jets_JES_up    = &jets_JES_up_tmp;
        br.jets_JES_down  = &jets_JES_down_tmp; 
      }
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
    if ( YEAR == "2016" and (sample.Contains("H2Mu_THQ") or sample.Contains("H2Mu_THW")) ) event_wgt *= br.MG_wgt;
    // do not need if MG_wgt is included in GEN_wgt
    // ntuples are updated in 2017 and 2018, not in 2016

    // Fill a map of LepMVA weights corresponding to each OPT+CAT category
    std::map<TString, float> lepMVA_wgts_mu;
    std::map<TString, float> lepMVA_wgts_ele;
    for (int iOpt = 0; iOpt < OPT_CUTS.size(); iOpt++) {
      for (int iCat = 0; iCat < CAT_CUTS.size(); iCat++) {
	lepMVA_wgts_mu [OPT_CUTS.at(iOpt)+"_"+CAT_CUTS.at(iCat)] = 1.0;
        lepMVA_wgts_ele[OPT_CUTS.at(iOpt)+"_"+CAT_CUTS.at(iCat)] = 1.0;
      }
    }

    // Initialize the selected Higgs candidate dimuon pair
    MuPairInfo     H_pair;     // When filling histograms, have only one candidate pair at a time
    TLorentzVector H_pair_vec(0,0,0,0);

    bool MU = false;  // Says whether event has 3 muons or 1 electron + 2 muons

    ///////////////////////////////////////////////////////////////
    ///  Loop through possible Higgs candidate pairs (up to 2)  ///
    ///////////////////////////////////////////////////////////////

    for (int iPair = 0; iPair < 2; iPair++) {

      // Each candidate pair defines a unique "event" - i.e. one collision "event" can sometimes enter the tree / histograms twice
      // For each "event", we set all the branch values to their default value (-99)
      for (std::map<TString, float>::iterator       it = b_map_flt.begin(); it != b_map_flt.end(); ++it) b_map_flt[it->first] = -99.0;
      for (std::map<TString, int>::iterator         it = b_map_int.begin(); it != b_map_int.end(); ++it) b_map_int[it->first] = -99;
      for (std::map<TString, std::string>::iterator it = b_map_str.begin(); it != b_map_str.end(); ++it) b_map_str[it->first] = "-99";


      /////////////////////////////////////////////////////////////////////////////////////////
      ///  Loop through alternate, optional selection cuts defined in src/SelectionCuts.cc  ///
      /////////////////////////////////////////////////////////////////////////////////////////

      bool pass_opt_cuts = false; // Check if event passes at least one optional cut
      bool pass_cat_cuts = false; // Check if event passes at least one category cut

      for (int iOpt = 0; iOpt < OPT_CUTS.size(); iOpt++) {
	std::string OPT_CUT = OPT_CUTS.at(iOpt);
	ASSERT( OPT_CUT == "3lep" || OPT_CUT == "3mu" || OPT_CUT == "e2mu" || OPT_CUT == "3lep_allMass" || OPT_CUT == "3mu_allMass" || OPT_CUT == "e2mu_allMass" || OPT_CUT == "3lep_hiPt" || OPT_CUT == "3mu_hiPt",
	       "OPT_CUT == '3lep' || OPT_CUT == '3mu' || OPT_CUT == 'e2mu' || OPT_CUT == '3lep_allMass' || OPT_CUT == '3mu_allMass' || OPT_CUT == 'e2mu_allMass' || OPT_CUT == '3lep_hiPt' || OPT_CUT == '3mu_hiPt'" );

	// Reset object selection to original state
	obj_sel = obj_sel_orig;

	// Assume event fails the optional cut
	BookAndFill(b_map_int, out_tree, sample, "OPT_"+OPT_CUT, 0 );
	for (int iCat = 0; iCat < CAT_CUTS.size(); iCat++) {
	  // Assume event fails each optional x category cut as well
	  BookAndFill(b_map_int, out_tree, sample+"_"+OPT_CUT, "OPT_"+OPT_CUT+"_CAT_"+CAT_CUTS.at(iCat), 0 );
	  // Fill default weights for LepMVA scale factors
	  BookAndFill(b_map_flt, out_tree, sample+"_"+OPT_CUT, "lepMVA_wgt_mu_OPT_" +OPT_CUT+"_CAT_"+CAT_CUTS.at(iCat), -99.0 );
          BookAndFill(b_map_flt, out_tree, sample+"_"+OPT_CUT, "lepMVA_wgt_ele_OPT_"+OPT_CUT+"_CAT_"+CAT_CUTS.at(iCat), -99.0 );
	}

	// All categories require >=2 opposite-charge muons, and == 3 leptons with charge summing to +/-1
	// All opposite-charge lepton pairs must have mass > 12 GeV
	if (true) {
	  if ( SelectedEles(obj_sel, br).size() == 0 ) {
	    if ( SelectedMuPairs(obj_sel, br).size() != 2 ) continue;
	    if ( FourVec( SelectedMuPairs(obj_sel, br).at(0), PTC).M() < 12 ) continue;
	    if ( FourVec( SelectedMuPairs(obj_sel, br).at(1), PTC).M() < 12 ) continue;
	    MU = true;
	  } else if ( SelectedEles(obj_sel, br).size() == 1 ) {
	    if ( SelectedMuPairs(obj_sel, br).size() != 1 ) continue;
	    if ( iPair != 0 ) continue;  // Only one Higgs pair possible, hence iPair must be 0
	    int iMuOS = (SelectedMuons(obj_sel, br).at(0).charge != SelectedEles(obj_sel, br).at(0).charge ? 0 : 1);
	    if ( ( FourVec(SelectedMuons(obj_sel, br).at(iMuOS), PTC) + FourVec(SelectedEles(obj_sel, br).at(0)) ).M() < 12 ) continue;
	    MU = false;
	  } else continue;

	}

	// "allMass" option no longer in use. control region criteria moved to CAT_CUTS
	// assign H_pair under inspection
	H_pair = SelectedMuPairs(obj_sel, br).at(iPair);
	H_pair_vec = FourVecFSRGeoFit(H_pair, PTC, std::stoi(YEAR), "FSRGeoFit", *br.muons, SelectedPhots(obj_sel, br));

	// optional cuts for 3 muon or e2mu category
	if ( StartsWith(OPT_CUT, "3mu") ) {
	  if (!MU) continue;
	}
	else if ( StartsWith(OPT_CUT, "e2mu") ) {
	  if (MU) continue;
	} else ASSERT( StartsWith(OPT_CUT, "3lep"), "StartsWith(OPT_CUT, '3lep')" );
	pass_opt_cuts = true;

	// Now we know the event passes the optional cut
	b_map_int["OPT_"+OPT_CUT] = 1;

	
	//////////////////////////////////////////////////////////////////
	/// Loop through category cuts defined in src/CategoryCuts.cc  ///
	//////////////////////////////////////////////////////////////////
	
	for (int iCat = 0; iCat < CAT_CUTS.size(); iCat++) {

	  //////////////////////////////////////////////////////
	  ///  Compute variables relevent for category cuts  ///
	  //////////////////////////////////////////////////////

	  // Reset object selection to original state
	  obj_sel = obj_sel_orig;

	  // Get selected muons and electrons in event
	  MuonInfos muons = SelectedMuons(obj_sel, br);
	  EleInfos  eles  = SelectedEles(obj_sel, br);
	  JetInfos  jets  = SelectedJets(obj_sel, br, SYS);
          PhotInfos phots = SelectedPhots(obj_sel, br);
          // also save jets JES up and down. This uses the jet_JES_up/down in ntuples, regardless of the SYS value
          JetInfos  jets_JES_up;
          JetInfos  jets_JES_down;
          if (not sample.Contains("SingleMu")) {
            jets_JES_up   = SelectedJets(obj_sel, br, "JES_up");
            jets_JES_down = SelectedJets(obj_sel, br, "JES_down");
          }
          

	  int sum_lep_charge = 0;
	  if (MU) {
	    ASSERT(muons.size() == 3 && eles.size() == 0, "muons.size() == 3 && eles.size() == 0");
	    sum_lep_charge = muons.at(0).charge + muons.at(1).charge + muons.at(2).charge;
	  } else {
	    ASSERT(muons.size() == 2 && eles.size() == 1, "muons.size() == 2 && eles.size() == 1");
	    sum_lep_charge = muons.at(0).charge + muons.at(1).charge + eles.at(0).charge;
	  }
	  ASSERT(abs(sum_lep_charge) == 1, "abs(sum_lep_charge) == 1");
	  
	  MuPairInfo H_true;  // Dimuon pair matched to GEN-level Higgs decay
	  MuPairInfo Z_true;  // Dimuon pair matched to GEN-level Z boson decay
	  MuonInfo   muH1;    // Higher pT muon from chosen Higgs candidate pair
	  MuonInfo   muH2;    // Lower pT muon from chosen Higgs candidate pair
	  MuonInfo   muW;     // Muon not from chosen Higgs candidate pair
	  EleInfo    ele;     // Electron
	  MuonInfo   muSS1;   // Same-sign muon with higher pT
	  MuonInfo   muSS2;   // Same-sign muon with lower pT
	  MuonInfo   muOS;    // Opposite-sign muon

	  TLorentzVector H_GEN_vec(0,0,0,0);
	  TLorentzVector Z_GEN_vec(0,0,0,0);
	  TLorentzVector H_true_vec(0,0,0,0);
	  TLorentzVector Z_true_vec(0,0,0,0);
	  TLorentzVector muH1_vec(0,0,0,0);
	  TLorentzVector muH2_vec(0,0,0,0);
	  TLorentzVector lep_vec(0,0,0,0);      // Lepton not chosen to be in Higgs candidate pair
	  TLorentzVector lep_vecT(0,0,0,0);     // Transverse component only (eta = 0, pZ = 0, mass = 0)
	  TLorentzVector OS_pair_vec(0,0,0,0);  // OS lepton pair not chosen as Higgs candidate (mu-mu or ele-mu)
	  TLorentzVector lepSS1_vec(0,0,0,0);   // SS lepton with higher pT
	  TLorentzVector lepSS2_vec(0,0,0,0);   // SS lepton with lower pT
	  TLorentzVector muSS_vec(0,0,0,0);     // SS muon from the chosen Higgs candidate pair
	  TLorentzVector muSS_vecT(0,0,0,0);    // Transverse component only (eta = 0, pZ = 0, mass = 0)
	  TLorentzVector muOS_vec(0,0,0,0);     // OS muon
	  TLorentzVector muOS_vecT(0,0,0,0);    // Transverse component only (eta = 0, pZ = 0, mass = 0)

	  muH1     = br.muons->at(H_pair.iMu1);
	  muH2     = br.muons->at(H_pair.iMu2);
	  muH1_vec = FourVecFSRGeoFit(muH1, H_pair.iMu1, PTC, std::stoi(YEAR), "FSRGeoFit", phots);
	  muH2_vec = FourVecFSRGeoFit(muH2, H_pair.iMu2, PTC, std::stoi(YEAR), "FSRGeoFit", phots);

	  // Look for a GEN Higgs or Z boson
	  if (not isData) {
	    for (int i = 0; i < br.nGenParents; i++) {
	      if (abs((br.genParents->at(i)).ID) == 25) {
		if (FourVec(br.genParents->at(i)).M() > H_GEN_vec.M()) {
		  H_GEN_vec = FourVec(br.genParents->at(i));
		}
	      }
	      if (abs((br.genParents->at(i)).ID) == 23) {
		if (FourVec(br.genParents->at(i)).M() > Z_GEN_vec.M() &&
		    FourVec(br.genParents->at(i)).M() > 0) {
		  Z_GEN_vec = FourVec(br.genParents->at(i));
		}
	      }
	    }
	  }

	  // Loop over selected dimuon pairs
	  for (const auto & muPair : SelectedMuPairs(obj_sel, br)) {
	    // Check if the pair matches a GEN muon pair from H
	    if (not isData) {
	      if ( IsGenMatched( muPair, *br.muons, *br.genMuons, *br.genParents, "H") ) {
		H_true     = muPair;
		H_true_vec = FourVecFSRGeoFit(muPair, PTC, std::stoi(YEAR), "FSRGeoFit", *br.muons, phots);
	      }
	      if ( IsGenMatched( muPair, *br.muons, *br.genMuons, *br.genParents, "Z") ||
		   IsGenMatched( muPair, *br.muons, *br.genMuons, *br.genParents, "gamma") ||
		   IsGenMatched( muPair, *br.muons, *br.genMuons, *br.genParents, "light_quark") ) {
		Z_true     = muPair;
		Z_true_vec = FourVecFSRGeoFit(muPair, PTC, std::stoi(YEAR), "FSRGeoFit", *br.muons, phots);
	      }
	    }
	    // Check if the pair does not match the selected Higgs candidate pair
	    if ( muPair.iMu1 != H_pair.iMu1 || muPair.iMu2 != H_pair.iMu2 ) {
	      OS_pair_vec = FourVecFSRGeoFit(muPair, PTC, std::stoi(YEAR), "FSRGeoFit", *br.muons, phots);
	    }
	  }

	  // For 3 muon events
	  if (MU) {
	    // Find the muon not in the H pair, and the same-sign and opposite-sign muons
	    for (int i = 0; i < br.nMuons; i++) {
	      MuonInfo mu = br.muons->at(i);
	      if (not MuonPass(obj_sel, mu, br)) continue;
	      // Find the W muon candidate and the same-sign muon from the Higgs
	      if ( (mu.phi != muH1.phi || mu.eta != muH1.eta) &&
		   (mu.phi != muH2.phi || mu.eta != muH2.eta) ) {
		ASSERT(muW.pt <= 0, "muW.pt <= 0"); // We should not have found a W candidate before
		muW = mu;
		lep_vec  = FourVecFSRGeoFit(mu, i, PTC, std::stoi(YEAR), "FSRGeoFit", phots);
		lep_vecT = FourVecFSRGeoFit(mu, i, PTC, std::stoi(YEAR), "FSRGeoFit", phots, "T");
	      } else if ( mu.charge == sum_lep_charge ) {
		muSS_vec  = FourVecFSRGeoFit(mu, i, PTC, std::stoi(YEAR), "FSRGeoFit", phots);
		muSS_vecT = FourVecFSRGeoFit(mu, i, PTC, std::stoi(YEAR), "FSRGeoFit", phots, "T");
	      }
	      // Find the opposte-charge muon
	      if (mu.charge != sum_lep_charge) {
		muOS      = mu;
		muOS_vec  = FourVecFSRGeoFit(mu, i, PTC, std::stoi(YEAR), "FSRGeoFit", phots);
		muOS_vecT = FourVecFSRGeoFit(mu, i, PTC, std::stoi(YEAR), "FSRGeoFit", phots, "T");
	      }

	      // Loop through all other muons
	      for (int j = 0; j < br.nMuons; j++) {
		MuonInfo mu2 = br.muons->at(j);
		if (not MuonPass(obj_sel, mu2, br)) continue;
		// Skip if mu2 is the same as mu
		if (mu.charge == mu2.charge && (mu.pt != mu2.pt || mu.eta != mu2.eta)) {
		  TLorentzVector mu_vec  = FourVecFSRGeoFit(mu, i, PTC, std::stoi(YEAR), "FSRGeoFit", phots);
		  TLorentzVector mu2_vec = FourVecFSRGeoFit(mu, j, PTC, std::stoi(YEAR), "FSRGeoFit", phots);
		  muSS1 = ( mu_vec.Pt() > mu2_vec.Pt() ? mu : mu2 );
		  muSS2 = ( mu_vec.Pt() > mu2_vec.Pt() ? mu2 : mu );
		  lepSS1_vec  = ( mu_vec.Pt() > mu2_vec.Pt() ? mu_vec : mu2_vec );
		  lepSS2_vec  = ( mu_vec.Pt() > mu2_vec.Pt() ? mu2_vec : mu_vec );
		}
	      }
	    }
	    ASSERT( MuonPt(muW, PTC) >= obj_sel.mu_pt_min, "MuonPt(muW, PTC) >= obj_sel.mu_pt_min" ); // We should always find a W candidate
	    ASSERT( muSS1.charge == muSS2.charge && muOS.charge == -1*sum_lep_charge,
		   "muSS1.charge == muSS2.charge && muOS.charge == -1*sum_lep_charge" ); // We should always find two SS and one OS muon
	  }
	  // For ele + 2 muon events
	  else {
	    ele      = eles.at(0);
	    lep_vec  = FourVec(ele);
	    lep_vecT = FourVec(ele, "T");
	    for (int i = 0; i < br.nMuons; i++) {
	      MuonInfo mu = br.muons->at(i);
	      if (not MuonPass(obj_sel, mu, br)) continue;
	      if (mu.charge == ele.charge) { 
	 	muSS1 = mu;  
		muSS_vec  = FourVecFSRGeoFit(mu, i, PTC, std::stoi(YEAR), "FSRGeoFit", phots); 
		muSS_vecT = FourVecFSRGeoFit(mu, i, PTC, std::stoi(YEAR), "FSRGeoFit", phots, "T"); 
	      }
	      else { 
		muOS  = mu;  
		muOS_vec  = FourVecFSRGeoFit(mu, i, PTC, std::stoi(YEAR), "FSRGeoFit", phots); 
		muOS_vecT = FourVecFSRGeoFit(mu, i, PTC, std::stoi(YEAR), "FSRGeoFit", phots, "T"); 
	      }
	    }
	    lepSS1_vec  = ( muSS_vec.Pt() >= lep_vec.Pt() ? muSS_vec : lep_vec );
	    lepSS2_vec  = ( muSS_vec.Pt() <  lep_vec.Pt() ? muSS_vec : lep_vec );
	    OS_pair_vec = lep_vec + muOS_vec;

	    ASSERT( muSS1.charge == ele.charge && muOS.charge == -1*sum_lep_charge,
		   "muSS1.charge == ele.charge && muOS.charge == -1*sum_lep_charge"); // We should always find one SS and one OS muon
	  }
	  
	  ///////////////////////////////////////////
	  ///  Apply the category selection cuts  ///
	  ///////////////////////////////////////////

	  std::string CAT_CUT   = CAT_CUTS.at(iCat);
	  std::string CAT_UNCUT = CAT_CUT; // Track what sub-strings don't match any known cuts
	  bool pass_cat_cut = true;

	  if ( CAT_CUT.find("SR") != std::string::npos ) {       // signal region
	    if ( H_pair_vec.M() < 110 || H_pair_vec.M() > 150 )   { pass_cat_cut = false; continue; } // H cand in mass [110, 150]
	    if ( MU && abs(OS_pair_vec.M() - 91) < 10 )           { pass_cat_cut = false; continue; } // no muPair at Z mass
	    if ( MU && OS_pair_vec.Pt() > H_pair_vec.Pt() &&
		 OS_pair_vec.M() > 110 && OS_pair_vec.M() < 150)  { pass_cat_cut = false; continue; } // select higher pt pair in [110, 150]
	    CAT_UNCUT.erase( CAT_UNCUT.find("SR"), std::string("SR").length() );
 	  }
	  if ( CAT_CUT.find("_CR") != std::string::npos ) {       // control region
	    if ( H_pair_vec.M() < 81 || H_pair_vec.M() > 101 )                  { pass_cat_cut = false; continue; } // "H" cand in mass [81, 101]
	    if ( MU && OS_pair_vec.M() > 110 && OS_pair_vec.M() < 150 )         { pass_cat_cut = false; continue; } // no muPair in [110, 150]
	    if ( MU && abs(OS_pair_vec.M() - 91) < abs(H_pair_vec.M() -91) )    { pass_cat_cut = false; continue; } // select pair closest to 91 GeV
	    CAT_UNCUT.erase( CAT_UNCUT.find("_CR"), std::string("_CR").length() );
	  }
          if ( CAT_CUT.find("WZCR") != std::string::npos ) {       // WZ control region
            if ( H_pair_vec.M() < 76 || H_pair_vec.M() > 106 )                  { pass_cat_cut = false; continue; } // "H" cand in mass [81, 101]
            if ( MU && OS_pair_vec.M() > 110 && OS_pair_vec.M() < 150 )         { pass_cat_cut = false; continue; } // no muPair in [110, 150]
            if ( MU && abs(OS_pair_vec.M() - 91) < abs(H_pair_vec.M() -91) )    { pass_cat_cut = false; continue; } // select pair closest to 91 GeV
            if ( FourVec(*br.met).Pt() < 30 )                                   { pass_cat_cut = false; continue; } // require MET > 30 GeV
            CAT_UNCUT.erase( CAT_UNCUT.find("WZCR"), std::string("WZCR").length() );
          }
          if ( CAT_CUT.find("DYCR") != std::string::npos ) {       // DY control region
            if ( H_pair_vec.M() < 76 || H_pair_vec.M() > 106 )                  { pass_cat_cut = false; continue; } // "H" cand in mass [81, 101]
            if ( MU && OS_pair_vec.M() > 110 && OS_pair_vec.M() < 150 )         { pass_cat_cut = false; continue; } // no muPair in [110, 150]
            if ( MU && abs(OS_pair_vec.M() - 91) < abs(H_pair_vec.M() -91) )    { pass_cat_cut = false; continue; } // select pair closest to 91 GeV
            if ( FourVec(*br.met).Pt() > 30 )                                   { pass_cat_cut = false; continue; } // require MET < 30 GeV
            CAT_UNCUT.erase( CAT_UNCUT.find("DYCR"), std::string("DYCR").length() );
          }
          if ( CAT_CUT.find("SMPWZ") != std::string::npos ) {
            obj_sel.mu_MVA_min  = 0.4;
            obj_sel.ele_MVA_min = 0.8;  // tight lepMVA for electron
            obj_sel.mu_mIso_max  = 0.4; // miniIso for muon
            obj_sel.ele_mIso_max = 0.4; // miniIso for electron
            TLorentzVector trilep_v  = lep_vec + H_pair_vec;
            if ( SelectedMuons(obj_sel, br).size() + SelectedEles(obj_sel, br).size() != 3 ) { pass_cat_cut = false; continue; } 
            if (!MU and !ele.passConversionVeto )                                            { pass_cat_cut = false; continue; } 
            if (!MU and ele.missingInnerHits != 0 )                                          { pass_cat_cut = false; continue; } 
            if ( trilep_v.M() < 100 )                                                        { pass_cat_cut = false; continue; } // M(3l) > 100
            if ( SelectedJets(obj_sel, br, SYS, "BTagMedium").size() > 0 )                   { pass_cat_cut = false; continue; } // veto B-med
            CAT_UNCUT.erase( CAT_UNCUT.find("SMPWZ"), std::string("SMPWZ").length() );
          }
	  if ( CAT_CUT.find("noZ10") != std::string::npos ) {
	    if ( (abs(OS_pair_vec.M() - 91) < 10 && MU) ||
		  abs(H_pair_vec.M()  - 91) < 10        )         { pass_cat_cut = false; continue; }
	    CAT_UNCUT.erase( CAT_UNCUT.find("noZ10"), std::string("noZ10").length() );
	  }
	  if ( CAT_CUT.find("noZ5") != std::string::npos ) {
	    if ( (abs(OS_pair_vec.M() - 91) < 5 && MU) ||
		  abs(H_pair_vec.M()  - 91) < 5        )          { pass_cat_cut = false; continue; }
	    CAT_UNCUT.erase( CAT_UNCUT.find("noZ5"), std::string("noZ5").length() );
	  }
	  if ( CAT_CUT.find("onZ10") != std::string::npos ) {
	    if ( (abs(OS_pair_vec.M() - 91) > 10 || !MU) &&
		  abs(H_pair_vec.M()  - 91) > 10          )       { pass_cat_cut = false; continue; }
	    CAT_UNCUT.erase( CAT_UNCUT.find("onZ10"), std::string("onZ10").length() );
	  }
	  if ( CAT_CUT.find("hiPt") != std::string::npos ) {
	    if ( MU && H_pair_vec.Pt() < OS_pair_vec.Pt() &&
		 OS_pair_vec.M() > 110 && OS_pair_vec.M() < 150 ) { pass_cat_cut = false; continue; }
	    CAT_UNCUT.erase( CAT_UNCUT.find("hiPt"), std::string("hiPt").length() );
	  }
	  if ( CAT_CUT.find("onlyHiPt") != std::string::npos ) {
	    if ( MU && H_pair_vec.Pt() < OS_pair_vec.Pt() )       { pass_cat_cut = false; continue; }
	    CAT_UNCUT.erase( CAT_UNCUT.find("onlyHiPt"), std::string("onlyHiPt").length() );
	  }
	  if ( CAT_CUT.find("ele20") != std::string::npos ) {
	    obj_sel.ele_pt_min = 20;
	    if ( !MU && SelectedEles(obj_sel, br).size() != 1 )   { pass_cat_cut = false; continue; }
	    CAT_UNCUT.erase( CAT_UNCUT.find("ele20"), std::string("ele20").length() );
	  }
	  if ( CAT_CUT.find("lepW20") != std::string::npos ) {
	    obj_sel.ele_pt_min = 20;
	    if ( !MU && SelectedEles(obj_sel, br).size() != 1 )   { pass_cat_cut = false; continue; }
	    if (  MU && lep_vec.Pt() < 20 )                       { pass_cat_cut = false; continue; }
	    CAT_UNCUT.erase( CAT_UNCUT.find("lepW20"), std::string("lepW20").length() );
	  }
	  if ( CAT_CUT.find("lep20") != std::string::npos ) {
	    obj_sel.mu_pt_min  = 20;
	    obj_sel.ele_pt_min = 20;
	    if ( SelectedMuons(obj_sel, br).size() + SelectedEles(obj_sel, br).size() != 3 ) { pass_cat_cut = false; continue; }
	    CAT_UNCUT.erase( CAT_UNCUT.find("lep20"), std::string("lep20").length() );
	  }
	  if ( CAT_CUT.find("looseLepMVA") != std::string::npos ) {
	    obj_sel.mu_MVA_min  = -0.4;
	    obj_sel.ele_MVA_min = -0.4;
	    if ( SelectedMuons(obj_sel, br).size() + SelectedEles(obj_sel, br).size() != 3 ) { pass_cat_cut = false; continue; }
	    CAT_UNCUT.erase( CAT_UNCUT.find("looseLepMVA"), std::string("looseLepMVA").length() );
	  }
	  if ( CAT_CUT.find("medLepMVA") != std::string::npos ) {
	    obj_sel.mu_MVA_min  = 0.4;
	    obj_sel.ele_MVA_min = 0.4;
	    if ( SelectedMuons(obj_sel, br).size() + SelectedEles(obj_sel, br).size() != 3 ) { pass_cat_cut = false; continue; }
	    CAT_UNCUT.erase( CAT_UNCUT.find("medLepMVA"), std::string("medLepMVA").length() );
	  }
	  if ( CAT_CUT.find("muMeleT_lepMVA") != std::string::npos ) {
	    obj_sel.mu_MVA_min  = 0.4;
	    obj_sel.ele_MVA_min = 0.8;
	    if ( SelectedMuons(obj_sel, br).size() + SelectedEles(obj_sel, br).size() != 3 ) { pass_cat_cut = false; continue; }
	    CAT_UNCUT.erase( CAT_UNCUT.find("muMeleT_LepMVA"), std::string("muMeleT_LepMVA").length() );
	  }
	  if ( CAT_CUT.find("tightLepMVA") != std::string::npos ) {
	    obj_sel.mu_MVA_min  = 0.8;
	    obj_sel.ele_MVA_min = 0.8;
	    if ( SelectedMuons(obj_sel, br).size() + SelectedEles(obj_sel, br).size() != 3 ) { pass_cat_cut = false; continue; }
	    CAT_UNCUT.erase( CAT_UNCUT.find("tightLepMVA"), std::string("tightLepMVA").length() );
	  }
	  if ( CAT_CUT.find("tightLepCut") != std::string::npos ) {
	    if (        muH1_vec.Pt() < 20 || !muH1.isTightID || muH1.relIso > 0.12 ) { pass_cat_cut = false; continue; };
	    if (        muH2_vec.Pt() < 20 || !muH2.isTightID || muH2.relIso > 0.12 ) { pass_cat_cut = false; continue; };
	    if (  MU && (lep_vec.Pt() < 20 || !muW.isTightID  || muW.relIso > 0.12) ) { pass_cat_cut = false; continue; };
	    if ( !MU && (lep_vec.Pt() < 20 || !ele.isTightID  || ele.relIso > 0.12) ) { pass_cat_cut = false; continue; };
	    CAT_UNCUT.erase( CAT_UNCUT.find("tightLepCut"), std::string("tightLepCut").length() );
	  }
	  if ( CAT_CUT.find("Bveto") != std::string::npos ) {
	    if ( SelectedJets(obj_sel, br, SYS, "BTagMedium").size() > 0 ) { pass_cat_cut = false; continue; }
	    if ( SelectedJets(obj_sel, br, SYS, "BTagLoose").size()  > 1 ) { pass_cat_cut = false; continue; }
	    CAT_UNCUT.erase( CAT_UNCUT.find("Bveto"), std::string("Bveto").length() );
	  }
          if ( CAT_CUT.find("Bpass") != std::string::npos ) {
            if ( SelectedJets(obj_sel, br, SYS, "BTagMedium").size() == 0  and
                 SelectedJets(obj_sel, br, SYS, "BTagLoose").size()  <= 1 ) { pass_cat_cut = false; continue; }
            CAT_UNCUT.erase( CAT_UNCUT.find("Bpass"), std::string("Bpass").length() );
          }
          // options to fix data/MC disagreement for electron pt [20, 30]
          if ( CAT_CUT.find("MissHits0") != std::string::npos ) {
            if (!MU and ele.missingInnerHits != 0 ) {pass_cat_cut = false; continue; };
            CAT_UNCUT.erase( CAT_UNCUT.find("MissHits0"), std::string("MissHits0").length() );
          }
          if ( CAT_CUT.find("MissHits1") != std::string::npos ) {
            if (!MU and ele.missingInnerHits > 1 ) {pass_cat_cut = false; continue; };
            CAT_UNCUT.erase( CAT_UNCUT.find("MissHits1"), std::string("MissHits1").length() );
          }
          if ( CAT_CUT.find("ConVetoAll") != std::string::npos ) {  // not in use, Conversion Veto now applies to electrons as default
            if (!MU and !ele.passConversionVeto ) {pass_cat_cut = false; continue; };
            CAT_UNCUT.erase( CAT_UNCUT.find("ConVetoAll"), std::string("ConVetoAll").length() );
          }
          if ( CAT_CUT.find("ConVeto30") != std::string::npos ) {   // not in use, Conversion Veto now applies to electrons as default
            if (!MU and ele.pt < 30 and !ele.passConversionVeto ) {pass_cat_cut = false; continue; };
            CAT_UNCUT.erase( CAT_UNCUT.find("ConVeto30"), std::string("ConVeto30").length() );
          }
          if ( CAT_CUT.find("eTID") != std::string::npos ) {
            if (!MU and ele.pt < 30 and !ele.isTightID) {pass_cat_cut = false; continue; };
            CAT_UNCUT.erase( CAT_UNCUT.find("eTID"), std::string("eTID").length() );
          }
          if ( CAT_CUT.find("eTlep") != std::string::npos ) {
            if (!MU and ele.pt < 30 and ele.lepMVA<0.8) {pass_cat_cut = false; continue; };
            CAT_UNCUT.erase( CAT_UNCUT.find("eTlep"), std::string("eTlep").length() );
          }

	  // Remove "_" characters left over after all known category sub-strings have been removed
	  while ( CAT_UNCUT.find("_") != std::string::npos ) {
	    CAT_UNCUT.erase( CAT_UNCUT.find("_"), std::string("_").length() );
	  }
	  // If CAT_CUT contains unknown elements, look for it in the central repository
	  if ( CAT_UNCUT.length() > 0 ) pass_cat_cut = InCategory(obj_sel, br, SYS, CAT_CUT, verbose);

	  ///  *** SPECIAL CUTS ***  ///
	  // // Only keep signal events if the chosen Higgs candidate pair really comes from the Higgs
	  // if (  sample.Contains("H2Mu") && H_true_vec.M() != H_pair_vec.M() ) pass_cat_cut = false;
	  // Throw away background events if there is a real di-muon pair from a Higgs
	  if ( !sample.Contains("H2Mu") && H_true_vec.M() > 0 ) pass_cat_cut = false;

	  if (not pass_cat_cut) continue;
	  if (verbose) std::cout << "\nPassed cut " << OPT_CUT << ", in category " << CAT_CUT << std::endl;
	  std::string h_pre = (std::string) sample + "_"+OPT_CUT+"_"+CAT_CUT+"_";

	  // Now we know the event passes the optional + category cut
	  b_map_int["OPT_"+OPT_CUT+"_CAT_"+CAT_CUT] = 1;
	  pass_cat_cuts = true;


	  // Compute lepMVA muon and electron efficiency scale factors
	  if ( CAT_CUT.find("LepMVA") != std::string::npos or CAT_CUT.find("SMPWZ") != std::string::npos ) {
	    std::string SFm, SFe;
	    if ( CAT_CUT.find("looseLepMVA")    != std::string::npos ) { SFm = "mu_L"; SFe = "ele_L"; }
	    if ( CAT_CUT.find("medLepMVA")      != std::string::npos ) { SFm = "mu_M"; SFe = "ele_M"; }
	    if ( CAT_CUT.find("muMeleT_lepMVA") != std::string::npos ) { SFm = "mu_M"; SFe = "ele_T"; }
            if ( CAT_CUT.find("SMPWZ")          != std::string::npos ) { SFm = "mu_M"; SFe = "ele_T"; }
	    if ( CAT_CUT.find("tightLepMVA")    != std::string::npos ) { SFm = "mu_T"; SFe = "ele_T"; }

	    lepMVA_wgts_mu[OPT_CUT+"_"+CAT_CUT]  = LepMVASF(lepSF[SFm], muH1.pt, abs(muH1.eta), (SYS.find("LepMVA_SF_mu") != std::string::npos ? SYS : "noSys") );
	    lepMVA_wgts_mu[OPT_CUT+"_"+CAT_CUT] *= LepMVASF(lepSF[SFm], muH2.pt, abs(muH2.eta), (SYS.find("LepMVA_SF_mu") != std::string::npos ? SYS : "noSys") );
	    if (MU) lepMVA_wgts_mu [OPT_CUT+"_"+CAT_CUT] *= LepMVASF(lepSF[SFm],  muW.pt, muW.eta, (SYS.find("LepMVA_SF_mu")  != std::string::npos ? SYS : "noSys") );
	    else    lepMVA_wgts_ele[OPT_CUT+"_"+CAT_CUT] *= LepMVASF(lepSF[SFe],  ele.pt, ele.eta, (SYS.find("LepMVA_SF_ele") != std::string::npos ? SYS : "noSys") );
	  }

	  // Store the lepMVA weight separately (varies depending on category selection)
	  float lepMVA_wgt_mu  = (isData ? 1.0 : lepMVA_wgts_mu [OPT_CUT+"_"+CAT_CUT]);
          float lepMVA_wgt_ele = (isData ? 1.0 : lepMVA_wgts_ele[OPT_CUT+"_"+CAT_CUT]);
	  b_map_flt["lepMVA_wgt_mu_OPT_" +OPT_CUT+"_CAT_"+CAT_CUT] = lepMVA_wgt_mu;
          b_map_flt["lepMVA_wgt_ele_OPT_"+OPT_CUT+"_CAT_"+CAT_CUT] = lepMVA_wgt_ele;


          // get k-factor for ZZ and ggZZ processes, using GEN Mass(ZZ)
          float k_factor = 1.0;
          if ( sample.Contains("ggZZ") or sample.Contains("ZZ_4l") ) {
              TLorentzVector Pa1_vec, Pa2_vec; // initial value (0,0,0,0)
              GenParentInfo genPa1;
              int same_flavor = 1;
              for (const auto & genPa : *br.genParents) {
                if (genPa.ID != 23 or genPa.daughter_1_ID + genPa.daughter_2_ID != 0) continue;  // legit Z that decays to lepton pair 
                Pa1_vec = FourVec(genPa);
                genPa1 = genPa;
                break;
              }
              if (Pa1_vec.M()==0) std::cout << "bizzare case: no gen parent 1" << std::endl;
              for (const auto & genPa2 : *br.genParents) {
                if (genPa2.ID != 23 or genPa2.daughter_1_ID + genPa2.daughter_2_ID != 0) continue;  // legit Z that decays to lepton pair 
                // for some reason, in 10% of the cases, two Z parents have exactly the same daughter index (genPa1.daughter_1_idx = genPa2.daughter_1_idx, also for daughter_2)
                // not sure why. For the moment do not use daughter idx to match gen parents  -- XWZ 2020.05.06
                if ( genPa2.pt == genPa1.pt and genPa2.eta == genPa1.eta and genPa2.phi == genPa1.phi and genPa2.mass == genPa1.mass) continue; //make sure genPa2 is not genPa1
                if ( abs(genPa2.daughter_1_ID) != abs(genPa1.daughter_1_ID) ) same_flavor = 2;
                Pa2_vec = FourVec(genPa2);
                break;
              }
              if (Pa2_vec.M()==0) std::cout << "bizzare case: no gen parent 2" << std::endl;
              if ( Pa1_vec.M()==0 or Pa2_vec.M()==0 ) std::cout << "bizzare case: not 2 Z parents found.\n" << std::endl;
              else {
                TLorentzVector ZZ_vec = Pa1_vec + Pa2_vec;
                if (sample.Contains("ggZZ"))  k_factor = ggZZ_kf->Eval( ZZ_vec.M() );
                if (sample.Contains("ZZ_4l")) k_factor = HZZ4LHelper::kfactor_qqZZ_qcd_M( ZZ_vec.M(), same_flavor );
              }
          }
          b_map_flt["k_factor"] = k_factor;


	  // Compute Btag SF
	  float Btag_wgt = 1.0;
	  // only get B_SF for MC
	  if ( (CAT_CUT.find("Bveto") != std::string::npos or CAT_CUT.find("Bpass") != std::string::npos or CAT_CUT.find("SMPWZ") != std::string::npos) and (not isData) ) {
	    std::string jet_FLV = "none";
	    std::string B_sys   = "none";
	    if      ( SYS == "B_SF_up")   B_sys = "up";
	    else if ( SYS == "B_SF_down") B_sys = "down";
	    else    			  B_sys = "central";

	    for (const auto & jet : jets) {
	      float jet_SF = 1.0;
	      if ( abs(jet.eta) > 2.4 ) continue;

	      if ( JetPass(obj_sel, jet, br, "BTagMedium") ) {
		std::cout << "\nBizzare case: having medium tagged jet" << std::endl;
		if      ( abs(jet.partonID) == 5 ) jet_SF = readerM.eval_auto_bounds(B_sys, BTagEntry::FLAV_B, jet.eta, jet.pt);
		else if ( abs(jet.partonID) == 4 ) jet_SF = readerM.eval_auto_bounds(B_sys, BTagEntry::FLAV_C, jet.eta, jet.pt);
		else				   jet_SF = readerM.eval_auto_bounds(B_sys, BTagEntry::FLAV_UDSG, jet.eta, jet.pt);
	      } // end of if ( JetPass(obj_sel, jet, br, "BTagMedium") )
	      else if ( JetPass(obj_sel, jet, br, "BTagLoose") ) {
		float loose_SF  = 1.0;
	        float medium_SF = 1.0;
		if      ( abs(jet.partonID) == 5 ) {
		  loose_SF  = readerL.eval_auto_bounds(B_sys, BTagEntry::FLAV_B, jet.eta, jet.pt); 
		  medium_SF = readerM.eval_auto_bounds(B_sys, BTagEntry::FLAV_B, jet.eta, jet.pt);
		  jet_SF = CalBtagSF( BTE, "FLAV_B", loose_SF, medium_SF);		  
		}
		else if ( abs(jet.partonID) == 4 ) {
		  loose_SF  = readerL.eval_auto_bounds(B_sys, BTagEntry::FLAV_C, jet.eta, jet.pt);         
                  medium_SF = readerM.eval_auto_bounds(B_sys, BTagEntry::FLAV_C, jet.eta, jet.pt);
                  jet_SF = CalBtagSF( BTE, "FLAV_C", loose_SF, medium_SF);
		}
	  	else {
		  loose_SF  = readerL.eval_auto_bounds(B_sys, BTagEntry::FLAV_UDSG, jet.eta, jet.pt);         
                  medium_SF = readerM.eval_auto_bounds(B_sys, BTagEntry::FLAV_UDSG, jet.eta, jet.pt);
                  jet_SF = CalBtagSF( BTE, "FLAV_UDSG", loose_SF, medium_SF);
		}
	      } // end of if ( JetPass(obj_sel, jet, br, "BTagLoose") )
	      else {
		float loose_SF  = 1.0;
		if      ( abs(jet.partonID) == 5 ) {
		  loose_SF  = readerL.eval_auto_bounds(B_sys, BTagEntry::FLAV_B, jet.eta, jet.pt); 
		  jet_SF = CalBtagSF( BTE, "FLAV_B", loose_SF, 0.0);
		}
		else if ( abs(jet.partonID) == 4 ) {
		  loose_SF  = readerL.eval_auto_bounds(B_sys, BTagEntry::FLAV_C, jet.eta, jet.pt);
                  jet_SF = CalBtagSF( BTE, "FLAV_C", loose_SF, 0.0);
		}
		else {
		  loose_SF  = readerL.eval_auto_bounds(B_sys, BTagEntry::FLAV_UDSG, jet.eta, jet.pt);
                  jet_SF = CalBtagSF( BTE, "FLAV_UDSG", loose_SF, 0.0);
		}
	      } //end of else, (not BTagLoose)
	      Btag_wgt *= jet_SF;
	      if (jet_SF < 0.8 or jet_SF > 1.1) std::cout << "jet_SF = " << jet_SF << ", jet_ID = " << jet.partonID << ", jet_pt = " << jet.pt << ", jet_eta = " << jet.eta << std::endl;
	    } // end of for (const auto & jet : jets)

	  } // end of if ( CAT_CUT.find("Bveto" != std::string::npos and (not isData) )
          b_map_flt["Btag_wgt"] = Btag_wgt;

	  //////////////////////////////////////
	  ///  Compute a few more variables  ///
	  //////////////////////////////////////

	  // Full event variables
	  TLorentzVector MET_vec  = FourVec(*br.met);
          TLorentzVector MET_JES_up_vec;
          TLorentzVector MET_JES_down_vec;
          if (not sample.Contains("SingleMu")) {
            MET_JES_up_vec       = FourVec(*br.met_JES_up);
            MET_JES_down_vec     = FourVec(*br.met_JES_down);
          }
	  TLorentzVector lep_MET_vec          = lep_vecT + MET_vec;
          TLorentzVector lep_MET_JES_up_vec   = lep_vecT + MET_JES_up_vec;
          TLorentzVector lep_MET_JES_down_vec = lep_vecT + MET_JES_down_vec;
	  TLorentzVector evt_vec     = FourVec(muons, PTC, eles, jets);

	  TLorentzVector MHT_vec            = FourVec(muons, PTC, eles, jets, "T");
          TLorentzVector MHT_JES_up_vec     = FourVec(muons, PTC, eles, jets_JES_up, "T");          
          TLorentzVector MHT_JES_down_vec   = FourVec(muons, PTC, eles, jets_JES_down, "T");

	  MHT_vec         .RotateZ(TMath::Pi());
          MHT_JES_up_vec  .RotateZ(TMath::Pi());
          MHT_JES_down_vec.RotateZ(TMath::Pi());
	  TLorentzVector lep_MHT_vec          = lep_vecT + MHT_vec;
          TLorentzVector lep_MHT_JES_up_vec   = lep_vecT + MHT_JES_up_vec;
          TLorentzVector lep_MHT_JES_down_vec = lep_vecT + MHT_JES_down_vec;


	  // Leptons ordered by pT
	  TLorentzVector lep1_vec(0,0,0,0);
	  TLorentzVector lep2_vec(0,0,0,0);
	  TLorentzVector lep3_vec(0,0,0,0);

	  if ( lepSS1_vec.Pt() > muOS_vec.Pt() ) {
	    lep1_vec = lepSS1_vec;
	    lep2_vec = (muOS_vec.Pt() > lepSS2_vec.Pt() ? muOS_vec : lepSS2_vec);
	    lep3_vec = (muOS_vec.Pt() > lepSS2_vec.Pt() ? lepSS2_vec : muOS_vec);
	  } else {
	    lep1_vec = muOS_vec;
	    lep2_vec = lepSS1_vec;
	    lep3_vec = lepSS2_vec;
	  }

	  TLorentzVector SS_pair_vec = lepSS1_vec + lepSS2_vec;
	  TLorentzVector trilep_vec  = lep_vec + H_pair_vec;

	  float muSS_MVA = -9;
	  float ele_MVA  = -9;
          float ele_iso  = -9;
          float ele_mIso = -9;
	  float lepSS1_iso;
	  float lepSS2_iso;
	  float lepSS1_MVA;
	  float lepSS2_MVA;
	  float lepSS1_SIP;
	  float lepSS2_SIP;
	  float lepSS1_seg;
	  float lepSS2_seg;
	  bool  lepSS1_tID;
	  bool  lepSS2_tID;
	  if (MU) {
	    lepSS1_iso = muSS1.relIso;
	    lepSS2_iso = muSS2.relIso;
	    lepSS1_MVA = muSS1.lepMVA;
	    lepSS2_MVA = muSS2.lepMVA;
	    lepSS1_SIP = muSS1.SIP_3D;
	    lepSS2_SIP = muSS2.SIP_3D;
	    lepSS1_seg = muSS1.segCompat;
	    lepSS2_seg = muSS2.segCompat;
	    lepSS1_tID = muSS1.isTightID;
	    lepSS2_tID = muSS2.isTightID;
	  } else {
	    muSS_MVA   = muSS1.lepMVA;
	    ele_MVA    = ele.lepMVA;
            ele_iso    = ele.relIso;
            ele_mIso   = ele.miniIso;
	    lepSS1_iso = (muSS_vec.Pt() > ele.pt ? muSS1.relIso : ele.relIso);
	    lepSS2_iso = (muSS_vec.Pt() > ele.pt ? ele.relIso : muSS1.relIso);
	    lepSS1_MVA = (muSS_vec.Pt() > ele.pt ? muSS1.lepMVA : ele.lepMVA);
	    lepSS2_MVA = (muSS_vec.Pt() > ele.pt ? ele.lepMVA : muSS1.lepMVA);
	    lepSS1_SIP = (muSS_vec.Pt() > ele.pt ? muSS1.SIP_3D : ele.SIP_3D);
	    lepSS2_SIP = (muSS_vec.Pt() > ele.pt ? ele.SIP_3D : muSS1.SIP_3D);
	    lepSS1_seg = (muSS_vec.Pt() > ele.pt ? muSS1.segCompat : -0.01);
	    lepSS2_seg = (muSS_vec.Pt() > ele.pt ? -0.01 : muSS1.segCompat);
	    lepSS1_tID = (muSS_vec.Pt() > ele.pt ? muSS1.isTightID : ele.isTightID);
	    lepSS2_tID = (muSS_vec.Pt() > ele.pt ? ele.isTightID : muSS1.isTightID);
	  }


	  ///////////////////////////////////////////////////
	  ///  Generate and fill branches and histograms  ///
	  ///////////////////////////////////////////////////

	  // Tuple containing maps and common options for "BookAndFill"
	  std::tuple< const TString, std::map<TString, float> &, TTree *, std::map<TString, TH1*> &, const TString >
	    tupF{ hist_tree, b_map_flt, out_tree, h_map_1D, h_pre };
	  std::tuple< const TString, std::map<TString, int> &,   TTree *, std::map<TString, TH1*> &, const TString >
	    tupI{ hist_tree, b_map_int, out_tree, h_map_1D, h_pre };

	  // Store sample and event information
	  BookAndFill(b_map_str, out_tree, h_pre, "sample",    sample );
          BookAndFill(b_map_int, out_tree, h_pre, "Sample_ID", getSampleID(sample) );
          BookAndFill(b_map_int, out_tree, h_pre, "run",       br.event->run   );
          BookAndFill(b_map_int, out_tree, h_pre, "lumi",      br.event->lumi  );
	  BookAndFill(b_map_int, out_tree, h_pre, "event",     br.event->event );
          
	  BookAndFill(b_map_flt, out_tree, h_pre, "samp_wgt",  samp_weight );  // Sample cross section x luminosity weight

	  // Store event weights
	  BookAndFill(tupF, "PU_wgt",      40, -2, 2, isData ? 1.0 : br.PU_wgt  );
          if (YEAR == "2016" || YEAR == "2017") BookAndFill(tupF, "prefire_wgt", 40, -2, 2, isData ? 1.0 : br.l1pref_wgt);
	  BookAndFill(tupF, "muon_wgt",    40, -2, 2, isData ? 1.0 : MuonWeight(br, evt_wgt, verbose) );
	  BookAndFill(tupF, "GEN_wgt",     40, -2, 2, isData ? 1.0 : br.GEN_wgt );
	  BookAndFill(tupF, "event_wgt",   40, -2, 2, isData ? 1.0 : event_wgt  );  // Event-by-event PU, NLO, eff SF, etc. weights

	  // Scale event weight by lepMVA weight (varies by category)
	  float cat_evt_wgt = event_wgt * lepMVA_wgt_mu * lepMVA_wgt_ele * Btag_wgt * k_factor;
	  ASSERT(lepMVA_wgt_mu  > 0, "lepMVA_wgt_mu  > 0");
          ASSERT(lepMVA_wgt_ele > 0, "lepMVA_wgt_ele > 0");
	  ASSERT(Btag_wgt > 0, "Btag_wgt > 0");

	  // Plot kinematic histograms
	  BookAndFill(tupI, "nPV",   12, -0.5, 59.5, br.nVertices, cat_evt_wgt );
	  BookAndFill(tupI, "nEles",  2, -0.5,  1.5, eles.size(),  cat_evt_wgt );
          BookAndFill(tupI, "nPhots_selected",  4,  -0.5,  3.5, phots.size(),  cat_evt_wgt );
          BookAndFill(tupI, "nPhots_branch",  4,  -0.5,  3.5, br.nPhots,  cat_evt_wgt );

	  BookAndFill(tupI, "nJets",       8, -0.5, 7.5, jets.size(),                                    cat_evt_wgt );
	  BookAndFill(tupI, "nBJetsLoose", 6, -0.5, 5.5, SelectedJets(obj_sel, br, SYS, "BTagLoose").size(),  cat_evt_wgt );
//	  BookAndFill(tupI, "nBJetsMed",   4, -0.5, 3.5, SelectedJets(obj_sel, br, SYS, "BTagMedium").size(), cat_evt_wgt );
//	  BookAndFill(tupI, "nBJetsTight", 4, -0.5, 3.5, SelectedJets(obj_sel, br, SYS, "BTagTight").size(),  cat_evt_wgt );
//	  BookAndFill(tupI, "nJetsCent",   5, -0.5, 4.5, SelectedJets(obj_sel, br, SYS, "Central").size(),    cat_evt_wgt );
//	  BookAndFill(tupI, "nJetsFwd",    5, -0.5, 4.5, SelectedJets(obj_sel, br, SYS, "Forward").size(),    cat_evt_wgt );

	  BookAndFill(tupF, "jet1_pt",  30,    0, 150, (jets.size() > 0 ? jets.at(0).pt      : -9), cat_evt_wgt, false );
	  BookAndFill(tupF, "jet2_pt",  20,    0, 100, (jets.size() > 1 ? jets.at(1).pt      : -9), cat_evt_wgt, false );
	  BookAndFill(tupF, "jet1_eta", 20, -5.0, 5.0, (jets.size() > 0 ? jets.at(0).eta     : -9), cat_evt_wgt, false );
	  BookAndFill(tupF, "jet2_eta", 20, -5.0, 5.0, (jets.size() > 1 ? jets.at(1).eta     : -9), cat_evt_wgt, false );
	  BookAndFill(tupF, "jet1_CSV", 50, -1.0, 1.0, (jets.size() > 0 ? JetCSV(jets.at(0)) : -9), cat_evt_wgt, false );
	  BookAndFill(tupF, "jet2_CSV", 50, -1.0, 1.0, (jets.size() > 1 ? JetCSV(jets.at(1)) : -9), cat_evt_wgt, false );

	  BookAndFill(tupF, "MET",           20, 0,  200, MET_vec.Pt(),          cat_evt_wgt );
          BookAndFill(tupF, "MET_JES_up",    20, 0,  200, MET_JES_up_vec.Pt(),   cat_evt_wgt );
          BookAndFill(tupF, "MET_JES_down",  20, 0,  200, MET_JES_down_vec.Pt(), cat_evt_wgt );
	  BookAndFill(tupF, "MHT",           20, 0,  200, MHT_vec.Pt(),          cat_evt_wgt );
          BookAndFill(tupF, "MHT_JES_up",    20, 0,  200, MHT_JES_up_vec.Pt(),   cat_evt_wgt );
          BookAndFill(tupF, "MHT_JES_down",  20, 0,  200, MHT_JES_down_vec.Pt(), cat_evt_wgt );
	  BookAndFill(tupF, "MASS", 30, 0, 1500, evt_vec.M(),  cat_evt_wgt );

	  BookAndFill(tupF, "lep1_pt", 30, 0, 300, lep1_vec.Pt(), cat_evt_wgt );
	  BookAndFill(tupF, "lep2_pt", 20, 0, 200, lep2_vec.Pt(), cat_evt_wgt );
	  BookAndFill(tupF, "lep3_pt", 20, 0, 100, lep3_vec.Pt(), cat_evt_wgt );

	  BookAndFill(tupF, "muH1_pt", 30, 0, 300, muH1_vec.Pt(), cat_evt_wgt );
	  BookAndFill(tupF, "muH2_pt", 20, 0, 200, muH2_vec.Pt(), cat_evt_wgt );
	  BookAndFill(tupF, "lep_pt",  20, 0, 200, lep_vec.Pt(),  cat_evt_wgt );

	  BookAndFill(tupF, "muOS_pt",   20, 0, 200, muOS_vec.Pt(),   cat_evt_wgt );
	  BookAndFill(tupF, "muSS_pt",   20, 0, 200, muSS_vec.Pt(),   cat_evt_wgt );  // "Wrong combo" of lep_pt
	  BookAndFill(tupF, "lepSS1_pt", 20, 0, 200, lepSS1_vec.Pt(), cat_evt_wgt );
	  BookAndFill(tupF, "lepSS2_pt", 20, 0, 100, lepSS2_vec.Pt(), cat_evt_wgt );

	  BookAndFill(tupF, "lep1_eta", 24, -2.4, 2.4, lep1_vec.Eta(), cat_evt_wgt );
	  BookAndFill(tupF, "lep2_eta", 24, -2.4, 2.4, lep2_vec.Eta(), cat_evt_wgt );
	  BookAndFill(tupF, "lep3_eta", 24, -2.4, 2.4, lep3_vec.Eta(), cat_evt_wgt );

	  BookAndFill(tupF, "muH1_eta", 12, -2.4, 2.4, muH1_vec.Eta(), cat_evt_wgt );
	  BookAndFill(tupF, "muH2_eta", 12, -2.4, 2.4, muH2_vec.Eta(), cat_evt_wgt );
	  BookAndFill(tupF, "lep_eta",  12, -2.4, 2.4, lep_vec.Eta(),  cat_evt_wgt );

	  // BookAndFill(tupF, "muOS_iso",   15, 0, 0.3, muOS.relIso, cat_evt_wgt );
	  // BookAndFill(tupF, "lepSS1_iso", 15, 0, 0.3, lepSS1_iso,  cat_evt_wgt );
	  // BookAndFill(tupF, "lepSS2_iso", 15, 0, 0.3, lepSS2_iso,  cat_evt_wgt );

	  // BookAndFill(tupI, "muOS_tightID",   2, -0.5, 1.5, muOS.isTightID, cat_evt_wgt );
	  // BookAndFill(tupI, "lepSS1_tightID", 2, -0.5, 1.5, lepSS1_tID,     cat_evt_wgt );
	  // BookAndFill(tupI, "lepSS2_tightID", 2, -0.5, 1.5, lepSS2_tID,     cat_evt_wgt );

	  BookAndFill(tupF, "muOS_lepMVA",   40, -1, 1, muOS.lepMVA, cat_evt_wgt );
	  BookAndFill(tupF, "lepSS1_lepMVA", 40, -1, 1, lepSS1_MVA,  cat_evt_wgt );
	  BookAndFill(tupF, "lepSS2_lepMVA", 40, -1, 1, lepSS2_MVA,  cat_evt_wgt );
	  // BookAndFill(h_map_2D, h_pre+"lepSS2_vs_lepSS1_lepMVA", 40, -1, 1, 40, -1, 1, lepSS1_MVA, lepSS2_MVA,  cat_evt_wgt );

	  BookAndFill(tupF, "muSS_lepMVA",  40, -1, 1, muSS_MVA,  cat_evt_wgt, false );
	  BookAndFill(tupF, "ele_lepMVA",   40, -1, 1, ele_MVA,   cat_evt_wgt, false );
          BookAndFill(tupF, "ele_iso",      50,  0, 1, ele_iso,   cat_evt_wgt, false );
          BookAndFill(tupF, "ele_mIso",     50,  0, 1, ele_mIso,  cat_evt_wgt, false );
	  // BookAndFill(h_map_2D, h_pre+"ele_vs_muSS_lepMVA", 40, -1, 1, 40, -1, 1, muSS_MVA, ele_MVA, cat_evt_wgt, false );

	  // BookAndFill(tupF, "muOS_SIP",   50, 0, 10, muOS.SIP_3D, cat_evt_wgt );
	  // BookAndFill(tupF, "lepSS1_SIP", 50, 0, 10, lepSS1_SIP,  cat_evt_wgt );
	  // BookAndFill(tupF, "lepSS2_SIP", 50, 0, 10, lepSS2_SIP,  cat_evt_wgt );

	  // BookAndFill(tupF, "muOS_segCompat",   50, 0, 1, muOS.segCompat, cat_evt_wgt );
	  // BookAndFill(tupF, "lepSS1_segCompat", 50, 0, 1, lepSS1_seg,     cat_evt_wgt, false );
	  // BookAndFill(tupF, "lepSS2_segCompat", 50, 0, 1, lepSS2_seg,     cat_evt_wgt, false );

	  BookAndFill(tupF, "H_mass_GEN",          40, 110, 150, isData ? -99 : H_GEN_vec.M(),           cat_evt_wgt, false );  // Don't include overflow
	  BookAndFill(tupF, "Z_mass_GEN",          40,   1, 201, isData ? -99 : Z_GEN_vec.M(),           cat_evt_wgt, false );  // Don't include overflow
	  BookAndFill(tupF, "H_mass_true",         40, 110, 150, isData ? -99 : H_true_vec.M(), cat_evt_wgt, false );  // Don't include overflow
	  BookAndFill(tupF, "Z_mass_true",         40,   1, 201, isData ? -99 : Z_true_vec.M(), cat_evt_wgt, false );  // Don't include overflow
	  BookAndFill(tupF, "H_mass_GEN_or_true",  40, 110, 150, ((H_GEN_vec.M() > 0) || (H_true_vec.M() > 0)) ? H_pair_vec.M() : -99, cat_evt_wgt, false );

	  BookAndFill(b_map_flt, out_tree, h_pre, "H_pair_mass",                      H_pair_vec.M() );
          BookAndFill(b_map_flt, out_tree, h_pre, "H_pair_mass_PF",                   H_pair.mass    );
          BookAndFill(b_map_flt, out_tree, h_pre, "H_pair_mass_Roch",                 H_pair.mass_Roch ); 

	  BookAndFill(h_map_1D,            h_pre+ "H_pair_mass_zoomH", 800, 110, 150, H_pair_vec.M(),  cat_evt_wgt, false );  // Don't include overflow
	  BookAndFill(h_map_1D,            h_pre+ "H_pair_mass_zoomZ",  60,  76, 106, H_pair_vec.M(),  cat_evt_wgt, false );  // Don't include overflow
	  BookAndFill(h_map_1D,            h_pre+ "H_pair_mass_window",  8, 110, 150, H_pair_vec.M(),  cat_evt_wgt, false );  // Don't include overflow
	  BookAndFill(h_map_1D,            h_pre+ "H_pair_mass_wide",   40,   0, 400, H_pair_vec.M(),  cat_evt_wgt );
	  BookAndFill(h_map_1D,            h_pre+ "OS_pair_mass_zoomZ", 60,  76, 106, OS_pair_vec.M(), cat_evt_wgt, false );  // Don't include overflow
	  BookAndFill(h_map_1D, 	   h_pre+ "Btag_wgt",	       100,   0,   2, Btag_wgt,	       1.0, false );

	  BookAndFill(tupF, "H_pair_mass_err",   40,   0,   4, H_pair.massErr,   cat_evt_wgt );
	  BookAndFill(tupF, "H_pair_pt",         30,   0, 300, H_pair_vec.Pt(),  cat_evt_wgt );
          BookAndFill(tupF, "H_pair_eta",        40,   -4,  4, H_pair_vec.Eta(), cat_evt_wgt );
	  BookAndFill(tupF, "OS_pair_mass",      40,   0, 400, OS_pair_vec.M(),  cat_evt_wgt );  // "Wrong combo" of H_pair_mass
	  BookAndFill(tupF, "OS_pair_pt",        30,   0, 300, OS_pair_vec.Pt(), cat_evt_wgt );  // "Wrong combo" of H_pair_pt
	  BookAndFill(tupF, "SS_pair_mass",      40,   0, 400, SS_pair_vec.M(),  cat_evt_wgt );
	  BookAndFill(tupF, "SS_pair_pt",        30,   0, 300, SS_pair_vec.Pt(), cat_evt_wgt );
	  BookAndFill(tupF, "trilep_mass",       30,   0, 600, trilep_vec.M(),   cat_evt_wgt );

	  // More useful variables for WH vs. Z+jets and WH vs. WZ
	  BookAndFill(tupI, "lep_charge",           3, -1.5, 1.5, sum_lep_charge,                    cat_evt_wgt );
	  BookAndFill(tupF, "lep_H_pair_dEta",     20, -5.0, 5.0, SignedDEta( lep_vec, H_pair_vec ), cat_evt_wgt );
	  BookAndFill(tupF, "muSS_OS_pair_dEta",   20, -5.0, 5.0, SignedDEta(muSS_vec, OS_pair_vec), cat_evt_wgt );  // "Wrong combo" of lep_H_pair_dEta
	  BookAndFill(tupF, "lep_H_pair_dR",       20,    0, 6.0, lep_vec .DeltaR(H_pair_vec),       cat_evt_wgt );
	  BookAndFill(tupF, "muSS_OS_pair_dR",     20,    0, 6.0, muSS_vec.DeltaR(OS_pair_vec),      cat_evt_wgt );  // "Wrong combo" of lep_H_pair_dR
	  BookAndFill(tupF, "lep_muSS_cosThStar",  10, -1.0, 1.0, CosThetaStar(lep_vec, muSS_vec),   cat_evt_wgt );
	  BookAndFill(tupF, "muSS_lep_cosThStar",  10, -1.0, 1.0, CosThetaStar(muSS_vec, lep_vec),   cat_evt_wgt );  // "Wrong combo" of lep_muSS_cosThStar
	  BookAndFill(tupF, "lep_muOS_cosThStar",  10, -1.0, 1.0, CosThetaStar( lep_vec, muOS_vec),  cat_evt_wgt );
	  BookAndFill(tupF, "muSS_muOS_cosThStar", 10, -1.0, 1.0, CosThetaStar(muSS_vec, muOS_vec),  cat_evt_wgt );  // "Wrong combo" of lep_muOS_cosThStar
	  BookAndFill(tupF, "lep_muSS_dEta",       20, -5.0, 5.0, SignedDEta(lep_vec, muSS_vec),     cat_evt_wgt );
	  BookAndFill(tupF, "lep_muSS_dR",         20,    0, 6.0, lep_vec.DeltaR(muSS_vec),          cat_evt_wgt );
	  BookAndFill(tupF, "lep_muOS_dEta",       20, -5.0, 5.0, SignedDEta( lep_vec, muOS_vec),    cat_evt_wgt );
	  BookAndFill(tupF, "muSS_muOS_dEta",      20, -5.0, 5.0, SignedDEta(muSS_vec, muOS_vec),    cat_evt_wgt );  // "Wrong combo" of lep_muOS_dEta
	  BookAndFill(tupF, "lep_muOS_dR",         20,    0, 6.0, lep_vec .DeltaR(muOS_vec),         cat_evt_wgt );
	  BookAndFill(tupF, "muSS_muOS_dR",        30,    0, 6.0, muSS_vec.DeltaR(muOS_vec),         cat_evt_wgt );  // "Wrong combo" of lep_muOS_dR
	  BookAndFill(tupF, "lep_MET_dPhi_abs",    16,    0, 3.2, abs( lep_vec.DeltaPhi(MET_vec)),   cat_evt_wgt );
	  BookAndFill(tupF, "muSS_MET_dPhi_abs",   16,    0, 3.2, abs(muSS_vec.DeltaPhi(MET_vec)),   cat_evt_wgt );  // "Wrong combo" of lep_MET_dPhi_abs
	  BookAndFill(tupF, "lep_MET_MT",          20,    0, 200, lep_MET_vec.M(),                   cat_evt_wgt );
	  BookAndFill(tupF, "muSS_MET_MT",         30,    0, 300, (muSS_vecT + MET_vec).M(),         cat_evt_wgt );  // "Wrong combo" of lep_MET_MT
	  BookAndFill(tupF, "lep_MHT_MT",          20,    0, 200, lep_MHT_vec.M(),                   cat_evt_wgt );
	  BookAndFill(tupF, "muSS_MHT_MT",         30,    0, 300, (muSS_vecT + MHT_vec).M(),         cat_evt_wgt );  // "Wrong combo" of lep_MHT_MT
	  BookAndFill(tupF, "lep_MHT_dPhi_abs",    16,    0, 3.2, abs( lep_vec.DeltaPhi(MHT_vec)),   cat_evt_wgt );
	  BookAndFill(tupF, "muSS_MHT_dPhi_abs",   16,    0, 3.2, abs(muSS_vec.DeltaPhi(MHT_vec)),   cat_evt_wgt );  // "Wrong combo" of lep_MHT_dPhi_abs
	  BookAndFill(tupF, "MHT_MET_dPhi_abs",    16,    0, 3.2, abs(MHT_vec.DeltaPhi(MET_vec)),    cat_evt_wgt );

          BookAndFill(tupF, "lep_MET_JES_up_dPhi_abs",    16,    0, 3.2, abs( lep_vec.DeltaPhi(MET_JES_up_vec)),     cat_evt_wgt );
          BookAndFill(tupF, "lep_MET_JES_down_dPhi_abs",  16,    0, 3.2, abs( lep_vec.DeltaPhi(MET_JES_down_vec)),   cat_evt_wgt );
          BookAndFill(tupF, "muSS_MET_JES_up_dPhi_abs",   16,    0, 3.2, abs(muSS_vec.DeltaPhi(MET_JES_up_vec)),     cat_evt_wgt );  // "Wrong combo" of lep_MET_dPhi_abs
          BookAndFill(tupF, "muSS_MET_JES_down_dPhi_abs", 16,    0, 3.2, abs(muSS_vec.DeltaPhi(MET_JES_down_vec)),   cat_evt_wgt );  // "Wrong combo" of lep_MET_dPhi_abs
          BookAndFill(tupF, "lep_MET_JES_up_MT",          20,    0, 200, lep_MET_JES_up_vec.M(),                     cat_evt_wgt );
          BookAndFill(tupF, "lep_MET_JES_down_MT",        20,    0, 200, lep_MET_JES_down_vec.M(),                   cat_evt_wgt );         
          BookAndFill(tupF, "muSS_MET_JES_up_MT",         30,    0, 300, (muSS_vecT + MET_JES_up_vec).M(),           cat_evt_wgt );  // "Wrong combo" of lep_MET_MT
          BookAndFill(tupF, "muSS_MET_JES_down_MT",       30,    0, 300, (muSS_vecT + MET_JES_down_vec).M(),         cat_evt_wgt );  // "Wrong combo" of lep_MET_MT
          BookAndFill(tupF, "lep_MHT_JES_up_MT",          20,    0, 200, lep_MHT_JES_up_vec.M(),                     cat_evt_wgt );
          BookAndFill(tupF, "lep_MHT_JES_down_MT",        20,    0, 200, lep_MHT_JES_down_vec.M(),                   cat_evt_wgt );
          BookAndFill(tupF, "muSS_MHT_JES_up_MT",         30,    0, 300, (muSS_vecT + MHT_JES_up_vec).M(),           cat_evt_wgt );  // "Wrong combo" of lep_MHT_MT
          BookAndFill(tupF, "muSS_MHT_JES_down_MT",       30,    0, 300, (muSS_vecT + MHT_JES_down_vec).M(),         cat_evt_wgt );  // "Wrong combo" of lep_MHT_MT
          BookAndFill(tupF, "lep_MHT_JES_up_dPhi_abs",    16,    0, 3.2, abs( lep_vec.DeltaPhi(MHT_JES_up_vec)),     cat_evt_wgt );
          BookAndFill(tupF, "lep_MHT_JES_down_dPhi_abs",  16,    0, 3.2, abs( lep_vec.DeltaPhi(MHT_JES_down_vec)),   cat_evt_wgt );
          BookAndFill(tupF, "muSS_MHT_JES_up_dPhi_abs",   16,    0, 3.2, abs(muSS_vec.DeltaPhi(MHT_JES_up_vec)),     cat_evt_wgt );  // "Wrong combo" of lep_MHT_dPhi_abs
          BookAndFill(tupF, "muSS_MHT_JES_down_dPhi_abs", 16,    0, 3.2, abs(muSS_vec.DeltaPhi(MHT_JES_down_vec)),   cat_evt_wgt );  // "Wrong combo" of lep_MHT_dPhi_abs

	  // Extra variables especially for Xunwu's BDTs
	  BookAndFill( b_map_flt, out_tree, h_pre, "dimu_mass",           b_map_flt["H_pair_mass"]        );  // In Xunwu's code: dimu_vec.M()
	  BookAndFill( b_map_flt, out_tree, h_pre, "dimu_pt",             b_map_flt["H_pair_pt"]          );  // In Xunwu's code: dimu_vec.Pt()
	  BookAndFill( b_map_flt, out_tree, h_pre, "lep_pt",              b_map_flt["lep_pt"]             );  // In Xunwu's code: lep_vec.Pt()
	  BookAndFill( b_map_flt, out_tree, h_pre, "ldimu_abs_dEta",  abs(b_map_flt["lep_H_pair_dEta"])   );  // In Xunwu's code: abs(lep_vec.Eta() - dimu_vec.Eta())
	  BookAndFill( b_map_flt, out_tree, h_pre, "ldimu_dR",            b_map_flt["lep_H_pair_dR"]      );  // In Xunwu's code: lep_vec.DeltaR(dimu_vec)
	  BookAndFill( b_map_flt, out_tree, h_pre, "cts_lmuSS",           b_map_flt["lep_muSS_cosThStar"] );  // In Xunwu's code: CosThetaStar(lep_vec, mu1_vec)
	  BookAndFill( b_map_flt, out_tree, h_pre, "cts_lmuOS",           b_map_flt["lep_muOS_cosThStar"] );  // In Xunwu's code: CosThetaStar(lep_vec, mu2_vec)
	  BookAndFill( b_map_flt, out_tree, h_pre, "lmuSS_abs_dEta",  abs(b_map_flt["lep_muSS_dEta"])     );  // In Xunwu's code: abs(lep_vec.Eta() - mu2_vec.Eta())
	  BookAndFill( b_map_flt, out_tree, h_pre, "lmuSS_dR",            b_map_flt["lep_muSS_dR"]        );  // In Xunwu's code: lep_vec.DeltaR(mu2_vec)
	  BookAndFill( b_map_flt, out_tree, h_pre, "lmuOS_abs_dEta",  abs(b_map_flt["lep_muOS_dEta"])     );  // In Xunwu's code: abs(lep_vec.Eta() - mu1_vec.Eta())
	  BookAndFill( b_map_flt, out_tree, h_pre, "lmuOS_dR",            b_map_flt["lep_muOS_dR"]        );  // In Xunwu's code: lep_vec.DeltaR(mu1_vec)
	  BookAndFill( b_map_flt, out_tree, h_pre, "met_pt",              b_map_flt["MET"]                );  // In Xunwu's code: met_vec.Pt()
	  BookAndFill( b_map_flt, out_tree, h_pre, "mt_lmet",             b_map_flt["lep_MET_MT"]         );  // In Xunwu's code: lMET_vec.M()
	  BookAndFill( b_map_flt, out_tree, h_pre, "abs_dPhi_lmet",       b_map_flt["lep_MET_dPhi_abs"]   );  // In Xunwu's code: abs(lep_vec.DeltaPhi(met_vec))  );
	  BookAndFill( b_map_flt, out_tree, h_pre, "mht_pt",            (FourVec(*br.mht)).Pt()           );  // In Xunwu's code: mht_vec.Pt()
	  BookAndFill( b_map_flt, out_tree, h_pre, "mt_lmht",           (FourVec(*br.mht) + lep_vecT).M() );  // In Xunwu's code: lMHT_vec.M() - MAY BE BUGGY IN XUNWU'S TRAINING!!! (AWB 15.05.2019)

	  // Extra variables especially for Andrew's BDTs
	  BookAndFill( b_map_flt, out_tree, h_pre, "muH1_eta_abs", abs(b_map_flt["muH1_eta"]) );
	  BookAndFill( b_map_flt, out_tree, h_pre, "muH2_eta_abs", abs(b_map_flt["muH2_eta"]) );

	  // Evaluate MVA output values
	  BookAndFill( b_map_flt, out_tree, h_pre, "BDT_AWB_v3_noMass",   BDT_AWB_3_noMass  .Evaluate(b_map_flt, b_map_int) );
	  b_map_flt["BDTG_UF_v1"] = b_map_flt["BDT_AWB_v3_noMass"];  // Reset input value for v3 re-trained noMassBDT vs. mass
	  BookAndFill( b_map_flt, out_tree, h_pre, "BDT_AWB_v3_retrain",  BDT_AWB_3_retrain .Evaluate(b_map_flt, b_map_int) );
	  b_map_flt["BDTG_UF_v1"] = b_map_flt["BDT_AWB_v2_noMass"];  // ... and re-reset to avoid run-time errors
	  BookAndFill( b_map_flt, out_tree, h_pre, "BDT_final",   BDT_final  .Evaluate(b_map_flt, b_map_int) );

	  // Combine non-mass and with-mass BDT distributions into one plot
	  // Compress and shift non-mass BDT distribution from [-0.8, 0.9] to [-1.0, 0.4]
	  double noMass = (b_map_flt["BDT_AWB_v3_noMass"] * 0.8) - 0.3;
	  noMass = std::max(-0.999, std::min(0.399, noMass));

	  // Use only even-numbered MC events to fill histograms
	  if ( (b_map_int["event"] % 2) == 0 || isData ) {
	    float two = (isData ? 1.0 : 2.0);  // Scale MC by a factor of 2
	    BookAndFill( h_map_1D, h_pre+"BDT_AWB_v3_noMass",   20, -1, 1, b_map_flt["BDT_AWB_v3_noMass"],   cat_evt_wgt*two );
	    BookAndFill( h_map_1D, h_pre+"BDT_AWB_v3_retrain",  20, -1, 1, b_map_flt["BDT_AWB_v3_retrain"],  cat_evt_wgt*two );
	    BookAndFill( h_map_1D, h_pre+"BDT_final",           20, -1, 1, b_map_flt["BDT_final"],           cat_evt_wgt*two );

	    BookAndFill( h_map_1D, h_pre+"BDT_AWB_v3_noMass_zoom",   100, -1, 1, b_map_flt["BDT_AWB_v3_noMass"],   cat_evt_wgt*two );
	    BookAndFill( h_map_1D, h_pre+"BDT_AWB_v3_retrain_zoom",  100, -1, 1, b_map_flt["BDT_AWB_v3_retrain"],  cat_evt_wgt*two );
	    BookAndFill( h_map_1D, h_pre+"BDT_final_zoom",           100, -1, 1, b_map_flt["BDT_final"],           cat_evt_wgt*two );


	    // Write dimuon mass histogram in different no-mass BDT bins
	         if ( b_map_flt["BDT_AWB_v3_noMass"] > 0.6 )
	      BookAndFill( h_map_1D, h_pre+"H_pair_mass_BDT_p06_p10_zoomH", 800, 110, 150, b_map_flt["H_pair_mass"], cat_evt_wgt*two );
	    else if ( b_map_flt["BDT_AWB_v3_noMass"] > 0.2 )
	      BookAndFill( h_map_1D, h_pre+"H_pair_mass_BDT_p02_p06_zoomH", 800, 110, 150, b_map_flt["H_pair_mass"], cat_evt_wgt*two );
	    else if ( b_map_flt["BDT_AWB_v3_noMass"] > -0.2 )
	      BookAndFill( h_map_1D, h_pre+"H_pair_mass_BDT_n02_p02_zoomH", 800, 110, 150, b_map_flt["H_pair_mass"], cat_evt_wgt*two );
	    else
	      BookAndFill( h_map_1D, h_pre+"H_pair_mass_BDT_n10_n02_zoomH", 800, 110, 150, b_map_flt["H_pair_mass"], cat_evt_wgt*two );

	         if ( b_map_flt["BDT_AWB_v3_noMass"] > 0.76 )
	      BookAndFill( h_map_1D, h_pre+"H_pair_mass_BDT_p076_p10_zoomH", 800, 110, 150, b_map_flt["H_pair_mass"], cat_evt_wgt*two );
	    else if ( b_map_flt["BDT_AWB_v3_noMass"] > 0.68 )
	      BookAndFill( h_map_1D, h_pre+"H_pair_mass_BDT_p068_p076_zoomH", 800, 110, 150, b_map_flt["H_pair_mass"], cat_evt_wgt*two );
	    else if ( b_map_flt["BDT_AWB_v3_noMass"] > 0.60 )
	      BookAndFill( h_map_1D, h_pre+"H_pair_mass_BDT_p060_p068_zoomH", 800, 110, 150, b_map_flt["H_pair_mass"], cat_evt_wgt*two );
	    else
	      BookAndFill( h_map_1D, h_pre+"H_pair_mass_BDT_n100_p060_zoomH", 800, 110, 150, b_map_flt["H_pair_mass"], cat_evt_wgt*two );


            if (b_map_flt["BDT_final"] >= 0.3)
              BookAndFill( h_map_1D, h_pre+"H_pair_mass_BDT_final_p03_p10_zoomH", 800, 110, 150, b_map_flt["H_pair_mass"], cat_evt_wgt*two );
            else if (b_map_flt["BDT_final"] >= -0.1)
              BookAndFill( h_map_1D, h_pre+"H_pair_mass_BDT_final_n01_p03_zoomH", 800, 110, 150, b_map_flt["H_pair_mass"], cat_evt_wgt*two );
            else
              BookAndFill( h_map_1D, h_pre+"H_pair_mass_BDT_final_n10_n01_zoomH", 800, 110, 150, b_map_flt["H_pair_mass"], cat_evt_wgt*two );

            if (b_map_flt["BDT_final"] >= -0.1 and b_map_flt["BDT_final"] < 0.1)
              BookAndFill( h_map_1D, h_pre+"H_pair_mass_BDT_final_n01_p01_zoomH", 800, 110, 150, b_map_flt["H_pair_mass"], cat_evt_wgt*two );
            if (b_map_flt["BDT_final"] >=  0.1 and b_map_flt["BDT_final"] < 0.3)
              BookAndFill( h_map_1D, h_pre+"H_pair_mass_BDT_final_p01_p03_zoomH", 800, 110, 150, b_map_flt["H_pair_mass"], cat_evt_wgt*two );


	  } // End conditional: if ( (b_map_int["event"] % 2) == 0 || isData )

        
	} // End loop: for (int iCat = 0; iCat < CAT_CUTS.size(); iCat++)
      } // End loop: for (int iOpt = 0; iOpt < OPT_CUTS.size(); iOpt++)

      // Fill branches in output tuple tree, if event passed at least one category
      if (pass_opt_cuts && pass_cat_cuts) {

	if (n_map_int != -99 && n_map_int != b_map_int.size()) {
	  std::cout << "\n\nBizzare error!!! b_map_int = " << n_map_int << ", b_map_int.size() = " << b_map_int.size() << std::endl;
	  for (std::map<TString, int>::iterator it = b_map_int.begin(); it != b_map_int.end(); ++it) {
	    std::cout << it->first << " = " << it->second << std::endl;
	  }
	  ASSERT(false, "false");
	}
	if (n_map_flt != -99 && n_map_flt != b_map_flt.size()) {
	  std::cout << "\n\nBizzare error!!! b_map_flt = " << n_map_flt << ", b_map_flt.size() = " << b_map_flt.size() << std::endl;
	  for (std::map<TString, float>::iterator it = b_map_flt.begin(); it != b_map_flt.end(); ++it) {
	    std::cout << it->first << " = " << it->second << std::endl;
	  }
	  ASSERT(false, "false");
	}
	if (n_map_str != -99 && n_map_str != b_map_str.size()) {
	  std::cout << "\n\nBizzare error!!! b_map_str = " << n_map_str << ", b_map_str.size() = " << b_map_str.size() << std::endl;
	  for (std::map<TString, std::string>::iterator it = b_map_str.begin(); it != b_map_str.end(); ++it) {
	    std::cout << it->first << " = " << it->second << std::endl;
	  }
	  ASSERT(false, "false");
	}

	n_map_int = b_map_int.size();
	n_map_flt = b_map_flt.size();
	n_map_str = b_map_str.size();

	out_tree->Fill();
      }

    } // End loop: for (int iPair = 0; iPair < 2; iPair++)

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
      if (out_file_str.Length() > 0) out_file_name.Form( "%s/histos_%s_%s_%s_%s_%s.root",    out_dir.Data(), sample.Data(), selStr.c_str(), optCatStr.c_str(), SYS.c_str(), out_file_str.Data() );
      else                           out_file_name.Form( "%s/histos_%s_%s_%s_%s_%d_%d.root", out_dir.Data(), sample.Data(), selStr.c_str(), optCatStr.c_str(), SYS.c_str(), MIN_FILE, MAX_FILE );
      std::cout << "\nCreating output file " << out_file_name.Data() << std::endl;
      TFile* out_file = TFile::Open( out_file_name, "RECREATE" );
      
      // Write output file
      if (verbose) std::cout << "\nWriting output file " << out_file_name.Data() << std::endl;
      out_file->cd();

      // Write out 1D histograms
      for (std::map<TString, TH1*>::iterator it = h_map_1D.begin(); it != h_map_1D.end(); ++it) {
	std::string h_name = it->second->GetName();
	if ( StartsWith(h_name, (sample+"_"+optCatStr+"_").Data()) ) { // Histogram name starts with SAMP+OPT+CAT
	  // Remove optional selection and category cuts from histogram names
	  h_name.erase( h_name.find(optCatStr+"_"), optCatStr.length() + 1 );
	  it->second->SetName(h_name.c_str());
	  it->second->SetTitle(h_name.c_str());
	  // Re-scale shape-systematic shifted histograms to nominal yield
	  if ( EndsWith(h_name, "_UP") || EndsWith(h_name, "_DOWN") ) { // Histogram name ends with "UP" or "DOWN"
	    for (std::map<TString, TH1*>::iterator it2 = h_map_1D.begin(); it2 != h_map_1D.end(); ++it2) {
	      if (it->first == it2->first+"_UP" || it->first == it2->first+"_DOWN")
		it->second->Scale( it2->second->Integral() / it->second->Integral() );
	    }
	  }
	  // std::cout << "  * Writing 1D histogram " << it->second->GetName() << std::endl;
	  it->second->Write();
	}
      }
      // Write out 2D histograms
      for (std::map<TString, TH2*>::iterator it = h_map_2D.begin(); it != h_map_2D.end(); ++it) {
	std::string h_name = it->second->GetName();
	if ( StartsWith(h_name, (sample+"_"+optCatStr+"_").Data()) ) { // Histogram name starts with SAMP+OPT+CAT
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


  // Only write output tree file if there are some events
  if (out_tree->GetEntries() > 0) {
    std::cout << "\n******* Writing output tuple file " << out_tuple_name.Data()
	      << " with " << out_tree->GetEntries() << " events *******" << std::endl;
    out_tuple->cd();
    out_tree->Write();
    out_tuple->Write();
    out_tuple->Close();
  } else {
    std::cout << "\n******* NOT writing output tuple file " << out_tuple_name.Data()
	      << " - " << out_tree->GetEntries() << " events! *******" << std::endl;
  }


  std::cout << "\nExiting WH_lep_systematics()\n";
  
} // End void WH_lep_systematics()
