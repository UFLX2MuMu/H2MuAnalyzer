/////////////////////////////////////////////////////////////
///   Macro to make minintuple for faster BDT training    ///
///         make minintuple in selected categories        ///
///                Xunwu Zuo  08.11.2018                  ///
/////////////////////////////////////////////////////////////


//make map of trees, each contain different branches targeted for training in that category
//each tree writen in corresponding out root file
//everything defined before entry loop, calculated in entry loop, cat loop


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
#include "H2MuAnalyzer/MakeHistos/interface/MiniNTupleHelper.h"
#include "H2MuAnalyzer/MakeHistos/interface/SampleID.h"
#include "H2MuAnalyzer/MakeHistos/interface/KinematicAngles.h"
#include "H2MuAnalyzer/MakeHistos/interface/ReadMVA.h"            // Read and evaluate XMLs for MVA
#include "H2MuAnalyzer/MakeHistos/interface/DASTutorialObjects.h"

// #include "H2MuAnalyzer/MakeHistos/interface/SampleDatabase2016.h" // Input data and MC samples

// Load the library of the local, compiled H2MuAnalyzer/MakeHistos directory
R__LOAD_LIBRARY(../../../tmp/slc6_amd64_gcc630/src/H2MuAnalyzer/MakeHistos/src/H2MuAnalyzerMakeHistos/libH2MuAnalyzerMakeHistos.so)


// Hard-coded options for running locally / manually
// Options passed in as arguments to ReadNTupleChain when running in batch mode
const int MIN_FILE = 1;     // Minimum index of input files to process
const int MAX_FILE = 1;     // Maximum index of input files to process
const int MAX_EVT  = 100000;    // Maximum number of events to process
const int PRT_EVT  = 1000;  // Print every N events
const float SAMP_WGT = 1.0;
const bool verbose = false; // Print extra information


//const TString IN_DIR    = "/eos/cms/store/group/phys_higgs/HiggsExo/H2Mu/UF/ntuples/2018/102X/prod-v18.1.6.skim3l/ZH_HToMuMu_ZToAll_M125_TuneCP5_PSweights_13TeV_powheg_pythia8/H2Mu_ZH_125/190528_111720/0000";
//const TString SAMPLE    = "H2Mu_ZH_125";

//const TString IN_DIR   = "/eos/cms/store/user/bortigno/h2mm/ntuples/2016/94X_v3/STR/ZH_HToMuMu_ZToAll_M125_TuneCP5_PSweights_13TeV_powheg_pythia8/H2Mu_ZH_125/190625_204256/0000";
//const TString SAMPLE   = "H2Mu_ZH_125";

//const TString IN_DIR   = "/eos/cms/store/group/phys_higgs/HiggsExo/H2Mu/UF/ntuples/2017/94X_v2/2019_01_15_LepMVA_3l_test_v1/ZH_HToMuMu_ZToAll_M125_13TeV_powheg_pythia8/H2Mu_ZH_125";
//const TString SAMPLE   = "H2Mu_ZH_125";

const TString IN_DIR    = "/eos/cms/store/group/phys_higgs/HiggsExo/H2Mu/UF/ntuples/2018/102X/prod-v18.1.6.skim3l/ZZTo4L_TuneCP5_13TeV_powheg_pythia8/ZZ_4l/190528_113414/0000";
const TString SAMPLE    = "ZZ_4l";

//const TString IN_DIR    = "/eos/cms/store/user/bortigno/h2mm/ntuples/2016/94X_v3/STR/ZZTo4L_13TeV-amcatnloFXFX-pythia8/ZZ_4l_amc/190625_205403/0000";
//const TString SAMPLE    = "ZZ_4l_amc";

//const TString IN_DIR   = "/eos/cms/store/group/phys_higgs/HiggsExo/H2Mu/UF/ntuples/2017/94X_v2/2019_01_15_LepMVA_3l_test_v1/ZZTo4L_13TeV_powheg_pythia8/ZZ_4l";
//const TString SAMPLE   = "ZZ_4l";
//const TString IN_DIR   = "/eos/cms/store/group/phys_higgs/HiggsExo/H2Mu/UF/ntuples/Moriond17/Mar13_hiM/SingleMuon";
//const TString SAMPLE   = "SingleMu";
const std::string YEAR = "2018";
const std::string SLIM  = (YEAR == "2017" ? "Slim" : "notSlim");  // "Slim" or "notSlim" - Some 2017 NTuples are "Slim"
const TString OUT_DIR  = "plots";
const TString HIST_TREE = "Tree"; // "Hist", "Tree", or "HistTree" to output histograms, trees, or both. Not in use in this macro

const std::vector<std::string> SEL_CUTS = {"PreselRun2"}; // Cuts which every event must pass
const std::vector<std::string> OPT_CUTS = {"NONE"}; // Multiple selection cuts, applied independently in parallel
const std::vector<std::string> CAT_CUTS = {"NONE"}; // Event selection categories, also applied in parallel


