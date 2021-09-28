#include "H2MuAnalyzer/MakeHistos/interface/DASTutorialObjects.h"

DasMuon ConvertMuontoDas(const MuonInfo & muon) {
    DasMuon das_muon;
    das_muon.charge = muon.charge;
    das_muon.pt     = muon.pt_Roch;
    das_muon.eta    = muon.eta;
    das_muon.phi    = muon.phi;
    das_muon.lepMVA = muon.lepMVA;
    return das_muon;
}


DasEle  ConvertEletoDas (const EleInfo  & ele) {
    DasEle das_ele;
    das_ele.charge = ele.charge;
    das_ele.pt     = ele.pt;
    das_ele.eta    = ele.eta;
    das_ele.phi    = ele.phi;
    das_ele.lepMVA = ele.lepMVA;
    return das_ele;
}


DasJet  ConvertJettoDas (const JetInfo  & jet) {
    DasJet das_jet;
    das_jet.pt      = jet.pt;
    das_jet.eta     = jet.eta;
    das_jet.phi     = jet.phi;
    das_jet.DeepCSV = jet.deepCSV;
    return das_jet;
}


DasMET  ConvertMETtoDas (const MetInfo  & MET) {
    DasMET das_met;
    das_met.pt  = MET.pt;
    das_met.phi = MET.phi;
    return das_met;
}

void DasMET::init() {
    pt  = -999;
    phi = -999;
}
