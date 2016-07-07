// -*- C++ -*-
//
// Package:    temp/Mu3Selector
// Class:      Mu3Selector
// 
/**\class Mu3Selector Mu3Selector.cc temp/Mu3Selector/plugins/Mu3Selector.cc

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

class Mu3Selector : public edm::EDFilter {
   public:
      explicit Mu3Selector(const edm::ParameterSet&);
      ~Mu3Selector();

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
      double Cut_;
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
Mu3Selector::Mu3Selector(const edm::ParameterSet& iConfig):
  muonTag_(consumes<reco::MuonRefVector>(iConfig.getParameter<edm::InputTag>("muonTag"))),
  genParticleTag_(consumes<reco::GenParticleCollection>(iConfig.getParameter<edm::InputTag>("genParticleTag"))),
  Cut_(iConfig.getParameter<double>("dRCut"))
{
   //now do what ever initialization is needed
   produces<reco::MuonRefVector>();
}


Mu3Selector::~Mu3Selector()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
Mu3Selector::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
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

   if((pMuons->size())<=1)
   {
     LargerThan0=false;
   }
   else
   {
     double max=0.0;
     reco::MuonRef maxMuon;
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
 
     std::auto_ptr<reco::MuonRefVector> muonColl(new reco::MuonRefVector);
     for(reco::MuonRefVector::const_iterator iMuon=pMuons->begin();
         iMuon!=pMuons->end();++iMuon)
     {
       if(((*iMuon)->pt()< (maxMuon->pt()))&& (deltaR(**iMuon, *maxMuon)> Cut_))
       {
	muonColl->push_back(*iMuon);
        LargerThan0=true;
       }
       else
          continue;
     }

     iEvent.put(muonColl);
   }

   return LargerThan0;
}

// ------------ method called once each job just before starting event loop  ------------
void 
Mu3Selector::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
Mu3Selector::endJob() {
}

// ------------ method called when starting to processes a run  ------------
/*
void
Mu3Selector::beginRun(edm::Run const&, edm::EventSetup const&)
{ 
}
*/
 
// ------------ method called when ending the processing of a run  ------------
/*
void
Mu3Selector::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when starting to processes a luminosity block  ------------
/*
void
Mu3Selector::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
Mu3Selector::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
Mu3Selector::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(Mu3Selector);
