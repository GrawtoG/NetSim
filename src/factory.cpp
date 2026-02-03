#include "factory.hxx"
#include <map>
#include <stdexcept>
#include <sstream>

bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors) {
    if (node_colors[sender] == NodeColor::VERIFIED) return true;
    node_colors[sender] = NodeColor::VISITED;
    const auto& prefs = sender->receiver_preferences_.get_preferences();
    if (prefs.empty()) throw std::logic_error("Brak odbiorców");

    bool can_reach_storehouse = false;

    for (auto& [receiver, probability] : prefs) {
        if (receiver->get_receiver_type() == ReceiverType::STOREHOUSE) {
            can_reach_storehouse = true;
        } else if (receiver->get_receiver_type() == ReceiverType::WORKER) {
            Worker* worker = dynamic_cast<Worker*>(receiver);
            PackageSender* next_sender = dynamic_cast<PackageSender*>(worker);
            
            if (next_sender == sender) continue;

            if (node_colors[next_sender] == NodeColor::UNVISITED) {
                if (has_reachable_storehouse(next_sender, node_colors)) {
                    can_reach_storehouse = true;
                }
            } else if (node_colors[next_sender] == NodeColor::VERIFIED) {
                can_reach_storehouse = true;
            }
        }
    }

    node_colors[sender] = NodeColor::VERIFIED;

    if (!can_reach_storehouse) {
        throw std::logic_error("Brak ścieżki do magazynu");
    }

    return true;
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
static std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream token_stream(s);
    while (std::getline(token_stream, token, delimiter)) {
        if (!token.empty()) tokens.push_back(token);
    }
    return tokens;
}

Factory load_factory_structure(std::istream& is) {
    Factory factory;
    std::string line;
    while (std::getline(is, line)) {
        if (line.empty() || line[0] == ';') continue;
        auto tokens = split(line, ' ');
        std::map<std::string, std::string> params;
        for (size_t i = 1; i < tokens.size(); ++i) {
            auto kv = split(tokens[i], '=');
            if (kv.size() == 2) params[kv[0]] = kv[1];
        }
        if (tokens[0] == "LOADING_RAMP") {
            factory.add_ramp(Ramp(std::stoi(params.at("id")), std::stoi(params.at("delivery-interval"))));
        } else if (tokens[0] == "WORKER") {
            PackageQueueType q_type = (params.at("queue-type") == "LIFO") ? PackageQueueType::LIFO : PackageQueueType::FIFO;
            factory.add_worker(Worker(std::stoi(params.at("id")), std::stoi(params.at("processing-time")), std::make_unique<PackageQueue>(q_type)));
        } else if (tokens[0] == "STOREHOUSE") {
            factory.add_storehouse(Storehouse(std::stoi(params.at("id"))));
        } else if (tokens[0] == "LINK") {
            auto src = split(params.at("src"), '-');
            auto dest = split(params.at("dest"), '-');
            IPackageReceiver* receiver = (dest[0] == "worker") ? (IPackageReceiver*)&(*factory.find_worker_by_id(std::stoi(dest[1]))) : (IPackageReceiver*)&(*factory.find_storehouse_by_id(std::stoi(dest[1])));
            if (src[0] == "ramp") factory.find_ramp_by_id(std::stoi(src[1]))->receiver_preferences_.add_receiver(receiver);
            else factory.find_worker_by_id(std::stoi(src[1]))->receiver_preferences_.add_receiver(receiver);
        }
    }
    return factory;
}

void save_factory_structure(Factory& factory, std::ostream& os) {
    for (auto it = factory.ramp_cbegin(); it != factory.ramp_cend(); ++it) {
        os << "LOADING_RAMP id=" << it->get_id() << " delivery-interval=" << it->get_delivery_interval() << "\n";
    }
    for (auto it = factory.worker_cbegin(); it != factory.worker_cend(); ++it) {
        os << "WORKER id=" << it->get_id() << " processing-time=" << it->get_processing_duration() << " queue-type=" << (it->get_queue()->get_queue_type() == PackageQueueType::LIFO ? "LIFO" : "FIFO") << "\n";
    }
    for (auto it = factory.storehouse_cbegin(); it != factory.storehouse_cend(); ++it) {
        os << "STOREHOUSE id=" << it->get_id() << "\n";
    }
    auto save_links = [&](const auto& sender, std::string name) {
        for (auto& [rec, prob] : sender.receiver_preferences_.get_preferences()) {
            os << "LINK src=" << name << "-" << sender.get_id() << " dest=" << (rec->get_receiver_type() == ReceiverType::WORKER ? "worker" : "store") << "-" << rec->get_id() << "\n";
        }
    };
    for (auto it = factory.ramp_cbegin(); it != factory.ramp_cend(); ++it) save_links(*it, "ramp");
    for (auto it = factory.worker_cbegin(); it != factory.worker_cend(); ++it) save_links(*it, "worker");
}
