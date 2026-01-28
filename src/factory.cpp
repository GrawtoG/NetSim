#include "factory.hxx"
#include <map>
#include <stdexcept>

bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors) {
    if (node_colors[sender] == NodeColor::VERIFIED) return true;
    node_colors[sender] = NodeColor::VISITED;

    const auto& prefs = sender->receiver_preferences_.get_preferences();
    if (prefs.empty()) throw std::logic_error("Brak odbiorców");

    bool path_to_store = false;
    for (auto& [receiver, prob] : prefs) {
        if (receiver->get_receiver_type() == ReceiverType::STOREHOUSE) {
            path_to_store = true;
        } else if (receiver->get_receiver_type() == ReceiverType::WORKER) {
            Worker* worker = dynamic_cast<Worker*>(receiver);
            PackageSender* sendrecv = dynamic_cast<PackageSender*>(worker);
            if (sendrecv == sender) continue;
            
            if (node_colors[sendrecv] == NodeColor::UNVISITED) {
                if (has_reachable_storehouse(sendrecv, node_colors)) path_to_store = true;
            } else if (node_colors[sendrecv] == NodeColor::VERIFIED) {
                path_to_store = true;
            }
        }
    }
    node_colors[sender] = NodeColor::VERIFIED;
    return path_to_store;
}

bool Factory::is_consistent() const {
    std::map<const PackageSender*, NodeColor> colors;
    for (const auto& r : cont_r_) colors[dynamic_cast<const PackageSender*>(&r)] = NodeColor::UNVISITED;
    for (const auto& w : cont_w_) colors[dynamic_cast<const PackageSender*>(&w)] = NodeColor::UNVISITED;

    try {
        for (const auto& ramp : cont_r_) {
            if (!has_reachable_storehouse(dynamic_cast<const PackageSender*>(&ramp), colors)) return false;
        }
    } catch (const std::logic_error&) { return false; }
    return true;
}

void Factory::remove_worker(ElementID id) { remove_receiver(cont_w_, id); cont_w_.remove_by_id(id); }
void Factory::remove_storehouse(ElementID id) { remove_receiver(cont_s_, id); cont_s_.remove_by_id(id); }

void Factory::do_deliveries(Time t) { for (auto& r : cont_r_) r.deliver_goods(t); }
void Factory::do_work(Time t) { for (auto& w : cont_w_) w.do_work(t); }
void Factory::do_package_passing() {
    for (auto& r : cont_r_) r.send_package();
    for (auto& w : cont_w_) w.send_package();
}
