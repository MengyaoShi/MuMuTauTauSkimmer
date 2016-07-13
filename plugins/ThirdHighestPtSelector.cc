// -*- C++ -*-
//
// Package:    temp/ThirdHighestPtSelector
// Class:      ThirdHighestPtSelector
// 
/**\class ThirdHighestPtSelector ThirdHighestPtSelector.cc temp/ThirdHighestPtSelector/plugins/ThirdHighestPtSelector.cc

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

#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "Tools/Common/interface/GenTauDecayID.h"
#include "Tools/Common/interface/Common.h"

//
//
// class declaration
//

class ThirdHighestPtSelector : public edm::EDFilter {
   public:
      explicit ThirdHighestPtSelector(const edm::ParameterSet&);
      ~ThirdHighestPtSelector();

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
      edm::EDGetTokenT<reco::GenParticleCollection>  genParticleTag_;
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
ThirdHighestPtSelector::ThirdHighestPtSelector(const edm::ParameterSet& iConfig):
  muonTag_(consumes<reco::MuonRefVector>(iConfig.getParameter<edm::InputTag>("muonTag"))),
  genParticleTag_(consumes<reco::GenParticleCollection>(iConfig.getParameter<edm::InputTag>("genParticleTag")))
{
   //now do what ever initialization is needed
   produces<reco::MuonRefVector>();
}


ThirdHighestPtSelector::~ThirdHighestPtSelector()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
ThirdHighestPtSelector::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   bool LargerThan0=false;
   using namespace edm;

   edm::Handle<reco::MuonRefVector> pMuons;
   iEvent.getByToken(muonTag_, pMuons); 

  edm::Handle<reco::GenParticleCollection> pGenParticles;
  iEvent.getByToken(genParticleTag_, pGenParticles);

  std::vector<reco::GenParticle*> genObjPtrs;
  for (typename reco::GenParticleCollection::const_iterator iGenObj = pGenParticles->begin();
       iGenObj != pGenParticles->end(); ++iGenObj) {
    genObjPtrs.push_back(const_cast<reco::GenParticle*>(&(*iGenObj)));
  }

   if((pMuons->size())<=2)
   {
     LargerThan0=false;
   }
   else
   {
     double max=0.0;
     double secondMax=0.0;
     double thirdMax=0.0;
     reco::MuonRef maxMuon;
     reco::MuonRef secondMaxMuon;
     reco::MuonRef thirdMaxMuon;
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
    
     int nearestGenObjKey=-1;
     const reco::GenParticle* nearestGenObj=
       Common::nearestObject(maxMuon, genObjPtrs, nearestGenObjKey);
       if(abs(nearestGenObj->motherRef()->pdgId())==13)
           nearestGenObj=(&(*(nearestGenObj->motherRef())));
     std::cout<< " maxMuon's Nearest Gen Muon's mother=="<<nearestGenObj->motherRef()->pdgId()<<std::endl;
 
     for(reco::MuonRefVector::const_iterator iMuon=pMuons->begin();
         iMuon!=pMuons->end();++iMuon)
     {
       if(((*iMuon)->pt())< max &&(((*iMuon)->pt())>secondMax))
       {
         secondMax=(*iMuon)->pt();
         secondMaxMuon=(*iMuon);
       }
       else
         continue;
     }

     int nearestGenObjKey1=-1;
     const reco::GenParticle* nearestGenObj1=
       Common::nearestObject(secondMaxMuon, genObjPtrs, nearestGenObjKey1);
       if(abs(nearestGenObj1->motherRef()->pdgId())==13)
           nearestGenObj1=(&(*(nearestGenObj1->motherRef())));
     std::cout<< " secondMaxMuon's Nearest Gen Muon's mother=="<<nearestGenObj1->motherRef()->pdgId()<<std::endl;

     std::auto_ptr<reco::MuonRefVector> muonColl(new reco::MuonRefVector);
     for(reco::MuonRefVector::const_iterator iMuon=pMuons->begin();
         iMuon!=pMuons->end();++iMuon)
     {
       if(((*iMuon)->pt()< (maxMuon->pt()))&& ((*iMuon)->pt()<(secondMaxMuon->pt()))&&((*iMuon)->pt()>thirdMax))
       {
	thirdMax=(*iMuon)->pt();
        thirdMaxMuon=(*iMuon);
        LargerThan0=true;
       }
       else
          continue;
     }
     muonColl->push_back(thirdMaxMuon);
     iEvent.put(muonColl);
   }

   return LargerThan0;
}

// ------------ method called once each job just before starting event loop  ------------
void 
ThirdHighestPtSelector::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
ThirdHighestPtSelector::endJob() {
}

// ------------ method called when starting to processes a run  ------------
/*
void
ThirdHighestPtSelector::beginRun(edm::Run const&, edm::EventSetup const&)
{ 
}
*/
 
// ------------ method called when ending the processing of a run  ------------
/*
void
ThirdHighestPtSelector::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when starting to processes a luminosity block  ------------
/*
void
ThirdHighestPtSelector::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
ThirdHighestPtSelector::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ThirdHighestPtSelector::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(ThirdHighestPtSelector);
