#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include <string>
#include <Poco/ClassLoader.h>
#include <Poco/SortedDirectoryIterator.h>
#include "../include/view.hpp"

namespace nginx {

    class plugin {
    public:
        plugin() = delete;

        plugin(const std::string dir, Poco::ClassLoader<nginx::view>* class_loader)
        : dir(dir)
        , class_loader(class_loader) {
            this->load();
        }
        plugin(const plugin&) = delete;

        virtual~plugin() {
            this->unload();
        }
        plugin& operator=(const plugin& right) = delete;

        void update() {
            this->load();
        }
    private:

        void load() {
            if (!this->dir.empty() && Poco::File(this->dir).exists()) {
                Poco::Path libDir(this->dir);
                Poco::SortedDirectoryIterator it(libDir), jt;
                std::string libExt = Poco::SharedLibrary::suffix().substr(1);
                while (it != jt) {
                    if (Poco::Path(it->path()).getExtension() == libExt && it->canExecute()) {
                        if (!this->class_loader->isLibraryLoaded(it->path())) {
                            try {
                                this->class_loader->loadLibrary(it->path());
                            } catch (Poco::LibraryLoadException& e) {

                            }
                        }
                    }
                    ++it;
                }
            }
        }

        void unload() {
            std::vector<std::string> libpath;
            Poco::ClassLoader<nginx::view>::Iterator it = this->class_loader->begin();

            for (; it != this->class_loader->end(); ++it) {
                libpath.push_back(it->first);
            }
            for (auto &item : libpath) {
                this->class_loader->unloadLibrary(item);
            }
        }

    private:
        std::string dir;
        Poco::ClassLoader<nginx::view>* class_loader;

    };


}

#endif /* PLUGIN_HPP */

