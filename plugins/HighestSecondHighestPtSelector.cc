// -*- C++ -*-
//
// Package:    temp/HighestSecondHighestPtSelector
// Class:      HighestSecondHighestPtSelector
// 
/**\class HighestSecondHighestPtSelector HighestSecondHighestPtSelector.cc temp/HighestSecondHighestPtSelector/plugins/HighestSecondHighestPtSelector.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Mengyao Shi
//         Created:  Wed, 25 Nov 2015 16:25:51 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
//
//
// class declaration
//

class HighestSecondHighestPtSelector : public edm::EDFilter {
   public:
      explicit HighestSecondHighestPtSelector(const edm::ParameterSet&);
      ~HighestSecondHighestPtSelector();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() override;
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      
      //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

      // ----------member data ---------------------------
edm::EDGetTokenT<reco::MuonRefVector> muonTag_; 
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
HighestSecondHighestPtSelector::HighestSecondHighestPtSelector(const edm::ParameterSet& iConfig):
  muonTag_(consumes<reco::MuonRefVector>(iConfig.getParameter<edm::InputTag>("muonTag")))
{
   //now do what ever initialization is needed
   produces<reco::MuonRefVector>();
}


HighestSecondHighestPtSelector::~HighestSecondHighestPtSelector()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
HighestSecondHighestPtSelector::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   bool LargerThan0=true;
   using namespace edm;

   edm::Handle<reco::MuonRefVector> pMuons;
   iEvent.getByToken(muonTag_, pMuons); 
   if((pMuons->size())<=1)
   {
     LargerThan0=false;
   }
   else
   {
     double max=0.0;
     double secondMax=0.0;
     reco::MuonRef maxMuon;
     reco::MuonRef secondMaxMuon;
     for(reco::MuonRefVector::const_iterator iMuon=pMuons->begin();
         iMuon!=pMuons->end();++iMuon)
     {
       if(((*iMuon)->pt())> max)
       {
         max=(*iMuon)->pt();
         maxMuon=(*iMuon);
       }
       else
         continue;
     }
    

     for(reco::MuonRefVector::const_iterator iMuon=pMuons->begin();
         iMuon!=pMuons->end();++iMuon)
     {
       if(((*iMuon)->pt()< (maxMuon->pt()))&&((*iMuon)->pt()>secondMax )&&((*iMuon)->pdgId()==(-1)*((maxMuon)->pdgId())))
       {
         secondMax=(*iMuon)->pt();
         secondMaxMuon=(*iMuon);
       }
       else 
  	  continue;
     }
     if(secondMaxMuon.isNull())
     {LargerThan0=0; return LargerThan0;
     }

     std::auto_ptr<reco::MuonRefVector> muonColl(new reco::MuonRefVector);
     muonColl->push_back(maxMuon);
     muonColl->push_back(secondMaxMuon);
     iEvent.put(muonColl);
   }

   return LargerThan0;
}

// ------------ method called once each job just before starting event loop  ------------
void 
HighestSecondHighestPtSelector::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HighestSecondHighestPtSelector::endJob() {
}

// ------------ method called when starting to processes a run  ------------
/*
void
HighestSecondHighestPtSelector::beginRun(edm::Run const&, edm::EventSetup const&)
{ 
}
*/
 
// ------------ method called when ending the processing of a run  ------------
/*
void
HighestSecondHighestPtSelector::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when starting to processes a luminosity block  ------------
/*
void
HighestSecondHighestPtSelector::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
HighestSecondHighestPtSelector::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
HighestSecondHighestPtSelector::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(HighestSecondHighestPtSelector);
