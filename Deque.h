// ----------------------
// projects/deque/Deque.h
// Copyright (C) 2013
// Glenn P. Downing
// ----------------------

#ifndef Deque_h
#define Deque_h

#define COLUMNS 50

// --------
// includes
// --------

#include <algorithm> // copy, equal, lexicographical_compare, max, swap
#include <cassert> // assert
#include <iterator> // iterator, bidirectional_iterator_tag
#include <memory> // allocator
#include <stdexcept> // out_of_range
#include <utility> // !=, <=, >, >=

// -----
// using
// -----

using std::rel_ops::operator!=;
using std::rel_ops::operator<=;
using std::rel_ops::operator>;
using std::rel_ops::operator>=;

using namespace std;

// -------
// destroy
// -------

template <typename A, typename BI>
BI destroy (A& a, BI b, BI e) {
    while (b != e) {
        --e;
        a.destroy(&*e);}
    return b;}

// ------------------
// uninitialized_copy
template <typename A, typename II, typename BI>
BI uninitialized_copy (A& a, II b, II e, BI x) {
    BI p = x;
    assert(b!=e);
    try {
        while (b != e) {
            a.construct(&*x, *b);
            ++b;
            ++x;}}
    catch (...) {
        destroy(a, p, x);
        throw;}
    return x;}

// ------------------
// uninitialized_fill
template <typename A, typename BI, typename U>
    BI uninitialized_fill (A& a, BI b, BI e, const U& v) {
    BI p = b;
    assert(p == b);
    try {
        while (b != e) {
            a.construct(&*b, v);
            ++b;}}
    catch (...) {
        destroy(a, p, b);
        throw;}
    return e;}

// -----
// MyDeque
// -----

template < typename T, typename A = std::allocator<T> >
class MyDeque {
    public:
    // --------
    // typedefs
    // --------

    typedef A   allocator_type;
    typedef typename allocator_type::value_type value_type; // T

    typedef typename allocator_type::size_type  size_type;
    typedef typename allocator_type::difference_type    difference_type;

    typedef typename allocator_type::pointer    pointer;    // T*
    typedef typename allocator_type::const_pointer  const_pointer;

    typedef typename allocator_type::reference  reference;
    typedef typename allocator_type::const_reference    const_reference;

    typedef typename allocator_type::template rebind<T*>::other allocatorPointer_type;

public:
    // -----------
    // operator ==
    // -----------

    /**
     * @param lhs a const MyDeque reference
     * @param rhs a const MyDeque reference
     * @return a true if equal or false if not 
     * Checks if lhs equals rhs
     * (lhs == rhs) => true or false)
    */
    friend bool operator == (const MyDeque& lhs, const MyDeque& rhs) {
        return (lhs.size() == rhs.size()) && (equal(lhs.begin(), lhs.end(), rhs.begin()));}

    // ----------
    // operator <
    // ----------

    /**
     * @param lhs a const MyDeque reference
     * @param rhs a const MyDeque reference
     * @return a true if less than or false if greater or equal 
     * Checks if lhs is less than rhs
     * (lhs < rhs) => true or false)
     */
    friend bool operator < (const MyDeque& lhs, const MyDeque& rhs) {
        return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end() );}

