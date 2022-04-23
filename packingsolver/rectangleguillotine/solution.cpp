#include "packingsolver/rectangleguillotine/solution.hpp"

using namespace packingsolver;
using namespace packingsolver::rectangleguillotine;

/************************************ Node ************************************/

std::ostream& print(std::ostream& os,
        const std::vector<Solution::Node>& res,
        SolutionNodeId id, std::string tab)
{
    os << tab << res[id] << std::endl;
    for (SolutionNodeId c: res[id].children)
        print(os, res, c, tab + "  ");
    return os;
}

std::ostream& packingsolver::rectangleguillotine::operator<<(std::ostream &os, const Solution::Node& node)
{
    os
        << "id " << node.id
        << " f " << node.f
        << " d " << node.d
        << " i " << node.i
        << " l " << node.l
        << " r " << node.r
        << " b " << node.b
        << " t " << node.t
        << " j " << node.j;
    return os;
}

/********************************** Solution **********************************/

Solution::Solution(const Solution& solution):
    instance_(solution.instance_),
    nodes_(solution.nodes_),
    number_of_items_(solution.number_of_items_),
    number_of_bins_(solution.number_of_bins_),
    area_(solution.area_),
    full_area_(solution.full_area_),
    item_area_(solution.item_area_),
    profit_(solution.profit_),
    cost_(solution.cost_),
    width_(solution.width_),
    height_(solution.height_),
    bin_copies_(solution.bin_copies_),
    item_copies_(solution.item_copies_)
{
}

Solution& Solution::operator=(const Solution& solution)
{
    if (this != &solution) {
        if (&instance_ != &solution.instance_) {
            throw std::runtime_error(
                    "Assign a solution to a solution from a different instance.");
        }

        nodes_           = solution.nodes_;
        number_of_items_ = solution.number_of_items_;
        number_of_bins_  = solution.number_of_bins_;
        area_            = solution.area_;
        full_area_       = solution.full_area_;
        item_area_       = solution.item_area_;
        profit_          = solution.profit_;
        cost_            = solution.cost_;
        width_           = solution.width_;
        height_          = solution.height_;
        bin_copies_      = solution.bin_copies_;
        item_copies_     = solution.item_copies_;
        assert(number_of_items_ >= 0);
    }
    return *this;
}

void Solution::add_node(const Node& node)
{
    nodes_.push_back(node);
    if (number_of_bins_ < node.i + 1) {
        number_of_bins_ = node.i + 1;
        bin_copies_[node.bin_type_id]++;
        cost_ += instance_.bin_type(node.bin_type_id).cost;
    }
    if (node.d == 0) {
        assert(node.b == 0);
        assert(node.l == 0);
        area_      += node.r * node.t;
        full_area_ += node.r * node.t;
    }
    if (node.j >= 0) {
        number_of_items_++;
        item_area_ += instance().item_type(node.j).rect.area();
        profit_    += instance().item_type(node.j).profit;
        item_copies_[node.j]++;
    }
    if (node.j == -3) // Subtract residual area
        area_ -= (node.t - node.b) * (node.r - node.l);
    // Update width_ and height_
    if (node.r != instance().bin(node.i).rect.w && width_ < node.r)
        width_ = node.r;
    if (node.t != instance().bin(node.i).rect.h && height_ < node.t)
        height_ = node.t;
}

Solution::Solution(const Instance& instance, const std::vector<Solution::Node>& nodes):
    instance_(instance),
    bin_copies_(instance.number_of_bin_types(), 0),
    item_copies_(instance.number_of_item_types(), 0)
{
    for (const Solution::Node& node: nodes)
        add_node(node);
}

void Solution::append(
        const Solution& solution,
        BinTypeId bin_type_id,
        const std::vector<ItemTypeId>& item_type_ids,
        BinPos copies)
{
    SolutionNodeId offset_node = nodes_.size();
    BinPos offset_bin = number_of_bins_;
    for (BinPos i = 0; i < copies; ++i) {
        for (Solution::Node node: solution.nodes()) {
            node.bin_type_id = bin_type_id;
            node.id += offset_node;
            if (node.f != -1)
                node.f += offset_node;
            if (node.j >= 0)
                node.j = item_type_ids[node.j];
            if (node.j == -3)
                node.j = -1;
            for (SolutionNodeId& child: node.children)
                child += offset_node;
            node.i += i + offset_bin;
            add_node(node);
        }
    }
}

