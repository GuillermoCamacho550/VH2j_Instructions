
Connect to lxplus
ssh -Y lxplus.cern.ch -o ServerAliveInterval=240
#If you're using Putty (Windows)
Host name: [YourUsername]@lxplus.cern.ch

Set the Bash shell
bash -l


Setup a CMSSW release
export SCRAM_ARCH=slc6_amd64_gcc630
cmsrel CMSSW_9_4_9
cd CMSSW_9_4_9/src
cmsenv


Connect to GitHub with SSH
You need to generate an SSH key,
https://help.github.com/en/articles/connecting-to-github-with-ssh
https://github.com/settings/keys

If the key is not permanently added, the following should be done in every new login.

eval "$(ssh-agent -s)"
ssh-add ~/.ssh/id_rsa
ssh-add -l


Get the latino main code
cd $CMSSW_BASE/src/
git clone --branch 13TeV git@github.com:latinos/setup.git LatinosSetup
source LatinosSetup/SetupShapeOnly.sh


Create your user configuration
cd $CMSSW_BASE/src/LatinoAnalysis/Tools/python
cp userConfig_TEMPLATE.py userConfig.py

emacs -nw userConfig.py
# Replace the following line
baseDir = '/afs/cern.ch/user/x/xjanssen/cms/HWW2015/'
# with your own directory
baseDir = '/afs/cern.ch/user/[YourInitial]/[YourUsername]/cms/HWW2015/' 
# Add the following line
batchType = 'condor'
# Save and exit
C-x C-s
C-x C-c

Get PlotsConfigurations
cd $CMSSW_BASE/src/
git clone git@github.com:latinos/PlotsConfigurations.git


Compile
cd $CMSSW_BASE/src/
cmsenv
scram b -j 10


Produce a valid VOMS proxy
voms-proxy-init -voms cms -rfc --valid 168:0
cmsenv


Produce histograms
See https://github.com/latinos/PlotsConfigurations/tree/master/Configurations/WW/Full2017

cd $CMSSW_BASE/src/PlotsConfigurations/Configurations/WW/Full2017

mkShapesMulti.py --pycfg=configuration.py --inputDir=/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/ --doBatch=True --batchQueue=workday --treeName=Events --batchSplit=Samples,Files


Check job status
condor_q

And wait until all jobs have finished :)


Group (hadd) histograms
mkShapesMulti.py --pycfg=configuration.py --inputDir=/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/ --doHadd=True --batchSplit=Samples,Files --doNotCleanup


Draw distributions
mkPlot.py --pycfg=configuration.py --inputFile=rootFile/plots_WW.root --minLogC=0.01 --minLogCratio=0.01 --maxLogC=1000 --maxLogCratio=1000 --showIntegralLegend=1

#Error: attempt to access a null pointer
#Make sure the samples.py and plot.py files have the same contents: remove from plot.py the proccesses that are not in samples.py
#Example: 
-> shapeName= SS_0j_em/mpmet/histo_ggZH_htt
--> <ROOT.TObject object at 0x(nil)>
[...]
ReferenceError: attempt to access a null pointer
#this error means ggZH_htt is in plot.py but not in sample.py
emacs -nw plot.py  #Open plot.py in emacs
C-s "ggZH_htt"  #Control+S, search ggZH_htt in plot.py
Delete 'ggZH_htt' from the 'samples' bracket in group plot
c-s "ggZH_htt"  #Search it again
Then delete the whole "plot['ggZH_htt']={ ... }" block, including the brackets and content inside
C-x C-s  #Save the changes
C-x C-c  #exit 
#Now run mkPlot.py again. If the same error appears with a different shapeName, repeats these steps until mkPlot.py runs (12-13 times)

 # Copy or link to the www area

cp plotWW/*.png  /eos/user/[YourInitial]/[YourUser]/www/[PlotDirectory]  #Copy the png plots
cp plotWW/*.root  /eos/user/[YourInitial]/[YourUser]/www/[PlotDirectory2]  #Copy the ROOT files of the plots
[PlotDirectory] is the path of the sub-directory where you want to store the plots

 # Create a www area (EOS Website)

Follow the instructions at https://cernbox-manual.web.cern.ch/cernbox-manual/en/web/personal_website_content.html
Once it is created, you can manage your site. Go to https://webservices.web.cern.ch/webservices/Services/ManageSite/ and click on your website on the left. (http://cern.ch/[YourUser])
In order to allow access to your website, add an .htaccess file to your www directory. 
You can get it from "http://cern.ch/user/c/cprieels/public/ForJonatan" or create one yourself. I has to say "Options +Indexes"
To improve the usability of the website, add an index.php file to every directory and subdirectory.
You can get it from "https://github.com/piedraj/AnalysisCMS/blob/master/index.php"
If you have followed these steps and the website is unavailable (error 403) you can open a ticket with support at "https://cern.service-now.com/service-portal/service-element.do?name=eos-service"


 # Obtaining a VO CMS certificate

The full instructions are available at "https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideLcgAccess" under "How to register in the CMS VO"
In order to obtain a VOMS certificate first you must obtain a personal certificate (step 1,2), which consists of a private and a public key (PEM files)
Follow the instructions at: "https://twiki.cern.ch/twiki/bin/view/CMSPublic/PersonalCertificate" to install the certificate in your browser
You will need to export the certificate into a pair of PEM files, follow the instructions at: "https://ca.cern.ch/ca/Help/?kbid=024010" to export and install the certificate
Once you have your personal certificate, you can proceed with step 3. Go to "https://voms2.cern.ch:8443/voms/cms/register" and submit a registration
You will receive an email with a link you have to click to confirm your registration
Then you need to wait for an admin to finally confirm your registration (you will receive a notification email)
For a FAQ about VOMS go here: "https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideVomsFAQ"
You can verify the expiration date of your certificate with: "openssl x509  -subject -dates -noout  -in $HOME/.globus/usercert.pem"
To check if you can generate proxies, run "grid-proxy-init -debug -verify"
to check if youre a member of the CMS VO, run "voms-proxy-init -voms cms"
