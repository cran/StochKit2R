// [[Rcpp::depends(BH)]]

#include <string>

#include "SSA_Direct.h"
#include "SSA_ODM.h"
#include "StandardDriverUtilities.h"
#include "ssaStochKit2Rtemplate.h"
#include "solver_helper_functions.h"

#include <Rcpp.h>

#if defined(_OPENMP)
#include "omp.h"
#endif

using namespace Rcpp;

//'@title C++ Interface to Gillespie Stochastic Simulation Algorithm
//'
//'@description
//'\code{ssa} Called by StochKit2R ssa function, do not call this C++ interface directly
//'
//'@param StochKit2Rmodel R list (Rcpp List built from buildStochKit2Rmodel output)
//'@param outputDirNameString Character string with path to output directory. Should end in path separator.
//'@param time Simulation time of each realization
//'@param realizations Number of realizations
//'@param intervals Number of output intervals. Default 0 outputs at end time only. 1=keep data at start and end time, 2=keep data at start, middle, and end times, etc. Note data is stored at (intervals+1) equally spaced time points.
//'@param keepStats Keep means and variances data
//'@param keepTrajectories Keep trajectory data
//'@param keepHistograms Keep histogram data
//'@param bins Number of histogram bins
//'@param seed Seed for random number generator
//'@param p  Override default and specify the number of processes (threads) to use. By default (=0), the number of processes will be determined automatically
//'@return NULL
//'@keywords internal
// [[Rcpp::export]]
void ssaStochKit2RInterface(Rcpp::List StochKit2Rmodel, std::string outputDirNameString, double time, int realizations, int intervals, bool keepStats, bool keepTrajectories, bool keepHistograms, int bins, unsigned int seed, int p) {

    Rcout << "StochKit2R MESSAGE: determining appropriate method...";

    int numberOfReactions=((Rcpp::List)StochKit2Rmodel[2]).size();
    int numberOfSpecies=((Rcpp::List)StochKit2Rmodel[1]).size();
    int n=1;//number of threads, may be set >1 later
    
#if defined(_OPENMP)
    if (p!=0) {
        omp_set_num_threads(p); //force use of p threads per user's request
    }
#pragma omp parallel
    {
        
#pragma omp single
        {
            n=omp_get_num_threads();
        }
    }
#pragma omp barrier
#endif
    
    std::string method=chooseMethod(numberOfReactions, realizations, n);
    
    if (numberOfSpecies<denseStoichiometryCutoff) {
        if (method=="odm") {
            Rcout << "running odm dense...\n";
            ssaStochKit2Rtemplate<STOCHKIT::StandardDriverTypes::denseStoichiometryType,
            STOCHKIT::SSA_ODM<STOCHKIT::StandardDriverTypes::populationType,
            STOCHKIT::StandardDriverTypes::denseStoichiometryType,
            STOCHKIT::StandardDriverTypes::propensitiesType,
            STOCHKIT::StandardDriverTypes::graphType> >(StochKit2Rmodel, outputDirNameString, time, realizations, intervals, keepStats, keepTrajectories, keepHistograms, bins, seed, p);
        } else if (method=="direct"){
            Rcout << "running direct dense...\n";
            ssaStochKit2Rtemplate<STOCHKIT::StandardDriverTypes::denseStoichiometryType,
                              STOCHKIT::SSA_Direct<STOCHKIT::StandardDriverTypes::populationType,
                                                   STOCHKIT::StandardDriverTypes::denseStoichiometryType,
                                                   STOCHKIT::StandardDriverTypes::propensitiesType,
                                                   STOCHKIT::StandardDriverTypes::graphType> >(StochKit2Rmodel, outputDirNameString, time, realizations, intervals, keepStats, keepTrajectories, keepHistograms, bins, seed, p);
        } else if (method=="constant") {
            Rcout << "running direct constant dense...\n";
            ssaStochKit2Rtemplate<STOCHKIT::StandardDriverTypes::denseStoichiometryType,
            STOCHKIT::SSA_ConstantTime<STOCHKIT::StandardDriverTypes::populationType,
            STOCHKIT::StandardDriverTypes::denseStoichiometryType,
            STOCHKIT::StandardDriverTypes::propensitiesType,
            STOCHKIT::StandardDriverTypes::graphType> >(StochKit2Rmodel, outputDirNameString, time, realizations, intervals, keepStats, keepTrajectories, keepHistograms, bins, seed, p);
        }
    } else {
        if (method=="odm") {
            Rcout << "running odm sparse...\n";
            ssaStochKit2Rtemplate<STOCHKIT::StandardDriverTypes::sparseStoichiometryType,
            STOCHKIT::SSA_ODM<STOCHKIT::StandardDriverTypes::populationType,
            STOCHKIT::StandardDriverTypes::sparseStoichiometryType,
            STOCHKIT::StandardDriverTypes::propensitiesType,
            STOCHKIT::StandardDriverTypes::graphType> >(StochKit2Rmodel, outputDirNameString, time, realizations, intervals, keepStats, keepTrajectories, keepHistograms, bins, seed, p);
        } else if (method=="direct"){
            Rcout << "running direct sparse...\n";
            ssaStochKit2Rtemplate<STOCHKIT::StandardDriverTypes::sparseStoichiometryType,
            STOCHKIT::SSA_Direct<STOCHKIT::StandardDriverTypes::populationType,
            STOCHKIT::StandardDriverTypes::sparseStoichiometryType,
            STOCHKIT::StandardDriverTypes::propensitiesType,
            STOCHKIT::StandardDriverTypes::graphType> >(StochKit2Rmodel, outputDirNameString, time, realizations, intervals, keepStats, keepTrajectories, keepHistograms, bins, seed, p);
        } else if (method=="constant") {
            Rcout << "running direct constant sparse...\n";
            ssaStochKit2Rtemplate<STOCHKIT::StandardDriverTypes::sparseStoichiometryType,
            STOCHKIT::SSA_ConstantTime<STOCHKIT::StandardDriverTypes::populationType,
            STOCHKIT::StandardDriverTypes::sparseStoichiometryType,
            STOCHKIT::StandardDriverTypes::propensitiesType,
            STOCHKIT::StandardDriverTypes::graphType> >(StochKit2Rmodel, outputDirNameString, time, realizations, intervals, keepStats, keepTrajectories, keepHistograms, bins, seed, p);
        }
    }
    Rcout << "done!\n";
}
