#include "module.hpp"
#include "module_init_failed.hpp"
#include "types.hpp"

#include <map>
#include <unordered_map>
#include <vector>

#include <stdio.h>

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

typedef std::vector<struct module_info *> ModuleVec;
typedef std::map<int, ModuleVec> OrderedModuleMap;
typedef std::map<int, OrderedModuleMap> SeqModuleMap;

SeqModuleMap&
_sorted_modules() {
    static SeqModuleMap sorted_modules;
    return sorted_modules;
}

void
_add_module(const wild::module::Definition *id, int seq, int order, struct module_info *module) {
    _sorted_modules()[seq][order].push_back(module);
    _mapped_modules()[id] = module;
}

struct module_info *
_get_module(const wild::module::Definition *id) {
    return _mapped_modules()[id];
}

}

namespace wild { namespace module {

void
Init() {
    SeqModuleMap const& seq_modules = _sorted_modules();
    for (SeqModuleMap::value_type const& seq_value : seq_modules) {
        OrderedModuleMap const& ordered_modules = seq_value.second;
        for (OrderedModuleMap::value_type const& ordered_value : ordered_modules) {
            ModuleVec const& modules = ordered_value.second;
            for (struct module_info *m : modules) {
                int err = m->init();
                if (err != 0) {
                    char buf[1024];
                    snprintf(buf, sizeof buf,
                            "failed to init module[%s seq(%d) order(%d)]",
                            m->name, seq_value.first, ordered_value.first);
                    throw InitFailed(err, buf);
                }
            }
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