bool Solution::operator<(const Solution& solution) const
{
    switch (instance().objective()) {
    case Objective::Default: {
        if (solution.profit() < profit())
            return false;
        if (solution.profit() > profit())
            return true;
        return solution.waste() < waste();
    } case Objective::BinPacking: {
        if (!solution.full())
            return false;
        if (!full())
            return true;
        return solution.number_of_bins() < number_of_bins();
    } case Objective::BinPackingWithLeftovers: {
        if (!solution.full())
            return false;
        if (!full())
            return true;
        return solution.waste() < waste();
    } case Objective::StripPackingWidth: {
        if (!solution.full())
            return false;
        if (!full())
            return true;
        return solution.width() < width();
    } case Objective::StripPackingHeight: {
        if (!solution.full())
            return false;
        if (!full())
            return true;
        return solution.height() < height();
    } case Objective::Knapsack: {
        return solution.profit() > profit();
    } case Objective::VariableSizedBinPacking: {
        if (!solution.full())
            return false;
        if (!full())
            return true;
        return solution.cost() < cost();
    } default: {
        std::stringstream ss;
        ss << "Solution rectangleguillotine::Solution does not support objective \""
            << instance().objective() << "\"";
        throw std::logic_error(ss.str());
    }
    }
}

void Solution::display(
        const std::stringstream& algorithm,
        Info& info) const
{
    info.output->number_of_solutions++;
    double t = info.elapsed_time();

    std::streamsize precision = std::cout.precision();
    std::string sol_str = "Solution" + std::to_string(info.output->number_of_solutions);
    switch (instance().objective()) {
    case Objective::Default: {
        FFOT_PUT(info, sol_str, "Profit", profit());
        FFOT_PUT(info, sol_str, "Full", (full())? 1: 0);
        FFOT_PUT(info, sol_str, "Waste", waste());
        FFOT_VER(info,
                   std::setw(12) << std::fixed << std::setprecision(3) << t << std::defaultfloat << std::setprecision(precision)
                << std::setw(12) << profit()
                << std::setw(6) << full()
                << std::setw(12) << waste()
                << std::setw(32) << algorithm.str()
                << std::endl);
        break;
    } case Objective::BinPacking: {
        FFOT_PUT(info, sol_str, "NumberOfBins", number_of_bins());
        FFOT_PUT(info, sol_str, "FullWastePercentage", 100 * full_waste_percentage());
        FFOT_VER(info,
                   std::setw(12) << std::fixed << std::setprecision(3) << t << std::defaultfloat << std::setprecision(precision)
                << std::setw(8) << number_of_bins()
                << std::setw(16) << std::fixed << std::setprecision(2) << 100 * full_waste_percentage() << std::defaultfloat << std::setprecision(precision)
                << std::setw(32) << algorithm.str()
                << std::endl);
        break;
    } case Objective::BinPackingWithLeftovers: {
        FFOT_PUT(info, sol_str, "Waste", waste());
        FFOT_PUT(info, sol_str, "WastePercentage", 100 * waste_percentage());
        FFOT_VER(info,
                   std::setw(12) << std::fixed << std::setprecision(3) << t << std::defaultfloat << std::setprecision(precision)
                << std::setw(12) << waste()
                << std::setw(12) << std::fixed << std::setprecision(2) << 100 * waste_percentage() << std::defaultfloat << std::setprecision(precision)
                << std::setw(32) << algorithm.str()
                << std::endl);
        break;
    } case Objective::StripPackingWidth: {
        FFOT_PUT(info, sol_str, "Width", width());
        FFOT_VER(info,
                   std::setw(12) << std::fixed << std::setprecision(3) << t << std::defaultfloat << std::setprecision(precision)
                << std::setw(12) << width()
                << std::setw(32) << algorithm.str()
                << std::endl);
        break;
    } case Objective::StripPackingHeight: {
        FFOT_PUT(info, sol_str, "Height", height());
        FFOT_VER(info,
                   std::setw(12) << std::fixed << std::setprecision(3) << t << std::defaultfloat << std::setprecision(precision)
                << std::setw(12) << height()
                << std::setw(32) << algorithm.str()
                << std::endl);
        break;
    } case Objective::Knapsack: {
        FFOT_PUT(info, sol_str, "Profit", profit());
        FFOT_VER(info,
                   std::setw(12) << std::fixed << std::setprecision(3) << t << std::defaultfloat << std::setprecision(precision)
                << std::setw(14) << profit()
                << std::setw(10) << number_of_items()
                << std::setw(32) << algorithm.str()
                << std::endl);
        break;
    } case Objective::VariableSizedBinPacking: {
        FFOT_PUT(info, sol_str, "Cost", cost());
        FFOT_PUT(info, sol_str, "NumberOfBins", number_of_bins());
        FFOT_PUT(info, sol_str, "FullWastePercentage", 100 * full_waste_percentage());
        FFOT_VER(info,
                   std::setw(12) << std::fixed << std::setprecision(3) << t << std::defaultfloat << std::setprecision(precision)
                << std::setw(14) << cost()
                << std::setw(8) << number_of_bins()
                << std::setw(16) << std::fixed << std::setprecision(2) << 100 * full_waste_percentage() << std::defaultfloat << std::setprecision(precision)
                << std::setw(32) << algorithm.str()
                << std::endl);
        break;
    } default: {
        std::stringstream ss;
        ss << "Solution rectangleguillotine::Solution does not support objective \""
            << instance().objective() << "\"";
        throw std::logic_error(ss.str());
    }
    }
    FFOT_PUT(info, sol_str, "Algorithm", algorithm.str());
    FFOT_PUT(info, sol_str, "Time", t);

    if (!info.output->only_write_at_the_end) {
        info.write_json_output();
        write(info);
    }
}

