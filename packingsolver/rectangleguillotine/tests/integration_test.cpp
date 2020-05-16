#include "packingsolver/rectangleguillotine/branching_scheme.hpp"
#include "packingsolver/algorithms/astar.hpp"

#include <gtest/gtest.h>

using namespace packingsolver;
using namespace packingsolver::rectangleguillotine;

TEST(RectangleGuillotineBranchingScheme, ConvertionDefect)
{
    /**
     * Test added to fix a bug when converting nodes to solutions with
     * two-staged guillotine cuts and defects.
     *
     * |-------------------------|------------------------| 3210
     * |                         |                        |
     * |                         |                        | 3000
     * |                         |                        |
     * |                         |                        |
     * |                         |                        |
     * |                         |                        |
     * |                         |                        |
     * |                         | x                      |
     * |                         |------------------------| 500
     * |                         |                        |
     * |-------------------------|------------------------|
     *                         3000                     6000
     */

    Info info;

    Instance instance(Objective::BinPackingWithLeftovers);
    instance.add_item(3000, 3210, -1, 1, false, true);
    instance.add_item(3000, 500, -1, 1, false, true);
    instance.add_bin(6000, 3210);
    instance.add_defect(0, 3100, 600, 2, 2);

    BranchingScheme::Parameters p;
    p.set_roadef2018();
    p.cut_type_1 = CutType1::TwoStagedGuillotine;
    BranchingScheme branching_scheme(instance, p);
    BranchingScheme::Node node(branching_scheme);

    BranchingScheme::Insertion i0 = {.j1 = 0, .j2 = -1, .df = -2, .x1 = 3210, .y2 = 3000, .x3 = 3210, .x1_max = 3210, .y2_max = 6000, .z1 = 0, .z2 = 0};
    std::vector<BranchingScheme::Insertion> is0 = node.children(info);
    EXPECT_NE(std::find(is0.begin(), is0.end(), i0), is0.end());
    node.apply_insertion(i0, info);

    BranchingScheme::Insertion i1 = {.j1 = 1, .j2 = -1, .df = 1, .x1 = 3210, .y2 = 6000, .x3 = 500, .x1_max = 3210, .y2_max = 6000, .z1 = 0, .z2 = 0};
    std::vector<BranchingScheme::Insertion> is1 = node.children(info);
    EXPECT_NE(std::find(is1.begin(), is1.end(), i1), is1.end());
    node.apply_insertion(i1, info);

    Solution solution(instance);
    solution = node.convert(solution);
}

TEST(RectangleGuillotineBranchingScheme, IntegrationC1)
{
    Info info = Info()
        //.set_log2stderr(true)
        ;
    Instance instance(Objective::BinPackingWithLeftovers,
            "data/rectangle/tests/C1_items.csv",
            "data/rectangle/tests/C1_bins.csv",
            "data/rectangle/tests/C1_defects.csv");
    BranchingScheme::Parameters p;
    p.set_roadef2018();
    BranchingScheme branching_scheme(instance, p);
    Solution solution(instance);
    AStar<Solution, BranchingScheme> astar(solution, branching_scheme, 0, 5, info);
    astar.run();
    EXPECT_EQ(solution.waste(), 0);
}

TEST(RectangleGuillotineBranchingScheme, IntegrationC2)
{
    Info info;
    Instance instance(Objective::BinPackingWithLeftovers,
            "data/rectangle/tests/C2_items.csv",
            "data/rectangle/tests/C2_bins.csv",
            "data/rectangle/tests/C2_defects.csv");
    BranchingScheme::Parameters p;
    p.set_roadef2018();
    BranchingScheme branching_scheme(instance, p);
    Solution solution(instance);
    AStar<Solution, BranchingScheme> astar(solution, branching_scheme, 0, 5, info);
    astar.run();
    EXPECT_EQ(solution.waste(), 210 * 5700);
}

TEST(RectangleGuillotineBranchingScheme, IntegrationC3)
{
    Info info;
    Instance instance(Objective::BinPackingWithLeftovers,
            "data/rectangle/tests/C3_items.csv",
            "data/rectangle/tests/C3_bins.csv",
            "data/rectangle/tests/C3_defects.csv");
    BranchingScheme::Parameters p;
    p.set_roadef2018();
    BranchingScheme branching_scheme(instance, p);
    Solution solution(instance);
    AStar<Solution, BranchingScheme> astar(solution, branching_scheme, 0, 5, info);
    astar.run();
    EXPECT_EQ(solution.waste(), 0);
}

