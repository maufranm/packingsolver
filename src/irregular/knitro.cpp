/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *  This example demonstrates how to use Knitro to solve the following
 *  simple nonlinear optimization problem.  This model is test problem
 *  HS15 from the Hock & Schittkowski collection.
 *
 *  min   100 (x1 - x0^2)^2 + (1 - x0)^2
 *
 *  s.t.  x0 x1 >= 1
 *        x0 + x1^2 >= 0
 *
 *        x0 <= 0.5
 *
 *  The standard start point (-2, 1) usually converges to the standard
 *  minimum at (0.5, 2.0), with final objective = 306.5.
 *  Sometimes the solver converges to another local minimum
 *  at (-0.79212, -1.26243), with final objective = 360.4.
 *++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "knitrocpp/knitro.hpp"

#include <iostream>

int main(int, char**)
{
    // Create a new Knitro context.
    knitrocpp::Context knitro_context;

    // Add variables.
    struct Variables
    {   
        std::vector<knitrocpp::VariableId> liste_var = {};
        //for (tous les polygons in liste_binaire_poly)
        // knitrocpp::VariableId x_i
        // knitrocpp::VariableId x_i+1
        // liste_var.push_back(x_i)
        // liste_var.push_back(x_i+1)        
    };
    Variables variables;
    
    //for all i
    //liste_var[i].x = knitro_context.add_var();
    //de meme pour x
    variables.x0 = knitro_context.add_var();
    variables.x1 = knitro_context.add_var();

    // Variable bounds. bord container ?
    // x0 <= 0.5
    knitro_context.set_var_upbnd(
            variables.x0,
            0.5);

    // Initial point.   aller chercher pos init pour chaque poly 
    knitro_context.set_var_primal_init_value(
            variables.x0,
            -2.0);
    knitro_context.set_var_primal_init_value(
            variables.x1,
            1.0);

    
    // Objective.
    // min 100 (x1 - x0^2)^2 + (1 - x0)^2
    CB_context* callback_context = knitro_context.add_eval_callback(
            true,  // evaluate objective?
            {},  // constraints
            [&variables](
                const knitrocpp::Context&,
                CB_context*,
                KN_eval_request_ptr const eval_request,
                KN_eval_result_ptr const eval_result)
            {
                const double* x = eval_request->x;  //contient val de cgaque var
                //double tmp1 = x[variables.x1] - x[variables.x0] * x[variables.x0];
                //double tmp2 = 1.0 - x[variables.x0];
                *eval_result->obj = overlap(x);
                return 0;
            });
    knitro_context.set_cb_grad(
            callback_context,
            nullptr,
            nullptr,
            nullptr,
            [&variables](
                const knitrocpp::Context&,
                CB_context*,
                KN_eval_request_ptr const eval_request,
                KN_eval_result_ptr const eval_result)
            {   
                const double* x = eval_request->x;
                //double tmp = x[variables.x1] - x[variables.x0] * x[variables.x0];
                //forall i nb poly
                //if i in list_poly, i in list binaire_choix_poly
                //evalresult-> objGrad[liste_var[2*i]] = grad(x,i,0); coord x
                //evalresult-> objGrad[liste_var[2*i+1]] =  grad(x,i,1); coord y !!!! DEPLACEMENT DE POLYGONE 1 MOYEN, (pas poly 2)
                
                eval_result->objGrad[variables.x0]
                    = (-400.0 * tmp * x[variables.x0])
                    - (2.0 * (1.0 - x[variables.x0]));
                eval_result->objGrad[variables.x1] = 200.0 * tmp;
                return 0;
            });

    // Solve.
    int knitro_return_status = knitro_context.solve();

    // Print results.
    std::cout
        << "Return status: " << knitro_return_status << std::endl
        << "Objective value: " << knitro_context.get_obj_value() << std::endl
        << "Absolute feasibility error: " << knitro_context.get_abs_feas_error() << std::endl
        << "Relative feasibility error: " << knitro_context.get_rel_feas_error() << std::endl
        << "Absolute optimality error: " << knitro_context.get_abs_opt_error() << std::endl
        << "Relative optimality error: " << knitro_context.get_rel_opt_error() << std::endl
        << "x0: " << knitro_context.get_var_primal_value(variables.x0) << std::endl
        << "x1: " << knitro_context.get_var_primal_value(variables.x1) << std::endl
        ;
}