private:
    // ----
    // data
    // ----

    allocator_type _a; 
    allocatorPointer_type _aPointer; 

    pointer* _outerFront;    

    pointer _front;  
    pointer _back; 
    pointer _b; 
    pointer _e; 

    private:
        // -----
        // valid
        // -----

        bool valid () const {
            return (!_front && !_back && !_b && !_e) || ((_front <= _b) && (_b <= _e) && (_e <= _back));}

    public:
        // --------
        // iterator
        // --------

        class iterator {
            public:
                // --------
                // typedefs
                // --------

                typedef std::bidirectional_iterator_tag   iterator_category;
                typedef typename MyDeque::value_type      value_type;
                typedef typename MyDeque::difference_type difference_type;
                typedef typename MyDeque::pointer         pointer;
                typedef typename MyDeque::reference       reference;
                typedef typename MyDeque::size_type       size_type;

            public:
                // -----------
                // operator ==
                // -----------

                /**
                 * @param lhs a const iterator reference
                 * @param rhs a const itierator reference
                 * @return a true if equal or false if not
                 * Checks if lhs equals rhs 
                 * (lhs == rhs) => true or false
                 */
                friend bool operator == (const iterator& lhs, const iterator& rhs) {
                    return lhs._deque == rhs._deque && lhs._idx == rhs._idx;}

                /**
                 * @param lhs a const iterator reference
                 * @param rhs a const iterator reference
                 * @return a false if equal or true if not
                 * Checks if lhs doesn't equal rhs 
                 * (lhs != rhs) => true or false
                 */
                friend bool operator != (const iterator& lhs, const iterator& rhs) {
                    return !(lhs == rhs);}

                // ----------
                // operator +
                // ----------

                /**
                 * @param lhs an iterator
                 * @param rhs a difference_type which is the amount being added.
                 * @return an iterator
                 * Adds rhs to lhs
                 * (lhs + rhs) => iterator
                 */
                friend iterator operator + (iterator lhs, difference_type rhs) {
                    return lhs += rhs;}

                // ----------
                // operator -
                // ----------

                /**
                 * @param lhs an iterator
                 * @param rhs a difference_type which is the amount being subtracted.
                 * @return an iterator
                 * Subtracts rhs from lhs
                 * (lhs - rhs) => iterator
                 */
                friend iterator operator - (iterator lhs, difference_type rhs) {
                    return lhs -= rhs;}

            private:
                // ----
                // data
                // ----

                MyDeque* _deque;

                size_type _idx;

            private:
                // -----
                // valid
                // -----

                bool valid () const {
                    return (_idx >= 0);}

            public:
                // -----------
                // constructor
                // -----------

                /**
                * @param d a pointer to MyDeque
                * @param i a size_type
                */
                iterator (MyDeque* d, size_type i = 0)
                    : _deque(d), _idx(i) {
                    assert(valid());}

                // Default copy, destructor, and copy assignment.
                // iterator (const iterator&);
                // ~iterator ();
                // iterator& operator = (const iterator&);

                // ----------
                // operator *
                // ----------

                /**
                * @return reference
                */
                reference operator * () const {
                    return (*_deque)[_idx];}

                // -----------
                // operator ->
                // -----------

                /**
                 * @return pointer
                 * Dereference the pointer to this
                 */
                pointer operator -> () const {
                return &**this;}

                // -----------
                // operator ++
                // -----------

                /**
                * @return reference to iterator
                * Pre-increment this 
                */
                iterator& operator ++ () {
                    ++_idx;
                    assert(valid());
                    return *this;}

                /**
                * @return an iterator
                * Post-increment this 
                */
                iterator operator ++ (int) {
                    iterator x = *this;
                    ++(*this);
                    assert(valid());
                    return x;}

                // -----------
                // operator --
                // -----------

                /**
                * @return reference to iterator
                * Pre-decrement this 
                */
                iterator& operator -- () {
                    --_idx;
                    assert(valid());
                    return *this;}

                /**
                * @return an iterator
                * Post-decrement this 
                */
                iterator operator -- (int) {
                    iterator x = *this;
                    --(*this);
                    assert(valid());
                    return x;}

                // -----------
                // operator +=
                // -----------

                /**
                 * @param d a difference_type which is the amount that is added
                 * @return an iterator reference
                 * (this + d) => this
                 */
                iterator& operator += (difference_type d) {
                    _idx += d;
                    assert(valid());
                    return *this;}

                // -----------
                // operator -=
                // -----------
                
                /**
                 * @param d a difference_type which is the amount that is subtracted
                 * @return an iterator reference
                 * (this - d) => this
                 */
                iterator& operator -= (difference_type d) {
                    _idx -= d;
                    assert(valid());
                    return *this;}};

    public:
        // --------------
        // const_iterator
        // --------------

        class const_iterator {
            public:
            // --------
            // typedefs
            typedef std::bidirectional_iterator_tag   iterator_category;
            typedef typename MyDeque::value_type      value_type;
            typedef typename MyDeque::difference_type difference_type;
            typedef typename MyDeque::const_pointer   pointer;
            typedef typename MyDeque::const_reference reference;
            typedef typename MyDeque::size_type       size_type;

            public:
                // -----------
                // operator ==
                // -----------

                /**
                 * @param lhs a const iterator reference
                 * @param rhs a const iterator reference
                 * @return a true if equal or false if not
                 * Checks if lhs equals rhs 
                 * (lhs == rhs) => true or false
                 */
                friend bool operator == (const const_iterator& lhs, const const_iterator& rhs) {
                    return (lhs._cDeque == rhs._cDeque) && (lhs._idx == rhs._idx);}

                /**
                 * @param lhs a const iterator reference
                 * @param rhs a const iterator reference
                 * @return a false if equal or true if not
                 * Checks if lhs doesn't equal rhs 
                 * (lhs != rhs) => true or false
                 */
                friend bool operator != (const const_iterator& lhs, const const_iterator& rhs) {
                    return !(lhs == rhs);}

                // ----------
                // operator +
                // ----------

                /**
                 * @param lhs a const_iterator
                 * @param rhs a difference_type which is the amount being added.
                 * @return an const_iterator
                 * Adds rhs to lhs
                 * (lhs + rhs) => const_iterator
                 */
                friend const_iterator operator + (const_iterator lhs, difference_type rhs) {
                    return lhs += rhs;}

                // ----------
                // operator -
                // ----------

                /**
                 * @param lhs an const_iterator
                 * @param rhs a difference_type which is the amount being added.
                 * @return an const_iterator
                 * Adds rhs to lhs
                 * (lhs - rhs) => const_iterator
                 */
                friend const_iterator operator - (const_iterator lhs, difference_type rhs) {
                    return lhs -= rhs;}

            private:
                // ----
                // data
                // ----

                const MyDeque* _cDeque;

                size_type _idx;

            private:
                // -----
                // valid
                // -----

                bool valid () const {
                    return _idx >= 0;}

            public:
            // -----------
            // constructor
            // -----------

            /**
             * @param d a const pointer to MyDeque
             * @param i a size_type
             */
            const_iterator (const MyDeque* d, size_type i)
                : _cDeque(d), _idx(i) {
                assert(valid());}

            // Default copy, destructor, and copy assignment.
            // const_iterator (const const_iterator&);
            // ~const_iterator ();
            // const_iterator& operator = (const const_iterator&);

            // ----------
            // operator *
            // ----------

            /**
            * @return dereferenced pointer
            */
            reference operator * () const {
                return (*_cDeque)[_idx];}

            // -----------
            // operator ->
            // -----------

            /**
             * @return pointer
             * Dereference the pointer to this 
             */
            pointer operator -> () const {
                return &**this;}

            // -----------
            // operator ++
            // -----------

            /**
             * @return reference to const_iterator
             * Pre-increment this
             */
            const_iterator& operator ++ () {
                ++_idx;
                assert(valid());
                return *this;}

            /**
             * @return a const_iterator
             * Post-increment this
             */
            const_iterator operator ++ (int) {
                const_iterator x = *this;
                ++(*this);
                assert(valid());
                return x;}

            // -----------
            // operator --
            // -----------

            /**
             * @return a const_iterator
             * Pre-decrement this 
             */
            const_iterator& operator -- () {
                --_idx;
                assert(valid());
                return *this;}

            /**
             * @return a const_iterator
             * Post-decrement this 
             */
            const_iterator operator -- (int) {
                const_iterator x = *this;
                --(*this);
                assert(valid());
                return x;}

            // -----------
            // operator +=
            // -----------

            /**
             * @param rhs a difference_type which is the amount that is added
             * @return an const_iterator reference
             * (this + rhs) => this
             */
            const_iterator& operator += (difference_type rhs) {
                _idx += rhs;
                assert(valid());
                return *this;}

            // -----------
            // operator -=
            // -----------

            /**
             * @param rhs a difference_type which is the amount that is substracted
             * @return a const_iterator reference
             * (this - rhs) => this
             */
            const_iterator& operator -= (difference_type rhs) {
                _idx -= rhs;
                assert(valid());
                return *this;}};

    public:
        // ------------
        // constructors
        // ------------

        /**
         * @param a a const allocator_type reference that is defaulted 
         * Default constructor
         */
        explicit MyDeque (const allocator_type& a = allocator_type() )
            : _a(a), _aPointer(), _outerFront(0), _front(0), _back(0), _b(0), _e(0) {
            assert(valid() );}

        /**
         * @param s a size_type
         * @param v a const_reference that is defaulted
         * @param a a const allocator_type reference that is defaulted
         * Constructor with size specification
         */
        explicit MyDeque (size_type s, const_reference v = value_type(), const allocator_type& a = allocator_type())
            : _a(a), _aPointer() {
            size_type rows = s / COLUMNS;
            if (s % COLUMNS != 0)
                ++rows;
            _outerFront = _aPointer.allocate(rows);
            
            for (size_type i = 0; i < rows; ++i) 
                _outerFront[i] = _a.allocate(COLUMNS);
            
            for (size_type i = 0; i < rows; ++i) 
                _a.deallocate(_outerFront[i], COLUMNS);
            
            _aPointer.deallocate(_outerFront, rows);
            _front = _b = _a.allocate(s);
            _e = _back = _b + s;
            uninitialized_fill(_a, begin(), end(), v);
            assert(valid());}

        /**
         * @param that a const MyDeque reference
         * Copy constructor
         */
        MyDeque (const MyDeque& that)
            : _a(that._a), _aPointer() {
            _outerFront = 0; 
            _front = _b = _a.allocate(that.size());
            _e = _back = _front + that.size();
            uninitialized_copy(_a, that.begin(), that.end(), begin());
            assert(valid());}

        // ----------
        // destructor
        // ----------

        /**
         * MyDeque destructor
         * Clear and deallocate MyDeque memory.Destroys this Deque
         */
        ~MyDeque () {
            if (_front) {
                clear();
                _a.deallocate(_front, (_back - _front));}
            assert(valid() );}

        // ----------
        // operator =
        // ----------

        /**
         * @param rhs a const MyDeque reference
         * @return a MyDeque reference
         * Assign a MyDeque to equal another
         */
        MyDeque& operator = (const MyDeque& rhs) {
            if (this == &rhs)
                return *this;
            if (rhs.size() == size())
                copy(rhs.begin(), rhs.end(), begin());
            else if (rhs.size() < size()) {
                copy(rhs.begin(), rhs.end(), begin());
                resize(rhs.size());
            }    
            else if (rhs.size() <= (unsigned)(_back - _b)) {
                copy(rhs.begin(), rhs.begin() + size(), begin());
                _e = &(*uninitialized_copy(_a, rhs.begin() + size(), rhs.end(), end()));
            }
            else {
                clear();
                MyDeque d(rhs);
                swap(d);
            }
            assert(valid());
            return *this;}

        // -----------
        // operator []
        // -----------

        /**
         * @param index a size_type
         * @return a reference
         * Indexing operator
         */
        reference operator [] (size_type index) {
            return *(_b + index);}

        /**
         * @param index a size_type
         * @return a const reference
         * Indexing operator
         */
        const_reference operator [] (size_type index) const {
            return const_cast<MyDeque*>(this)->operator[](index);}

        // --
        // at
        // --

        /**
         * @param index a size_type
         * @return a reference
         * Return the element at the given index
         * Throw if index is out of bounds
         */
        reference at (size_type index) {
            if (index >= size())
                throw std::out_of_range("Out of bounds.");
            return (*this)[index];}

        /**
         * @param index a size_type
         * @return a const_reference
         * Return the element at the given index
         */
        const_reference at (size_type index) const {
            return const_cast<MyDeque*>(this)->at(index);}

        // ----
        // back
        // ----

        /**
         * @return a reference
         * Returns a reference to the last element of MyDeque
         */
        reference back () {
            assert(size() != 0);
            return *(_e - 1);}

        /**
         * @return a const reference
         * Returns a const reference to the last element of MyDeque
         */
        const_reference back () const {
            return const_cast<MyDeque*>(this)->back();}

        // -----
        // begin
        // -----

        /**
         * @return an iterator
         * Returns the beginning iterator of MyDeque
         */
        iterator begin () {
            return iterator(this, 0);}

        /**
         * @return an const_iterator
         * Returns the beginning iterator of MyDeque
         */
        const_iterator begin () const {
            return const_iterator(this, 0);}

        // -----
        // clear
        // -----

        /**
         * Clears MyDeque without deallocating
         */
        void clear () {
            resize(0);
            assert(valid());}

        // -----
        // empty
        // -----

        /**
         * @return a bool
         * Check if MyDeque is empty or not
         */
        bool empty () const {
            return !size();}

        // ---
        // end
        // ---

        /**
         * @return an iterator
         * Returns the end iterator of MyDeque
         */
        iterator end () {
            return iterator(this, size());}

        /**
         * @return a const_iterator
         * Returns the end iterator of MyDeque
         */
        const_iterator end () const {
            return const_iterator(this, size());}

        // -----
        // erase
        // -----

        /**
         * @param i an iterator
         * @return an iterator
         * Erases the element that is being pointed to by the iterator
         */
        iterator erase (iterator i) {
            if(i != end() - 1) {
                copy(i + 1, end(), i);
                resize(size() - 1);
            }
            else
                pop_back();
            assert(valid() );
            return iterator(this, 0);}

        // -----
        // front
        // -----

        /**
         * @return a reference
         * Returns the front element of this Mydeque
         */
        reference front () {
            return *(_b);}

        /**
         * @return a const_reference
         * Returns the front element of this const Mydeque
         */
        const_reference front () const {
            return const_cast<MyDeque*>(this)->front();}

        // ------
        // insert
        // ------

        /**
         * @param i an iterator
         * @param v a const_reference which is the value inserted
         * @return an iterator with inserted value
         * Insert the const_reference v into MyDeque
         */
        iterator insert (iterator i, const_reference v) {
            if(i != end()) {
                resize(size() + 1);
                copy(i, end(), i + 1);
                *i = v;
            }
            else
                push_back(v);
            assert(valid());
            return iterator(this);}

        // --------
        // pop_back
        // --------

        /**
        * Removes the back element of MyDeque
        */
        void pop_back () {
            assert(size() != 0);
            resize(size() - 1);
            assert(valid());}

        /**
         * Removes the front element of MyDeque
         */
        void pop_front () {
            assert(size() != 0);
            destroy(_a, begin(), begin() + 1);
            ++_b;
            assert(valid());}

        // ---------
        // push_back
        // ---------

        /**
         * @param v a const_reference
         * Pushes value v onto the back of MyDequeAppends a copy of v at the end
         */
        void push_back (const_reference v) {
            resize(size() + 1, v);
            assert(valid());}

        // ----------
        // push_front

        /**
         * @param v a const reference
         * Pushes value v onto the front of MyDeque
         */
        void push_front (const_reference v) {
            if (_front == _b) {
                resize(size() + 1);
                pop_back();
            }    
            --_b;
            _a.construct(&*begin(), v); 
            assert(valid());}

        // ------
        // resize
        // ------

        /**
         * @param s a size_type
         * @param v a const reference that is defaulted
         * Resize the MyDeque with v filling up the new spaces
         */
        void resize (size_type s, const_reference v = value_type()) {
            if (s == size())
                return;
            if (s < size())
                _e = &*destroy(_a, begin() + s, end() );
            else if ((s <= (unsigned)(_e -_front)) && (s <= (unsigned)(_back - _b)))
                _e = &*uninitialized_fill(_a, end(), begin() + s, v);
            // Not large enough capacity
            else { 
                size_type c = 2 * size();
                if (s > c)
                    c = s;
                if (c == s)
                    c += 3;
                size_type b_offset = (c - s) / 2;
                size_type e_offset = b_offset;
                if ((c - s) % 2)
                    ++e_offset;
                
                MyDeque d(c, v);
                copy(begin(), end(), d.begin() + b_offset);
                destroy(d._a, d.begin(), d.begin() + b_offset);
                destroy(d._a, d.end() - e_offset, d.end());
                d._b += b_offset;
                d._e -= e_offset;
                swap(d);}
            assert(valid());}

        // ----
        // size
        // ----
        /**
         * @return size_type
         * Return the size of the MyDeque
         */
        size_type size () const {
            return _e - _b;}

        // ----
        // swap
        // ----

        /**
         * @param that a MyDeque reference
         * Swap the properties of that to this.
         */
        void swap (MyDeque& that) {
        if (_a != that._a) {
            MyDeque d(*this);
            *this = that;
            that = d;
        }
        else {
            std::swap(_front, that._front);
            std::swap(_back, that._back);
            std::swap(_b, that._b);
            std::swap(_e, that._e);
        }
        assert(valid() );}};

#endif // Deque_h
