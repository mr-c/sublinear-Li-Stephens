#ifndef PENALTY_SET_H
#define PENALTY_SET_H

#include "math.hpp"
#include "DP_map.hpp"

using namespace std;

// stores a shared set of penalty-derived coefficients for use in calculations
// according to our model
struct penaltySet{
  int H;
  double log_H;
  double rho;
  double mu;
  double one_minus_mu;
  double one_minus_2mu;
  
  // log of (1 - 2*rho)
  double alpha_value;
  // log of (1 - 2*rho + H*rho)
  double beta_value;
  
  penaltySet(double logRho, double logMu, int H);
  ~penaltySet();
  
  double span_coefficient(size_t l) const;
  double alpha(size_t l) const;
  double beta(size_t l) const;
  double span_mutation_penalty(size_t l, size_t a) const;
  
  DPUpdateMap get_match_map(double last_sum) const;
  DPUpdateMap get_non_match_map(double last_sum) const;
  DPUpdateMap get_current_map(double last_sum, bool match_is_rare) const;
  double get_minority_map_correction(bool match_is_rare) const;
  void update_S(double& S, const vector<double>& summands, bool match_is_rare) const;
  
  // double mu_val(alleleValue from, alleleValue to) const;
  // double mu_loss_val(alleleValue from) const;
  // double rho_val(size_t position) const;
  // double rho_loss_val(size_t position) const;
};

#endif