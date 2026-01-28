#ifndef NETSIM_FACTORY_HXX
#define NETSIM_FACTORY_HXX

#include "nodes.hxx"
#include "types.hxx"
#include <list>
#include <map>
#include <string>
#include <algorithm>

template<class Node>
class NodeCollection {
public:
    using container_t = std::list<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;
    const_iterator begin() const { return container_.begin(); }
    const_iterator end() const { return container_.end(); }
    void add(Node&& node) { container_.emplace_back(std::move(node)); }
    void remove_by_id(ElementID id) { 
        container_.remove_if([id](const Node& elem) { return elem.get_id() == id; }); 
    }
    iterator find_by_id(ElementID id) {
        return std::find_if(container_.begin(), container_.end(), [id](const Node& elem) { return elem.get_id() == id; });
    }
    const_iterator find_by_id(ElementID id) const {
        return std::find_if(container_.cbegin(), container_.cend(), [id](const Node& elem) { return elem.get_id() == id; });
    }

    iterator begin() { return container_.begin(); }
    iterator end() { return container_.end(); }
    const_iterator cbegin() const { return container_.cbegin(); }
    const_iterator cend() const { return container_.cend(); }

private:
    container_t container_;
};

class Factory {
public:

    bool is_consistent() const;
    void do_deliveries(Time t);
    void do_package_passing();
    void do_work(Time t);
    void remove_worker(ElementID id);
    void remove_storehouse(ElementID id);

private:
    template<class Node>
    void remove_receiver(NodeCollection<Node>& collection, ElementID id);

    NodeCollection<Ramp> cont_r_;
    NodeCollection<Worker> cont_w_;
    NodeCollection<Storehouse> cont_s_;
};

template<class Node>
void Factory::remove_receiver(NodeCollection<Node>& collection, ElementID id) {
    auto iter = collection.find_by_id(id);
    if (iter == collection.end()) return;

    IPackageReceiver* receiver_ptr = dynamic_cast<IPackageReceiver*>(&(*iter));

    for (auto& ramp : cont_r_) { ramp.receiver_preferences_.remove_receiver(receiver_ptr); }
    for (auto& worker : cont_w_) { worker.receiver_preferences_.remove_receiver(receiver_ptr); }
}

#endif
