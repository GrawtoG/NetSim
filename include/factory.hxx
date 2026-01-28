#pragma once
#include <algorithm>

#include "nodes.hxx"
#include "storage_types.hxx"

bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors);

template<typename Node>
class NodeCollection {
public:
    using container_t = std::list<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    void add(Node&& node) {container.emplace_back(std::move(node));}
    void remove_by_id(ElementID id) {container.remove_if([id](const Node& elem) {return elem.get_id() == id;});
    }

    NodeCollection<Node>::iterator find_by_id(ElementID id) {
        return std::find_if(container.begin(), container.end(),
                            [id](const Node& elem) {
                                return elem.get_id() == id;
                            });
    }

    NodeCollection<Node>::const_iterator find_by_id(ElementID id) const {
        return std::find_if(container.cbegin(), container.cend(),
                            [id](const Node& elem) {
                                return elem.get_id() == id;
                            });
    }

    iterator begin() { return container.begin(); }
    iterator end() { return container.end(); }

    const_iterator begin() const { return container.cbegin(); }
    const_iterator end() const { return container.cend(); }
    const_iterator cbegin() const { return container.cbegin(); }
    const_iterator cend() const { return container.cend(); }

private:
    container_t container;
};


class Factory {
private:

    template<class Node>
    void remove_receiver(NodeCollection<Node>& collection, ElementID id);

    NodeCollection<Ramp> container_ramps;
    NodeCollection<Worker> container_workers;
    NodeCollection<Storehouse> container_storehouse;

public:

    void add_ramp(Ramp&& r) { container_ramps.add(std::move(r)); }
    void remove_ramp(ElementID id) { container_ramps.remove_by_id(id); }

    void add_worker(Worker&& w) { container_workers.add(std::move(w)); }
    void remove_worker(ElementID id);

    void add_storehouse(Storehouse&& s) { container_storehouse.add(std::move(s)); }
    void remove_storehouse(ElementID id);

    bool is_consistent() const;
    void do_deliveries(Time);
    void do_package_passing();
    void do_work(Time);

    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) { return container_ramps.find_by_id(id); }
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const { return container_ramps.find_by_id(id); }
    NodeCollection<Ramp>::const_iterator ramp_cbegin() const { return container_ramps.cbegin(); }
    NodeCollection<Ramp>::const_iterator ramp_cend() const { return container_ramps.cend(); }
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id) { return container_workers.find_by_id(id); }
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const { return container_workers.find_by_id(id); }
    NodeCollection<Worker>::const_iterator worker_cbegin() const { return container_workers.cbegin(); }
    NodeCollection<Worker>::const_iterator worker_cend() const { return container_workers.cend(); }
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id) { return container_storehouse.find_by_id(id); }
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const {
        return container_storehouse.find_by_id(id);
    }
    NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const { return container_storehouse.cbegin(); }
    NodeCollection<Storehouse>::const_iterator storehouse_cend() const { return container_storehouse.cend(); }
};

