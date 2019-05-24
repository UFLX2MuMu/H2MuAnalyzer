#include "H2MuAnalyzer/MakeHistos/interface/ObjectHelper.h"


////////////////////////
///  Muon functions  ///
////////////////////////

// Return loose, medium, or tight muon ID
bool MuonID ( const MuonInfo & muon, const std::string muon_ID ) {
  if ( muon_ID == "loose" ) return muon.isLooseID;
  if ( muon_ID == "medium") return muon.isMediumID;
  if ( muon_ID == "tight" ) return muon.isTightID;
  std::cout << "\n\nInside ObjectSelections.cc, invalid option muon_ID = " << muon_ID << std::endl;
  assert(false);
}

// Return if muon fired HLT trigger
bool MuonTrig ( const MuonInfo & muon, const std::string year) {
  if        ( year == "2016" ) {
    // [2] = HLT_IsoMu24, [3] = HLT_IsoTkMu24
    return ( muon.isHltMatched[2] || muon.isHltMatched[3] );
  } else if ( year == "2017" ) {
    // [2] = HLT_IsoMu27, [3] = HLT_IsoTkMu27
    return ( muon.isHltMatched[2] || muon.isHltMatched[3] );
  } else if ( year == "2018" ) {
    // [2] = HLT_IsoMu24, [6] = HLT_Mu50, [8] = HLT_TkMu100
    return ( muon.isHltMatched[2] || muon.isHltMatched[6] || muon.isHltMatched[8] );
  }
  std::cout << "\n\nInside ObjectSelections.cc, invalid option year = " << year << std::endl;
  assert(false);
}

// Return PF, Rochester, or Kalman corrected muon pT
float MuonPt ( const MuonInfo & muon, const std::string pt_corr ) {
  if ( pt_corr == "PF"   ) return muon.pt;
  if ( pt_corr == "Roch" ) return muon.pt_Roch;
  if ( pt_corr == "KaMu" ) return muon.pt_KaMu;
  std::cout << "\n\nInside ObjectSelections.cc, invalid option pt_corr = " << pt_corr << std::endl;
  assert(false);
}

// Return PF, Rochester, or Kalman corrected dimuon invariant pT
float MuPairPt ( const MuPairInfo & muPair, const std::string pt_corr ) {
  if ( pt_corr == "PF"   ) return muPair.pt;
  if ( pt_corr == "Roch" ) return muPair.pt_Roch;
  if ( pt_corr == "KaMu" ) return muPair.pt_KaMu;
  std::cout << "\n\nInside ObjectSelections.cc, invalid option pt_corr = " << pt_corr << std::endl;
  assert(false);
}

// Return PF, Rochester, or Kalman corrected dimuon invariant mass
float MuPairMass ( const MuPairInfo & muPair, const std::string pt_corr ) {
  if ( pt_corr == "PF"   ) return muPair.mass;
  if ( pt_corr == "Roch" ) return muPair.mass_Roch;
  if ( pt_corr == "KaMu" ) return muPair.mass_KaMu;
  std::cout << "\n\nInside ObjectSelections.cc, invalid option pt_corr = " << pt_corr << std::endl;
  assert(false);
}

// Return PF, Rochester, or Kalman corrected dimuon invariant mass uncertainty
float MuPairMassErr ( const MuPairInfo & muPair, const std::string pt_corr ) {
  if ( pt_corr == "PF"   ) return muPair.massErr;
  if ( pt_corr == "Roch" ) return muPair.massErr_Roch;
  if ( pt_corr == "KaMu" ) return muPair.massErr_KaMu;
  std::cout << "\n\nInside ObjectSelections.cc, invalid option pt_corr = " << pt_corr << std::endl;
  assert(false);
}

