#include "repository/Repository.h"

#include "gtest/gtest.h"
#include <random>
//#include <ctime>
#include <chrono>

using namespace polaris;


TEST(Repository_Test, Normal_Cumulative_Distribution_Test)
{
    RNG_Components::Implementations::MT_Normal<NT> nrm1;
    float p1 = nrm1.Cumulative_Distribution<float>(0.5);
    nrm1.maximum();

    //RNG_Components::Prototypes::RNG<RNG_Components::Implementations::MT_Normal<NT> >* rng = &nrm1;
    //float rng_max = rng->maximum();

    RNG_Components::Implementations::MT_Probability<NT> prb;
    prb.Initialize<NT>();//Cumulative_Distribution<float>(0.5);

    RNG_Components::Implementations::MT_Uniform<NT> unfrm;
    unfrm.Initialize<NT>();//Cumulative_Distribution<float>(0.5);

    //EXPECT_NEAR((population-expected_population)/expected_population,0,.025);
}

TEST(Repository_Test, Bivariate_Normal_Cumulative_Distribution_Test)
{
    float p2 = GLOBALS::Bivariate_Normal_Distribution->Cumulative_Distribution_Value<float>(0, 0, 1);
}


TEST(Repository_Test, Basic_Units_Test)
{
    //Basic_Units::Implementations::Acceleration_Implementation
}
