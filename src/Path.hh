#ifndef PATH_HH
#define PATH_HH 1

/****************************************************************************
** Path.hh
**
** Path of a de Bruijn graph
** Routines to extract sequence and coverage information
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

#include <sstream>
#include <string>
#include <vector>

#include "Edge.hh"
#include "Node.hh"

using namespace std;

// Path_t
//////////////////////////////////////////////////////////////////////////

class Path_t {
 public:
  // members
  //////////////////////////////////////////////////////////////

  vector<Node_t *> nodes_m;
  vector<Edge_t *> edges_m;
  vector<Edgedir_t> edgedir_m;
  Ori_t dir_m;
  int len_m;
  int hasCycle_m;

  int match_bp;
  int snp_bp;
  int ins_bp;
  int del_bp;
  int K;
  int score;
  int flag;

  Path_t(int k) {
    reset();
    K = k;
  }

  // copy constructor
  //////////////////////////////////////////////////////////////

  Path_t(const Path_t &o, int k) {
    K = k;
    nodes_m = o.nodes_m;
    edges_m = o.edges_m;
    edgedir_m = o.edgedir_m;
    dir_m = o.dir_m;

    hasCycle_m = o.hasCycle_m;

    len_m = o.len_m;
    match_bp = o.match_bp;
    snp_bp = o.snp_bp;
    ins_bp = o.ins_bp;
    del_bp = o.del_bp;
    score = o.score;
    flag = o.flag;
  }

  // copy constructor
  //////////////////////////////////////////////////////////////

  Path_t(Path_t *&o, int k) {
    K = k;
    nodes_m = o->nodes_m;
    edges_m = o->edges_m;
    edgedir_m = o->edgedir_m;
    dir_m = o->dir_m;

    hasCycle_m = o->hasCycle_m;

    len_m = o->len_m;
    match_bp = o->match_bp;
    snp_bp = o->snp_bp;
    ins_bp = o->ins_bp;
    del_bp = o->del_bp;
    score = o->score;
    flag = o->flag;
  }

  // reset
  //////////////////////////////////////////////////////////////

  void reset() {
    nodes_m.clear();
    edges_m.clear();
    edgedir_m.clear();

    hasCycle_m = 0;
    len_m = 0;
    match_bp = 0;
    snp_bp = 0;
    ins_bp = 0;
    del_bp = 0;
    score = 0;
    flag = 1;
  }

  int strlen() { return len_m + K - 2; }
  Node_t *curNode() const { return nodes_m[nodes_m.size() - 1]; }

  string pathstr();
  int pathlen();
  string str();
  float cov(char sample);
  float mincov(char sample);
  float maxcov(char sample);
  Node_t *pathcontig(int pos);
  int hasCycle(Node_t *node);
  bool hasTumorOnlyNode();
  vector<cov_t> covDistr(char sample);
  vector<float> readCovNodes();
};

#endif
