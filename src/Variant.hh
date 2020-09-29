#ifndef VARIANT_HH
#define VARIANT_HH 1

/****************************************************************************
** Variant.hh
**
** Class for storing basic variant information storage
**
**  Authors: Giuseppe Narzisi
**    Date: November 5, 2015
**
*****************************************************************************/

/************************** COPYRIGHT ***************************************
**
** New York Genome Center
**
** SOFTWARE COPYRIGHT NOTICE AGREEMENT
** This software and its documentation are copyright (2016) by the New York
** Genome Center. All rights are reserved. This software is supplied without
** any warranty or guaranteed support whatsoever. The New York Genome Center
** cannot be responsible for its use, misuse, or functionality.
**
** Version: 1.0.0
** Author: Giuseppe Narzisi
**
*************************** /COPYRIGHT **************************************/

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <iostream>
#include <limits>
#include <string>

#include "FET.hh"
#include "util.hh"

using namespace std;

struct Filters {
  // filter thresholds]
  double minPhredFisherSTR;
  double minPhredFisher;
  double maxVafNormal;
  double minVafTumor;
  int minCovNormal;
  int maxCovNormal;
  int minCovTumor;
  int maxCovTumor;
  int minAltCntTumor;
  int maxAltCntNormal;
  int minStrandBias;
};

class Variant_t {
 public:
  unsigned short kmer;
  string chr;
  int pos;
  char type;
  unsigned short len;
  string ref;
  string alt;
  string str;
  char status;  // T=somatic, S=shared
  int ref_cov_normal_fwd;
  int ref_cov_normal_rev;
  int ref_cov_tumor_fwd;
  int ref_cov_tumor_rev;
  int alt_cov_normal_fwd;
  int alt_cov_normal_rev;
  int alt_cov_tumor_fwd;
  int alt_cov_tumor_rev;
  char prev_bp_ref;  // base-pair preceding the mutation in reference
  char prev_bp_alt;  // base-pair preceding the mutation in alternative

  string GT_normal;  // normal gentype
  string GT_tumor;   // tumor genotype

  Filters* filters;  // filter thresholds

  Variant_t(string chr_, int pos_, string ref_, string alt_,
            int ref_cov_normal_fwd_, int ref_cov_normal_rev_,
            int ref_cov_tumor_fwd_, int ref_cov_tumor_rev_,
            int alt_cov_normal_fwd_, int alt_cov_normal_rev_,
            int alt_cov_tumor_fwd_, int alt_cov_tumor_rev_, char prev_bp_ref_,
            char prev_bp_alt_, Filters* fs, int k, string str_, char code) {
    kmer = k;
    str = str_;
    filters = fs;
    chr = chr_;
    pos = pos_;

    /*
    if(ref_.at(0) == '-') { type = 'I'; ref_ = ""; len = alt_.length(); }  //
    deletion if(alt_.at(0) == '-') { type = 'D'; alt_ = ""; len = ref_.length();
    }  // insertion if(ref_.size()==1 && alt_.size()==1 && ref_.at(0)!='-' &&
    alt_.at(0)!='-') { type = 'S'; pos++; } // snp
    */

    if (code == '^') {
      type = 'I';
      ref_ = "";
      len = alt_.length();
    }  // deletion
    if (code == 'v') {
      type = 'D';
      alt_ = "";
      len = ref_.length();
    }  // insertion
    if (code == 'x') {
      type = 'S';
      pos++;
    }                   // snp
    if (code == 'c') {  // complex
      type = 'C';
      ref_.erase(remove(ref_.begin(), ref_.end(), '-'), ref_.end());
      alt_.erase(remove(alt_.begin(), alt_.end(), '-'), alt_.end());

      unsigned short rl = ref_.length();
      unsigned short al = alt_.length();
      if (rl == al) {
        len = al;
      } else if (rl > al) {
        len = rl - al;
      } else {
        len = al - rl;
      }
      // cout << "R:" << ref_.length() << " A:" << alt_.length() << " LEN:" <<
      // len << endl;
    }

    if (type != 'S') {
      ref = prev_bp_alt_ + ref_;
      alt = prev_bp_alt_ + alt_;
    } else {
      alt = alt_;
      ref = ref_;
      len = 1;
    }

    ref_cov_normal_fwd = ref_cov_normal_fwd_;
    ref_cov_normal_rev = ref_cov_normal_rev_;
    ref_cov_tumor_fwd = ref_cov_tumor_fwd_;
    ref_cov_tumor_rev = ref_cov_tumor_rev_;

    alt_cov_normal_fwd = alt_cov_normal_fwd_;
    alt_cov_normal_rev = alt_cov_normal_rev_;
    alt_cov_tumor_fwd = alt_cov_tumor_fwd_;
    alt_cov_tumor_rev = alt_cov_tumor_rev_;
    prev_bp_ref = prev_bp_ref_;
    prev_bp_alt = prev_bp_alt_;

    // compute genotype
    reGenotype();
  }

  void printVCF();
  string genotype(int R, int A);
  string getGenotypeNormal() { return GT_normal; }
  string getGenotypeTumor() { return GT_tumor; }
  void reGenotype();
  char bestState(int Rn, int An, int Rt, int At);
  string getSignature();
  string getPosition();
  double compute_FET_score();
  double compute_SB_score();
};

#endif
