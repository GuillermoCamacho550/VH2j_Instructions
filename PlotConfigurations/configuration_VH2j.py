# example of configuration file
treeName= 'Events'

tag = 'WW'


# used by mkShape to define output directory for root files
outputDir = 'rootFile'

# file with TTree aliases
aliasesFile = 'aliases.py'

# file with list of variables
variablesFile = 'variables_VH2j.py'

# file with list of cuts
cutsFile = 'cuts_VH2j_individual.py' 

# file with list of samples
samplesFile = 'samples_toponly.py' 

# file with list of samples
plotFile = 'plot_VH2j_toponly.py' 



# luminosity to normalize to (in 1/fb)
lumi = 41.5

# used by mkPlot to define output directory for plots
# different from "outputDir" to do things more tidy
# outputDirPlots = '~/www/plotCR'
outputDirPlots = 'plotWW'


# used by mkDatacards to define output directory for datacards
outputDirDatacard = 'datacards'


# structure file for datacard
structureFile = 'structure_VH2j.py'


# nuisances file for mkDatacards and for mkShape
nuisancesFile = 'nuisances_base.py'


