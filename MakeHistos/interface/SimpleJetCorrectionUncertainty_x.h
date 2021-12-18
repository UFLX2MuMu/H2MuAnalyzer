#ifndef SimpleJetCorrectionUncertainty_x_h
#define SimpleJetCorrectionUncertainty_x_h

#include <iostream>
#include <assert.h>
#include <string>
#include <vector>
class JetCorrectorParameters_x;

class SimpleJetCorrectionUncertainty_x 
{
 public:
  SimpleJetCorrectionUncertainty_x();
  SimpleJetCorrectionUncertainty_x(const std::string& fDataFile);
  SimpleJetCorrectionUncertainty_x(const JetCorrectorParameters_x& fParameters);
  ~SimpleJetCorrectionUncertainty_x();
  const JetCorrectorParameters_x& parameters() const {return *mParameters;}
  float uncertainty(const std::vector<float>& fX, float fY, bool fDirection) const;

 private:
  SimpleJetCorrectionUncertainty_x(const SimpleJetCorrectionUncertainty_x&) = delete;
  SimpleJetCorrectionUncertainty_x& operator= (const SimpleJetCorrectionUncertainty_x&) = delete;
  int findBin(const std::vector<float>& v, float x) const;
  float uncertaintyBin(unsigned fBin, float fY, bool fDirection) const;
  float linearInterpolation (float fZ, const float fX[2], const float fY[2]) const;
  JetCorrectorParameters_x* mParameters;
};

#endif

