import FWCore.ParameterSet.Config as cms
from subprocess import *
import FWCore.Utilities.FileUtils as FileUtils
mylist=FileUtils.loadListFromFile('/afs/cern.ch/user/m/mshi/CMSSW_7_6_3/src/GGHAA2Mu2TauAnalysis/testLowMassDY.txt')
process = cms.Process("testSKIM1")


process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True),
                SkipEvent = cms.untracked.vstring('ProductNotFound'))

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )
process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(*mylist))


#require event to fire IsoMu24_eta2p1
process.Mu1Mu2Analyzer=cms.EDAnalyzer(
        'Mu1Mu2Analyzer',
        genParticleTag = cms.InputTag('genParticles'),
        Mu1Mu2=cms.InputTag('Isolation'),
        particleFlow=cms.InputTag('particleFlow')
)
process.MuMuTauTauRecoAnalyzer=cms.EDAnalyzer(
	'MuMuTauTauRecoAnalyzer',
          tauTag=cms.InputTag('muHadTauSelector','','SKIM'),
 jetMuonMapTag=cms.InputTag('CleanJets','muonValMap','SKIM'),
        Mu1Mu2= cms.InputTag('Isolation'),
        genParticleTag=cms.InputTag('genParticles'),
   	muHadMassBins=cms.vdouble(0.0, 1.0, 2.0,3.0, 4.0, 12.0),
  	FourBInvMassBins=cms.vdouble(0.0, 200.0,400.0,600.0, 800.0, 1000.0) 
)
process.noSelectionSequence = cms.Sequence(
  process.Mu1Mu2Analyzer*
  process.MuMuTauTauRecoAnalyzer
)

process.TFileService = cms.Service("TFileService",
    fileName =  cms.string('testLowMassDY.root')
)
#no selection path
process.p = cms.Path(process.noSelectionSequence)
