#ifndef GLOVE_BUFFER_H
#define GLOVE_BUFFER_H

#include <iostream>
#include <new>

using namespace std;


namespace xtl {

    template<typename _Tp>
    class LoopQue_impl {
    public:
        static int addsize(int max_size) {
            return max_size * sizeof(_Tp);
        }

        LoopQue_impl(int msize) : max_size(msize), _front(0), _rear(0), _size(0) {}

        _Tp &front() { return data[_front]; }

        void push(const _Tp &value) {
            data[_rear] = value;
            _rear = (_rear + 1) % max_size;
            _size++;
        }

        void pop() {
            _front = (_front + 1) % max_size;
            _size--;
        }

        int check_pop(_Tp &tv) {
            if (empty())
                return -1;

            tv = front();
            pop();
        }

        int check_push(const _Tp &value) {
            if (full())
                return -1;

            push(value);
        }

        bool full() const { return _size == max_size; }

        bool empty() const { return _size == 0; }

        int size() const { return _size; }

        int capacity() const { return max_size; }

    private:
        int32_t _front;  // front index
        int32_t _rear;   // rear index
        int32_t _size;   // queue data record number

        const int32_t max_size; // queue capacity

        _Tp data[0];    // data record occupy symbol
    };

    template<typename _Tp>
    struct LoopQue_allocate {
        LoopQue_impl<_Tp> &allocate(int msize) {
            char *p = new char[sizeof(LoopQue_impl<_Tp>) + LoopQue_impl<_Tp>::addsize(msize)];
            return *(new(p) LoopQue_impl<_Tp>(msize));
        }

        void deallocate(void *p) {
            delete[] (char *) p;
        }
    };

    template<typename _Tp, typename Alloc = LoopQue_allocate<_Tp> >
    class LoopQue {
    public:
        typedef _Tp value_type;

        LoopQue(int msize) : impl(alloc.allocate(msize)) {}

        ~LoopQue() { alloc.deallocate((void *) &impl); }

        value_type &front() { return impl.front(); }

        const value_type &front() const { return impl.front; }

        void push(const value_type &value) { impl.push(value); }

        void pop() { impl.pop(); }

        int check_pop(value_type &tv) { return impl.check_pop(tv); }

        int check_push(const value_type &value) { return impl.check_push(value); }

        bool full() const { return impl.full(); }

        bool empty() const { return impl.empty(); }

        int size() const { return impl.size(); }

    private:
        Alloc alloc;
        LoopQue_impl<_Tp> &impl;
    };

} // end of < namespace stl >


#endif //GLOVE_BUFFER_H