// Command-line options for running in batch.  Running "root -b -l -q macros/ReadNTupleChain.C" will use hard-coded options above.
void SimpleTreeForDas( TString sample = "", TString in_dir = "", TString out_dir = "",
	     std::vector<TString> in_files = {}, TString out_file_str = "",
	     int max_evt = 0, int prt_evt = 0, float samp_weight = 1.0,
	     TString hist_tree = "", std::string SYS = "" ) {
  
  // Set variables to hard-coded values if they are not initialized
  if (sample.Length()      == 0) sample      = SAMPLE;
  if (in_dir.Length()      == 0) in_dir      = IN_DIR;
  if (out_dir.Length()     == 0) out_dir     = OUT_DIR;
  if (max_evt              == 0) max_evt     = MAX_EVT;
  if (prt_evt              == 0) prt_evt     = PRT_EVT;
  if (samp_weight          == 0) samp_weight = SAMP_WGT;
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
      if (YEAR == "2017") in_file_name.Form("%s/NTuple_0.root", in_dir.Data());
      else   		  in_file_name.Form("%s/tuple_%d.root", in_dir.Data(), i);
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
      SetBranchAddresses(*in_chain, br, {YEAR, SLIM, "GEN", "Wgts"}, (SYS.find("JES_") != std::string::npos ? SYS : "noSys"), false); // Options in {} include "JES", "Flags", and "SFs"
  }

  // creating output file

  TString out_file_name;
  if (out_file_str.Length() > 0) out_file_name.Form( "%s/histos_%s_%s.root",    out_dir.Data(), sample.Data(), out_file_str.Data() );
  else                           out_file_name.Form( "%s/histos_%s_%d_%d.root", out_dir.Data(), sample.Data(), MIN_FILE, MAX_FILE );
  std::cout << "\nCreating output file " << out_file_name.Data() << std::endl;
  TFile * Out_File = TFile::Open( out_file_name, "RECREATE" );

  // plant TTree and add branches
  TTree * Out_Tree = PlantTree("tree", "simple_tree");

  // Configuration for object selection, event selection, and object weighting
  ObjectSelectionConfig obj_sel;
  EventSelectionConfig  evt_sel;
  EventWeightConfig     evt_wgt;
  ConfigureObjectSelection(obj_sel, YEAR);
  ConfigureEventSelection (evt_sel, YEAR);
  ConfigureEventWeight    (evt_wgt, YEAR);

  evt_sel.muPair_mass_min = 60; // Require at least one Higgs candidate pair, default 60

  if (verbose) obj_sel.Print();
  if (verbose) evt_sel.Print();
  if (verbose) evt_wgt.Print();

  std::string PTC = obj_sel.mu_pt_corr; // Store muon pT correction in a shorter string; not changed later


  // variables to add
  std::vector<int>   muon_charge;
  std::vector<float> muon_pt;
  std::vector<float> muon_eta;
  std::vector<float> muon_phi;
  std::vector<float> muon_lepMVA;

  std::vector<int>   electron_charge;
  std::vector<float> electron_pt;
  std::vector<float> electron_eta;
  std::vector<float> electron_phi;
  std::vector<float> electron_lepMVA;

  std::vector<float> jet_pt;
  std::vector<float> jet_eta;
  std::vector<float> jet_phi;
  std::vector<float> jet_DeepCSV;

  int	   nMuons;
  int	   nElectrons;
  int	   nJets;
  float    MET_pt;
  float    MET_phi;

  float    event_wgt;
  float    xsec_norm;
  int	   sample_type;
  TString  sample_name;

  Out_Tree->Branch("nMuons",	   &nMuons,		"nMuons/I");
  Out_Tree->Branch("muon_charge",  &muon_charge  );
  Out_Tree->Branch("muon_pt",      &muon_pt      );
  Out_Tree->Branch("muon_eta",     &muon_eta     );
  Out_Tree->Branch("muon_phi",     &muon_phi     );
  Out_Tree->Branch("muon_lepMVA",  &muon_lepMVA  );

  Out_Tree->Branch("nElectrons",        &nElectrons,    "nElectrons/I");
  Out_Tree->Branch("electron_charge",   &electron_charge  );
  Out_Tree->Branch("electron_pt",       &electron_pt      );
  Out_Tree->Branch("electron_eta",      &electron_eta     );
  Out_Tree->Branch("electron_phi",      &electron_phi     );
  Out_Tree->Branch("electron_lepMVA",   &electron_lepMVA  );

  Out_Tree->Branch("nJets",        &nJets,              "nJets/I");
  Out_Tree->Branch("jet_pt",       &jet_pt      );
  Out_Tree->Branch("jet_eta",      &jet_eta     );
  Out_Tree->Branch("jet_phi",      &jet_phi     );
  Out_Tree->Branch("jet_DeepCSV",  &jet_DeepCSV  );

  Out_Tree->Branch("MET_pt", 		& MET_pt,		"MET_pt/F");
  Out_Tree->Branch("MET_phi",           & MET_phi,              "MET_phi/F");

  Out_Tree->Branch("event_wgt",         & event_wgt,            "event_wgt/F");
  Out_Tree->Branch("xsec_norm",         & xsec_norm,            "xsec_norm/F");
  Out_Tree->Branch("sample_type",       & sample_type,	        "sample_type/I");
  Out_Tree->Branch("sample_name",       & sample_name,          "sample_name/C");


  std::cout << "\n******* About to enter the event loop *******" << std::endl;
  for (int iEvt = 0; iEvt < in_chain->GetEntries(); iEvt++) {
    
    if (iEvt > max_evt && max_evt > 0) break;
    if ( (iEvt % prt_evt) == 0 ) {
      std::cout << "\n*********************" << std::endl;
      std::cout << "Looking at event " << iEvt <<  std::endl;
      std::cout << "*********************" << std::endl;
    }

    if ( sample.Contains("SingleMu")     and iEvt % 5000 >= 62 ) continue;
    if ( not sample.Contains("SingleMu") and iEvt % 5000 >= 50 ) continue;

    
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

    // For 2016 NTuples, convert "SlimJets" collection into regular jets
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

    if (pass_sel_cuts) {
      
      for (int iOpt = 0; iOpt < OPT_CUTS.size(); iOpt++) {
	std::string OPT_CUT = OPT_CUTS.at(iOpt);
	std::string h_pre = (std::string) sample + "_"+OPT_CUT;	
	///////////////////////////////////////////////////////////////
	///  select objects and get variables to be filled in tree  ///
	///////////////////////////////////////////////////////////////

	// init variables
	muon_charge  .clear();
        muon_pt      .clear();
        muon_eta     .clear();
        muon_phi     .clear();  
        muon_lepMVA  .clear();
                
        electron_charge   .clear();
        electron_pt       .clear();
	electron_eta      .clear();
        electron_phi      .clear();
        electron_lepMVA   .clear();
                  
        jet_pt       .clear();
        jet_eta      .clear();
        jet_phi      .clear();
        jet_DeepCSV  .clear();

	nMuons		    = -999;
	nElectrons	    = -999;
	nJets		    = -999;
	MET_pt		    = -999;
	MET_phi		    = -999;

	event_wgt           = 0;
        xsec_norm           = 0;
 	sample_type	    = -999;
	sample_name         = "none";

	// fill values
	event_wgt   = ( sample.Contains("SingleMu") ? 1.0 : EventWeight(br, evt_wgt, verbose) );
	xsec_norm   = samp_weight;
	sample_type = getSampleID(sample);

	for (const auto & muon     : SelectedMuons(obj_sel, br)) {
	  muon_charge .push_back( muon.charge  );
	  muon_pt     .push_back( muon.pt_Roch );
	  muon_eta    .push_back( muon.eta     );
	  muon_phi    .push_back( muon.phi     );
          muon_lepMVA .push_back( muon.lepMVA  );
	}
        for (const auto & electron : SelectedEles (obj_sel, br)) {
	  electron_charge .push_back( electron.charge );
          electron_pt     .push_back( electron.pt     );
          electron_eta    .push_back( electron.eta    );
          electron_phi    .push_back( electron.phi    );
          electron_lepMVA .push_back( electron.lepMVA );
	}
	for (const auto & jet      : SelectedJets (obj_sel, br)) {
	  jet_pt      .push_back( jet.pt      );
	  jet_eta     .push_back( jet.eta     );
 	  jet_phi     .push_back( jet.phi     );
	  jet_DeepCSV .push_back( jet.deepCSV );
	}


	nMuons     = muon_pt.size();
	nElectrons = electron_pt.size();
	nJets	   = jet_pt.size();
	MET_pt     = (br.met)->pt;
	MET_phi    = (br.met)->phi;


	  ///////////////////////////////////////////
	  ///  Apply the category selection cuts  ///
	  ///////////////////////////////////////////
        for (int iCat = 0; iCat < CAT_CUTS.size(); iCat++) {  
	  std::string CAT_CUT = CAT_CUTS.at(iCat);
	  bool pass_cat_cut = true;

	  if (CAT_CUT == "something") {
	  }
	  else { pass_cat_cut = InCategory(obj_sel, br, CAT_CUT, verbose); }

	  if (not pass_cat_cut) continue;
	  if (verbose) std::cout << "\nPassed cut " << OPT_CUT << ", in category " << CAT_CUT << std::endl;

	  
	} // End loop: for (int iCat = 0; iCat < CAT_CUTS.size(); iCat++)
	Out_Tree->Fill();  // again, only one opt sel
      } // End loop: for (int iOpt = 0; iOpt < OPT_CUTS.size(); iOpt++)
    } // End conditional: if (pass_sel_cuts)

  } // End loop: for (int iEvt = 0; iEvt < in_chain->GetEntries(); iEvt++)
  std::cout << "\n******* Leaving the event loop *******" << std::endl;
     
  // Write output file
  Out_File->cd();
  Out_Tree->Write();
  Out_File->Write();
      
  
  std::cout << "\nExiting ()\n";
  
} // End void SimpleTreeForDas()