// Determine if dimuon pair is matched to GEN pair
bool IsGenMatched( const MuPairInfo & muPair, const MuonInfos & muons, const GenMuonInfos & genMuons, const GenParentInfos & genParents, const std::string gen_ID ) {

  TLorentzVector mu_vec1 = FourVec( muons.at(muPair.iMu1), "PF" );
  TLorentzVector mu_vec2 = FourVec( muons.at(muPair.iMu2), "PF" );
  bool mu1_matched = false;
  bool mu2_matched = false;
  bool Pa_matched = false;

  if (gen_ID == "Z" or gen_ID == "H") { // dimuon properly matched to a gen-Z or gen-H
    int Pa_ID = 23;
    if (gen_ID == "H") Pa_ID = 25;
    for (const auto & genPa : genParents) {
	if (genPa.ID != Pa_ID or genPa.daughter_1_ID + genPa.daughter_2_ID != 0) continue;  // legit Z or H 
	if (genPa.daughter_1_ID != 13 and genPa.daughter_1_ID != -13) continue;		// decays to dimuon
	if (genPa.daughter_1_idx < 0 or genPa.daughter_2_idx < 0) continue;		// present in genMuons collection
	TLorentzVector gen_vec1 = FourVec( genMuons.at(genPa.daughter_1_idx) );
	TLorentzVector gen_vec2 = FourVec( genMuons.at(genPa.daughter_2_idx) );
	
	if ( mu_vec1.DeltaR(gen_vec1)<0.05 and mu_vec2.DeltaR(gen_vec2)<0.05 ) Pa_matched = true;
	if ( mu_vec1.DeltaR(gen_vec2)<0.05 and mu_vec2.DeltaR(gen_vec1)<0.05 ) Pa_matched = true;
    }
  } // end if (gen_ID == "Z" or gen_ID == "H")

  else if (gen_ID == "gamma" or gen_ID == "tau" or gen_ID == "light_quark") { // both muons from gamma or quarks or taus, no GenParent object
    for (const auto & genMu : genMuons) {
      if      (gen_ID == "gamma")         { if (genMu.mother_ID != 22) continue;}
      else if (gen_ID == "tau") 	  { if (genMu.mother_ID != 15 and genMu.mother_ID != -15) continue; }
      else if (gen_ID == "light_quark")   { if (genMu.mother_ID != 1 and genMu.mother_ID != 2 and genMu.mother_ID != -1 and genMu.mother_ID != -2 ) continue; }

      TLorentzVector gen_vec = FourVec( genMu );
      if ( mu_vec1.DeltaR(gen_vec) < 0.05 ) mu1_matched = true;
      if ( mu_vec2.DeltaR(gen_vec) < 0.05 ) mu2_matched = true;
    }
  } // end else if (gen_ID == "gamma" or gen_ID == "tau" or gen_ID == "light_quark")
  else assert(gen_ID == "Z" || gen_ID == "H" || gen_ID == "tau" || gen_ID == "light_quark" || gen_ID == "gamma");

  return ( (mu1_matched && mu2_matched) || Pa_matched );
} // End function: bool IsMatchedToGen()


////////////////////////////
///  Electron functions  ///
////////////////////////////

// Return loose, medium, or tight electron ID
bool EleID ( const EleInfo & ele, const std::string ele_ID ) {
  if ( ele_ID == "loose" ) return ele.isLooseID;
  if ( ele_ID == "medium") return ele.isMediumID;
  if ( ele_ID == "tight" ) return ele.isTightID;
  std::cout << "\n\nInside ObjectSelections.cc, invalid option ele_ID = " << ele_ID << std::endl;
  assert(false);
}