TEST(RectangleGuillotineBranchingScheme, IntegrationC11)
{
    /*
     *
     * |--------------|---------------| |------------------------------| 3210
     * |              |    x     x    | |                              |
     * |              |---------------| |                              | 3000
     * |              |               | |                              |
     * |              |               | |                              |
     * |              |               | |                              |
     * |              |               | |---|                          | 1520
     * |              |               | |   |                          |
     * |     0        |               | | 2 |                          |
     * |              |               | |   |                          |
     * |              |       1       | |---|                          | 20
     * |              |               | | x                            |
     * |--------------|---------------| |------------------------------|
     *              3000           6000 0  210
     *                   4000   5000     10
     *
     */

    Info info;
    Instance instance(Objective::BinPackingWithLeftovers,
            "data/rectangle/tests/C11_items.csv",
            "data/rectangle/tests/C11_bins.csv",
            "data/rectangle/tests/C11_defects.csv");
    BranchingScheme::Parameters p;
    p.set_roadef2018();
    p.cut_type_2 = CutType2::Exact;
    BranchingScheme branching_scheme(instance, p);
    Solution solution(instance);
    AStar<Solution, BranchingScheme> astar(solution, branching_scheme, 0, 5, info);
    astar.run();
    EXPECT_EQ(solution.waste(), 3210 * (6000 + 210) - instance.item_area());
}

TEST(RectangleGuillotineBranchingScheme, IntegrationDefect1)
{
    /**
     *
     * |-------------------------|------------------------| 3210
     * |-------------------------|                        |
     * |                         |                        |
     * |            1            |                        |
     * |                         |                        |
     * |-------------------------|                        | 1500
     * |                 xxx                              |
     * |----------|      xxx                              | 1000
     * |          |      xxx                              |
     * |     0    |                                       |
     * |          |                                       |
     * |----------|---------------------------------------|
     *           500    1000   1500                     6000
     */

    Info info;

    Instance instance(Objective::BinPackingWithLeftovers);
    instance.add_item(500, 1000, -1, 1, false, true);
    instance.add_item(1500, 1600, -1, 1, false, false);
    instance.add_bin(6000, 3210);
    instance.add_defect(0, 950, 950, 100, 100);

    BranchingScheme::Parameters p;
    p.set_roadef2018();
    BranchingScheme branching_scheme(instance, p);
    BranchingScheme::Node node(branching_scheme);

    Solution solution(instance);
    AStar<Solution, BranchingScheme> astar(solution, branching_scheme, 0, 5, info);
    astar.run();
    EXPECT_EQ(solution.waste(), 3210 * 1500 - instance.item_area());
}

TEST(RectangleGuillotineBranchingScheme, IntegrationDefect2)
{
    /**
     *
     * |-------------------------|------------------------| 3210
     * |-------------------------|                        |
     * |                         |                        |
     * |            2            |                        |
     * |                         |                        |
     * |------------------|------|                        | 1500
     * |                 x|      |                        | 1250
     * |----------|       |      |                        | 1000
     * |          |       |  1   |                        |
     * |     0    |       |      |                        |
     * |          |       |      |                        |
     * |----------|-------|------|------------------------|
     *           500    1000   1500                     6000
     */

    Info info;

    Instance instance(Objective::BinPackingWithLeftovers);
    instance.add_item(500, 1000, -1, 1, false, true);
    instance.add_item(500, 1500, -1, 1, false, false);
    instance.add_item(1500, 1600, -1, 1, false, false);
    instance.add_bin(6000, 3210);
    instance.add_defect(0, 950, 1250, 50, 50);

    BranchingScheme::Parameters p;
    p.set_roadef2018();
    BranchingScheme branching_scheme(instance, p);
    BranchingScheme::Node node(branching_scheme);

    Solution solution(instance);
    AStar<Solution, BranchingScheme> astar(solution, branching_scheme, 0, 5, info);
    astar.run();
    EXPECT_EQ(solution.waste(), 3210 * 1500 - instance.item_area());
}

TEST(RectangleGuillotineBranchingScheme, IntegrationDefect3)
{
    /**
     *
     * |----------------------------|---------------------| 3210
     * |                            |                     |
     * |                            |                     |
     * |                            |                     |
     * |                            |                     |
     * |                            |                     |
     * |                          x |         2           |
     * |-----------------------|    |                     | 1000
     * |           1           |    |                     |
     * |-----------------------|-|  |                     | 500
     * |            0            |  |                     |
     * |-------------------------|--|---------------------|
     *                         2900 3050                6000
     *                       2800
     */

    Info info;

    Instance instance(Objective::BinPackingWithLeftovers);
    instance.add_item(500, 2900, -1, 1, false, true);
    instance.add_item(500, 2800, -1, 1, false, false);
    instance.add_item(2950, 3210, -1, 1, false, false);
    instance.add_bin(6000, 3210);
    instance.add_defect(0, 2950, 1500, 100, 100);

    BranchingScheme::Parameters p;
    p.set_roadef2018();
    BranchingScheme branching_scheme(instance, p);
    BranchingScheme::Node node(branching_scheme);

    Solution solution(instance);
    AStar<Solution, BranchingScheme> astar(solution, branching_scheme, 0, 5, info);
    astar.run();
    EXPECT_EQ(solution.waste(), 3210 * 6000 - instance.item_area());
}

