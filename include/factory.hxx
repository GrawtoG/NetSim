#ifndef NETSIM_FACTORY_HXX
#define NETSIM_FACTORY_HXX

#include "nodes.hxx"
#include "types.hxx"
#include <list>
#include <map>
#include <algorithm>
#include <istream>
#include <ostream>

template<class Node>
class NodeCollection {
public:
    using container_t = std::list<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

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
    const_iterator begin() const { return container_.cbegin(); }
    const_iterator end() const { return container_.cend(); }
    const_iterator cbegin() const { return container_.cbegin(); }
    const_iterator cend() const { return container_.cend(); }
private:
    container_t container_;
};

class Factory {
public:
    void add_ramp(Ramp&& r) { cont_r_.add(std::move(r)); }
    void remove_ramp(ElementID id) { cont_r_.remove_by_id(id); }
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) { return cont_r_.find_by_id(id); }
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const { return cont_r_.find_by_id(id); }
    NodeCollection<Ramp>::const_iterator ramp_cbegin() const { return cont_r_.cbegin(); }
    NodeCollection<Ramp>::const_iterator ramp_cend() const { return cont_r_.cend(); }

    void add_worker(Worker&& w) { cont_w_.add(std::move(w)); }
    void remove_worker(ElementID id);
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id) { return cont_w_.find_by_id(id); }
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const { return cont_w_.find_by_id(id); }
    NodeCollection<Worker>::const_iterator worker_cbegin() const { return cont_w_.cbegin(); }
    NodeCollection<Worker>::const_iterator worker_cend() const { return cont_w_.cend(); }

    void add_storehouse(Storehouse&& s) { cont_s_.add(std::move(s)); }
    void remove_storehouse(ElementID id);
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id) { return cont_s_.find_by_id(id); }
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const { return cont_s_.find_by_id(id); }
    NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const { return cont_s_.cbegin(); }
    NodeCollection<Storehouse>::const_iterator storehouse_cend() const { return cont_s_.cend(); }

    bool is_consistent() const;
    void do_deliveries(Time t);
    void do_package_passing();
    void do_work(Time t);

private:
    template<class Node>
    void remove_receiver(NodeCollection<Node>& collection, ElementID id);

    NodeCollection<Ramp> cont_r_;
    NodeCollection<Worker> cont_w_;
    NodeCollection<Storehouse> cont_s_;
};

Factory load_factory_structure(std::istream& is);
void save_factory_structure(Factory& factory, std::ostream& os);

template<class Node>
void Factory::remove_receiver(NodeCollection<Node>& collection, ElementID id) {
    auto iter = collection.find_by_id(id);
    if (iter == collection.end()) return;
    IPackageReceiver* receiver_ptr = dynamic_cast<IPackageReceiver*>(&(*iter));
    for (auto& ramp : cont_r_) { ramp.receiver_preferences_.remove_receiver(receiver_ptr); }
    for (auto& worker : cont_w_) { worker.receiver_preferences_.remove_receiver(receiver_ptr); }
}

#endif
