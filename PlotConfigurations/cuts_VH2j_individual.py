 # cuts for VH2j analysis - G Camacho
 # Sequential cuts for efficiency analysis

_tmp = [ 
     'Lepton_pdgId[0]*Lepton_pdgId[1] == -11*13', #electron & muon with op. charge
       ]

supercut = ' && '.join(_tmp)

def addcut(name, exprs):
    cuts[name] = ' && '.join(exprs)


_tmp = [
       'Lepton_pdgId[0]*Lepton_pdgId[1] == -11*13', #electron & muon with op. charge
       ]

addcut('VH_2j_emu_0', _tmp)

_tmp = [
       'Lepton_pt[0]>25.',
       'Lepton_pt[1]>10.',
       '(abs(Lepton_pdgId[1]) == 13 || Lepton_pt[1]>13.)', #if its an electron, pt2>13
       '(nLepton>=2 && Alt$(Lepton_pt[2],0)<10.)',    
       ]

addcut('VH_2j_emu_1', _tmp)


_tmp = [
      'mll>12.',
      'ptll>30',

      ]

addcut('VH_2j_emu_2', _tmp)

_tmp = [
      'PuppiMET_pt > 20.',

      ]

addcut('VH_2j_emu_3', _tmp)

_tmp = [
      'Sum$(CleanJet_pt>30.)>=2', #At least 2 jets
      ]

addcut('VH_2j_emu_4', _tmp)

_tmp = [
      'abs(CleanJet_eta[0])<2.5',
      'abs(CleanJet_eta[1])<2.5',
      ]

addcut('VH_2j_emu_5', _tmp)

_tmp = [     
      'detajj<3.5',# delta eta 2 jets 
      ]

addcut('VH_2j_emu_6', _tmp)

_tmp = [
      'mth>60.',
      'mth<125.',

      ]

addcut('VH_2j_emu_7', _tmp)

_tmp = [
     'drll<2.',   #DeltaRll<2

      ]

addcut('VH_2j_emu_8', _tmp)

_tmp = [
      '(Sum$(CleanJet_pt > 20. && Jet_btagDeepB[CleanJet_jetIdx] > 0.1522) == 0)', #no bjets with pt>20

      ]

addcut('VH_2j_emu_9', _tmp)

_tmp = [
      'mjj>65.',
      'mjj<105.',

      ]

addcut('VH_2j_emu_10', _tmp)

_tmp = [
      'Jet_qgl[CleanJet_jetIdx[0]]>0.4', #QGL cuts 31/5/19
      'Jet_qgl[CleanJet_jetIdx[1]]>0.3',

      ]

addcut('VH_2j_emu_11', _tmp)

