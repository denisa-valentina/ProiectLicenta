#include "BDT.h"
#include "parameters.h"

bool (*split_fn)(const input_t*, const threshold_t*) = !strcmp(splitting_convention,"<=") ? [](const input_t *a, const threshold_t *b) { return *a <= *b; } : [](const input_t *a, const threshold_t *b) { return *a < *b;};

template<>
void BDT::BDT<n_trees, n_classes, n_features, input_arr_t, score_t, weight_t, threshold_t>::tree_scores(input_arr_t x, score_t scores[fn_classes(n_classes)][n_trees]) const {
  scores[0][0] = tree_0_0.decision_function(x, split_fn);
  scores[0][1] = tree_0_1.decision_function(x, split_fn);
  scores[0][2] = tree_0_2.decision_function(x, split_fn);
  scores[0][3] = tree_0_3.decision_function(x, split_fn);
  scores[0][4] = tree_0_4.decision_function(x, split_fn);
  scores[0][5] = tree_0_5.decision_function(x, split_fn);
  scores[0][6] = tree_0_6.decision_function(x, split_fn);
  scores[0][7] = tree_0_7.decision_function(x, split_fn);
  scores[0][8] = tree_0_8.decision_function(x, split_fn);
  scores[0][9] = tree_0_9.decision_function(x, split_fn);
  scores[0][10] = tree_0_10.decision_function(x, split_fn);
  scores[0][11] = tree_0_11.decision_function(x, split_fn);
  scores[0][12] = tree_0_12.decision_function(x, split_fn);
  scores[0][13] = tree_0_13.decision_function(x, split_fn);
  scores[0][14] = tree_0_14.decision_function(x, split_fn);
  scores[0][15] = tree_0_15.decision_function(x, split_fn);
  scores[0][16] = tree_0_16.decision_function(x, split_fn);
  scores[0][17] = tree_0_17.decision_function(x, split_fn);
  scores[0][18] = tree_0_18.decision_function(x, split_fn);
  scores[0][19] = tree_0_19.decision_function(x, split_fn);
  scores[0][20] = tree_0_20.decision_function(x, split_fn);
  scores[0][21] = tree_0_21.decision_function(x, split_fn);
  scores[0][22] = tree_0_22.decision_function(x, split_fn);
  scores[0][23] = tree_0_23.decision_function(x, split_fn);
  scores[0][24] = tree_0_24.decision_function(x, split_fn);
  scores[0][25] = tree_0_25.decision_function(x, split_fn);
  scores[0][26] = tree_0_26.decision_function(x, split_fn);
  scores[0][27] = tree_0_27.decision_function(x, split_fn);
  scores[0][28] = tree_0_28.decision_function(x, split_fn);
  scores[0][29] = tree_0_29.decision_function(x, split_fn);
}