void Solution::algorithm_start(Info& info) const
{
    FFOT_VER(info,
               "===================================" << std::endl
            << "           PackingSolver           " << std::endl
            << "===================================" << std::endl
            << std::endl
            << "Problem type" << std::endl
            << "------------" << std::endl
            << "rectangleguillotine" << std::endl
            << std::endl
            << "Instance" << std::endl
            << "--------" << std::endl
            << "Objective:                " << instance().objective() << std::endl
            << "Number of item types:     " << instance().number_of_item_types() << std::endl
            << "Number of items:          " << instance().number_of_items() << std::endl
            << "Number of bin types:      " << instance().number_of_bin_types() << std::endl
            << "Number of bins:           " << instance().number_of_bins() << std::endl
            << "Number of stacks:         " << instance().number_of_stacks() << std::endl
            << "Number of defects:        " << instance().number_of_defects() << std::endl
            << std::endl);

    switch (instance().objective()) {
    case Objective::Default: {
        FFOT_VER(info,
                std::setw(12) << "Time"
                << std::setw(12) << "Profit"
                << std::setw(6) << "Full"
                << std::setw(12) << "Waste"
                << std::setw(32) << "Comment"
                << std::endl
                << std::setw(12) << "----"
                << std::setw(12) << "------"
                << std::setw(6) << "----"
                << std::setw(12) << "-----"
                << std::setw(32) << "-------"
                << std::endl);
        break;
    } case Objective::BinPacking: {
        FFOT_VER(info,
                std::setw(12) << "Time"
                << std::setw(8) << "Bins"
                << std::setw(16) << "Full waste (%)"
                << std::setw(32) << "Comment"
                << std::endl
                << std::setw(12) << "----"
                << std::setw(8) << "----"
                << std::setw(16) << "--------------"
                << std::setw(32) << "-------"
                << std::endl);
        break;
    } case Objective::BinPackingWithLeftovers: {
        FFOT_VER(info,
                std::setw(12) << "Time"
                << std::setw(12) << "Waste"
                << std::setw(12) << "Waste (%)"
                << std::setw(32) << "Comment"
                << std::endl
                << std::setw(12) << "----"
                << std::setw(12) << "-----"
                << std::setw(12) << "---------"
                << std::setw(32) << "-------"
                << std::endl);
        break;
    } case Objective::StripPackingWidth: {
        FFOT_VER(info,
                std::setw(12) << "Time"
                << std::setw(12) << "Width"
                << std::setw(32) << "Comment"
                << std::endl
                << std::setw(12) << "----"
                << std::setw(12) << "-----"
                << std::setw(32) << "-------"
                << std::endl);
        break;
    } case Objective::StripPackingHeight: {
        FFOT_VER(info,
                std::setw(12) << "Time"
                << std::setw(12) << "Height"
                << std::setw(32) << "Comment"
                << std::endl
                << std::setw(12) << "----"
                << std::setw(12) << "-----"
                << std::setw(32) << "-------"
                << std::endl);
        break;
    } case Objective::Knapsack: {
        FFOT_VER(info,
                std::setw(12) << "Time"
                << std::setw(14) << "Profit"
                << std::setw(10) << "# items"
                << std::setw(32) << "Comment"
                << std::endl
                << std::setw(12) << "----"
                << std::setw(14) << "------"
                << std::setw(10) << "-------"
                << std::setw(32) << "-------"
                << std::endl);
        break;
    } case Objective::VariableSizedBinPacking: {
        FFOT_VER(info,
                std::setw(12) << "Time"
                << std::setw(14) << "Cost"
                << std::setw(8) << "# bins"
                << std::setw(16) << "Full waste (%)"
                << std::setw(32) << "Comment"
                << std::endl
                << std::setw(12) << "----"
                << std::setw(14) << "-----"
                << std::setw(8) << "------"
                << std::setw(16) << "--------------"
                << std::setw(32) << "-------"
                << std::endl);
        break;
    } default: {
        std::stringstream ss;
        ss << "Solution rectangleguillotine::Solution does not support objective \""
            << instance().objective() << "\"";
        throw std::logic_error(ss.str());
    }
    }
}