///////////////////
///  lepMVA SF  ///
///////////////////
float GetLepMVASF(const std::string lep_type, float pt, float eta, float lepMVA_cut) {
  if (lepMVA_cut < -1 or lepMVA_cut > 1) { 
    std::cout << "\nlepMVA cut out of range" << std::endl;
    return 0.0;
  }
  if ( pt > 200 ) return 1.0; // pt out of range
  if ( std::abs(eta) > 2.4) return 0.0; // eta out of range

  float abs_eta = std::abs(eta);
  float scale_factor = 0.0;

  TH3D * SF_mu = new TH3D();
  TH2D * SF_ele_loose = new TH2D();
  TH2D * SF_ele_medium = new TH2D();
  TH2D * SF_ele_tight = new TH2D();

  if (lep_type == "muon") {
    TFile * in_file = new TFile("/afs/cern.ch/work/x/xzuo/public/H2Mu/2018/Histograms/lepMVA_SF_v1/SF_muon_variable_eta_bin.root", "READ");
    SF_mu = (TH3D *) (in_file->Get("SFs/SF_3Dmuon")->Clone());
    int pt_bin   = SF_mu->GetXaxis()->FindBin(pt);
    int eta_bin  = SF_mu->GetYaxis()->FindBin(abs_eta);
    int MVA_bin  = SF_mu->GetZaxis()->FindBin(lepMVA_cut);
    scale_factor = SF_mu->GetBinContent(pt_bin, eta_bin, MVA_bin);
    SF_mu->Delete();
    in_file->Close();
    return scale_factor;
  } // end of if (lep_type == "muon") 
  else if (lep_type == "ele") {
    TFile * in_file = new TFile("/afs/cern.ch/work/x/xzuo/h2mm_944/src/H2MuAnalyzer/LepMVA_efficiency/python/scaleFactors_ele_2017.root", "READ");
    if (lepMVA_cut == -0.4) {
	SF_ele_loose = (TH2D * ) (in_file->Get("EleToTTVLeptonMvattZ4l")->Clone());
	int pt_bin   = SF_ele_loose->GetXaxis()->FindBin(pt);
	int eta_bin  = SF_ele_loose->GetYaxis()->FindBin(abs_eta);
	scale_factor = SF_ele_loose->GetBinContent(pt_bin, eta_bin);
	SF_ele_loose->Delete();
    }
    else if (lepMVA_cut == 0.4) {
	SF_ele_medium = (TH2D *) (in_file->Get("EleToTTVLeptonMvattZ3l")->Clone());
	int pt_bin   = SF_ele_medium->GetXaxis()->FindBin(pt);
        int eta_bin  = SF_ele_medium->GetYaxis()->FindBin(abs_eta);
	scale_factor = SF_ele_medium->GetBinContent(pt_bin, eta_bin);
	SF_ele_medium->Delete();
    }
    else if (lepMVA_cut == 0.8) {
	SF_ele_tight = (TH2D *) (in_file->Get("EleToTTVLeptonMvatZq")->Clone());
	int pt_bin   = SF_ele_tight->GetXaxis()->FindBin(pt);
        int eta_bin  = SF_ele_tight->GetYaxis()->FindBin(abs_eta);
        scale_factor = SF_ele_tight->GetBinContent(pt_bin, eta_bin);
	SF_ele_tight->Delete();
    }
    else scale_factor = 1.0;

    in_file->Close();
    return scale_factor;
  } // end of else if (lep_type == "ele")
  return scale_factor;
}



///////////////////////
///  Jet functions  ///
///////////////////////

 // Return loose, medium, or tight jet PU ID from 2016, 2017, or 2018
bool JetPUID ( const JetInfo & jet, const std::string PU_ID, const std::string year ) {

  float puID_cut = 999;  // Minimum puID value to pass cut

  if (year == "2016") return true;
  
  else if (year == "2017" || year == "2018") { // What is the true PU ID for 2018? - AWB 03.05.2019

    if (jet.pt >= 50) return true;

    // Where do these numbers come from?!?  No recommendation on the following twiki - AWB 24.09.2018
    // https://twiki.cern.ch/twiki/bin/viewauth/CMS/PileupJetID
    if ( PU_ID == "loose" ) {
      if      ( abs(jet.eta) < 2.50 ) {
	puID_cut = (jet.pt < 30 ? -0.97 : -0.89); }
      else if ( abs(jet.eta) < 2.75 ) {
	puID_cut = (jet.pt < 30 ? -0.68 : -0.52); }
      else if ( abs(jet.eta) < 3.00 ) {
	puID_cut = (jet.pt < 30 ? -0.53 : -0.38); }
      else if ( abs(jet.eta) < 5.00 ) {
	puID_cut = (jet.pt < 30 ? -0.47 : -0.30); }
      else { std::cout << "Inside JetPUID, invalid jet eta = " << jet.eta << std::endl; assert(false); }
    }
    else if ( PU_ID == "medium" ) {
      if      ( abs(jet.eta) < 2.50 ) {
	puID_cut = (jet.pt < 30 ? +0.18 : +0.61); }
      else if ( abs(jet.eta) < 2.75 ) {
	puID_cut = (jet.pt < 30 ? -0.55 : -0.35); }
      else if ( abs(jet.eta) < 3.00 ) {
	puID_cut = (jet.pt < 30 ? -0.42 : -0.23); }
      else if ( abs(jet.eta) < 5.00 ) {
	puID_cut = (jet.pt < 30 ? -0.36 : -0.17); }
      else { std::cout << "Inside JetPUID, invalid jet eta = " << jet.eta << std::endl; assert(false); }
    }
    else if ( PU_ID == "tight" ) {
      if      ( abs(jet.eta) < 2.50 ) {
	puID_cut = (jet.pt < 30 ? +0.69 : +0.86); }
      else if ( abs(jet.eta) < 2.75 ) {
	puID_cut = (jet.pt < 30 ? -0.35 : -0.10); }
      else if ( abs(jet.eta) < 3.00 ) {
	puID_cut = (jet.pt < 30 ? -0.42 : -0.23); }
      else if ( abs(jet.eta) < 5.00 ) {
	puID_cut = (jet.pt < 30 ? -0.36 : -0.17); }
      else { std::cout << "Inside JetPUID, invalid jet eta = " << jet.eta << std::endl; assert(false); }
    }

  } // End conditional: if (year == "2017")

  else { std::cout << "Inside JetPUID, invalid year = " << year << std::endl; assert(false); }

  return (jet.puID >= puID_cut);

} // End function: bool JetPUID ()


