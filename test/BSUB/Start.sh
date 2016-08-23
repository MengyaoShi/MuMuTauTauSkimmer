#!/bin/bash
#parse arguments
if [ $# -ne 4 ]
    then
    echo "Usage: ./Start.sh cfg_name script_name dir_name queue"
    exit 0
fi

cfg_name=$1
script_name=$2
dir_name=$3
queue=$4

mkdir -p BSUB/$dir_name
cd BSUB/$dir_name

#make directory on EOS
EOS_dir_query=`cmsLs /store/user/mshi/${dir_name}`
EOS_dir_query=`echo $EOS_dir_query | grep "No such file or directory"`
if [ "EOS_dir_query" != "" ]
    then
    cmsMkdir /store/user/mshi/${dir_name}
fi

sed -e "s%DIRNAME%${dir_name}%g" -e ../../${cfg_name}.py >  ${cfg_name}_${dir_name}.py
sed -e "s%GENERATOR%${cfg_name}_${dir_name}%g" -e "s%DIRNAME%${dir_name}%g" ../../${script_name}.sh > ${script_name}_${dir_name}.sh
chmod u+x ${script_name}_${dir_name}.sh
bsub -q $queue -J ${cfg_name}_${dir_name} < ${script_name}_${dir_name}.sh

cd ../../
