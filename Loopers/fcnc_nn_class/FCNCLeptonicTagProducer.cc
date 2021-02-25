#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/FCNCLeptonicTag.h"
#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/DataFormats/interface/Muon.h"
#include "flashgg/DataFormats/interface/Met.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "DataFormats/TrackReco/interface/HitPattern.h"
#include "flashgg/Taggers/interface/LeptonSelection2018.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "DataFormats/Common/interface/RefToPtr.h"

#include "flashgg/Taggers/interface/BDT_resolvedTopTagger.h"
#include "flashgg/Taggers/interface/TTH_DNN_Helper.h"
#include "flashgg/Taggers/interface/TopRecoHelper.h"
#include "flashgg/Taggers/interface/ANN_LeptonicTopRecoHelper.h"

#include <vector>
#include <algorithm>
#include <string>
#include <utility>
#include "TLorentzVector.h"
#include "TMath.h"
#include "TMVA/Reader.h"


using namespace std;
using namespace edm;


namespace flashgg {
    class FCNCLeptonicTagProducer : public EDProducer
    {

    public:
        FCNCLeptonicTagProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;
        int  chooseCategory( float tthmvavalue , bool debug_ );
        int  chooseCategory( float mva1_value, float mva2_value);

        const  reco::GenParticle* motherID(const reco::GenParticle* gp);
        bool PassFrixione(Handle<View<reco::GenParticle> > genParticles, const reco::GenParticle* gp, int nBinsForFrix, double cone_frix);
        vector<int> IsPromptAfterOverlapRemove(Handle<View<reco::GenParticle> > genParticles, const edm::Ptr<reco::GenParticle> genPho);
        int  GenPhoIndex(Handle<View<reco::GenParticle> > genParticles, const flashgg::Photon* pho, int usedIndex);
        double NearestDr(Handle<View<reco::GenParticle> > genParticles, const reco::GenParticle* gp);

        struct Sorter {
            bool operator()( const std::pair<unsigned int, float>  pair1, const std::pair<unsigned int,float>  pair2 )
            {
                return ( pair1.second > pair2.second );
            };
        };
        

        std::vector<edm::EDGetTokenT<View<flashgg::Jet> > > tokenJets_;
        std::vector<std::vector<edm::EDGetTokenT<edm::View<flashgg::Jet>>>> jetTokens_;
        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        std::vector<edm::EDGetTokenT<edm::View<DiPhotonCandidate>>> diPhotonTokens_;
        std::vector<edm::EDGetTokenT<edm::View<DiPhotonMVAResult>>> mvaResultTokens_;
        //EDGetTokenT<View<Jet> > thejetToken_;
        std::vector<edm::InputTag> inputTagJets_;
        EDGetTokenT<View<Electron> > electronToken_;
        EDGetTokenT<View<flashgg::Muon> > muonToken_;
        EDGetTokenT<View<flashgg::Met> > METToken_;
        std::vector<edm::EDGetTokenT<edm::View<flashgg::Met>>> metTokens_;
        EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;
        EDGetTokenT<View<Photon> > photonToken_;
        EDGetTokenT<View<reco::Vertex> > vertexToken_;
        EDGetTokenT<View<reco::GenParticle> > genParticleToken_;
        string systLabel_;

        typedef std::vector<edm::Handle<edm::View<flashgg::Jet> > > JetCollectionVector;

        unique_ptr<TMVA::Reader> DiphotonMva_;
        FileInPath MVAweightfile_;

        FileInPath topTaggerXMLfile_;
        FileInPath fcncTaggerXMLfile_tt_;
        FileInPath fcncTaggerXMLfile_st_;

        unique_ptr<TMVA::Reader>TThMva_RunII_;
        FileInPath tthMVA_RunII_weightfile_;

        unique_ptr<TMVA::Reader> FCNCMva_;
        FileInPath fcncHutMVAWeightFile_;
        FileInPath fcncHctMVAWeightFile_;

        unique_ptr<TMVA::Reader> FCNC_BDTNRB_RunII_;
        FileInPath fcncHutBDTNRBWeightFile_;
        FileInPath fcncHctBDTNRBWeightFile_;

        unique_ptr<TMVA::Reader> FCNC_BDTSMH_RunII_;
        FileInPath fcncHutBDTSMHWeightFile_;
        FileInPath fcncHctBDTSMHWeightFile_;

        FileInPath tthVsttGGDNNfile_;

        //Thresholds

        double MuonEtaCut_;
        double MuonPtCut_;
        double MuonIsoCut_;
        double MuonPhotonDrCut_;
        int    MinNLep_;
        int    MaxNLep_;

        double ElePtCut_;
        std::vector<double> EleEtaCuts_;
        double ElePhotonDrCut_;
        double ElePhotonZMassCut_;
        double DeltaRTrkEle_;

        double LeptonsZMassCut_;

        double DiLeptonJetThreshold_;
        double DiLeptonbJetThreshold_;
        double DiLeptonMVAThreshold_;

        double leadPhoOverMassThreshold_;
        double subleadPhoOverMassThreshold_;

        vector<double> MVAThreshold_;
        vector<double> MVAThreshold_BDTNRB;
        vector<double> MVAThreshold_BDTSMH;    

        double deltaRJetLeadPhoThreshold_;
        double deltaRJetSubLeadPhoThreshold_;
        double jetsNumberThreshold_;
        double bjetsNumberThreshold_;
        double jetPtThreshold_;
        double jetEtaThreshold_;
        double deltaRJetLepton_;
        double leadingJetPtThreshold_;
        vector<double> bDiscriminator_;
        string bTag_;
        double PhoMVAThreshold_;

        bool UseCutBasedDiphoId_;
        bool debug_;
        bool SplitDiLeptEv_;
        vector<double> CutBasedDiphoId_;

        float leadeta_;
        float subleadeta_;
        float leadptom_;
        float subleadptom_;
        float leadIDMVA_;
        float subleadIDMVA_;
        float deltaphi_;
        float leadPSV_;
        float subleadPSV_;
        float nJets_;
        float nJets_bTagMedium_;
        float jet_pt1_;
        float jet_pt2_;
        float jet_pt3_;
        float jet_eta1_;
        float jet_eta2_;
        float jet_eta3_;
        float bTag1_;
        float bTag2_;
        float MetPt_;
        float MetPhi_;
        float lepton_leadPt_;
        float lepton_leadEta_;

        float lepton_leadPhi_;
     
        float minPhoID_;
        float maxPhoID_;
        float maxBTagVal_noBB_;
        float secondMaxBTagVal_noBB_;

        float diPhoY_;
        float diPhoPtoM_;
        float diPhoCosPhi_;
        float diPhoDeltaR_;

        float btag_1_;
        float btag_noBB_1_;
        float jetPt_1_;
        float jetEta_1_;
        float jetPhi_1_;
        float btag_2_;
        float btag_noBB_2_;
        float jetPt_2_;
        float jetEta_2_;
        float jetPhi_2_;
        float btag_3_;
        float btag_noBB_3_;
        float jetPt_3_;
        float jetEta_3_;
        float jetPhi_3_;
        float btag_4_;
        float btag_noBB_4_;
        float jetPt_4_;
        float jetEta_4_;
        float jetPhi_4_;

        float ht_;
        float helicity_angle_;
        float top_tag_score_;
        float fcnc_tag_score_tt_;
        float fcnc_tag_score_st_;

        float lepton_nTight_;

        float dnn_score_0_;
       
        float tthMvaVal_RunII_;

        float fcncMvaVal_;
        float fcncMvaVal_NRB_;
        float fcncMvaVal_SMH_;

        //------------------------------//
        float chi2_neutrino_pz_;
        float chi2_tbw_mass_;
        float chi2_tbw_pt_;
        float chi2_tbw_eta_;
        float chi2_tbw_deltaR_dipho_;
        float chi2_qjet_pt_;
        float chi2_qjet_eta_;
        float chi2_qjet_btag_;
        float chi2_qjet_deltaR_dipho_;
        float chi2_tqh_ptOverM_;
        float chi2_tqh_eta_;
        float chi2_tqh_deltaR_tbw_;
        float chi2_tqh_deltaR_dipho_;
        //------------------------------//
        
        BDT_resolvedTopTagger *topTagger;
        ANN_LeptonicTopTagger *fcncTagger;
        TTH_DNN_Helper* dnn;

        bool modifySystematicsWorkflow;
        std::vector<std::string> systematicsLabels;

        std::string inputDiPhotonName_;
        std::vector<std::string> inputDiPhotonSuffixes_;

        std::string inputMVAResultName_;

        std::string nominalJetsName_;
        std::string systematicsJetsName_;
        std::vector<std::string> inputJetsSuffixes_;
        unsigned int inputJetsCollSize_;

        std::string nominalMetName_;
        std::string systematicsMetName_;
        std::vector<std::string> inputMetSuffixes_;

        bool useLargeMVAs;

        std::string coupling_;
        int splitFactor_;
    };

    const reco::GenParticle* FCNCLeptonicTagProducer::motherID(const reco::GenParticle* gp)
    {
        const reco::GenParticle* mom_lead = gp;
        //cout << "in id: " << gp->pdgId() << endl;
        while( mom_lead->numberOfMothers() > 0 ) {
             for(uint j = 0; j < mom_lead->numberOfMothers(); ++j) {
                 mom_lead = dynamic_cast<const reco::GenParticle*>( mom_lead->mother(j) );
                 //cout << j << "th id: " << mom_lead->pdgId() << ", gpid: " << gp->pdgId() << endl;
                 if( mom_lead->pdgId() != gp->pdgId() )
                     return mom_lead; 
                     //break;
                }
             }

        return mom_lead;
    }

    bool FCNCLeptonicTagProducer::PassFrixione(Handle<View<reco::GenParticle> > genParticles, const reco::GenParticle* gp, int nBinsForFrix, double cone_frix)
    {

        bool passFrix = true;

        double pho_et = gp->p4().Et();
        double pho_eta = gp->p4().eta();
        double pho_phi = gp->p4().phi();
        const double initConeFrix = 1E-10;
        double ets[nBinsForFrix] = {};
   
        for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ )
           {
           int status = genParticles->ptrAt( genLoop )->status();
           if (!(status >= 21 && status <=24)) continue;
           int pdgid = genParticles->ptrAt( genLoop )->pdgId();
           if (!(abs(pdgid) == 11 || abs(pdgid) == 13 || abs(pdgid) == 15 || abs(pdgid) < 10 || abs(pdgid) == 21) ) continue; 
           double et = genParticles->ptrAt( genLoop )->p4().Et();
           double eta = genParticles->ptrAt( genLoop )->p4().eta();
           double phi = genParticles->ptrAt( genLoop )->p4().phi();
           double dR = deltaR(pho_eta,pho_phi,eta,phi); 
           for (int j = 0; j < nBinsForFrix; j++) { 
               double cone_var = (initConeFrix + j*cone_frix/nBinsForFrix);
               if (dR < cone_var && cone_var < cone_frix) ets[j] += et/(1-cos(cone_var) )*(1-cos(cone_frix) );
               }
           }

           for (int k = 0; k < nBinsForFrix; k++) {
               if (ets[k] > pho_et) {
                  passFrix = false; break;
                  }
           }

