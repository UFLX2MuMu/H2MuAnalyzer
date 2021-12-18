#include "H2MuAnalyzer/MakeHistos/interface/BtagSFHelper.h"

void GetBtagEff( BtagEffEntry & bee, const std::string _year, const std::string _sample) {


  if (_year == "2016" || _year == "2017" || _year == "2018") {
    bee.year   = _year;
    bee.sample = _sample;

    std::string file_name = "data/deepCSV/Btag_efficiency/Btag_efficiency_3l_" + _year  + ".csv";
    std::fstream b_file(file_name);
    std::string line, item;

    while( std::getline(b_file, line) ) {
      std::stringstream s(line);
      std::getline(s, item, ','); // first column, sample
      if( item != _sample ) continue;

      std::getline(s, item, ','); // second column, jet flavor
      if (item == " FLAV_B") {
	std::getline(s, item, ','); // third column, nJets
	bee.nJets_B = std::stoi(item);
	std::getline(s, item, ','); // fourth column, Loose Eff
	bee.LEff_B = std::stof(item);
	std::getline(s, item, ','); // fifth column, Medium Eff
	bee.MEff_B = std::stof(item);
      }
      else if (item == " FLAV_C") {
	std::getline(s, item, ','); // third column, nJets
	bee.nJets_C = std::stoi(item);
	std::getline(s, item, ','); // fourth column, Loose Eff
	bee.LEff_C = std::stof(item);
	std::getline(s, item, ','); // fifth column, Medium Eff
	bee.MEff_C = std::stof(item);
      }
      else if (item == " FLAV_UDSG") {
	std::getline(s, item, ','); // third column, nJets
	bee.nJets_UDSG = std::stoi(item);
	std::getline(s, item, ','); // fourth column, Loose Eff
	bee.LEff_UDSG = std::stof(item);
	std::getline(s, item, ','); // fifth column, Medium Eff
	bee.MEff_UDSG = std::stof(item);
      }

    } // end of while( std::getline(b_file, line) )

  } // end of if (_year == "2016" || _year == "2017" || _year == "2018")

  else {
    std::cout << "Inside ConfigureEventWeight, invalid year = " << _year << std::endl;
    assert(false);
  }

} // end of function void GetBtagEff( BtagEffEntry & bee, const std::string _year, const std::string _sample)


float CalBtagSF( const BtagEffEntry & bee, const std::string jet_FLV, const float SF_L, const float SF_M) {
// no need to send in the jet_pt and jet_eta, for central SF values the eta and pt binning are inclusive
// for the moment only work for two cases: 
  float proper_SF  = 0.0;
  float loose_eff  = 0.0;
  float medium_eff = 0.0;

  if (SF_M == 0.0) { // not loose tagged
    if      (jet_FLV == "FLAV_B") {
      if (bee.nJets_B < 10 or bee.LEff_B < 0 or bee.LEff_B >= 1.0) loose_eff = 0.0;  // low stats or unreasonable values
      else loose_eff = bee.LEff_B;
    }
    else if (jet_FLV == "FLAV_C") {
      if (bee.nJets_C < 10 or bee.LEff_C < 0 or bee.LEff_C >= 1.0) loose_eff = 0.0;  // low stats or unreasonable values
      else loose_eff = bee.LEff_C;
    }
    else if (jet_FLV == "FLAV_UDSG") {
      if (bee.nJets_UDSG < 10 or bee.LEff_UDSG < 0 or bee.LEff_UDSG >= 1.0) loose_eff = 0.0;  // low stats or unreasonable values
      else loose_eff = bee.LEff_UDSG;
    }
    else {
      std::cout << "invalid jet_FLV input: " << jet_FLV << "\nsomething is wrong" << std::endl;
    }
    proper_SF = (1.0 - SF_L * loose_eff) / (1.0 - loose_eff);
  } // end of if (SF_M == 0.0) 
 
  else if (SF_M > 0) { // loose tagged, not medium tagged
    if      (jet_FLV == "FLAV_B") {
      loose_eff  = bee.LEff_B;
      medium_eff = bee.MEff_B;
      if (bee.nJets_B < 10 or bee.LEff_B < 0 or bee.LEff_B >= 1.0) loose_eff  = 1.0;  // low stats or unreasonable values
      if (bee.nJets_B < 10 or bee.MEff_B < 0 or bee.MEff_B >= 1.0) medium_eff = 0.0;  // low stats or unreasonable values
    }
    else if (jet_FLV == "FLAV_C") {
      loose_eff  = bee.LEff_C;
      medium_eff = bee.MEff_C;
      if (bee.nJets_C < 10 or bee.LEff_C < 0 or bee.LEff_C >= 1.0) loose_eff  = 1.0;  // low stats or unreasonable values
      if (bee.nJets_C < 10 or bee.MEff_C < 0 or bee.MEff_C >= 1.0) medium_eff = 0.0;  // low stats or unreasonable values
    }
    else if (jet_FLV == "FLAV_UDSG") {
      loose_eff  = bee.LEff_UDSG;
      medium_eff = bee.MEff_UDSG;
      if (bee.nJets_UDSG < 10 or bee.LEff_UDSG < 0 or bee.LEff_UDSG >= 1.0) loose_eff  = 1.0;  // low stats or unreasonable values
      if (bee.nJets_UDSG < 10 or bee.MEff_UDSG < 0 or bee.MEff_UDSG >= 1.0) medium_eff = 0.0;  // low stats or unreasonable values
    }
    else {
      std::cout << "invalid jet_FLV input: " << jet_FLV << "\nsomething is wrong" << std::endl;
    }
    // get loose_eff and medium_eff
    proper_SF = (SF_L * loose_eff - SF_M * medium_eff) / (loose_eff - medium_eff);
  }

  else std::cout << "Bizzare case: SF_medium < 0, not understood. Setting SF = 0.0 (not using this event)" << std::endl;

  return proper_SF;
} // end of function float CalBtagSF( BtagEffEntry & bee, float jet_pt, float jet_eta, int jet_FLV, float SF_L, float SF_M)

