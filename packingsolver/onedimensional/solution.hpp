#pragma once

#include "packingsolver/onedimensional/instance.hpp"

#include <sstream>

namespace packingsolver
{
namespace onedimensional
{

struct SolutionItem
{
    /** Item type. */
    ItemTypeId item_type_id;

    /** Initial z-coordinate of the item. */
    Length start;
};

struct SolutionBin
{
    /** Bin type. */
    BinTypeId bin_type_id;

    /** Number of copies. */
    BinPos copies;

    /** End. */
    Length end = 0;

    /** Weight. */
    Weight weight = 0;

    /** Stacks. */
    std::vector<SolutionItem> items;
};

/**
 * Solution class for a problem of type "onedimensional".
 */
class Solution
{

public:

    /*
     * Constructors and destructor.
     */

    /** Standard constructor. */
    Solution(const Instance& instance):
        instance_(&instance),
        bin_copies_(instance.number_of_bin_types(), 0),
        item_copies_(instance.number_of_item_types(), 0)
    { }

    /** Add a bin at the end of the solution. */
    BinPos add_bin(
            BinTypeId i,
            BinPos copies);

    /** Add an item to the solution. */
    void add_item(
            BinPos i,
            ItemTypeId j);

    void append(
            const Solution& solution,
            BinPos bin_pos,
            BinPos copies,
            const std::vector<BinTypeId>& bin_type_ids = {},
            const std::vector<ItemTypeId>& item_type_ids = {});

    void append(
            const Solution& solution,
            const std::vector<BinTypeId>& bin_type_ids,
            const std::vector<ItemTypeId>& item_type_ids);

    /*
     * Getters
     */

    /** Get the instance. */
    inline const Instance& instance() const { return *instance_; }

    /** Get the number of items in the solution. */
    inline ItemPos number_of_items() const { return number_of_items_; }

    /** Return 'tree' iff the solution contains all items. */
    inline bool full() const { return number_of_items() == instance().number_of_items(); }

    /** Get the number of bins in the solution. */
    inline BinPos number_of_bins() const { return number_of_bins_; }

    /** Get the number of different bins in the solution. */
    inline BinPos number_of_different_bins() const { return bins_.size(); }

    /** Get the profit of the solution. */
    inline Profit profit() const { return profit_; }

    /** Get the cost of the solution. */
    inline Profit cost() const { return cost_; }

    /** Get the length of the solution. */
    inline Volume length() const { return length_; }

    /** Get the total length of the items of the solution. */
    inline Area item_length() const { return item_length_; }

    /** Get the total length of the bins of the solution. */
    inline Area bin_length() const { return bin_length_; }

    /** Get the waste of the solution. */
    inline Area waste() const { return length_ - item_length_; }

    /** Get the fraction of waste of the solution. */
    inline double waste_percentage() const { return (length() == 0)? 0: (double)waste() / length(); }

    /** Get the waste of the solution including the residual. */
    inline Area full_waste() const { return bin_length() - item_length(); }

    /** Get the fraction of waste of the solution including the residual. */
    inline double full_waste_percentage() const { return (bin_length() == 0)? 0.0: (double)full_waste() / bin_length(); }

    /** Get the number of copies of item 'j' in the solution. */
    inline ItemPos item_copies(ItemTypeId j) const { return item_copies_[j]; }

    /** Get a bin. */
    const SolutionBin& bin(BinPos i) const { return bins_[i]; }

    /** Get the number of copies of bin 'i' in the solution. */
    inline BinPos bin_copies(BinTypeId i) const { return bin_copies_[i]; }

    bool operator<(const Solution& solution) const;

    /** CSV export */
    void write(Info& info) const;

    void algorithm_start(Info& info, Algorithm algorithm) const;

    void algorithm_end(Info& info) const;

    void display(
            const std::stringstream& algorithm,
            Info& info) const;

private:

    /*
     * Private attributes.
     */

    /** Instance. */
    const Instance* instance_;

    /** Bins. */
    std::vector<SolutionBin> bins_;

    /** Number of bins. */
    BinPos number_of_bins_ = 0;

    /** Number of items. */
    BinPos number_of_items_ = 0;

    /** Total length of the solution. */
    Volume length_ = 0;

    /** Total length of the bins of the solution. */
    Volume bin_length_ = 0;

    /** Total length of the items of the solution. */
    Volume item_length_ = 0;

    /** Profit of the solution. */
    Profit profit_ = 0;

    /** Cost of the solution. */
    Profit cost_ = 0;

    /** Number of copies of each bin type in the solution. */
    std::vector<BinPos> bin_copies_;

    /** Number of copies of each item type in the solution. */
    std::vector<ItemPos> item_copies_;

};

std::ostream& operator<<(std::ostream &os, const SolutionItem& item);

std::ostream& operator<<(std::ostream &os, const Solution& solution);

}
}
