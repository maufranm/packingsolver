#include "packingsolver/irregular/optimize.hpp"

#include "packingsolver/irregular/minlp.hpp"
#include "packingsolver/algorithms/column_generation.hpp"

#include "treesearchsolver/iterative_beam_search.hpp"
#include "treesearchsolver/anytime_column_search.hpp"

#include <thread>

using namespace packingsolver;
using namespace packingsolver::irregular;

Output packingsolver::irregular::optimize(
        const Instance& instance,
        OptimizeOptionalParameters parameters)
{
    Output output(instance);

    Algorithm algorithm = Algorithm::Minlp;
    std::stringstream ss;
    ss << algorithm;
    parameters.info.add_to_json("Algorithm", "Algorithm", ss.str());

    output.solution_pool.best().algorithm_start(parameters.info, algorithm);

    if (algorithm == Algorithm::Minlp) {
        MinlpOptionalParameters minlp_parameters;
        minlp_parameters.output_nl_path = parameters.output_nl_path;
        minlp_parameters.info = Info(parameters.info, false, "");
        auto minlp_output = minlp(instance, minlp_parameters);
        std::stringstream ss;
        output.solution_pool.add(minlp_output.solution_pool.best(), ss, parameters.info);
    }

    output.solution_pool.best().algorithm_end(parameters.info);
    return output;
}

