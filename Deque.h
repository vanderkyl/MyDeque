// ----------------------
// projects/deque/Deque.h
// Copyright (C) 2013
// Glenn P. Downing
// ----------------------

#ifndef Deque_h
#define Deque_h

// --------
// includes
// --------

#include <algorithm> // equal, lexicographical_compare
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
// ------------------

template <typename A, typename II, typename BI>
BI uninitialized_copy (A& a, II b, II e, BI x) {
    BI p = x;
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
// ------------------

template <typename A, typename BI, typename U>
BI uninitialized_fill (A& a, BI b, BI e, const U& v) {
    BI p = b;
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

        typedef A allocator_type;
        typedef typename allocator_type::value_type value_type;

        typedef typename allocator_type::size_type size_type;
        typedef typename allocator_type::difference_type difference_type;

        typedef typename allocator_type::pointer pointer;
        typedef typename allocator_type::const_pointer const_pointer;

        typedef typename allocator_type::reference reference;
        typedef typename allocator_type::const_reference const_reference;

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
            return lexicographical_compare(lhs._front, lhs._back, rhs._front, rhs._back);}

    private:
        // ----
        // data
        // ----

        // Allocator for the rows of T
        allocator_type _a;
        // Allocator for the columns of T*
        typename A::template rebind<T*>::other _aPointer;

        // Pointers to container, front, and back pointers.
        pointer* deque;
        pointer _front;
        pointer _back;
        
        // Pointer to the row which is selected
        pointer* _outerFront;

        int _rows;
        size_type _size;
        size_type _capacity;
        
        

    private:
        // -----
        // valid
        // -----

      bool valid () const {
          int row = _size / 10;
          if((_front == *_outerFront) && (_size % 10 == 0) && (_size > 0))
              --row;
          // Check if pointers are pointing somewhere or the front and back pointers are inside the bounds
          return (!_front && !_back && !_outerFront) || ((_front < *_outerFront + 10) && (_back <= *(_outerFront + row) + 10));}

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
                friend bool operator == (const iterator& lhs, const iterator& rhs) {
                    return lhs._p == rhs._p;}

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

                // The row that _p is on
                pointer* _r;

                pointer _p;
                pointer _e;

                int _idx;
                size_type _size;

            private:
                // -----
                // valid
                // -----

                bool valid () const {
                    // Check that _p is pointing to a T or that _p is pointing somewhere
                    return (sizeof(*_p) == sizeof(T)) || !_p;}

            public:
                // -----------
                // constructor
                // -----------

                /**
                 * constructs an iterator with value v, row r, index i, size my_size, and end and front.
                 */
                iterator (pointer* r, pointer p, pointer e, int i, int s) 
                    : _r(r), _p(p), _e(e), _idx(i), _size(s) {
                    assert(valid());}

                // Default copy, destructor, and copy assignment.
                // iterator (const iterator&);
                // ~iterator ();
                // iterator& operator = (const iterator&);

                pointer i_pointer()
                {
                    return _p;
                }

                // ----------
                // operator *
                // ----------

                /**
                 * @return dereferenced pointer _p
                 */
                reference operator * () const {
                    return *_p;}

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
                 * Pre-increment this iterator
                 */          
                iterator& operator ++ () {
                    if((_p + 1) == _e)
                        _p = _e;
                    // If _idx is at the end of the row and _p isn't at the _e pointer
                    // or _idx is beyond the end of the row and _p equals the _e pointer
                    else if((_idx == 9 && _p != _e) || (_idx == 10 && _p == _e)) {
                        ++_r;
                        if(_p == _e)
                            _idx = 1;
                        else
                            _idx = 0;
                        _p = *_r;
                    }
                    else {
                        ++_p;
                        ++_idx;
                    }
                    assert(valid());
                    return *this;}

                /**
                 * @return an iterator
                 * Post-increment this iterator
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
                 * Pre-decrement this iterator
                 */
                iterator& operator -- () {
                    // If the index is at the beginning of row.
                    // Go to end of previous row
                    if(_idx == 0) {
                        _r = _r - 1;
                        _idx = 9;
                        _p = *_r + _idx;
                    }
                    else {
                        --_p;
                        --_idx;
                    }
                    assert(valid());
                    return *this;}

                /**
                 * @return an iterator
                 * Post-decrement this iterator
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
                    if((_idx + d > 9 && _p != _e) || (_p == _e && _idx + d > 10)) {
                    int idx_diff = 10 - _idx;
                    int temp_d = d - idx_diff;
                        if(!(d <= 10 && *_r + d == _e))
                            ++_r;
                        else
                            temp_d = d;
                        while(temp_d > 9) {
                            if(*_r + temp_d == _e) {
                                break;
                            }
                            temp_d -= 10;
                            ++_r;
                        }
                        _idx = temp_d;
                        _p = *_r + temp_d;
                    }
                    else {
                    _p += d;
                    _idx += d;
                    }
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
                    if(_idx - d < 0) {
                        int temp = d - (_idx + 1);
                        --_r;
                        while(temp > 9) {
                            temp -= 10;
                            --_r;
                        }
                        _idx = 9 - temp;
                        _p = *_r + _idx;
                    }
                    else {
                        _p -= d;
                        _idx -= d;
                    }
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
                // --------

                typedef std::bidirectional_iterator_tag   iterator_category;
                typedef typename MyDeque::value_type      value_type;
                typedef typename MyDeque::difference_type difference_type;
                typedef typename MyDeque::const_pointer   pointer;
                typedef typename MyDeque::const_reference reference;

            public:
                // -----------
                // operator ==
                // -----------

                /*
                 * @param lhs a const iterator reference
                 * @param rhs a const iterator reference
                 * @return a true if equal or false if not
                 * Checks if lhs equals rhs 
                 * (lhs == rhs) => true or false
                 */
                friend bool operator == (const const_iterator& lhs, const const_iterator& rhs) {
                  return lhs._cp == rhs._cp;}

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
                friend const_iterator operator + (const_iterator lhs, difference_type rhs) {
                    return lhs += rhs;}

                // ----------
                // operator -
                // ----------

                /**
                 * decrements lhs by rhs.
                 */
                friend const_iterator operator - (const_iterator lhs, difference_type rhs) {
                    return lhs -= rhs;}

            private:
                // ----
                // data
                // ----

                const pointer* _r;
                pointer _cp;
                pointer _ce;

                int _idx;
                size_type _size;

            private:
                // -----
                // valid
                // -----

                bool valid () const {
                    return !_cp || (sizeof(*_cp) == sizeof(T));}

            public:
                // -----------
                // constructor
                // -----------

                /**
                 * constructs an iterator with value v, row r, index i, size my_size, and end and front.
                 */
                const_iterator (const pointer* r, pointer p, pointer e, int i, int s) 
                    : _r(r), _cp(p), _ce(e), _idx(i), _size(s) {
                    assert(valid());}

                // Default copy, destructor, and copy assignment.
                // const_iterator (const const_iterator&);
                // ~const_iterator ();
                // const_iterator& operator = (const const_iterator&);

                /*
                 * @return pointer
                 * Returns iterator pointer to help with methods
                 */
                pointer i_pointer() {
                    return _cp;
                }

                // ----------
                // operator *
                // ----------

                /**
                 * @return dereferenced pointer _cp
                 */
                reference operator * () const {
                    return *_cp;}

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
                 * @return reference to const iterator
                 * Pre-increment this const iterator
                 */ 
                const_iterator& operator ++ () {
                    if((_cp + 1) == _ce)
                        _cp = _ce;
                    // If _idx is at the end of the row and _cp isn't at the _ce pointer
                    // or _idx is beyond the end of the row and _cp equals the _ce pointer
                    else if((_idx == 9 && _cp != _ce) || (_idx == 10 && _cp == _ce)) {
                        _r = _r + 1;
                        if(_cp == _ce)
                          _idx = 1;
                        else
                          _idx = 0;
                        _cp = *_r;
                    }
                    else {
                        ++_cp;
                        ++_idx;
                    }
                    assert(valid());
                    return *this;}

                /**
                 * @return a const iterator
                 * Post-increment this const iterator
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
                 * @return an iterator
                 * Pre-decrement this const iterator
                 */
                const_iterator& operator -- () {
                    if(_idx == 0) {
                        _r = _r - 1;
                        _idx = 9;
                        _cp = *_r + _idx;
                    }
                    else {
                        --_cp;
                        --_idx;
                    }
                    assert(valid());
                    return *this;}

                /**
                 * @return an iterator
                 * Post-decrement this const iterator
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
                 * @param d a difference_type which is the amount that is added
                 * @return an iterator reference
                 * (this + d) => this
                 */      
                const_iterator& operator += (difference_type d) {
                    if((_idx + d > 9 && _cp != _ce) || (_cp == _ce && _idx + d > 10)) {
                        int temp = d - (10 - _idx);
                        if(!(d <= 10 && *_r + d == _ce))
                            ++_r;
                        else
                            temp = d;
                        while(temp > 9)
                        {
                            if(*_r + temp == _ce){
                                break;
                            }
                            temp -= 10;
                            ++_r;
                        }
                        _idx = temp;
                        _cp = *_r + temp;
                    }
                    else {
                        _cp += d;
                        _idx += d;
                    }
                    assert(valid());
                    return *this;}

                // -----------
                // operator -=
                // -----------

                /**
                 * @param d a difference_type which is the amount that is substracted
                 * @return a const iterator reference
                 * (this - d) => this
                 */
                const_iterator& operator -= (difference_type d) {
                    if(_idx - d < 0) {
                        int temp = d - (_idx + 1);
                        --_r;
                        while(temp > 9) {
                            temp -= 10;
                            --_r;
                        }
                        _idx = 9 - temp;
                        _cp = *_r + _idx;
                    }
                    else { 
                        _cp -= d;
                        _idx -= d;
                    }
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
        explicit MyDeque (const allocator_type& a = allocator_type()) 
            : _a(a), _front(0), _back(_front), _rows(10), _size(0), _capacity(0) {
            //_rows = 10;
            deque = _aPointer.allocate(_rows);
            //_front = 0;
            //_back = _front;
            //_capacity = 0;
            //_size = 0;
            _outerFront = deque;
            assert(valid());}

        /**
         * @param s a size_type
         * @param v a const reference that is defaulted
         * @param a a const allocator_type reference that is defaulted
         * Constructor with size specification
         */
        explicit MyDeque (size_type s, const_reference v = value_type(), const allocator_type& a = allocator_type()) : _a(a), _size(s) {
            assert(s > 0);
            _rows = s / 10;
            if (s % 10 != 0)
                ++_rows;
            _capacity = _rows * 10;
            // Allocate rows and columns
            deque = _aPointer.allocate(_rows);
            for(int i = 0; i < _rows; i++)
                *(deque + i) = _a.allocate(10);
            
            int temp_s = _size;
            // Only fill one row
            if(_size < 10){
                uninitialized_fill(_a, *deque, *deque + temp_s, v);
                _front = *deque;
                _back = _front + temp_s;
            }
            // Fill rows and columns
            else {
                // Start in the middle
                int begin = (_capacity - _size) / 2;
                uninitialized_fill(_a, *deque + begin, *deque + 10, v);
                temp_s -= 10 - begin;
                int row = 1;
                while(temp_s > 10) {
                    uninitialized_fill(_a, *(deque + row), *(deque + row) + 10, v);
                    temp_s -= 10;
                    ++row;
                }
                if(temp_s > 0)
                    uninitialized_fill(_a, *(deque + row), *(deque + row) + temp_s, v);
                if(temp_s == 0)
                    temp_s = 10;
                _front = *deque + begin;
                _back = *(deque + (_rows - 1)) + temp_s;
            }
            _outerFront = deque;
            assert(valid());}

        /**
         * @param that a const MyDeque reference
         * Copy constructor
         */
        MyDeque (const MyDeque& that) : _a(that._a), _aPointer(that._aPointer), _size(that._size) {
            _rows = _size / 10;
            while(_size % 10 != 0)
              ++_rows;
            _capacity = _rows * 10;
            // Allocate the rows and columns
            deque = _aPointer.allocate(_rows);
            for(int i = 0; i < _rows; i++)
              *(deque + i) = _a.allocate(10);

            int temp_s = _size;
            int begin = (_capacity - _size) / 2;
            if(_size < 10) {
                uninitialized_copy(_a, that._front, *that._outerFront + 10, *deque + begin);
                temp_s -= (10 - begin);
                int row = 1;
                while(temp_s > 10) {
                    uninitialized_copy(_a, *(that._outerFront + row), *(that._outerFront + row) + 10, *(deque + row));
                    temp_s -= 10;
                    ++row;
                }
                if(temp_s > 0) {
                    uninitialized_copy(_a, *(that._outerFront + row), that._back, *(deque + row));
                    ++row;
                }
                _front = *deque + begin;
                _back = *(deque + row) + temp_s;
            }
            else {
                uninitialized_copy(_a, that._front, that._back, *deque + begin);
                _front = *deque + begin;
                _back = *deque + temp_s;
            }
            _outerFront = deque;
            assert(valid());}


        // --------------------
        // capacity constructor
        // --------------------

        /*
         * @param that a MyDeque reference
         * @param c a size_type which is the new capacity
         * A copy constructor with capacity change.
         */ 
        MyDeque (const MyDeque& that, size_type c) : _a(that._a), _aPointer(that._aPointer) {
            _size = that._size;
            _rows = c / 10;
            if (c % 10 != 0)
                ++_rows;
            _capacity = _rows * 10;
            int thatRow = 0;
            int temp_s = _size;

            deque = _aPointer.allocate(_rows);
            for(int i = 0; i < _rows; i++)
              *(deque + i) = _a.allocate(10);
            
            if(that._size <= 10)
                uninitialized_copy(_a, that.begin(), that.end(), *deque);
            else
            {
                uninitialized_copy(_a, that.begin(), that.begin() + 10, *deque);
                const_iterator it = that.begin() + 10;
                temp_s = _size - 10;
                thatRow = 1;
                while(temp_s > 10)
                {
                    uninitialized_copy(_a, it, it+10, *(deque + thatRow));
                    it += 10;
                    temp_s -= 10;
                    thatRow++;
                }
                uninitialized_copy(_a, it, that.end(), *(deque + thatRow));
            }
            _front = *deque;
            _back = *(deque + thatRow) + temp_s;
            _outerFront = deque;
            assert(valid());
        }

        // ----------
        // destructor
        // ----------

        /**
         * MyDeque destructor
         * Clear and deallocate MyDeque memory.
         */
        ~MyDeque () {
            if(_front)
            {
                clear();
                for(int i = 0; i < _rows; i++)
                    _a.deallocate(*(deque + i), 10);
                _aPointer.deallocate(deque, _rows);
            }
                /*assert(valid());*/}


        // ----------
        // operator =
        // ----------

        /**
         * @param rhs a const MyDeque reference
         * @return a MyDeque reference
         * Assign a MyDeque to equal another
         */
        MyDeque& operator = (const MyDeque& rhs) {
            if(this == &rhs)
                return *this;
            // If the sizes are the same, copy rhs to this
            else if (rhs.size() == size())
                std::copy(rhs.begin(), rhs.end(), begin());
            // If rhs's size is less than this, copy rhs to this and resize this
            else if (rhs.size() < size()) {
                std::copy(rhs.begin(), rhs.end(), begin());
                resize(rhs.size());
            }
            // If rhs's size is less than or equal to 
            else if (rhs.size() <= _capacity) {
                std::copy(rhs.begin(), rhs.end(), begin());
                iterator it = uninitialized_copy(_a, rhs.begin() + size(), rhs.end(), end());
                _size = rhs.size();
                _back = (it - 1).i_pointer();
                _back++;
            }
            else {
                clear();
                if(rhs.size() > _capacity) {
                    MyDeque<T> x(*this, rhs.size());
                    swap(x);
                }
                iterator it = uninitialized_copy(_a, rhs.begin(), rhs.end(), begin());
                _size = rhs.size();
                _back = (it - 1).i_pointer();
                _back++;
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
            return *(begin() + index);}

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
         */
        reference at (size_type index) {
            return *(begin() + index);}

        /**
         * @param index a size_type
         * @return a const reference
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
            return *(end() - 1);}

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
            int index = _front - *_outerFront;
            return iterator(_outerFront, _front, _back, index, _size);}

        /**
         * @return an const iterator
         * Returns the beginning iterator of MyDeque
         */
        const_iterator begin () const {
            int index = _front - *_outerFront;
            return const_iterator(_outerFront, _front, _back, index, _size);}

        // -----
        // clear
        // -----

        /**
* clears the deque (does not deallocate).
*/
        void clear () {
            destroy(_a, begin(), end());
            _front = *(deque + (_rows/2)) + 5;
            _back = _front;
            _outerFront = deque + (_rows/2);
            _size = 0;
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
            int rowOffset = 0;
            int index = 0;
            if((_front - *_outerFront) == 0 && (_size%10) == 0) {
                rowOffset = (_size / 10) - 1;
                index = 10;
            }
            else {
                rowOffset = _size / 10;
                index = _back - *(_outerFront + rowOffset);
            }
            return iterator(_outerFront + rowOffset, _back, _back, index, _size);}

        /**
         * @return a const iterator
         * Returns the end iterator of MyDeque
         */
          const_iterator end () const {
            int rowOffset = 0;
            int index = 0;
            if((_front - *_outerFront) == 0 && (_size%10) == 0) {
                rowOffset = (_size / 10) - 1;
                index = 10;
            }
            else {
                rowOffset = _size / 10;
                index = _back - *(_outerFront + rowOffset);
            }
            return const_iterator(_outerFront + rowOffset, _back, _back, index, _size);}

        // -----
        // erase
        // -----

        /**
         * @param an iterator
         * @return an iterator
         * Clears the data without deallocation
         */
        iterator erase (iterator i) {
            if(i == begin()) {
                _front = (begin() + 1).i_pointer();
                --_size;
                _a.destroy(&*i);
                i = begin();
            }
            assert(valid());
            return i;}

        // -----
        // front
        // -----

        /**
         * @return a reference
         * Returns the front element of this Mydeque
         */
        reference front () {
            return *_front;}

        /**
         * @return a const reference
         * Returns the front element of this const Mydeque
         */
        const_reference front () const {
            return const_cast<MyDeque*>(this)->front();}

        // ------
        // insert
        // ------

        /**
         * @param i an iterator
         * @param v a const reference which is the value inserted
         * @return an iterator with inserted value
         * Insert the const reference v into MyDeque
         */
        iterator insert (iterator i, const_reference v) {
            int counter = 0;
            iterator temp_i = i;
            if(begin() == i) {
                push_front(v);
                temp_i = begin();
            }
            else if(end() == i) {
                push_back(v);
                temp_i = (end() - 1);
            }
            else
            {
                if(_size == _capacity) {
                    while(begin() + counter != i)
                        ++counter;
                    resize(_capacity * 2);
                    i = begin() + counter;
                }
                if(!(end() == begin() + _capacity)){
                    temp_i = end();
                    while(!(temp_i == i)){
                        *temp_i = *(temp_i - 1);
                        --temp_i;
                    }
                    *temp_i = v;
                    ++_size;
                    _back = (end() + 1).i_pointer();
                } else {
                    temp_i = begin() - 1;
                    while(!(temp_i == i)){
                        *temp_i = *(temp_i + 1);
                        ++temp_i;
                    }
                    *temp_i = v;
                    ++_size;
                    _front = (begin() - 1).i_pointer();
                }
            }
            assert(valid());
            return temp_i;}

        // ---
        // pop
        // ---

        /**
* removes the very last element.
*/
        void pop_back () {
            _a.destroy(_back-1);
            _back = (end()-1).i_pointer();
            --_size;
            assert(valid());}

        /**
* removes the very first element.
*/
        void pop_front () {
            _a.destroy(_front);
            ++_front;
            --_size;
            assert(valid());}

        // ----
        // push
        // ----

        /**
* inserts an element to the back of the deque.
*/
        void push_back (const_reference v) {
            resize(_size+1, v);
            assert(valid());}

        /**
* inserts an element to the front of the deque.
*/
        void push_front (const_reference v) {
            if(_size == _capacity)
            {
                resize(_capacity * 2);
            }
            if(_front == *deque)
            {
                iterator temp = end();
                while(!(temp == begin()))
                {
                    *temp = *(temp - 1);
                    temp--;
                }
                *temp = v;
                _back = (end() + 1).i_pointer();
            }
            else
            {
                *(begin() - 1) = v;
                _front = (begin() - 1).i_pointer();
            }
            _size++;
            assert(valid());}

        // ------
        // resize
        // ------

        /**
* resizes the deque to size s and fills it with values v.
*/
        void resize (size_type s, const_reference v = value_type()) {
            if(s == _size)
                return;
            else if(s < _size) {
                iterator it = destroy(_a, begin() + s, end());
                _back = (it-1).i_pointer() + 1;
                _size = s;
            }
            else if(s <= _capacity) {
                _size = s;
                int offset = (_capacity - _size) / 2;
                int rowOffset = 0;
                while(offset > 10) {
                    offset -= 10;
                    rowOffset++;
                }
                _front = *(deque + rowOffset) + offset;
                iterator it = uninitialized_fill(_a, begin(), (begin() + _size), v);
                _back = (it-1).i_pointer() + 1;
            }
            else {
                if(s > (2 * _size)) {
                    MyDeque d(s, v);
                    swap(d);
                }
                else {
                    MyDeque d(2 * _size, v);
                    swap(d);
                }
            }
            assert(valid());}

        // ----
        // size
        // ----

        /**
* returns the size of the deque.
*/
        size_type size () const {
            return _size;}

        // ----
        // swap
        // ----

        /**
* swaps this for that.
*/
        void swap (MyDeque& that) {
          if (_a == that._a) {
            std::swap(_front, that._front);
            std::swap(_back, that._back);
            std::swap(deque, that.deque);
            std::swap(_outerFront, that._outerFront);
            int temp_rows = _rows;
            _rows = that._rows;
            that._rows = temp_rows;
            int temp = _capacity;
            _capacity = that._capacity;
            that._capacity = temp;
            temp = _size;
            _size = that._size;
            that._size = temp;}
          else {
            MyDeque x(*this);
            *this = that;
            that = x;}
            assert(valid());}};

#endif // Deque_h
