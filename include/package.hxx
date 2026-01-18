#include "types.hxx"
#include <set>

class Package{
    public:
        Package();
        Package(ElementID id): Package_id(id) {}
        Package(Package&&) = default;

        ElementID get_id() const {return Package_id;}
        ~Package();
    private:
        ElementID Package_id;
        static std::set<ElementID> freed_ids;
        static std::set<ElementID> assigned_ids;
};

