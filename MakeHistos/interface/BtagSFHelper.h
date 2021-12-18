#ifndef BtagSFHelper
#define BtagSFHelper

#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>

struct BtagEffEntry {

  // specification
  std::string year   = "NONE";
  std::string sample = "NONE";

  // values
  int nJets_B    = 0;
  int nJets_C    = 0;
  int nJets_UDSG = 0;

  float LEff_B    = 0.0;
  float LEff_C    = 0.0;
  float LEff_UDSG = 0.0;
  float MEff_B    = 0.0;
  float MEff_C    = 0.0;
  float MEff_UDSG = 0.0;

  void Print() {
    std::cout << "\nPrinting Btag Efficiency values for year = " << year << " and sample = " << sample << std::endl;
    std::cout << "nJets_B = " << nJets_B << ", LEff_B = " << LEff_B << ", MEff_B = " << MEff_B << std::endl;
    std::cout << "nJets_C = " << nJets_C << ", LEff_C = " << LEff_C << ", MEff_C = " << MEff_C << std::endl;
    std::cout << "nJets_UDSG = " << nJets_UDSG << ", LEff_UDSG = " << LEff_UDSG << ", MEff_UDSG = " << MEff_UDSG << std::endl;
  } // end of Print()

}; 
// end of struct BtagEffEntry


void GetBtagEff( BtagEffEntry & bee, const std::string _year, const std::string _sample);

float CalBtagSF( const BtagEffEntry & bee, const std::string jet_FLV, const float SF_L, const float SF_M = 0.0);


#endif  // #ifndef BtagSFHelper
