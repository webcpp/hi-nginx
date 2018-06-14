#ifndef OBJECT_POOL_HPP
#define OBJECT_POOL_HPP

#include <memory>

#include "concurrentqueue.h"



namespace hi {

    template<typename T>
    class object_pool {
    public:

        object_pool() : object_pool(1024) {

        }

        object_pool(size_t n) : q(), size(n) {
            for (size_t i = 0; i<this->size; ++i) {
                this->q.enqueue(T());
            }
        }
        virtual~object_pool() = default;

        T get() {
            T obj;
            this->q.try_dequeue(obj);
            return obj;
        }

        void recycle(T&& obj) {
            this->q.enqueue(std::move(obj));
        }

        size_t length()const {
            return this->size;
        }
        
        static std::shared_ptr<hi::object_pool<T>> make_shared(size_t n) {
            return std::make_shared<hi::object_pool <T>> (n);
        }
    private:
        moodycamel::ConcurrentQueue<T> q;
        size_t size;
    };

}

#endif /* OBJECT_POOL_HPP */