/////////////////////////////
///  Kinematic functions  ///
/////////////////////////////

TLorentzVector FourVec( const MuonInfo & muon, const std::string pt_corr, const std::string opt ) {
  TLorentzVector vec;
  if (opt == "T")
    vec.SetPtEtaPhiM(MuonPt(muon, pt_corr),        0, muon.phi, 0.105658367 );
  else
    vec.SetPtEtaPhiM(MuonPt(muon, pt_corr), muon.eta, muon.phi, 0.105658367 );
  return vec;
}
TLorentzVector FourVec( const MuPairInfo & muPair, const std::string pt_corr, const std::string opt, const MuonInfos & muons ) {
  TLorentzVector vec;
  if (opt == "T") {
    TLorentzVector mu1_vec = FourVec( muons.at(muPair.iMu1), pt_corr, "T");
    TLorentzVector mu2_vec = FourVec( muons.at(muPair.iMu2), pt_corr, "T");
    vec = mu1_vec + mu2_vec;
  }
  else
    vec.SetPtEtaPhiM(MuPairPt(muPair, pt_corr), muPair.eta, muPair.phi, MuPairMass(muPair, pt_corr) );
  return vec;
}
TLorentzVector FourVec( const EleInfo & ele, const std::string opt ) {
  TLorentzVector vec;
  if (opt == "T")
    vec.SetPtEtaPhiM(ele.pt,       0, ele.phi, 0.000511 );
  else
    vec.SetPtEtaPhiM(ele.pt, ele.eta, ele.phi, 0.000511 );
  return vec;
}
TLorentzVector FourVec( const JetInfo & jet, const std::string opt ) {
  TLorentzVector vec;
  if (opt == "T")
    vec.SetPtEtaPhiM(jet.pt,       0, jet.phi, jet.mass );
  else
    vec.SetPtEtaPhiM(jet.pt, jet.eta, jet.phi, jet.mass );
  return vec;
}
TLorentzVector FourVec( const MetInfo & met, const std::string opt ) {
  TLorentzVector vec;
  vec.SetPtEtaPhiM(met.pt, 0, met.phi, 0 );
  return vec;
}
TLorentzVector FourVec( const MhtInfo & mht, const std::string opt ) {
  TLorentzVector vec;
  vec.SetPtEtaPhiM(mht.pt, 0, mht.phi, 0 );
  return vec;
}
TLorentzVector FourVec( const GenParentInfo & genPar, const std::string opt ) {
  TLorentzVector vec;
  if (opt == "T")
    vec.SetPtEtaPhiM(genPar.pt,          0, genPar.phi, genPar.mass );
  else
    vec.SetPtEtaPhiM(genPar.pt, genPar.eta, genPar.phi, genPar.mass );
  return vec;
}
TLorentzVector FourVec( const GenMuonInfo & genMu, const std::string opt ) {
  TLorentzVector vec;
  if (opt == "T")
    vec.SetPtEtaPhiM(genMu.pt,         0, genMu.phi, 0.105658367 );
  else
    vec.SetPtEtaPhiM(genMu.pt, genMu.eta, genMu.phi, 0.105658367 );
  return vec;
}
TLorentzVector FourVec( const GenJetInfo & genJet, const std::string opt ) {
  TLorentzVector vec;
  if (opt == "T")
    vec.SetPtEtaPhiM(genJet.pt,          0, genJet.phi, genJet.mass );
  else
    vec.SetPtEtaPhiM(genJet.pt, genJet.eta, genJet.phi, genJet.mass );
  return vec;
}


