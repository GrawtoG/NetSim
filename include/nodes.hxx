#pragma once
#include <map>
#include <queue>
#include "helpers.hxx"
#include "storage_types.hxx"
#include "types.hxx"
#include "package.hxx"

class IPackageReceiver {
public:
    using const_iterator = IPackageStockpile::const_iterator;
    virtual ~IPackageReceiver() = default;
    virtual void receive_package(Package&& p)=0;
    virtual ElementID get_id() const=0;
    virtual const_iterator begin() const = 0;
    virtual const_iterator end() const = 0;
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator cend() const = 0;

};
class ReceiverPreferences {
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;

    ReceiverPreferences(ProbabilityGenerator pg = probability_generator) : generate_probability(
            std::move(pg)) {};

    void add_receiver(IPackageReceiver* receiver);

    void remove_receiver(IPackageReceiver* receiver);

    IPackageReceiver* choose_receiver();

    const preferences_t& get_preferences() const {return this->preferences;};

    const_iterator cbegin() const { return preferences.cbegin(); };

    const_iterator cend() const { return preferences.cend(); };

    const_iterator begin() const { return preferences.begin(); };

    const_iterator end() const { return preferences.end(); };

private:
    preferences_t preferences;

    ProbabilityGenerator generate_probability;
};
class PackageSender {
public:
    PackageSender()=default;
    PackageSender(PackageSender&& p) = default;
    void send_package();
    const std::optional<Package>& get_sending_buffer() const{return buffer;};
    ReceiverPreferences receiver_preferences_;

protected:
    void push_package(Package&& package) {buffer.emplace(package);};
    std::optional<Package> buffer=std::nullopt;

};

class Ramp: public PackageSender {
    public:
        Ramp(ElementID id, TimeOffset di): ramp_id(id),delivery_interval(di) {};
        ElementID get_id() const {return ramp_id;};
        int get_delivery_interval() const {return delivery_interval;};
        void deliver_goods(Time t);


    private:
        ElementID ramp_id;
        TimeOffset delivery_interval;
        Time delivery_processing_start_time;

};
class Storehouse:public IPackageReceiver {
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> stockpile_ptr=std::make_unique<PackageQueue>(PackageQueueType::FIFO)): stockpile(std::move(stockpile_ptr)), storehouse_id(id) {};
    IPackageStockpile* get_stockpile() const {return stockpile.get(); };
    void add_products(std::list<Package>);
    void receive_package(Package&& package) override{stockpile->push(std::move(package));};
    ElementID get_id() const override{return storehouse_id;}
    const_iterator begin() const override{return stockpile->begin();};
    const_iterator end() const override{return stockpile->end();};
    const_iterator cbegin() const override{return stockpile->cbegin();};
    const_iterator cend() const override{return stockpile->cend();};
private:
        std::unique_ptr<IPackageStockpile> stockpile;
        ElementID storehouse_id;
};
class Worker:public IPackageReceiver, public PackageSender{
public:
    //Worker(ElementID id, IPackageQueue* queue_ptr, Time proc_time, PackageQueueType q_type ): queue(queue_ptr), worker_id(id), processing_time(proc_time), queue_type(q_type) {};
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q): PackageSender(), queue(std::move(q)), worker_id(id), processing_time(pd) {};
    ElementID get_worker_id() const {return worker_id;};
    TimeOffset get_processing_duration() const {return processing_time;};
    Time get_precessing_start_time() const {return package_processing_start_time;};
    void do_work(Time t);
    void receive_package(Package &&p) override{queue->push(std::move(p));};
    ElementID get_id() const override {return worker_id;};
    const_iterator begin() const override {return queue->begin();};
    const_iterator end() const override {return queue->end();};
    const_iterator cbegin() const override {return queue->cbegin();};
    const_iterator cend() const override {return queue->cend();};

private:
    std::unique_ptr<IPackageQueue> queue;
    ElementID worker_id;
    TimeOffset processing_time;
    PackageQueueType queue_type;
    Time package_processing_start_time;
};



