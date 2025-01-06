#include "irregular/local_search.hpp"
#include "irregular/glouton.hpp"

#include "packingsolver/irregular/algorithm_formatter.hpp"

using namespace packingsolver;
using namespace packingsolver::irregular;

LocalSearchOutput packingsolver::irregular::local_search(
        const Instance& instance,
        const LocalSearchParameters& parameters)
{
    LocalSearchOutput output(instance);
    AlgorithmFormatter algorithm_formatter(instance, parameters, output);
    algorithm_formatter.start();
    algorithm_formatter.print_header();

    if (instance.number_of_item_types() == 0)
        return output;

    // TODO do something here.
    Solution best_sol = glouton(instance);
    // Update best solution.
    std::stringstream ss;
    ss << "DEBUG: best_sol.number_of_items()" << best_sol.number_of_items();
    ss << "iteration " << "greedy";
    algorithm_formatter.update_solution(best_sol, ss.str());

    algorithm_formatter.end();
    return output;
}