float CosThetaStar( TLorentzVector vec1, TLorentzVector vec2 ) {
  TLorentzVector parent_vec = vec1 + vec2;
  TVector3 parent_p = parent_vec.BoostVector(), p1, p2;
  float cos_theta_star; // default to return the cos_theta_star of vec1, cannot tell charge from TLorentzVector.  - XWZ 23.10.2018

  vec1.Boost( -parent_p );
  p1 = vec1.BoostVector();

  cos_theta_star = parent_p * p1 / (parent_p.Mag() * p1.Mag());
  return cos_theta_star;
}

float Cos_CS_Theta( TLorentzVector vec1, TLorentzVector vec2 ) {
  TLorentzVector parent_vec = vec1 + vec2, beam_pos, beam_neg;
  TVector3 parent_p = parent_vec.BoostVector(), p1, p2, z_axis;
  float cos_cs_theta;
  
  beam_pos.SetXYZM(0,0, 6500, 0.93827);
  beam_neg.SetXYZM(0,0,-6500, 0.93827);

  vec1.Boost( -parent_p );
  beam_pos.Boost( -parent_p );
  beam_neg.Boost( -parent_p );

  z_axis = beam_pos.BoostVector().Unit() - beam_neg.BoostVector().Unit();
  p1 = vec1.BoostVector();
  cos_cs_theta = p1 * z_axis / (p1.Mag() * z_axis.Mag());
  return cos_cs_theta;
}

float Cos_CS_Phi( TLorentzVector vec1, TLorentzVector vec2 ) {
  TLorentzVector parent_vec = vec1 + vec2, beam_pos, beam_neg;
  TVector3 parent_p = parent_vec.BoostVector(), p1, p2, z_axis, norm_had, norm_lep;
  float cos_cs_phi;

  beam_pos.SetXYZM(0,0, 6500, 0.93827);
  beam_neg.SetXYZM(0,0,-6500, 0.93827);

  vec1.Boost( -parent_p );
  beam_pos.Boost( -parent_p );
  beam_neg.Boost( -parent_p );

  z_axis = beam_pos.BoostVector().Unit() - beam_neg.BoostVector().Unit();
  p1 = vec1.BoostVector();

  norm_had = z_axis.Cross( beam_pos.BoostVector() );
  norm_lep = z_axis.Cross( p1 );
  cos_cs_phi = norm_had * norm_lep / (norm_had.Mag() * norm_lep.Mag());
  return cos_cs_phi;
}

float Sin_CS_Phi( TLorentzVector vec1, TLorentzVector vec2 ) {
  TLorentzVector parent_vec = vec1 + vec2, beam_pos, beam_neg;
  TVector3 parent_p = parent_vec.BoostVector(), p1, p2, z_axis, norm_had, norm_lep;
  float sin_cs_phi;

  beam_pos.SetXYZM(0,0, 6500, 0.93827);
  beam_neg.SetXYZM(0,0,-6500, 0.93827);

  vec1.Boost( -parent_p );
  beam_pos.Boost( -parent_p );
  beam_neg.Boost( -parent_p );

  z_axis = beam_pos.BoostVector().Unit() - beam_neg.BoostVector().Unit();
  p1 = vec1.BoostVector();

  norm_had = z_axis.Cross( beam_pos.BoostVector() );
  norm_lep = z_axis.Cross( p1 );
  sin_cs_phi = norm_had.Cross(norm_lep).Mag() / (norm_had.Mag() * norm_lep.Mag());
  return sin_cs_phi;
}

// Return a new JetPair object, modeled on Ntupliser/DiMuons/src/JetPairHelper.cc
JetPairInfo MakeJetPair( TLorentzVector jet1_vec, TLorentzVector jet2_vec ) {

  JetPairInfo    jetPair;
  TLorentzVector pair_vec;

  jetPair.iJet1 = -99; // Unknown, doesn't matter
  jetPair.iJet2 = -99; // Unknown, doesn't matter

  pair_vec = jet1_vec + jet2_vec;

  jetPair.mass    = pair_vec.M();
  jetPair.pt      = pair_vec.Pt();
  jetPair.eta     = pair_vec.PseudoRapidity();
  jetPair.phi     = pair_vec.Phi();

  jetPair.dR   = jet1_vec.DeltaR(jet2_vec);
  jetPair.dEta = jet1_vec.PseudoRapidity() - jet2_vec.PseudoRapidity();
  jetPair.dPhi = jet1_vec.DeltaPhi(jet2_vec);

  return jetPair;

}
