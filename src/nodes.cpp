#include "nodes.hxx"
#include "storage_types.hxx"
void storehouse::receive_package(Package&& package) {
    stockpile->push(std::move(package));
}
void storehouse::add_products(std::list<Package> packages) {
    for (auto& package : packages){
        stockpile->push(std::move(package));
    }
}


