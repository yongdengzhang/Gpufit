#include "cpufit.h"
#include "interface.h"
#include "profile.h"

#include <string>

std::string last_error ;

int cpufit
(
    size_t n_fits,
    size_t n_points,
    float * data,
    float * weights,
    int model_id,
    float * initial_parameters,
    float tolerance,
    int max_n_iterations,
    int * parameters_to_fit,
    int estimator_id,
    size_t user_info_size,
    char * user_info,
    float * output_parameters,
    int * output_states,
    float * output_chi_squares,
    int * output_n_iterations
)
try
{
    std::chrono::high_resolution_clock::time_point t1, t2, t3;
    t1 = std::chrono::high_resolution_clock::now();
    __int32 n_points_32 = 0;
    if (n_points <= (unsigned int)(std::numeric_limits<__int32>::max()))
    {
        n_points_32 = __int32(n_points);
    }
    else
    {
        throw std::runtime_error("maximum number of data points per fit exceeded");
    }

    FitInterface fi(
        data,
        weights,
        n_fits,
        n_points_32,
        tolerance,
        max_n_iterations,
        estimator_id,
        initial_parameters,
        parameters_to_fit,
        user_info,
        user_info_size,
        output_parameters,
        output_states,
        output_chi_squares,
        output_n_iterations);
    t2 = std::chrono::high_resolution_clock::now();

    fi.fit(model_id);

    t3 = std::chrono::high_resolution_clock::now();

    profiler.initialize_LM += t2 - t1;
    profiler.all += t3 - t1;

	display_profiler_results();

	return STATUS_OK;
}
catch (std::exception & exception)
{
    last_error = exception.what();

    return STATUS_ERROR;
}
catch (...)
{
    last_error = "Unknown Error";

    return STATUS_ERROR;
}

char const * cpufit_get_last_error()
{
    return last_error.c_str();
}