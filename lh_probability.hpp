#ifndef LINEAR_HAPLO_PROBABILITY_H
#define LINEAR_HAPLO_PROBABILITY_H

#include "lh_math.hpp"
#include "lh_reference.hpp"
#include "lh_input_haplotype.hpp"
#include "lh_DP_map.hpp"
#include "lh_delay_multiplier.hpp"

using namespace std;

// stores a shared set of penalty-derived coefficients for use in calculations
// according to our model
struct penaltySet{
  int H;
  double log_H;
  double log_rho;
  double log_mu;
  double log_rho_complement;
  double log_mu_complement;
  double log_2mu_complement;
  
  // log of (1 - 2*rho)
  double log_ft_base;
  // log of (1 - 2*rho + H*rho)
  double log_fs_base;
  
  penaltySet(double logRho, double logMu, int H);
  ~penaltySet();
};

// A haplotypeMatrix is the matrix which iteratively calculates haplotype
// likelihood. It takes in a haplotypeCohort and a linearReferenceStructure, an
// inputHaplotype built against the linearReferenceStructure, and a penaltySet
// of mutation and recombination penalties. It calculates and returns the
// likelihood of the inputHaplotype relative to the haplotypeCohort when
// calculate_probability is called
struct haplotypeMatrix{
private:
  linearReferenceStructure* reference;
  haplotypeCohort* cohort;
  penaltySet* penalties;
  inputHaplotype* query;
  delayMap map;
  
  // This is used when the inputHaplotype begins with a span
  double initial_R;
  
  // trackers for the last indices extended. spans are indexed according to
  // the site preceding them, except for index -1, the span before site 0
  // -1 : nothing extended; i : index i last extended
  int last_extended = -1;
  // -2 : nothing extended; indexing as above
  int last_span_extended = -2;
  
  bool last_extended_is_span();
  
  size_t size();
  

public:
  double S;
  vector<double> R;
  
  haplotypeMatrix(linearReferenceStructure* ref, penaltySet* pen,
            haplotypeCohort* haplotypes, inputHaplotype* query);
  ~haplotypeMatrix();
  double calculate_probability();

  void account_for_initial_span();
  void extend_first_site();
  
  delayMap get_map();

  // TODO: implement these 
  // double estimate_probability_at_site(size_t j, alleleValue a);
  // helper functions for probability estimation
  // double minContinuing(int j);
  // double minMutating(int j);
  // double maxSwitching(int j);
  
  // checks for a span before the first site
  void initialize_probability();
  void extend_probability_at_site(size_t j, alleleValue a);
  void extend_probability_at_site(size_t j);
  void extend_probability_at_span_after(size_t j, 
              int augmentation_count);
              
  void take_snapshot();
  
  vector<size_t> get_matches(size_t i);
  size_t number_matching(size_t i);
  size_t number_not_matching(size_t i);
};

double calculate_R(double oldR, DPUpdateMap map);
double calculate_R(double oldR, double coefficient, double constant);

#endif