void Solution::algorithm_end(Info& info) const
{
    double t = info.elapsed_time();

    std::string sol_str = "Solution";
    FFOT_VER(info,
            std::endl
            << "Final statistics" << std::endl
            << "----------------" << std::endl);
    switch (instance().objective()) {
    case Objective::Default: {
        FFOT_PUT(info, sol_str, "Profit", profit());
        FFOT_PUT(info, sol_str, "Full", (full())? 1: 0);
        FFOT_PUT(info, sol_str, "Waste", waste());
        FFOT_VER(info,
                   "Profit:           " << profit() << std::endl
                << "Full:             " << full() << std::endl
                << "Waste:            " << waste() << std::endl);
        break;
    } case Objective::BinPacking: {
        FFOT_PUT(info, sol_str, "NumberOfBins", number_of_bins());
        FFOT_PUT(info, sol_str, "FullWaste", full_waste());
        FFOT_PUT(info, sol_str, "FullWastePercentage", 100 * full_waste_percentage());
        FFOT_VER(info, "Number of bins:   " << number_of_bins() << std::endl);
        FFOT_VER(info, "Full waste (%):   " << 100 * full_waste_percentage() << std::endl);
        break;
    } case Objective::BinPackingWithLeftovers: {
        FFOT_PUT(info, sol_str, "Waste", waste());
        FFOT_PUT(info, sol_str, "WastePercentage", 100 * waste_percentage());
        FFOT_VER(info,
                   "Waste:             " << waste() << std::endl
                << "Waste (%):         " << 100 * waste_percentage() << std::endl);
        break;
    } case Objective::StripPackingWidth: {
        FFOT_PUT(info, sol_str, "Width", width());
        FFOT_VER(info, "Width:             " << width() << std::endl);
        break;
    } case Objective::StripPackingHeight: {
        FFOT_PUT(info, sol_str, "Height", height());
        FFOT_VER(info, "Height:            " << height() << std::endl);
        break;
    } case Objective::Knapsack: {
        FFOT_PUT(info, sol_str, "Profit", profit());
        FFOT_VER(info,
                   "Profit:            " << profit() << std::endl
                << "Number of items:   " << number_of_items() << std::endl);
        break;
    } case Objective::VariableSizedBinPacking: {
        FFOT_PUT(info, sol_str, "Cost", cost());
        FFOT_PUT(info, sol_str, "NumberOfBins", number_of_bins());
        FFOT_PUT(info, sol_str, "FullWastePercentage", 100 * full_waste_percentage());
        FFOT_VER(info,
                   "Cost:              " << cost() << std::endl
                << "Number of bins:    " << number_of_bins() << std::endl
                << "Full waste (%):    " << 100 * full_waste_percentage() << std::endl);
        break;
    } default: {
        std::stringstream ss;
        ss << "Solution rectangleguillotine::Solution does not support objective \""
            << instance().objective() << "\"";
        throw std::logic_error(ss.str());
    }
    }
    FFOT_PUT(info, sol_str, "Time", t);
    FFOT_VER(info, "Time:              " << t << std::endl);

    info.write_json_output();
    write(info);
}

void Solution::write(Info& info) const
{
    if (info.output->certificate_path.empty())
        return;
    std::ofstream f{info.output->certificate_path};
    if (!f.good()) {
        throw std::runtime_error(
                "Unable to open file \"" + info.output->certificate_path + "\".");
    }

    f << "PLATE_ID,NODE_ID,X,Y,WIDTH,HEIGHT,TYPE,CUT,PARENT" << std::endl;
    for (const Solution::Node& n: nodes_) {
        f
            << n.i << ","
            << n.id << ","
            << n.l << ","
            << n.b << ","
            << n.r - n.l << ","
            << n.t - n.b << ","
            << n.j << ","
            << n.d << ",";
        if (n.f != -1)
            f << n.f;
        f << std::endl;
    }
    f.close();
}
