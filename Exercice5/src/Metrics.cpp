/************************************
 * Définition de la classe Metrics. *
 ************************************/

#include "Metrics.hpp"

/***********
 * speedup *
 ***********/

double 
Metrics::speedup(const double& seq, const double& par) {
  return seq / par;
}

/**************
 * efficiency *
 **************/

double 
Metrics::efficiency(const double& seq, 
		    const double& par,
		    const unsigned& procs) {
  return speedup(seq, par) / procs;
}

