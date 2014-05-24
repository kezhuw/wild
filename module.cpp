#include "module.hpp"
#include "module_init_failed.hpp"
#include "types.hpp"

#include <map>
#include <unordered_map>
#include <vector>

#include <cstdio>

struct module_info {
    const char *name;
    wild::module::InitFunction init;
};

namespace {

typedef std::unordered_map<const wild::module::Definition*, struct module_info *> ModuleMap;

ModuleMap&
_mapped_modules() {
    static ModuleMap mapped_modules;
    return mapped_modules;
}

struct Priority {
    int Seq;
    int Order;
};

inline bool operator<(Priority a, Priority b) {
    return (a.Seq < b.Seq) || (a.Seq == b.Seq && a.Order < b.Order);
}

typedef std::multimap<Priority, struct module_info*> PriorityMap;

PriorityMap&
_sorted_modules() {
    static PriorityMap sorted_modules;
    return sorted_modules;
}

void
_add_module(const wild::module::Definition *def, int seq, int order, struct module_info *module) {
    _sorted_modules().emplace(Priority{seq, order}, module);
    _mapped_modules()[def] = module;
}

struct module_info *
_get_module(const wild::module::Definition *def) {
    return _mapped_modules()[def];
}

}

namespace wild { namespace module {

void
Init() {
    PriorityMap const& modules = _sorted_modules();
    for (auto const& value : modules) {
        struct module_info *m = value.second;
        try {
            m->init();
        } catch (...) {
            Priority p = value.first;
            char buf[1024];
            snprintf(buf, sizeof buf,
                "fail to init module[%s seq(%d) order(%d)]",
                m->name, p.Seq, p.Order);
            std::throw_with_nested(InitFailed(buf));
        }
    }
}

Definition::Definition(int seq, const char *name, InitFunction init, int order) {
    _add_module(this, seq, order, new module_info{name, init});
}

const char *
Definition::Name() const {
    return _get_module(this)->name;
}

}}
