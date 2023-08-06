#pragma once

#include "packingsolver/rectangle/instance_builder.hpp"
#include "packingsolver/rectangle/solution.hpp"

#include "columngenerationsolver/linear_programming_solver.hpp"

namespace packingsolver
{
namespace rectangle
{

struct OptimizeOptionalParameters
{
    /** Number of threads. */
    Counter number_of_threads = 0;

    /** Algorithm. */
    Algorithm algorithm = Algorithm::Auto;


    /** Size of the queue in the tree search algorithm. */
    NodeId tree_search_queue_size = -1;

    /** Guides used in the tree search algorithm. */
    std::vector<GuideId> tree_search_guides;

    Solution* fixed_items = nullptr;


    /**
     * Time limit for the vbpp2bpp bin packing sub-problem of the column
     * generation algorithm.
     */
    double column_generation_vbpp2bpp_time_limit = -1;

    /**
     * Size of the queue for the vbpp2bpp bin packing sub-problem of the column
     * generation algorithm.
     */
    NodeId column_generation_vbpp2bpp_queue_size = 256;

    /**
     * Size of the queue for the pricing knapsack sub-problem of the column
     * generation algorithm.
     */
    NodeId column_generation_pricing_queue_size = 256;

    /** Linear programming solver. */
    columngenerationsolver::LinearProgrammingSolver linear_programming_solver
        = columngenerationsolver::LinearProgrammingSolver::CLP;


    /**
     * Size of the queue for the bin packing sub-problem of the dichotomic
     * search algorithm.
     */
    NodeId dichotomic_search_queue_size = 128;


    /** Info structure. */
    optimizationtools::Info info = optimizationtools::Info();
};

struct Output
{
    Output(const Instance& instance):
        solution_pool(instance, 1) { }

    SolutionPool<Instance, Solution> solution_pool;
};

Output optimize(
        const Instance& instance,
        OptimizeOptionalParameters parameters = {});

}
}

