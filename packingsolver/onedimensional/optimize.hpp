#pragma once

#include "packingsolver/onedimensional/solution.hpp"

#include "columngenerationsolver/linear_programming_solver.hpp"

namespace packingsolver
{
namespace onedimensional
{

struct Output: packingsolver::Output<Instance, Solution>
{
    Output(const Instance& instance):
        packingsolver::Output<Instance, Solution>(instance) { }
};

using NewSolutionCallback = std::function<void(const Output&)>;

struct OptimizeParameters: packingsolver::Parameters<Instance, Solution>
{
    /** New solution callback. */
    NewSolutionCallback new_solution_callback = [](const Output&) { };

    /** Enable anytime mode. */
    bool anytime = true;

    /** Force using a single thread. */
    bool sequential = false;

    /** Linear programming solver. */
    columngenerationsolver::LinearProgrammingSolver linear_programming_solver
        = columngenerationsolver::LinearProgrammingSolver::CLP;

    /** Guides used in the tree search algorithm. */
    std::vector<GuideId> tree_search_guides;

    /** Threshold to consider that a bin contains "many" items. */
    Counter many_items_in_bins_threshold = 16;

    /** Factor to consider that the number of copies of items is "high". */
    Counter many_item_type_copies_factor = 1;

    /**
     * Size of the queue for the pricing knapsack subproblem of the sequential
     * value correction algorithm.
     */
    NodeId sequential_value_correction_subproblem_queue_size = 256;

    /** Number of iterations of the sequential value correction algorithm. */
    Counter sequential_value_correction_number_of_iterations = 32;

    /**
     * Size of the queue for the pricing knapsack subproblem of the column
     * generation algorithm.
     */
    NodeId column_generation_subproblem_queue_size = 256;

    /*
     * Parameters for non-anytime mode
     */

    /** Size of the queue in the tree search algorithm. */
    NodeId not_anytime_tree_search_queue_size = 256;

    /**
     * Size of the queue in the single knapsack subproblem of the sequential
     * single knapsack algorithm.
     */
    NodeId not_anytime_sequential_single_knapsack_subproblem_queue_size = 256;

    /** Number of iterations of the sequential value correction algorithm. */
    Counter not_anytime_sequential_value_correction_number_of_iterations = 32;

    /**
     * Size of the queue in the bin packing subproblem of the dichotomic search
     * algorithm.
     */
    NodeId not_anytime_dichotomic_search_subproblem_beam_size = 256;
};

const Output optimize(
        const Instance& instance,
        const OptimizeParameters& parameters = {});

}
}