        return passFrix;
    }


    double FCNCLeptonicTagProducer::NearestDr(Handle<View<reco::GenParticle> > genParticles, const reco::GenParticle* gp)
    {

        double nearDr = 999;

        double pho_eta = gp->p4().eta();
        double pho_phi = gp->p4().phi();

        for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ )
           {
           int status = genParticles->ptrAt( genLoop )->status();
           if (!(status >= 21 && status <=24)) continue;
           int pdgid = genParticles->ptrAt( genLoop )->pdgId();
           if (!(abs(pdgid) == 11 || abs(pdgid) == 13 || abs(pdgid) == 15 || abs(pdgid) < 10 || abs(pdgid) == 21) ) continue;
           double eta = genParticles->ptrAt( genLoop )->p4().eta();
           double phi = genParticles->ptrAt( genLoop )->p4().phi();
           double dR = deltaR(pho_eta,pho_phi,eta,phi);
           if (dR < nearDr) {
              nearDr = dR;
              }
           }

        return nearDr;
    }


    vector<int> FCNCLeptonicTagProducer::IsPromptAfterOverlapRemove(Handle<View<reco::GenParticle> > genParticles, const edm::Ptr<reco::GenParticle> genPho)
    {

        vector<int> flags;
        bool isPromptAfterOverlapRemove = false;

        int simplemotherID = -1;
        int simplemotherStatus = -1;
        if (genPho->numberOfMothers() > 0) {
            simplemotherID = genPho->mother(0)->pdgId();
            simplemotherStatus = genPho->mother(0)->status();
        }
        const reco::GenParticle* mom = motherID(&(*genPho));
        const reco::GenParticle* mommom = motherID(mom);
        bool isMad = false;
        if (simplemotherID == 22 && simplemotherStatus >= 21 && simplemotherStatus <= 24) isMad = true;
        bool isFromWb = false;
        if (abs(mom->pdgId()) == 24 || abs(mommom->pdgId()) == 24 || (abs(mom->pdgId()) == 5 && abs(mommom->pdgId()) == 6) ) isFromWb = true;
        bool isFromQuark = false;
        if (abs(mom->pdgId()) == 21 || abs(mommom->pdgId()) == 21 || (abs(mom->pdgId()) <= 6 && abs(mommom->pdgId()) != 6 && abs(mommom->pdgId()) != 24 ) ) isFromQuark = true;
        bool isFromProton = false;
        if (abs(mom->pdgId()) == 2212) isFromProton = true;
        bool failFrix = !PassFrixione(genParticles, &(*genPho), 100, 0.05);
        bool isPythia = false;
        if (!isMad && genPho->isPromptFinalState() && ( isFromWb || (isFromQuark && failFrix) || isFromProton) ) isPythia = true;
        if (isMad || isPythia) isPromptAfterOverlapRemove = true;
        flags.push_back(isPromptAfterOverlapRemove ? 1 : 0);
        flags.push_back(isMad ? 1 : 0);
        flags.push_back(isPythia ? 1 : 0);
        flags.push_back((!failFrix) ? 1 : 0);
        flags.push_back(simplemotherID);
        flags.push_back(simplemotherStatus);
        flags.push_back(abs(mom->pdgId()));
        flags.push_back(abs(mommom->pdgId()));
        return flags;

    }


    int FCNCLeptonicTagProducer::GenPhoIndex(Handle<View<reco::GenParticle> > genParticles, const flashgg::Photon* pho, int usedIndex)
    {
        double maxDr = 0.2;
        double ptDiffMax = 99e15;
        int index = -1;
                
        for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
            int pdgid = genParticles->ptrAt( genLoop )->pdgId();
            if (pho->genMatchType() != 1) continue;
            if (int(genLoop) == usedIndex) continue;
            if (abs(pdgid) != 22) continue;
            if (genParticles->ptrAt( genLoop )->p4().pt() < 10) continue;
            if (!genParticles->ptrAt( genLoop )->isPromptFinalState()) continue;

            double gen_photon_candidate_pt = genParticles->ptrAt( genLoop )->p4().pt();
            double gen_photon_candidate_eta = genParticles->ptrAt( genLoop )->p4().eta();
            double gen_photon_candidate_phi = genParticles->ptrAt( genLoop )->p4().phi();
            double deltaR_ = deltaR(gen_photon_candidate_eta, gen_photon_candidate_phi, pho->p4().eta(), pho->p4().phi());

            if (deltaR_ > maxDr) continue;

            double ptdiff = abs(gen_photon_candidate_pt - pho->p4().pt());
            if (ptdiff < ptDiffMax) {
                ptDiffMax = ptdiff;
                index = genLoop;
            }
            
        } // end gen loop

        return index;
    }



    FCNCLeptonicTagProducer::FCNCLeptonicTagProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        //thejetToken_( consumes<View<flashgg::Jet> >( iConfig.getParameter<InputTag>( "JetTag" ) ) ),
        inputTagJets_( iConfig.getParameter<std::vector<edm::InputTag> >( "inputTagJets" ) ),
        electronToken_( consumes<View<flashgg::Electron> >( iConfig.getParameter<InputTag>( "ElectronTag" ) ) ),
        muonToken_( consumes<View<flashgg::Muon> >( iConfig.getParameter<InputTag>( "MuonTag" ) ) ),
        METToken_( consumes<View<flashgg::Met> >( iConfig.getParameter<InputTag>( "MetTag" ) ) ),
        mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getParameter<InputTag> ( "MVAResultTag" ) ) ),
        vertexToken_( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag> ( "VertexTag" ) ) ),
        genParticleToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag> ( "GenParticleTag" ) ) ),
        systLabel_( iConfig.getParameter<string> ( "SystLabel" ) )
    {
        systematicsLabels.push_back("");
        modifySystematicsWorkflow = iConfig.getParameter<bool> ( "ModifySystematicsWorkflow" );

        useLargeMVAs = iConfig.getParameter<bool> ( "UseLargeMVAs" );

        coupling_ = iConfig.getParameter<std::string>( "Coupling" );
        splitFactor_ = iConfig.getParameter<int>( "SplitFactor" );

        // Get diphoton candidates corresponding to each systematic
        inputDiPhotonName_= iConfig.getParameter<std::string>( "DiPhotonName" );
        inputDiPhotonSuffixes_= iConfig.getParameter<std::vector<std::string> > ( "DiPhotonSuffixes" );
        std::vector<edm::InputTag>  diPhotonTags;

        inputMVAResultName_ = iConfig.getParameter<std::string>( "MVAResultName" );
        std::vector<edm::InputTag>  mvaResultTags;

        diPhotonTags.push_back(edm::InputTag(inputDiPhotonName_)); // nominal diphotons
        mvaResultTags.push_back(edm::InputTag(inputMVAResultName_)); // nominal mva results

        for (auto & suffix : inputDiPhotonSuffixes_) {
            systematicsLabels.push_back(suffix);
            std::string inputName = inputDiPhotonName_;
            inputName.append(suffix);
            diPhotonTags.push_back(edm::InputTag(inputName));

            std::string mvaName = inputMVAResultName_;
            mvaName.append(suffix);
            mvaResultTags.push_back(edm::InputTag(mvaName));
        }

        for (auto & tag : diPhotonTags)
            diPhotonTokens_.push_back(consumes<edm::View<flashgg::DiPhotonCandidate>>(tag));
        
        for (auto & tag : mvaResultTags)
            mvaResultTokens_.push_back(consumes<edm::View<flashgg::DiPhotonMVAResult>>(tag));


        // Get jet collections corresponding to each systematic
        nominalJetsName_ = iConfig.getParameter<std::string> ( "JetsName" );
        systematicsJetsName_ = iConfig.getParameter<std::string> ( "SystematicsJetsName" );
        inputJetsCollSize_= iConfig.getParameter<unsigned int> ( "JetsCollSize" );
        inputJetsSuffixes_= iConfig.getParameter<std::vector<std::string> > ( "JetsSuffixes" );
        std::vector<std::vector<edm::InputTag>>  jetTags;

        jetTags.push_back(std::vector<edm::InputTag>(0));
        for (unsigned int i = 0; i < inputJetsCollSize_ ; i++) {
            jetTags[0].push_back(inputTagJets_[i]); // nominal jets
        }

        for (auto & suffix : inputJetsSuffixes_) {
            systematicsLabels.push_back(suffix);
            jetTags.push_back(std::vector<edm::InputTag>(0));
            int suffix_idx = jetTags.size() - 1;
            for (unsigned int i = 0; i < inputJetsCollSize_ ; i++) {
                std::string jetsName = systematicsJetsName_;
                jetsName.append(std::to_string(i));
                jetTags[suffix_idx].push_back(edm::InputTag(jetsName,suffix));
            }
        }

        for (unsigned int i = 0; i < jetTags.size(); i++) {
            jetTokens_.push_back(std::vector<edm::EDGetTokenT<edm::View<flashgg::Jet>>>(0));
            for (auto & tag : jetTags[i])
                jetTokens_[i].push_back(consumes<edm::View<flashgg::Jet>>(tag));
        }

        // Get MET corresponding to each systematic
        nominalMetName_ = iConfig.getParameter<std::string> ( "MetName" );
        systematicsMetName_ = iConfig.getParameter<std::string> ( "SystematicsMetName" );
        inputMetSuffixes_ = iConfig.getParameter<std::vector<std::string>> ( "MetSuffixes" );
        std::vector<edm::InputTag> metTags;
        metTags.push_back(edm::InputTag(nominalMetName_)); // nominal MET
        for (auto & suffix : inputMetSuffixes_) {
            systematicsLabels.push_back(suffix);
            metTags.push_back(edm::InputTag(systematicsMetName_, suffix));
        }

        for (auto & tag : metTags)
           metTokens_.push_back(consumes<edm::View<flashgg::Met>>(tag));

        leadPhoOverMassThreshold_ = iConfig.getParameter<double>( "leadPhoOverMassThreshold");
        subleadPhoOverMassThreshold_ = iConfig.getParameter<double>( "subleadPhoOverMassThreshold");
        if (coupling_ == "Hut") {
            MVAThreshold_ = iConfig.getParameter<std::vector<double>>( "MVAThresholdHut");
            MVAThreshold_BDTNRB = iConfig.getParameter<std::vector<double>>( "MVAThresholdHutNRB");
            MVAThreshold_BDTSMH = iConfig.getParameter<std::vector<double>>( "MVAThresholdHutSMH");
        }
        else if (coupling_ == "Hct") {
            MVAThreshold_ = iConfig.getParameter<std::vector<double>>( "MVAThresholdHct");
            MVAThreshold_BDTNRB = iConfig.getParameter<std::vector<double>>( "MVAThresholdHctNRB");
            MVAThreshold_BDTSMH = iConfig.getParameter<std::vector<double>>( "MVAThresholdHctSMH");
        }
        PhoMVAThreshold_ = iConfig.getParameter<double>( "PhoMVAThreshold");
        jetsNumberThreshold_ = iConfig.getParameter<double>( "jetsNumberThreshold");
        bjetsNumberThreshold_ = iConfig.getParameter<double>( "bjetsNumberThreshold");
        jetPtThreshold_ = iConfig.getParameter<double>( "jetPtThreshold");
        jetEtaThreshold_ = iConfig.getParameter<double>( "jetEtaThreshold");
        deltaRJetLepton_ = iConfig.getParameter<double>( "deltaRJetLepton");
        leadingJetPtThreshold_ = iConfig.getParameter<double>("leadingJetPtThreshold");

        MuonEtaCut_ = iConfig.getParameter<double>( "MuonEtaCut");
        MuonPtCut_ = iConfig.getParameter<double>( "MuonPtCut");
        MuonIsoCut_ = iConfig.getParameter<double>( "MuonIsoCut");
        MuonPhotonDrCut_ = iConfig.getParameter<double>( "MuonPhotonDrCut");

        MinNLep_ = iConfig.getParameter<int>( "MinNLep");
        MaxNLep_ = iConfig.getParameter<int>( "MaxNLep");
 
        EleEtaCuts_ = iConfig.getParameter<std::vector<double>>( "EleEtaCuts");
        ElePtCut_ = iConfig.getParameter<double>( "ElePtCut");
        ElePhotonDrCut_ = iConfig.getParameter<double>( "ElePhotonDrCut");
        ElePhotonZMassCut_ = iConfig.getParameter<double>( "ElePhotonZMassCut");
        DeltaRTrkEle_ = iConfig.getParameter<double>( "DeltaRTrkEle");

        LeptonsZMassCut_ = iConfig.getParameter<double>( "LeptonsZMassCut");

        DiLeptonJetThreshold_ = iConfig.getParameter<double>( "DiLeptonJetThreshold");
        DiLeptonbJetThreshold_ = iConfig.getParameter<double>( "DiLeptonbJetThreshold");
        DiLeptonMVAThreshold_ = iConfig.getParameter<double>( "DiLeptonMVAThreshold");

        deltaRJetLeadPhoThreshold_ = iConfig.getParameter<double>( "deltaRJetLeadPhoThreshold");
        deltaRJetSubLeadPhoThreshold_ = iConfig.getParameter<double>( "deltaRJetSubLeadPhoThreshold");

        bDiscriminator_ = iConfig.getParameter<vector<double > >( "bDiscriminator");
        bTag_ = iConfig.getParameter<string>( "bTag");

        UseCutBasedDiphoId_ = iConfig.getParameter<bool>( "UseCutBasedDiphoId" );
        debug_ = iConfig.getParameter<bool>( "debug" );
        SplitDiLeptEv_ = iConfig.getParameter<bool>( "SplitDiLeptEv" );
        CutBasedDiphoId_ = iConfig.getParameter<std::vector<double>>( "CutBasedDiphoId" );

        MVAweightfile_ = iConfig.getParameter<edm::FileInPath>( "MVAweightfile" );
        topTaggerXMLfile_ = iConfig.getParameter<edm::FileInPath>( "topTaggerXMLfile" );
        fcncTaggerXMLfile_tt_ = iConfig.getParameter<edm::FileInPath>( "fcncTaggerXMLfile_tt" );
        fcncTaggerXMLfile_st_ = iConfig.getParameter<edm::FileInPath>( "fcncTaggerXMLfile_st" );
        tthVsttGGDNNfile_ = iConfig.getParameter<edm::FileInPath>( "tthVsttGGDNNfile" );
        tthMVA_RunII_weightfile_ = iConfig.getParameter<edm::FileInPath>( "tthMVA_RunII_weightfile" );

        fcncHutMVAWeightFile_ = iConfig.getParameter<edm::FileInPath>( "fcncHutMVAWeightFile" );
        fcncHctMVAWeightFile_ = iConfig.getParameter<edm::FileInPath>( "fcncHctMVAWeightFile" );
    
        fcncHutBDTNRBWeightFile_ = iConfig.getParameter<edm::FileInPath>( "fcncHutBDTNRBAWeightFile" );
        fcncHctBDTNRBWeightFile_ = iConfig.getParameter<edm::FileInPath>( "fcncHctBDTNRBAWeightFile" );
        fcncHutBDTSMHWeightFile_ = iConfig.getParameter<edm::FileInPath>( "fcncHutBDTSMHAWeightFile" );
        fcncHctBDTSMHWeightFile_ = iConfig.getParameter<edm::FileInPath>( "fcncHctBDTSMHAWeightFile" ); 

        DiphotonMva_.reset( new TMVA::Reader( "!Color:Silent" ) );
        DiphotonMva_->AddVariable( "dipho_leadEta", &leadeta_ );
        DiphotonMva_->AddVariable( "dipho_subleadEta", &subleadeta_ );
        DiphotonMva_->AddVariable( "dipho_lead_ptoM", &leadptom_ );
        DiphotonMva_->AddVariable( "dipho_sublead_ptoM", &subleadptom_ );
        DiphotonMva_->AddVariable( "dipho_leadIDMVA", &leadIDMVA_ );
        DiphotonMva_->AddVariable( "dipho_subleadIDMVA", &subleadIDMVA_ );
        DiphotonMva_->AddVariable( "dipho_deltaphi", &deltaphi_ );
        DiphotonMva_->AddVariable( "dipho_lead_PSV", &leadPSV_ );
        DiphotonMva_->AddVariable( "dipho_sublead_PSV", &subleadPSV_ );
        DiphotonMva_->AddVariable( "nJets", &nJets_ );
        DiphotonMva_->AddVariable( "nJets_bTagMedium", &nJets_bTagMedium_ );
        DiphotonMva_->AddVariable( "jet1_pt", &jet_pt1_ );
        DiphotonMva_->AddVariable( "jet2_pt", &jet_pt2_ );
        DiphotonMva_->AddVariable( "jet3_pt", &jet_pt3_ );
        DiphotonMva_->AddVariable( "jet1_eta", &jet_eta1_ );
        DiphotonMva_->AddVariable( "jet2_eta", &jet_eta2_ );
        DiphotonMva_->AddVariable( "jet3_eta", &jet_eta3_ );
        DiphotonMva_->AddVariable( "bTag1", &bTag1_ );
        DiphotonMva_->AddVariable( "bTag2", &bTag2_ );
        DiphotonMva_->AddVariable( "MetPt", &MetPt_ );
        DiphotonMva_->AddVariable( "lepton_leadPt", &lepton_leadPt_ );
        DiphotonMva_->AddVariable( "lepton_leadEta", &lepton_leadEta_ );

        DiphotonMva_->BookMVA( "BDT", MVAweightfile_.fullPath() );

        TThMva_RunII_.reset( new TMVA::Reader( "!Color:Silent" ) );
 
        TThMva_RunII_->AddVariable("maxIDMVA_", &maxPhoID_);
        TThMva_RunII_->AddVariable("minIDMVA_", &minPhoID_);
        TThMva_RunII_->AddVariable("max2_btag_", &secondMaxBTagVal_noBB_);
        TThMva_RunII_->AddVariable("max1_btag_", &maxBTagVal_noBB_);
        TThMva_RunII_->AddVariable("dipho_delta_R", &diPhoDeltaR_);
        TThMva_RunII_->AddVariable("njets_", &nJets_);
        TThMva_RunII_->AddVariable("ht_", &ht_);
        TThMva_RunII_->AddVariable("leadptoM_", &leadptom_);
        TThMva_RunII_->AddVariable("subleadptoM_", &subleadptom_);
        TThMva_RunII_->AddVariable("lead_eta_", &leadeta_);
        TThMva_RunII_->AddVariable("sublead_eta_", &subleadeta_);
 
        TThMva_RunII_->AddVariable("jet1_pt_", &jetPt_1_);
        TThMva_RunII_->AddVariable("jet1_eta_", &jetEta_1_);
        TThMva_RunII_->AddVariable("jet1_btag_", &btag_noBB_1_);
        TThMva_RunII_->AddVariable("jet2_pt_", &jetPt_2_);
        TThMva_RunII_->AddVariable("jet2_eta_", &jetEta_2_);
        TThMva_RunII_->AddVariable("jet2_btag_", &btag_noBB_2_);
        TThMva_RunII_->AddVariable("jet3_pt_", &jetPt_3_);
        TThMva_RunII_->AddVariable("jet3_eta_", &jetEta_3_);
        TThMva_RunII_->AddVariable("jet3_btag_", &btag_noBB_3_);
        TThMva_RunII_->AddVariable("jet4_pt_", &jetPt_4_);
        TThMva_RunII_->AddVariable("jet4_eta_", &jetEta_4_);
        TThMva_RunII_->AddVariable("jet4_btag_", &btag_noBB_4_);
 
        TThMva_RunII_->AddVariable("leadPSV_", &leadPSV_);
        TThMva_RunII_->AddVariable("subleadPSV_", &subleadPSV_);
 
        TThMva_RunII_->AddVariable("dipho_cosphi_", &diPhoCosPhi_);
        TThMva_RunII_->AddVariable("dipho_rapidity_", &diPhoY_);
        TThMva_RunII_->AddVariable("met_", &MetPt_);
 
        TThMva_RunII_->AddVariable("dipho_pt_over_mass_", &diPhoPtoM_);
 
        TThMva_RunII_->AddVariable("helicity_angle_", &helicity_angle_);
        TThMva_RunII_->AddVariable("lep_pt_", &lepton_leadPt_);
        TThMva_RunII_->AddVariable("lep_eta_", &lepton_leadEta_); 
        TThMva_RunII_->AddVariable("n_lep_tight_", &lepton_nTight_);
 
        TThMva_RunII_->AddVariable("dnn_score_0", &dnn_score_0_);
         
        TThMva_RunII_->BookMVA( "BDT" , tthMVA_RunII_weightfile_.fullPath());     

        FCNC_BDTNRB_RunII_.reset( new TMVA::Reader( "!Color:Silent" ) );

        FCNC_BDTNRB_RunII_->AddVariable("helicity_angle_", &helicity_angle_);
        FCNC_BDTNRB_RunII_->AddVariable("dipho_pt_over_mass_", &diPhoPtoM_);
        FCNC_BDTNRB_RunII_->AddVariable("met_", &MetPt_);
        FCNC_BDTNRB_RunII_->AddVariable("dipho_rapidity_", &diPhoY_);
        FCNC_BDTNRB_RunII_->AddVariable("dipho_cosphi_", &diPhoCosPhi_);

        FCNC_BDTNRB_RunII_->AddVariable("subleadPSV_", &subleadPSV_);
        FCNC_BDTNRB_RunII_->AddVariable("leadPSV_", &leadPSV_);
        FCNC_BDTNRB_RunII_->AddVariable("jet3_btag_", &btag_noBB_3_);
        FCNC_BDTNRB_RunII_->AddVariable("jet3_eta_", &jetEta_3_);
        FCNC_BDTNRB_RunII_->AddVariable("jet3_pt_", &jetPt_3_);

        FCNC_BDTNRB_RunII_->AddVariable("jet2_btag_", &btag_noBB_2_);
        FCNC_BDTNRB_RunII_->AddVariable("jet2_eta_", &jetEta_2_);
        FCNC_BDTNRB_RunII_->AddVariable("jet2_pt_", &jetPt_2_);
        FCNC_BDTNRB_RunII_->AddVariable("jet1_btag_", &btag_noBB_1_);
        FCNC_BDTNRB_RunII_->AddVariable("jet1_eta_", &jetEta_1_);

        FCNC_BDTNRB_RunII_->AddVariable("jet1_pt_", &jetPt_1_);
        FCNC_BDTNRB_RunII_->AddVariable("sublead_eta_", &subleadeta_);
        FCNC_BDTNRB_RunII_->AddVariable("lead_eta_", &leadeta_);
        FCNC_BDTNRB_RunII_->AddVariable("subleadptoM_", &subleadptom_);
        FCNC_BDTNRB_RunII_->AddVariable("leadptoM_", &leadptom_);

        FCNC_BDTNRB_RunII_->AddVariable("ht_", &ht_);
        FCNC_BDTNRB_RunII_->AddVariable("njets_", &nJets_);
        FCNC_BDTNRB_RunII_->AddVariable("dipho_delta_R", &diPhoDeltaR_);
        FCNC_BDTNRB_RunII_->AddVariable("max1_btag_", &maxBTagVal_noBB_);
        FCNC_BDTNRB_RunII_->AddVariable("max2_btag_", &secondMaxBTagVal_noBB_);

        FCNC_BDTNRB_RunII_->AddVariable("minIDMVA_", &minPhoID_);
        FCNC_BDTNRB_RunII_->AddVariable("maxIDMVA_", &maxPhoID_);        
        FCNC_BDTNRB_RunII_->AddVariable("lep_pt_", &lepton_leadPt_);
        FCNC_BDTNRB_RunII_->AddVariable("lep_eta_", &lepton_leadEta_);
        FCNC_BDTNRB_RunII_->AddVariable("n_lep_tight_", &lepton_nTight_);

        FCNC_BDTNRB_RunII_->AddVariable("top_tag_score_", &top_tag_score_);
        //FCNC_BDTNRB_RunII_->AddVariable("fcnc_tag_score_tt_", &fcnc_tag_score_tt_); // not yet
        //FCNC_BDTNRB_RunII_->AddVariable("fcnc_tag_score_st_", &fcnc_tag_score_st_); // not yet
        FCNC_BDTNRB_RunII_->AddVariable("chi2_neutrino_pz_", &chi2_neutrino_pz_);
        FCNC_BDTNRB_RunII_->AddVariable("chi2_tbw_mass_", &chi2_tbw_mass_);
        FCNC_BDTNRB_RunII_->AddVariable("chi2_tbw_pt_", &chi2_tbw_pt_);
        FCNC_BDTNRB_RunII_->AddVariable("chi2_tbw_eta_", &chi2_tbw_eta_);

        FCNC_BDTNRB_RunII_->AddVariable("chi2_tbw_deltaR_dipho_", &chi2_tbw_deltaR_dipho_);
        FCNC_BDTNRB_RunII_->AddVariable("chi2_qjet_pt_", &chi2_qjet_pt_);
        FCNC_BDTNRB_RunII_->AddVariable("chi2_qjet_eta_", &chi2_qjet_eta_);
        FCNC_BDTNRB_RunII_->AddVariable("chi2_qjet_btag_", &chi2_qjet_btag_);
        FCNC_BDTNRB_RunII_->AddVariable("chi2_qjet_deltaR_dipho_", &chi2_qjet_deltaR_dipho_);

        FCNC_BDTNRB_RunII_->AddVariable("chi2_tqh_ptOverM_", &chi2_tqh_ptOverM_);
        FCNC_BDTNRB_RunII_->AddVariable("chi2_tqh_eta_", &chi2_tqh_eta_);
        FCNC_BDTNRB_RunII_->AddVariable("chi2_tqh_deltaR_tbw_", &chi2_tqh_deltaR_tbw_);
        FCNC_BDTNRB_RunII_->AddVariable("chi2_tqh_deltaR_dipho_", &chi2_tqh_deltaR_dipho_);

        FCNC_BDTSMH_RunII_.reset( new TMVA::Reader( "!Color:Silent" ) );

        FCNC_BDTSMH_RunII_->AddVariable("helicity_angle_", &helicity_angle_);
        FCNC_BDTSMH_RunII_->AddVariable("dipho_pt_over_mass_", &diPhoPtoM_);
        FCNC_BDTSMH_RunII_->AddVariable("met_", &MetPt_);
        FCNC_BDTSMH_RunII_->AddVariable("dipho_rapidity_", &diPhoY_);
        FCNC_BDTSMH_RunII_->AddVariable("dipho_cosphi_", &diPhoCosPhi_);

        FCNC_BDTSMH_RunII_->AddVariable("subleadPSV_", &subleadPSV_);
        FCNC_BDTSMH_RunII_->AddVariable("leadPSV_", &leadPSV_);
        FCNC_BDTSMH_RunII_->AddVariable("jet3_btag_", &btag_noBB_3_);
        FCNC_BDTSMH_RunII_->AddVariable("jet3_eta_", &jetEta_3_);
        FCNC_BDTSMH_RunII_->AddVariable("jet3_pt_", &jetPt_3_);

        FCNC_BDTSMH_RunII_->AddVariable("jet2_btag_", &btag_noBB_2_);
        FCNC_BDTSMH_RunII_->AddVariable("jet2_eta_", &jetEta_2_);
        FCNC_BDTSMH_RunII_->AddVariable("jet2_pt_", &jetPt_2_);
        FCNC_BDTSMH_RunII_->AddVariable("jet1_btag_", &btag_noBB_1_);
        FCNC_BDTSMH_RunII_->AddVariable("jet1_eta_", &jetEta_1_);

        FCNC_BDTSMH_RunII_->AddVariable("jet1_pt_", &jetPt_1_);
        FCNC_BDTSMH_RunII_->AddVariable("sublead_eta_", &subleadeta_);
        FCNC_BDTSMH_RunII_->AddVariable("lead_eta_", &leadeta_);
        FCNC_BDTSMH_RunII_->AddVariable("subleadptoM_", &subleadptom_);
        FCNC_BDTSMH_RunII_->AddVariable("leadptoM_", &leadptom_);

        FCNC_BDTSMH_RunII_->AddVariable("ht_", &ht_);
        FCNC_BDTSMH_RunII_->AddVariable("njets_", &nJets_);
        FCNC_BDTSMH_RunII_->AddVariable("dipho_delta_R", &diPhoDeltaR_);
        FCNC_BDTSMH_RunII_->AddVariable("max1_btag_", &maxBTagVal_noBB_);
        FCNC_BDTSMH_RunII_->AddVariable("max2_btag_", &secondMaxBTagVal_noBB_);

        FCNC_BDTSMH_RunII_->AddVariable("minIDMVA_", &minPhoID_);
        FCNC_BDTSMH_RunII_->AddVariable("maxIDMVA_", &maxPhoID_);
        FCNC_BDTSMH_RunII_->AddVariable("lep_pt_", &lepton_leadPt_);
        FCNC_BDTSMH_RunII_->AddVariable("lep_eta_", &lepton_leadEta_);
        FCNC_BDTSMH_RunII_->AddVariable("n_lep_tight_", &lepton_nTight_);

        FCNC_BDTSMH_RunII_->AddVariable("top_tag_score_", &top_tag_score_);
        //FCNC_BDTSMH_RunII_->AddVariable("fcnc_tag_score_tt_", &fcnc_tag_score_tt_); // not yet
        //FCNC_BDTSMH_RunII_->AddVariable("fcnc_tag_score_st_", &fcnc_tag_score_st_); // not yet
        FCNC_BDTSMH_RunII_->AddVariable("chi2_neutrino_pz_", &chi2_neutrino_pz_);
        FCNC_BDTSMH_RunII_->AddVariable("chi2_tbw_mass_", &chi2_tbw_mass_);
        FCNC_BDTSMH_RunII_->AddVariable("chi2_tbw_pt_", &chi2_tbw_pt_);
        FCNC_BDTSMH_RunII_->AddVariable("chi2_tbw_eta_", &chi2_tbw_eta_);

        FCNC_BDTSMH_RunII_->AddVariable("chi2_tbw_deltaR_dipho_", &chi2_tbw_deltaR_dipho_);
        FCNC_BDTSMH_RunII_->AddVariable("chi2_qjet_pt_", &chi2_qjet_pt_);
        FCNC_BDTSMH_RunII_->AddVariable("chi2_qjet_eta_", &chi2_qjet_eta_);
        FCNC_BDTSMH_RunII_->AddVariable("chi2_qjet_btag_", &chi2_qjet_btag_);
        FCNC_BDTSMH_RunII_->AddVariable("chi2_qjet_deltaR_dipho_", &chi2_qjet_deltaR_dipho_);

        FCNC_BDTSMH_RunII_->AddVariable("chi2_tqh_ptOverM_", &chi2_tqh_ptOverM_);
        FCNC_BDTSMH_RunII_->AddVariable("chi2_tqh_eta_", &chi2_tqh_eta_);
        FCNC_BDTSMH_RunII_->AddVariable("chi2_tqh_deltaR_tbw_", &chi2_tqh_deltaR_tbw_);
        FCNC_BDTSMH_RunII_->AddVariable("chi2_tqh_deltaR_dipho_", &chi2_tqh_deltaR_dipho_);

        if (coupling_ == "Hut") {
            std::cout << "Coupling selected as " << coupling_ << ", loading the following MVAs: " << fcncHutBDTNRBWeightFile_.fullPath() << ", " << fcncHutBDTSMHWeightFile_.fullPath() << std::endl;
            FCNC_BDTNRB_RunII_->BookMVA( "BDT" , fcncHutBDTNRBWeightFile_.fullPath());
            FCNC_BDTSMH_RunII_->BookMVA( "BDT" , fcncHutBDTSMHWeightFile_.fullPath());
        }
        else if (coupling_ == "Hct") {
            std::cout << "Coupling selected as " << coupling_ << ", loading the following MVAs: " << fcncHctBDTNRBWeightFile_.fullPath() << ", " << fcncHctBDTSMHWeightFile_.fullPath() << std::endl;
            FCNC_BDTNRB_RunII_->BookMVA( "BDT" , fcncHctBDTNRBWeightFile_.fullPath());
            FCNC_BDTSMH_RunII_->BookMVA( "BDT" , fcncHctBDTSMHWeightFile_.fullPath());
        }
  

        if (useLargeMVAs) {
            topTagger = new BDT_resolvedTopTagger(topTaggerXMLfile_.fullPath());
            fcncTagger = new ANN_LeptonicTopTagger(fcncTaggerXMLfile_tt_.fullPath(), fcncTaggerXMLfile_st_.fullPath());
            //fcncTagger->setDebug(true);
            //dnn = new TTH_DNN_Helper(tthVsttGGDNNfile_.fullPath());
            //dnn->SetInputShapes(19, 9, 8);
        }

        for (unsigned i = 0 ; i < inputTagJets_.size() ; i++) {
            auto token = consumes<View<flashgg::Jet> >(inputTagJets_[i]);
            tokenJets_.push_back(token);
        }

        if (modifySystematicsWorkflow) {
            for (auto & syst_name : systematicsLabels)
                produces<vector<FCNCLeptonicTag>>(syst_name);
        }

        else {
            produces<vector<FCNCLeptonicTag> >();
        }

    }

    int FCNCLeptonicTagProducer::chooseCategory( float tthmvavalue , bool debug_)
    {
        // should return 0 if mva above all the numbers, 1 if below the first, ..., boundaries.size()-N if below the Nth, ...
        int n;
        for( n = 0 ; n < ( int )MVAThreshold_.size() ; n++ ) {
            if( ( double )tthmvavalue > MVAThreshold_[MVAThreshold_.size() - n - 1] ) { return n; }
        }

      if(debug_)
        {   cout << "Checking class, thresholds: ";
            for(unsigned int i=0; i<MVAThreshold_.size(); ++i)
                cout << MVAThreshold_[i] << " ";
        }

        return -1; // Does not pass, object will not be produced
    }

    int FCNCLeptonicTagProducer::chooseCategory( float mva1_value, float mva2_value)
    {
        // returns i if mva1 > boundaries1[i] && mva2 > boundaries2[i]
        for (int i = 0; i < (int) MVAThreshold_BDTNRB.size(); i ++) {
            int idx = MVAThreshold_BDTNRB.size() - (i + 1);
            if ( mva1_value > MVAThreshold_BDTNRB[idx] && mva2_value > MVAThreshold_BDTSMH[idx] )
                return i;
        }
        return -1; // Does not pass, object will not be produced
    }
    void FCNCLeptonicTagProducer::produce( Event &evt, const EventSetup & )
    {
        unique_ptr<unsigned long long> evt_event (new unsigned long long);
        *evt_event = evt.id().event();

        bool skip;
        if (*evt_event % (unsigned int)splitFactor_ == (unsigned int)(splitFactor_-1)) {
            if (debug_)
                std::cout << "Event number modulo split factor is split factor minus 1, keeping event" << std::endl;
            skip = false;
        }
        else {
            if (debug_)
                std::cout << "Event number modulo split factor is not split factor minus 1, skipping event" << std::endl;
            skip = true;
        }

        JetCollectionVector Jets(inputJetsCollSize_);
        if (!modifySystematicsWorkflow) {
            for( size_t j = 0; j < inputTagJets_.size(); ++j ) {
                evt.getByToken( tokenJets_[j], Jets[j] );
            }
        }

        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        if (!modifySystematicsWorkflow)
            evt.getByToken( diPhotonToken_, diPhotons );

        Handle<View<flashgg::Muon> > theMuons;
        evt.getByToken( muonToken_, theMuons );

        Handle<View<flashgg::Electron> > theElectrons;
        evt.getByToken( electronToken_, theElectrons );

        Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
        if (!modifySystematicsWorkflow)
            evt.getByToken( mvaResultToken_, mvaResults );

        Handle<View<reco::GenParticle> > genParticles;

        Handle<View<reco::Vertex> > vertices;
        evt.getByToken( vertexToken_, vertices );

        Handle<View<flashgg::Met> > theMet_;
        if (!modifySystematicsWorkflow)
            evt.getByToken( METToken_, theMet_ );

        //std::unique_ptr<vector<TTHLeptonicTag> > tthltags( new vector<TTHLeptonicTag> );

        //assert( diPhotons->size() == mvaResults->size() );

        double idmva1 = 0.;
        double idmva2 = 0.;

        // Here we loop over all systematics (DiPhoton, Jets, Met), with the following logic:
        // idx = 0:                                                                     Nominal DiPhoton | Nominal Jets | Nominal Met
        // 0 < idx <= # DiPho Systs:                                                    SystVar DiPhoton | Nominal Jets | Nominal Met
        // # DiPho Systs < idx <= # DiPho Systs + Jet Systs:                            Nominal DiPhoton | SystVar Jets | Nominal Met
        // # Jet Systs + # DiPho Systs < idx <= # DiPho Systs + Jet Systs + Met Systs:  Nominal DiPhoton | Nominal Jets | SystVar Met
        if (!modifySystematicsWorkflow)
            systematicsLabels = {""};

        //cout << "Looping over " << systematicsLabels.size() << "systematics variations" << endl;
        //cout << inputDiPhotonSuffixes_.size() << "for diphotons, " << inputJetsSuffixes_.size() << " for jets, and " << inputMetSuffixes_.size() << " for met" << endl;

        for (unsigned int syst_idx = 0; syst_idx < systematicsLabels.size(); syst_idx++) {

            bool vary_dipho = ((syst_idx > 0) && (syst_idx <= inputDiPhotonSuffixes_.size()));
            bool vary_jets  = ((syst_idx > inputDiPhotonSuffixes_.size()) && (syst_idx <= (inputJetsSuffixes_.size() + inputDiPhotonSuffixes_.size())));
            bool vary_met   = (syst_idx > (inputJetsSuffixes_.size() + inputDiPhotonSuffixes_.size()));

            /*
            cout << "Syst idx: " << syst_idx << ", " << systematicsLabels[syst_idx] << endl;
            std::string vary_dipho_s = vary_dipho ? "No" : "Yes";
            std::string vary_jets_s = vary_jets ? "No" : "Yes";
            std::string vary_met_s = vary_met ? "No" : "Yes";
            cout << "Taking nominal dipho: " << vary_dipho_s << endl;
            cout << "Taking nominal jets:  " << vary_jets_s << endl;
            cout << "Taking nominal met:   " << vary_met_s << endl;
            if (vary_dipho)
                cout << "Choosing this diphoton systematic: " << inputDiPhotonSuffixes_[syst_idx-1] << endl;
            if (vary_jets)
                cout << "Choosing this jet systematic: " << inputJetsSuffixes_[syst_idx - (1 + inputDiPhotonSuffixes_.size())] << endl;
            if (vary_met)
                cout << "Choosing this met systematic: " << inputMetSuffixes_[syst_idx - (1 + inputJetsSuffixes_.size() + inputDiPhotonSuffixes_.size())] << endl;
            */

            // Select appropriate diphotons
            //Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
            if (modifySystematicsWorkflow) {
                if (vary_dipho) {
                    evt.getByToken(diPhotonTokens_[syst_idx], diPhotons);
                    evt.getByToken(mvaResultTokens_[syst_idx], mvaResults); 
                }
                else {
                    evt.getByToken(diPhotonTokens_[0], diPhotons);
                    evt.getByToken(mvaResultTokens_[0], mvaResults);
                }
                // Select appropriate MET
                //Handle<View<flashgg::Met> > theMet_;
                if (vary_met) {
                    int met_syst_idx = syst_idx - (inputJetsSuffixes_.size() + inputDiPhotonSuffixes_.size());
                    evt.getByToken(metTokens_[met_syst_idx], theMet_);
                }
                else
                    evt.getByToken(metTokens_[0], theMet_);
            }

            assert( diPhotons->size() == mvaResults->size() );

            std::unique_ptr<vector<FCNCLeptonicTag> > tthltags( new vector<FCNCLeptonicTag> );

            if( ! evt.isRealData() )
            {
                evt.getByToken( genParticleToken_, genParticles );
            }


            for( unsigned int diphoIndex = 0; diphoIndex < diPhotons->size(); diphoIndex++ )
            {
                if (skip)   continue;

                unsigned int jetCollectionIndex = diPhotons->ptrAt( diphoIndex )->jetCollectionIndex();

                edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt( diphoIndex );
                edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResults->ptrAt( diphoIndex );

                if( dipho->leadingPhoton()->pt() < ( dipho->mass() )*leadPhoOverMassThreshold_ ) { continue; }
                if( dipho->subLeadingPhoton()->pt() < ( dipho->mass() )*subleadPhoOverMassThreshold_ ) { continue; }
                idmva1 = dipho->leadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );
                idmva2 = dipho->subLeadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );

                if(debug_)
                    cout << "Photon pair with PhoIdMVA values: " << idmva1 << " " << idmva2 << endl;
     
                if( idmva1 < PhoMVAThreshold_ || idmva2 < PhoMVAThreshold_ ) { continue; }

                ht_ = 0.;
                btag_1_=-999;
                btag_noBB_1_ = -999;
                jetPt_1_=-999;
                jetEta_1_=-999;
                jetPhi_1_=-999;
                btag_2_=-999;
                btag_noBB_2_ = -999;
                jetPt_2_=-999;
                jetEta_2_=-999;
                jetPhi_2_=-999;
                btag_3_=-999;
                btag_noBB_3_ = -999;
                jetPt_3_=-999;
                jetEta_3_=-999;
                jetPhi_3_=-999;
                btag_4_=-999;
                btag_noBB_4_ = -999;
                jetPt_4_=-999;
                jetEta_4_=-999;
                jetPhi_4_=-999;

                bool passDiphotonSelection = true;
                if(UseCutBasedDiphoId_)
                {
                    assert(CutBasedDiphoId_.size()==6);
                    if(dipho->leadingPhoton()->pt()/dipho->mass() < CutBasedDiphoId_[0]) passDiphotonSelection = false;
                    if(dipho->subLeadingPhoton()->pt()/dipho->mass() < CutBasedDiphoId_[1]) passDiphotonSelection = false;
                    if(dipho->leadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() ) < CutBasedDiphoId_[2]) passDiphotonSelection = false;
                    if(dipho->subLeadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() ) < CutBasedDiphoId_[3]) passDiphotonSelection = false;
                    if(abs (dipho->leadingPhoton()->eta() - dipho->subLeadingPhoton()->eta()) > CutBasedDiphoId_[4]) passDiphotonSelection = false;
                    if(deltaPhi(dipho->leadingPhoton()->phi(), dipho->subLeadingPhoton()->phi() ) > CutBasedDiphoId_[5] ) passDiphotonSelection = false;
                }

                if(!passDiphotonSelection) continue;

                if(debug_)
                    cout << "Passed photon selection, checking leptons: " << idmva1 << " " << idmva2 << endl;

                std::vector<edm::Ptr<flashgg::Muon> >     Muons;
                std::vector<edm::Ptr<flashgg::Muon> >     MuonsTight;
                std::vector<edm::Ptr<flashgg::Electron> > Electrons;
                std::vector<edm::Ptr<flashgg::Electron> > ElectronsTight;

                std::vector<double> lepPt;
                std::vector<double> lepEta;
                std::vector<double> lepPhi;
                std::vector<double> lepE;
                std::vector<int>    lepType;

                if(theMuons->size()>0) {
                    Muons = selectMuons(theMuons->ptrs(), dipho, vertices->ptrs(), MuonPtCut_, MuonEtaCut_, MuonIsoCut_, MuonPhotonDrCut_, debug_);
                    MuonsTight = selectMuons(theMuons->ptrs(), dipho, vertices->ptrs(), MuonPtCut_, MuonEtaCut_, MuonIsoCut_, MuonPhotonDrCut_, debug_, 3);
                }
                if(theElectrons->size()>0) {
                    Electrons = selectElectrons(theElectrons->ptrs(), dipho, ElePtCut_, EleEtaCuts_, ElePhotonDrCut_, ElePhotonZMassCut_, DeltaRTrkEle_, debug_);
                    ElectronsTight = selectElectrons(theElectrons->ptrs(), dipho, ElePtCut_, EleEtaCuts_, ElePhotonDrCut_, ElePhotonZMassCut_, DeltaRTrkEle_, debug_, 3);
                }

                //If 2 same flavour leptons are found remove the pairs with mass compatible with a Z boson

                if(Muons.size()>=2)
                {
                    std::vector<edm::Ptr<flashgg::Muon>> Muons_0;
                    Muons_0 = Muons;
                    std::vector<int> badIndexes;

                    for(unsigned int i=0; i<Muons_0.size(); ++i)
                    {
                        for(unsigned int j=i+1; j<Muons_0.size(); ++j)
                        {
                            TLorentzVector l1, l2;
                            l1.SetPtEtaPhiE(Muons_0[i]->pt(), Muons_0[i]->eta(), Muons_0[i]->phi(), Muons_0[i]->energy());
                            l2.SetPtEtaPhiE(Muons_0[j]->pt(), Muons_0[j]->eta(), Muons_0[j]->phi(), Muons_0[j]->energy());

                            if(fabs((l1+l2).M() - 91.187) < LeptonsZMassCut_)
                            {
                                badIndexes.push_back(i);
                                badIndexes.push_back(j);
                            }
                        }
                    }

                    if(badIndexes.size()!=0)
                    {
                        Muons.clear();
                        for(unsigned int i=0; i<Muons_0.size(); ++i)
                        {
                           bool isBad = false;
                           for(unsigned int j=0; j<badIndexes.size(); ++j)
                           {
                              if(badIndexes[j]==(int)i)
                                   isBad = true;
                          }
                          if(!isBad) Muons.push_back(Muons_0[i]);
                        }
                    }
                }        

                if(Electrons.size()>=2)
                {
                    std::vector<int> badIndexes;
                    std::vector<edm::Ptr<flashgg::Electron> > Electrons_0;
                    Electrons_0 = Electrons;
                    for(unsigned int i=0; i<Electrons_0.size(); ++i)
                    {
                        for(unsigned int j=i+1; j<Electrons_0.size(); ++j)
                        {
                            TLorentzVector l1, l2;
                            l1.SetPtEtaPhiE(Electrons_0[i]->pt(), Electrons_0[i]->eta(), Electrons_0[i]->phi(), Electrons_0[i]->energy());
                            l2.SetPtEtaPhiE(Electrons_0[j]->pt(), Electrons_0[j]->eta(), Electrons_0[j]->phi(), Electrons_0[j]->energy());

                            if(fabs((l1+l2).M() - 91.187) < LeptonsZMassCut_)
                            {
                                badIndexes.push_back(i);
                                badIndexes.push_back(j);
                            }
                        }
                    }
                    if(badIndexes.size()!=0)
                    {
                        Electrons.clear();

                        for(unsigned int i=0; i<Electrons_0.size(); ++i)
                        {
                             bool isBad = false;
                             for(unsigned int j=0; j<badIndexes.size(); ++j)
                             {
                                 if(badIndexes[j]==(int)i)
                                     isBad = true;
                             }
                             if(!isBad) Electrons.push_back(Electrons_0[i]);
                        }
                     }
                 }        

                if( (Muons.size() + Electrons.size()) < (unsigned) MinNLep_ || (Muons.size() + Electrons.size()) > (unsigned) MaxNLep_) continue;

                // Fill lepton vectors            
                // ===================
                
                std::vector<std::pair< unsigned int, float > > sorter;

                if(debug_) cout<<" nMuons="<<Muons.size()<<" nElectrons="<<Electrons.size()<< endl;

                for(unsigned int i=0;i<Muons.size();i++){
                    float pt=Muons[i]->pt();
                    int index=100;
                    index+=i;
                    std::pair<unsigned int, float>pairToSort = std::make_pair(index, pt);
                    sorter.push_back( pairToSort );

                    if (useLargeMVAs) {
                        float lepton_id = Muons[i]->charge() < 0 ? 13. : -13.;
                        fcncTagger->addMuon(Muons[i]->pt(), Muons[i]->eta(), Muons[i]->phi(), Muons[i]->energy(), lepton_id);
                        fcncTagger->addLepton(Muons[i]->pt(), Muons[i]->eta(), Muons[i]->phi(), Muons[i]->energy(), lepton_id);
                    }

                    if(debug_) cout<<" muon "<< i <<" pt="<<pt<< endl;
                }
                for(unsigned int i=0;i<Electrons.size();i++){
                    float pt=Electrons[i]->pt();
                    int index=200;
                    index+=i;
                    std::pair<unsigned int, float>pairToSort = std::make_pair(index, pt);
                    sorter.push_back( pairToSort );

                    if (useLargeMVAs) {
                        float lepton_id = Electrons[i]->charge() < 0 ? 11. : -11.;
                        fcncTagger->addElectron(Electrons[i]->pt(), Electrons[i]->eta(), Electrons[i]->phi(), Electrons[i]->energy(), lepton_id);
                        fcncTagger->addLepton(Electrons[i]->pt(), Electrons[i]->eta(), Electrons[i]->phi(), Electrons[i]->energy(), lepton_id);
                    }

                    if(debug_) cout<<" elec "<< i <<" pt="<<pt<< endl;
                }
                // sort map by pt

                std::sort( sorter.begin(), sorter.end(), Sorter() );
                    
                // fill vectors
                for (unsigned int i=0;i<sorter.size();i++){

                    if(debug_) cout<<" Filling lepton vector index:"<<sorter[i].first<<" pt:" <<sorter[i].second << endl;

                    lepPt.push_back(sorter[i].second);                

                    int type=0;
                    type=(sorter[i].first)/100;
                    int n=(sorter[i].first)%100;

                    lepType.push_back(type);

                    if(debug_) cout<<" type="<<type<<" n="<<n<< endl;
                        
                    if(type==1){
                        if(debug_) cout<<"MUON LEPPTCHECK "<<   sorter[i].second<<" "<<Muons[n]->pt()<< endl;
                        lepEta.push_back(Muons[n]->eta());
                        lepPhi.push_back(Muons[n]->phi());
                        lepE.push_back(Muons[n]->energy());
                        
                    }else if(type==2){
                        if(debug_) cout<<"ELEC LEPPTCHECK "<<   sorter[i].second<<" "<<Electrons[n]->pt()<< endl;
                        lepEta.push_back(Electrons[n]->eta());
                        lepPhi.push_back(Electrons[n]->phi());
                        lepE.push_back(Electrons[n]->energy());
                    }                
                }
                
                int njet_ = 0;
                int njets_btagloose_ = 0;
                int njets_btagmedium_ = 0;
                int njets_btagtight_ = 0;
                std::vector<edm::Ptr<flashgg::Jet>> tagJets;
                std::vector<edm::Ptr<flashgg::Jet>> tagBJets;
                std::vector<float> bTags;
                std::vector<float> bTags_noBB;

                //JetCollectionVector Jets(inputJetsCollSize_);
                if (modifySystematicsWorkflow) {
                    int jet_syst_idx;
                    if (vary_jets)
                        jet_syst_idx = syst_idx - (inputDiPhotonSuffixes_.size());
                    else
                        jet_syst_idx = 0;

                    for (unsigned int i = 0; i < inputJetsCollSize_; i++)
                        evt.getByToken(jetTokens_[jet_syst_idx][i], Jets[i]);
                }

                if (useLargeMVAs) {
                    fcncTagger->addPhoton(dipho->leadingPhoton()->pt(), dipho->leadingPhoton()->eta(), dipho->leadingPhoton()->phi(), 0., idmva1);           
                    fcncTagger->addPhoton(dipho->subLeadingPhoton()->pt(), dipho->subLeadingPhoton()->eta(), dipho->subLeadingPhoton()->phi(), 0., idmva2);           
                }

                std::vector<TLorentzVector> jets;
                std::vector<double> btag_scores;
                for( unsigned int jetIndex = 0; jetIndex < Jets[jetCollectionIndex]->size() ; jetIndex++ )
                {
                    edm::Ptr<flashgg::Jet> thejet = Jets[jetCollectionIndex]->ptrAt( jetIndex );

                    if( fabs( thejet->eta() ) > jetEtaThreshold_ ) { continue; }
                    if(!thejet->passesJetID  ( flashgg::Loose ) ) { continue; }
                    if( thejet->pt() < jetPtThreshold_ ) { continue; }

                    float dRPhoLeadJet = deltaR( thejet->eta(), thejet->phi(), dipho->leadingPhoton()->eta(), dipho->leadingPhoton()->phi() ) ;
                    float dRPhoSubLeadJet = deltaR( thejet->eta(), thejet->phi(), dipho->subLeadingPhoton()->eta(), dipho->subLeadingPhoton()->phi() );

                    if( dRPhoLeadJet < deltaRJetLeadPhoThreshold_ || dRPhoSubLeadJet < deltaRJetSubLeadPhoThreshold_ ) { continue; }

                    bool passDrLeptons = 1;                

                    for( unsigned int muonIndex = 0; muonIndex < Muons.size(); muonIndex++ )
                    {
                        Ptr<flashgg::Muon> muon = Muons[muonIndex];

                        float dRLept = deltaR( thejet->eta(), thejet->phi(), muon->eta(),  muon->phi() ) ;
                        if( dRLept > deltaRJetLepton_) { continue; }

                        passDrLeptons = 0;
                        break;
                    }

                    if(passDrLeptons)
                    {   for( unsigned int eleIndex = 0; eleIndex < Electrons.size(); eleIndex++ )
                        {
                            Ptr<flashgg::Electron> ele = Electrons[eleIndex];
                        
                            float dRLept = deltaR( thejet->eta(), thejet->phi(), ele->eta(),  ele->phi() ) ;
                            if( dRLept > deltaRJetLepton_) { continue; }

                            passDrLeptons = 0;
                            break;
                        }
                    }

                    if(passDrLeptons)
                    {
                        njet_++;
                        ht_ += thejet->pt();
                        tagJets.push_back( thejet );

                        TLorentzVector jet_four_momentum_;
                        jet_four_momentum_.SetPtEtaPhiE(thejet->pt(), thejet->eta(), thejet->phi(), thejet->energy());
                        jets.push_back(jet_four_momentum_);

                        float bDiscriminatorValue = -2.;
                        if(bTag_ == "pfDeepCSV") bDiscriminatorValue = thejet->bDiscriminator("pfDeepCSVJetTags:probb")+thejet->bDiscriminator("pfDeepCSVJetTags:probbb") ;
                        else  bDiscriminatorValue = thejet->bDiscriminator( bTag_ );

                        btag_scores.push_back(bDiscriminatorValue);

                        float bDiscriminatorValue_noBB = -2;
                        if(bTag_ == "pfDeepCSV") bDiscriminatorValue_noBB = thejet->bDiscriminator("pfDeepCSVJetTags:probb");
                        else  bDiscriminatorValue_noBB = thejet->bDiscriminator( bTag_ );

                        bDiscriminatorValue >= 0. ? bTags.push_back(bDiscriminatorValue) : bTags.push_back(-1.);
                        bDiscriminatorValue_noBB >= 0. ? bTags_noBB.push_back(bDiscriminatorValue_noBB) : bTags_noBB.push_back(-1.);

                        if( bDiscriminatorValue > bDiscriminator_[0] ) njets_btagloose_++;
                        if( bDiscriminatorValue > bDiscriminator_[1] ) njets_btagmedium_++;
                        if( bDiscriminatorValue > bDiscriminator_[2] ) njets_btagtight_++;

                        if( bDiscriminatorValue > bDiscriminator_[1] )
                            tagBJets.push_back( thejet );
                    
                        if (useLargeMVAs) {
                          float cvsl = thejet->bDiscriminator("pfDeepCSVJetTags:probc") + thejet->bDiscriminator("pfDeepCSVJetTags:probudsg") ;
                          float cvsb = thejet->bDiscriminator("pfDeepCSVJetTags:probc") + thejet->bDiscriminator("pfDeepCSVJetTags:probb")+thejet->bDiscriminator("pfDeepCSVJetTags:probbb") ;
                          float ptD = thejet->userFloat("ptD") ;
                          float axis1 = thejet->userFloat("axis1") ;
                          int mult = thejet->userFloat("totalMult") ;

                          topTagger->addJet(thejet->pt(), thejet->eta(), thejet->phi(), thejet->mass(), bDiscriminatorValue, cvsl, cvsb, ptD, axis1, mult);
                          fcncTagger->addJet(thejet->pt(), thejet->eta(), thejet->phi(), thejet->mass(), bDiscriminatorValue);           
                        }

                    }
                     
                }

                if(njet_ < jetsNumberThreshold_ || njets_btagmedium_ < bjetsNumberThreshold_) continue;
     
                if(debug_)
                    cout << "Jets after selections " << njet_ << ", bJets " << njets_btagmedium_ << endl;

                std::sort(bTags.begin(),bTags.end(),std::greater<float>());
                std::sort(bTags_noBB.begin(),bTags_noBB.end(),std::greater<float>());

                // Set variables to compute MVA value
                if(tagJets.size()>0){
                    if(bTag_ == "pfDeepCSV") btag_1_=tagJets[0]->bDiscriminator("pfDeepCSVJetTags:probb")+tagJets[0]->bDiscriminator("pfDeepCSVJetTags:probbb") ;
                    else  btag_1_ = tagJets[0]->bDiscriminator( bTag_ );
                    if(bTag_ == "pfDeepCSV") btag_noBB_1_=tagJets[0]->bDiscriminator("pfDeepCSVJetTags:probb");
                    else  btag_noBB_1_ = tagJets[0]->bDiscriminator( bTag_ );
                    jetPt_1_=tagJets[0]->pt();
                    jetEta_1_=tagJets[0]->eta();
                    jetPhi_1_=tagJets[0]->phi();
                }

                if(tagJets.size()>1){
                    if(bTag_ == "pfDeepCSV") btag_2_=tagJets[1]->bDiscriminator("pfDeepCSVJetTags:probb")+tagJets[1]->bDiscriminator("pfDeepCSVJetTags:probbb") ;
                    else  btag_2_ = tagJets[1]->bDiscriminator( bTag_ );
                    if(bTag_ == "pfDeepCSV") btag_noBB_2_=tagJets[1]->bDiscriminator("pfDeepCSVJetTags:probb");
                    else  btag_noBB_2_ = tagJets[1]->bDiscriminator( bTag_ );
                    jetPt_2_=tagJets[1]->pt();
                    jetEta_2_=tagJets[1]->eta();
                    jetPhi_2_=tagJets[1]->phi();
                }

                if(tagJets.size()>2){
                    if(bTag_ == "pfDeepCSV") btag_3_=tagJets[2]->bDiscriminator("pfDeepCSVJetTags:probb")+tagJets[2]->bDiscriminator("pfDeepCSVJetTags:probbb") ;
                    else  btag_3_ = tagJets[2]->bDiscriminator( bTag_ );
                    if(bTag_ == "pfDeepCSV") btag_noBB_3_=tagJets[2]->bDiscriminator("pfDeepCSVJetTags:probb");
                    else  btag_noBB_3_ = tagJets[2]->bDiscriminator( bTag_ );
                    jetPt_3_=tagJets[2]->pt();
                    jetEta_3_=tagJets[2]->eta();
                    jetPhi_3_=tagJets[2]->phi();
                }
                /* kinematics of fourth leading jet not currently used in BDT
                if(tagJets.size()>3){
                    if(bTag_ == "pfDeepCSV") btag_4_=tagJets[3]->bDiscriminator("pfDeepCSVJetTags:probb")+tagJets[3]->bDiscriminator("pfDeepCSVJetTags:probbb") ;
                    else  btag_4_ = tagJets[3]->bDiscriminator( bTag_ );
                    if(bTag_ == "pfDeepCSV") btag_noBB_4_=tagJets[3]->bDiscriminator("pfDeepCSVJetTags:probb");
                    else  btag_noBB_4_ = tagJets[3]->bDiscriminator( bTag_ );
                    jetPt_4_=tagJets[3]->pt();
                    jetEta_4_=tagJets[3]->eta();
                    jetPhi_4_=tagJets[3]->phi();
                } */

                diPhoY_= dipho->rapidity();
                diPhoPtoM_= dipho->pt()/dipho->mass();
                diPhoCosPhi_=  abs(TMath::Cos( deltaPhi( dipho->leadingPhoton()->phi(), dipho->subLeadingPhoton()->phi() ) ));
                diPhoDeltaR_ = deltaR( dipho->leadingPhoton()->eta(),dipho->leadingPhoton()->phi(), dipho->subLeadingPhoton()->eta(),dipho->subLeadingPhoton()->phi());

                leadeta_ = dipho->leadingPhoton()->eta();
                subleadeta_ = dipho->subLeadingPhoton()->eta();
                leadptom_ = dipho->leadingPhoton()->pt()/dipho->mass();
                subleadptom_ = dipho->subLeadingPhoton()->pt()/dipho->mass();
                leadIDMVA_ = dipho->leadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );
                subleadIDMVA_ = dipho->subLeadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );
                deltaphi_ = deltaPhi( dipho->leadingPhoton()->phi(), dipho->subLeadingPhoton()->phi() );
                leadPSV_ = dipho->leadingPhoton()->hasPixelSeed();
                subleadPSV_ = dipho->subLeadingPhoton()->hasPixelSeed();
                nJets_ = njet_;
                nJets_bTagMedium_ = njets_btagmedium_;

                minPhoID_=TMath::Min( leadIDMVA_, subleadIDMVA_);
                maxPhoID_=TMath::Max( leadIDMVA_, subleadIDMVA_);

                maxBTagVal_noBB_ = bTags_noBB.size() > 0 ? bTags_noBB[0] : -1.;
                secondMaxBTagVal_noBB_ = bTags_noBB.size() > 1 ? bTags_noBB[1]: -1.;

                lepton_nTight_ = float(MuonsTight.size() + ElectronsTight.size());

                if(tagJets.size()==0)
                {
                    jet_pt1_ = -1;
                    jet_pt2_ = -1.;
                    jet_pt3_ = -1.;
                    jet_eta1_ = -5;
                    jet_eta2_ = -5.;
                    jet_eta3_ = -5.;

                    bTag1_ = -1;
                    bTag2_ = -1;
                
                }else if(tagJets.size()==1)
                {
                    jet_pt1_ = tagJets[0]->pt();
                    jet_pt2_ = -1.;
                    jet_pt3_ = -1.;
                    jet_eta1_ = tagJets[0]->eta();
                    jet_eta2_ = -5.;
                    jet_eta3_ = -5.;

                    bTag1_ = bTags[0];
                    bTag2_ = -1.;
                }
                else if(tagJets.size()==2)
                {
                    jet_pt1_ = tagJets[0]->pt();
                    jet_pt2_ = tagJets[1]->pt();
                    jet_pt3_ = -1.;
                    jet_eta1_ = tagJets[0]->eta();
                    jet_eta2_ = tagJets[1]->eta();
                    jet_eta3_ = -5.;

                    bTag1_ = bTags[0];
                    bTag2_ = bTags[1];
                }
                else
                {
                    jet_pt1_ = tagJets[0]->pt();
                    jet_pt2_ = tagJets[1]->pt();
                    jet_pt3_ = tagJets[2]->pt();
                    jet_eta1_ = tagJets[0]->eta();
                    jet_eta2_ = tagJets[1]->eta();
                    jet_eta3_ = tagJets[2]->eta();

                    bTag1_ = bTags[0];
                    bTag2_ = bTags[1];
                }

                if( theMet_ -> size() != 1 )
                    std::cout << "WARNING number of MET is not equal to 1" << std::endl;
                MetPt_ = theMet_->ptrAt( 0 ) -> getCorPt();
                MetPhi_ = theMet_->ptrAt( 0 ) -> phi();

                if (useLargeMVAs) {
                    fcncTagger->addMet(MetPt_, MetPhi_);
                }

                TLorentzVector pho1, pho2;
                pho1.SetPtEtaPhiE(dipho->leadingPhoton()->pt(), dipho->leadingPhoton()->eta(), dipho->leadingPhoton()->phi(), dipho->leadingPhoton()->energy());
                pho2.SetPtEtaPhiE(dipho->subLeadingPhoton()->pt(), dipho->subLeadingPhoton()->eta(), dipho->subLeadingPhoton()->phi(), dipho->subLeadingPhoton()->energy());
                helicity_angle_ = helicity(pho1, pho2);

                TLorentzVector diphoton = pho1 + pho2;

                std::vector<double> global_features;
                global_features.resize(19);
                global_features[0] = dipho->leadingPhoton()->eta();
                global_features[1] = dipho->subLeadingPhoton()->eta();
                global_features[2] = dipho->leadingPhoton()->phi();
                global_features[3] = dipho->subLeadingPhoton()->phi();
                global_features[4] = leadptom_;
                global_features[5] = subleadptom_;
                global_features[6] = maxPhoID_;
                global_features[7] = minPhoID_;
                global_features[8] = log(MetPt_);
                global_features[9] = MetPhi_;
                global_features[10] = leadPSV_;
                global_features[11] = subleadPSV_;
                global_features[12] = diPhoY_;
                global_features[13] = diPhoPtoM_;
                global_features[14] = diPhoDeltaR_;
                global_features[15] = maxBTagVal_noBB_;
                global_features[16] = secondMaxBTagVal_noBB_;
                global_features[17] = nJets_;
                global_features[18] = lepton_nTight_;

                if (useLargeMVAs) {
                    //dnn->SetInputs(tagJets, Muons, Electrons, global_features);
                    //dnn_score_0_ = dnn->EvaluateDNN();
                    dnn_score_0_ = -999;
                }

                vector<float> mvaEval; 
                vector<float> mvaEval_tt; 
                vector<float> mvaEval_st; 
                if (useLargeMVAs) {
                    mvaEval = topTagger->EvalMVA();
                    mvaEval_tt = fcncTagger->EvalMVA_tt();
                    mvaEval_st = fcncTagger->EvalMVA_st();
                    topTagger->clear();
                    fcncTagger->clear();
                }

                top_tag_score_ = mvaEval.size() > 0 ? (mvaEval[0] != - 99 ? mvaEval[0] : -1) : - 1;
                fcnc_tag_score_tt_ = mvaEval_tt.size() > 0 ? (mvaEval_tt[0] != - 99 ? mvaEval_tt[0] : -1) : - 1;
                fcnc_tag_score_st_ = mvaEval_st.size() > 0 ? (mvaEval_st[0] != - 99 ? mvaEval_st[0] : -1) : - 1;
                TLorentzVector leading_electron;
                TLorentzVector leading_muon;
                TLorentzVector leading_lepton;

                int leadMuIndex = 0;
                float leadMuPt = -1;
                int leadEleIndex = 0;
                float leadElePt = -1;

                for( unsigned int muonIndex = 0; muonIndex < Muons.size(); muonIndex++ )
                {
                    Ptr<flashgg::Muon> muon = Muons[muonIndex];

                    if(muon->pt()>leadMuPt)
                    {
                        leadMuPt = muon->pt();
                        leadMuIndex = muonIndex;
                        leading_muon.SetPtEtaPhiE(muon->pt(), muon->eta(), muon->phi(), muon->energy());
                    }
                }

                for( unsigned int eleIndex = 0; eleIndex < Electrons.size(); eleIndex++ )
                {
                    Ptr<flashgg::Electron> ele = Electrons[eleIndex];

                    if(ele->pt()>leadElePt)
                    {
                        leadElePt = ele->pt();
                        leadEleIndex = eleIndex;
                        leading_electron.SetPtEtaPhiE(ele->pt(), ele->eta(), ele->phi(), ele->energy());
                    }
                }

                if(leadMuPt>=leadElePt)
                {
                    lepton_leadPt_ = Muons[leadMuIndex]->pt();
                    lepton_leadEta_ = Muons[leadMuIndex]->eta();
                    leading_lepton = leading_muon;
                }
                else
                {
                    lepton_leadPt_ = Electrons[leadEleIndex]->pt();
                    lepton_leadEta_ = Electrons[leadEleIndex]->eta();
                    leading_lepton = leading_electron;
                }

                //------------------------------//
                //#quadratic equation related
                vector<int> indices_bjet = get_bjet_indices(jets, btag_scores);
                bool is_moreThanTwoJets_and_atLeastOneBjet = jets.size() > 2 && indices_bjet.size() > 0;
                bool is_moreThanOneJets_and_atLeastOneBjet = jets.size() > 1 && indices_bjet.size() > 0;
                // init reco leading lepton and MET
                TLorentzVector lepton = leading_lepton; // leading lepton
                float met_pt    = MetPt_;
                float met_px    = MetPt_ * TMath::Cos(MetPhi_);
                float met_py    = MetPt_ * TMath::Sin(MetPhi_);
                vector<double> met_info = { met_pt, met_px, met_py };
                // 4-vectors
                double neutrino_pz           = evaluate_neutrino_pz(lepton, met_info);
                TLorentzVector reco_neutrino = derive_reco_neutrino(lepton, met_info);
                TLorentzVector reco_wboson   = derive_reco_wboson(lepton, reco_neutrino);

                TLorentzVector _nothing_;
                int index_bjet               = std::max_element(btag_scores.begin(), btag_scores.end()) - btag_scores.begin();
                TLorentzVector bjet          = is_moreThanOneJets_and_atLeastOneBjet ? jets[index_bjet]                   : _nothing_;
                TLorentzVector reco_tbw      = is_moreThanOneJets_and_atLeastOneBjet ? derive_reco_tbw(reco_wboson, bjet) : _nothing_;

                int index_q                  = get_q_index_min_chi2(jets, index_bjet, diphoton);
                TLorentzVector chi2_qjet = is_moreThanTwoJets_and_atLeastOneBjet ? jets[index_q]                      : _nothing_;
                TLorentzVector chi2_tqh  = is_moreThanTwoJets_and_atLeastOneBjet ? chi2_qjet + diphoton           : _nothing_;
                // variables
                chi2_neutrino_pz_        = neutrino_pz;
                chi2_tbw_mass_           = is_moreThanOneJets_and_atLeastOneBjet ? reco_tbw.M()                       : -999.;
                chi2_tbw_pt_             = is_moreThanOneJets_and_atLeastOneBjet ? reco_tbw.Pt()                      : -999.;
                chi2_tbw_eta_            = is_moreThanOneJets_and_atLeastOneBjet ? reco_tbw.Eta()                     : -999.;
                chi2_tbw_deltaR_dipho_   = is_moreThanOneJets_and_atLeastOneBjet ? reco_tbw.DeltaR(diphoton)          : -999.;
                chi2_qjet_pt_            = is_moreThanTwoJets_and_atLeastOneBjet ? chi2_qjet.Pt()                 : -999.;
                chi2_qjet_eta_           = is_moreThanTwoJets_and_atLeastOneBjet ? chi2_qjet.Eta()                : -999.;
                chi2_qjet_btag_          = is_moreThanTwoJets_and_atLeastOneBjet ? btag_scores[index_q]               : -999.;
                chi2_qjet_deltaR_dipho_  = is_moreThanTwoJets_and_atLeastOneBjet ? chi2_qjet.DeltaR(diphoton)     : -999.;
                chi2_tqh_ptOverM_        = is_moreThanTwoJets_and_atLeastOneBjet ? chi2_tqh.Pt()/chi2_tqh.M() : -999.;
                chi2_tqh_eta_            = is_moreThanTwoJets_and_atLeastOneBjet ? chi2_tqh.Eta()                 : -999.;
                chi2_tqh_deltaR_tbw_     = is_moreThanTwoJets_and_atLeastOneBjet ? chi2_tqh.DeltaR(reco_tbw)      : -999.;
                chi2_tqh_deltaR_dipho_   = is_moreThanTwoJets_and_atLeastOneBjet ? chi2_tqh.DeltaR(diphoton)      : -999.;
                //------------------------------//
                
                float mvaValue = DiphotonMva_-> EvaluateMVA( "BDT" );

                tthMvaVal_RunII_ = convert_tmva_to_prob(TThMva_RunII_->EvaluateMVA( "BDT" ));
                //fcncMvaVal_ = convert_tmva_to_prob(FCNCMva_->EvaluateMVA( "BDT" ));

                fcncMvaVal_NRB_ = convert_tmva_to_prob(FCNC_BDTNRB_RunII_->EvaluateMVA( "BDT" ));
                fcncMvaVal_SMH_ = convert_tmva_to_prob(FCNC_BDTSMH_RunII_->EvaluateMVA( "BDT" ));

                if (debug_) {
                  cout << "TTH Leptonic Tag -- input MVA variables for Run II MVA: " << endl;
                  cout << "--------------------------------------------------------" << endl;
                  cout << "maxIDMVA_: " << maxPhoID_ << endl;
                  cout << "minIDMVA_: " << minPhoID_ << endl;
                  cout << "max2_btag_: " << secondMaxBTagVal_noBB_ << endl;
                  cout << "max1_btag_: " << maxBTagVal_noBB_ << endl;
                  cout << "dipho_delta_R_: " << diPhoDeltaR_ << endl;

                  cout << "njets_: " << nJets_ << endl;
                  cout << "ht_: " << ht_ << endl;
                  cout << "leadptoM_: " << leadptom_ << endl;
                  cout << "subleadptoM_: " << subleadptom_ << endl;
                  cout << "lead_eta_: " << leadeta_ << endl;
                  cout << "sublead_eta_: " << subleadeta_ << endl;

                  cout << "jet1_pt_: " << jetPt_1_ << endl;
                  cout << "jet1_eta_: " << jetEta_1_ << endl;
                  cout << "jet1_btag_: " << btag_noBB_1_ << endl;
                  cout << "jet2_pt_: " << jetPt_2_ << endl;
                  cout << "jet2_eta_: " << jetEta_2_ << endl;
                  cout << "jet2_btag_: " << btag_noBB_2_ << endl;
                  cout << "jet3_pt_: " << jetPt_3_ << endl;
                  cout << "jet3_eta_: " << jetEta_3_ << endl;
                  cout << "jet3_btag_: " << btag_noBB_3_ << endl;
                  cout << "jet4_pt_: " << jetPt_4_ << endl;
                  cout << "jet4_eta_: " << jetEta_4_ << endl;
                  cout << "jet4_btag_: " << btag_noBB_4_ << endl;

                  cout << "leadPSV_: " << leadPSV_ << endl;
                  cout << "subleadPSV_: " << subleadPSV_ << endl;

                  cout << "dipho_cosphi_: " << diPhoCosPhi_ << endl;
                  cout << "dipho_rapidity_: " << diPhoY_ << endl;
                  cout << "met_: " << MetPt_ << endl;
                  cout << "dipho_pt_over_mass_: " << diPhoPtoM_ << endl;
                  cout << "helicity_angle_: " << helicity_angle_ << endl;
                  cout << "top_tag_score_: " << top_tag_score_ << endl;
                  cout << "fcnc_tag_score_tt_: " << fcnc_tag_score_tt_ << endl;
                  cout << "fcnc_tag_score_st_: " << fcnc_tag_score_st_ << endl;

                  //------------------------------//
                  cout << "chi2_neutrino_pz_:" << chi2_neutrino_pz_ << endl;
                  cout << "chi2_tbw_mass_:" << chi2_tbw_mass_ << endl;
                  cout << "chi2_tbw_pt_:" << chi2_tbw_pt_ << endl;
                  cout << "chi2_tbw_eta_:" << chi2_tbw_eta_ << endl;
                  cout << "chi2_tbw_deltaR_dipho_:" << chi2_tbw_deltaR_dipho_ << endl;
                  cout << "chi2_qjet_pt_:" << chi2_qjet_pt_ << endl;
                  cout << "chi2_qjet_eta_:" << chi2_qjet_eta_ << endl;
                  cout << "chi2_qjet_btag_:" << chi2_qjet_btag_ << endl;
                  cout << "chi2_qjet_deltaR_dipho_:" << chi2_qjet_deltaR_dipho_ << endl;
                  cout << "chi2_tqh_ptOverM_:" << chi2_tqh_ptOverM_ << endl;
                  cout << "chi2_tqh_eta_:" << chi2_tqh_eta_ << endl;
                  cout << "chi2_tqh_deltaR_tbw_:" << chi2_tqh_deltaR_tbw_ << endl;
                  cout << "chi2_tqh_deltaR_dipho_:" << chi2_tqh_deltaR_dipho_ << endl;
                  //------------------------------//

                  cout << "lep_pt_: " << lepton_leadPt_ << endl;
                  cout << "lep_eta_: " << lepton_leadEta_ << endl;
                  cout << "n_lep_tight_: " << lepton_nTight_ << endl;

                  cout << "DNN Score 0: " << dnn_score_0_ << endl;
                  cout << endl;
                  
                  cout << "BDT NRB Score: " << fcncMvaVal_NRB_ << endl;
                  cout << "BDT SMH Score: " << fcncMvaVal_SMH_ << endl;

              }

              global_features.clear();

              //mvaValue = fcncMvaVal_;
              //mvaValue = tthMvaVal_RunII_; // use Run II MVA for categorization

                int catNumber = -1;
                //catNumber = chooseCategory( mvaValue , debug_);  
                catNumber = chooseCategory( fcncMvaVal_NRB_ , fcncMvaVal_SMH_ );

                if (debug_) {
                        if (catNumber >= 0)
                            std::cout << "[FCNCLeptonicTagProducer DEBUG] Event tagged (Category " << catNumber << ", BDT-NRB score: " << fcncMvaVal_NRB_ << ", BDT-SMH score: " << fcncMvaVal_SMH_ << std::endl;
                        else
                            std::cout << "[FCNCLeptonicTagProducer DEBUG] Event NOT tagged (BDT-NRB score: " << fcncMvaVal_NRB_ << ", BDT-SMH score: " << fcncMvaVal_SMH_ << std::endl;
                 }                   

                if(catNumber!=-1)
                {
                    FCNCLeptonicTag tthltags_obj( dipho, mvares );
                    tthltags_obj.setCategoryNumber(catNumber);

                    for( unsigned int i = 0; i < tagJets.size(); ++i )
                    {
                        //tthltags_obj.includeWeightsByLabel( *tagJets[i] , "JetBTagReshapeWeight");
                        tthltags_obj.includeWeightsByLabel( *tagJets[i] , "JetCTagReshapeWeight");
                    }


                    for( unsigned int i = 0; i < Muons.size(); ++i )
                        tthltags_obj.includeWeights( *Muons.at(i));

                    for( unsigned int i = 0; i < Electrons.size(); ++i )
                        tthltags_obj.includeWeights( *Electrons.at(i));

                    tthltags_obj.includeWeights( *dipho );
                    tthltags_obj.setJets( tagJets );
                    tthltags_obj.setMuons( Muons );
                    tthltags_obj.setElectrons( Electrons );
                    tthltags_obj.setDiPhotonIndex( diphoIndex );
                    std::string syst_label = modifySystematicsWorkflow ? systematicsLabels[syst_idx] : systLabel_;
                    tthltags_obj.setSystLabel( syst_label );
                    tthltags_obj.setMvaRes(mvaValue);
                    tthltags_obj.setLepPt( lepPt );
                    tthltags_obj.setLepE( lepE );
                    tthltags_obj.setLepEta( lepEta );
                    tthltags_obj.setLepPhi( lepPhi );
                    tthltags_obj.setLepType( lepType );

                    tthltags_obj.setLeadPrompt(-999);
                    tthltags_obj.setLeadMad(-999);
                    tthltags_obj.setLeadPythia(-999);
                    tthltags_obj.setLeadPassFrix(-999);
                    tthltags_obj.setLeadSimpleMomID(-999);
                    tthltags_obj.setLeadSimpleMomStatus(-999);
                    tthltags_obj.setLeadMomID(-999);
                    tthltags_obj.setLeadMomMomID(-999);
                    tthltags_obj.setLeadSmallestDr(-999);

                    tthltags_obj.setSubleadPrompt(-999);
                    tthltags_obj.setSubleadMad(-999);
                    tthltags_obj.setSubleadPythia(-999);
                    tthltags_obj.setSubleadPassFrix(-999);
                    tthltags_obj.setSubleadSimpleMomID(-999);
                    tthltags_obj.setSubleadSimpleMomStatus(-999);
                    tthltags_obj.setSubleadMomID(-999);
                    tthltags_obj.setSubleadMomMomID(-999);
                    tthltags_obj.setSubleadSmallestDr(-999);
                                
                    tthltags->push_back( tthltags_obj );
            
                    if( ! evt.isRealData() )
                    {
                        int gp_lead_index = GenPhoIndex(genParticles, dipho->leadingPhoton(), -1);
                        int gp_sublead_index = GenPhoIndex(genParticles, dipho->subLeadingPhoton(), gp_lead_index);
                        vector<int> leadFlags; leadFlags.clear();
                        vector<int> subleadFlags; subleadFlags.clear();

                        if (gp_lead_index != -1) {
                           const edm::Ptr<reco::GenParticle> gp_lead = genParticles->ptrAt(gp_lead_index);                
                           leadFlags = IsPromptAfterOverlapRemove(genParticles, gp_lead);
                           tthltags->back().setLeadPrompt(leadFlags[0]);
                           tthltags->back().setLeadMad(leadFlags[1]);
                           tthltags->back().setLeadPythia(leadFlags[2]);
                           tthltags->back().setLeadPassFrix(leadFlags[3]);
                           tthltags->back().setLeadSimpleMomID(leadFlags[4]);
                           tthltags->back().setLeadSimpleMomStatus(leadFlags[5]);
                           tthltags->back().setLeadMomID(leadFlags[6]);
                           tthltags->back().setLeadMomMomID(leadFlags[7]);
                           tthltags->back().setLeadSmallestDr(NearestDr(genParticles, &(*gp_lead)));

                           //cout << "leadPrompt: " << leadFlags[0] << endl;
                           } 

                       if (gp_sublead_index != -1) {
                           const edm::Ptr<reco::GenParticle> gp_sublead = genParticles->ptrAt(gp_sublead_index);
                           subleadFlags = IsPromptAfterOverlapRemove(genParticles, gp_sublead);
                           tthltags->back().setSubleadPrompt(subleadFlags[0]);
                           tthltags->back().setSubleadMad(subleadFlags[1]);
                           tthltags->back().setSubleadPythia(subleadFlags[2]);
                           tthltags->back().setSubleadPassFrix(subleadFlags[3]);
                           tthltags->back().setSubleadSimpleMomID(subleadFlags[4]);
                           tthltags->back().setSubleadSimpleMomStatus(subleadFlags[5]);
                           tthltags->back().setSubleadMomID(subleadFlags[6]);
                           tthltags->back().setSubleadMomMomID(subleadFlags[7]);
                           tthltags->back().setSubleadSmallestDr(NearestDr(genParticles, &(*gp_sublead)));
                           }

                    }
                }
            } //diPho loop end !
            evt.put( std::move( tthltags ), systematicsLabels[syst_idx] );
        } // syst loop end !
    }

}
typedef flashgg::FCNCLeptonicTagProducer FlashggFCNCLeptonicTagProducer;
DEFINE_FWK_MODULE( FlashggFCNCLeptonicTagProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
