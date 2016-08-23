#!/bin/bash

export SCRAM_ARCH=slc6_amd64_gcc493
cd /afs/cern.ch/user/m/mshi/CMSSW_7_6_3/src
eval `scramv1 runtime -sh`
cd -
source /afs/cern.ch/cms/caf/setup.sh
cp /afs/cern.ch/user/m/mshi/CMSSW_7_6_3/src/GGHAA2Mu2TauAnalysis/MuMuTauTauSkimmer/test/BSUB/DIRNAME/GENERATOR.py .
cmsRun GENERATOR.py
cmsStage -f DIRNAME.root H750a09_RECOAnalyzer.root /store/user/mshi/DIRNAME
rm DIRNAME.root GENERATOR.py H750a09_RECOAnalyzer.root 


exit 0

