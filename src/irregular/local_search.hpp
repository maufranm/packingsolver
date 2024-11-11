/**
 * Local search algorithm
 */

#pragma once

#include "packingsolver/irregular/solution.hpp"

namespace packingsolver
{
namespace irregular
{

struct LocalSearchOutput: packingsolver::Output<Instance, Solution>
{
    /** Constructor. */
    LocalSearchOutput(const Instance& instance):
        packingsolver::Output<Instance, Solution>(instance) { }


    /** Number of iterations. */
    Counter number_of_iterations = 0;
};

struct LocalSearchParameters: packingsolver::Parameters<Instance, Solution>
{
};

LocalSearchOutput local_search(
        const Instance& instance,
        const LocalSearchParameters& parameters);

}
}
