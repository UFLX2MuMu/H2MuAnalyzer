#ifndef JetCorrectionUncertainty_x_h
#define JetCorrectionUncertainty_x_h

//#include "CondFormats/Serialization/interface/Serializable.h"

#include <iostream>
#include <assert.h>

#include <string>
#include <vector>
class SimpleJetCorrectionUncertainty_x;
class JetCorrectorParameters_x;

class JetCorrectionUncertainty_x 
{
  public:
    JetCorrectionUncertainty_x();
    JetCorrectionUncertainty_x(const std::string& fDataFile);
    JetCorrectionUncertainty_x(const JetCorrectorParameters_x& fParameters);
    ~JetCorrectionUncertainty_x();

    void setParameters  (const std::string& fDataFile);
    void setJetEta      (float fEta);
    void setJetPt       (float fPt); 
    void setJetE        (float fE);
    void setJetPhi      (float fE);
    void setJetEMF      (float fEMF); 
    void setLepPx       (float fLepPx);
    void setLepPy       (float fLepPy);
    void setLepPz       (float fLepPz);
    void setAddLepToJet (bool fAddLepToJet) {mAddLepToJet = fAddLepToJet;}
    float getUncertainty(bool fDirection);

 private:
  JetCorrectionUncertainty_x(const JetCorrectionUncertainty_x&) = delete;
  JetCorrectionUncertainty_x& operator= (const JetCorrectionUncertainty_x&) = delete;
  std::vector<float> fillVector(const std::vector<std::string>& fNames);
  float getPtRel();
  //---- Member Data ---------
  float mJetE;
  float mJetEta;
  float mJetPt;
  float mJetPhi;
  float mJetEMF; 
  float mLepPx;
  float mLepPy;
  float mLepPz;
  bool  mAddLepToJet;
  bool  mIsJetEset;
  bool  mIsJetPtset;
  bool  mIsJetPhiset;
  bool  mIsJetEtaset;
  bool  mIsJetEMFset; 
  bool  mIsLepPxset;
  bool  mIsLepPyset;
  bool  mIsLepPzset;
  SimpleJetCorrectionUncertainty_x* mUncertainty;
};

#endif

