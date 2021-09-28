#ifndef DAS_OBJECTS
#define DAS_OBJECTS

#include <vector>
#include "Ntupliser/DiMuons/interface/NTupleBranches.h"

struct DasMuon {
    int   charge = -999;
    float pt     = -999;
    float eta    = -999;
    float phi    = -999;
    float lepMVA = -999;
};
struct DasEle {
    int   charge = -999;
    float pt     = -999;
    float eta    = -999;
    float phi    = -999;
    float lepMVA = -999;
};
struct DasJet {
    float pt      = -999;
    float eta     = -999;
    float phi     = -999;
    float DeepCSV = -999;
};
struct DasMET {
   float pt  = -999;
   float phi = -999;

   void init();
};

typedef std::vector<DasMuon> DasMuons;
typedef std::vector<DasEle>  DasEles;
typedef std::vector<DasJet>  DasJets;

DasMuon ConvertMuontoDas(const MuonInfo & muon);
DasEle  ConvertEletoDas (const EleInfo  & ele);
DasJet  ConvertJettoDas (const JetInfo  & jet);
DasMET  ConvertMETtoDas (const MetInfo  & MET);

#endif
