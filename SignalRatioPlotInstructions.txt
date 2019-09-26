 # Creating a signal ratio plot (Thanks to Lorenzo Viliani for the help)
 
-> Get "combine": 
#More info in: https://github.com/nucleosynthesis/HiggsAnalysis-CombinedLimit/wiki/gettingstarted
#Setting up the environment (once):
cd ~
mkdir combine
cd combine
export SCRAM_ARCH=slc6_amd64_gcc530
cmsrel CMSSW_8_1_0
cd CMSSW_8_1_0/src
cmsenv
git clone https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit

#Update to a reccomended tag - currently the reccomended tag is v7.0.12 :
cd $CMSSW_BASE/src/HiggsAnalysis/CombinedLimit
git fetch origin
git checkout v7.0.12 
scramv1 b clean #Always make a clean build
scramv1 b -j 10

-> Run a test with the HIG-16-042 results:

cd; mkdir test; cd test
cp /afs/cern.ch/user/l/lviliani/public/Full2016.txt . #Datacards with the HIG-16-042 data
wget https://raw.githubusercontent.com/latinos/PlotsConfigurations/master/Configurations/ggH/Full2016/scripts/CMS_lumi.py #Neccesary script 
wget https://raw.githubusercontent.com/latinos/PlotsConfigurations/master/Configurations/ggH/Full2016/scripts/tdrstyle.py #Neccesary script
wget https://raw.githubusercontent.com/latinos/PlotsConfigurations/master/Configurations/ggH/Full2016/scripts/multiSignalStrengthPlot.py #Main scripts
wget https://raw.githubusercontent.com/latinos/PlotsConfigurations/master/Configurations/ggH/Full2016/plot.py #plot.py file with the variables
emacs -nw plot.py  # Remove or comment all the "groupPlot" lines
python multiSignalStrengthPlot.py Full2016.txt --plotFile=plot.py #run the test

-> Run a test with some VH2j results:
Get the datacards and plot File from 
python multiSignalStrengthPlot.py datacard_1.txt --plotFile=plot_VH2j_noGroup.py

-> Create your own datacards:

CMS_9_4_9/src
cmsenv
cd $CMSSW_BASE/src/PlotsConfigurations/Configurations/WW/Full2017
emacs -nw structure.py
C-x 2 #open another emacs window
C-x C-f samples.py #Open samples.py in the second window
#Now make sure all the variables from samples.py are in structure.py. Add to structure.py any variables that are missing (ggH_hww, ZH_hww, fake....)
mkDatacards.py --pycfg=configuration.py --inputFile=rootFile/plots_WW.root 
#Your datacards will be created in Full2017/datacards/[channels]/[variables]

-> Copy the scripts to your workspace and run the script

mkdir scripts
cp $CMSSW_BASE/src/PlotsConfigurations/Configurations/ggH/Full2017/plotScripts/CMS_lumi.py   scripts/
cp $CMSSW_BASE/src/PlotsConfigurations/Configurations/ggH/Full2017/plotScripts/tdrstyle.py   scripts/
cp $CMSSW_BASE/src/PlotsConfigurations/Configurations/ggH/Full2017/plotScripts/multiSignalStrengthPlot.py    scripts/
cp plot.py scripts/
cp datacards/[SignalRegion]/events/datacard.txt scripts/
cd scripts/
emacs -nw plot.py #Remove/comment all the variables that you dont want to plot (leave ZH_hww, ggH_hww, etc). Also remove all the "groupPlot" lines.
python multiSignalStrengthPlot.py datacard.txt --plotFile=plot.py #Run the script

#If you get the error:
File "multiSignalStrengthPlot.py", line 115, in <module>
    overallSignalRate[c][s] +=  DC.exp[channel][s]
KeyError: 'H_htt'

#open the script
emacs -nw multiSignalStrengthPlot.py
#go to line 49 and un-comment it:
if "H_htt"  in signals: signals.remove("H_htt") 

#If you get a similar error with a signal you dont have, like bbH_hww:
#go to lines 61-70 and comment the line with that variable:
 ###  overallSignalRate[c]['bbH_hww'] = 0.

#If you get the "UNKNOWN CHANNEL" error go to lines 80-100 and change the name of the channel to the name youre using in the datacard, ie:
 ### elif 'vh' in channel: c='2-jet VH-tagged'  #old name "vh"
  elif 'VH_2j_emu' in channel: c='2-jet VH-tagged' #new name "VH_2j_emu"

#You can change the CMS_lumi in line 165
