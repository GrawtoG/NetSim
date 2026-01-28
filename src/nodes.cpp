#include "nodes.hxx"

#include <queue>
#include <queue>

#include "storage_types.hxx"

void Storehouse::add_products(std::list<Package> packages) {
    for (auto& package : packages){
        stockpile->push(std::move(package));
    }
}

void Worker::do_work(Time t) {
    if (!buffer.has_value() && !queue->empty()) {
        package_processing_start_time=t;
        push_package(queue->pop());
    }

    if (t-package_processing_start_time+1>processing_time) {
        send_package();
    }
}

void PackageSender::send_package() {
    if (!buffer.has_value()) return;
    receiver_preferences_.choose_receiver()->receive_package(std::move(*buffer));
    buffer.reset();
}


void ReceiverPreferences::add_receiver(IPackageReceiver *receiver) {
    preferences.emplace(receiver, 0.0);
    double chance=1.0/(preferences.size());
    for (auto& [_,c] : preferences) {
        c=chance;
    }
}

void ReceiverPreferences::remove_receiver(IPackageReceiver *receiver) {
    preferences.erase(receiver);
    double chance=1.0/(preferences.size());
    for (auto& [_,c] : preferences) {
        c=chance;
    }
}

IPackageReceiver* ReceiverPreferences::choose_receiver() {
    if (preferences.empty()) {
        return nullptr;
    }

    double lenght=generate_probability();
    for (auto& p : preferences) {
        lenght-=p.second;
        if (lenght<=0.0) {
            return p.first;
        }
    }
    return preferences.begin()->first;
}
void Ramp::deliver_goods(Time t) {
    if ((t - 1) % delivery_interval == 0) {
        push_package(Package());
    }
}
