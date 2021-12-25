#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <vector>
#include <iostream>
#include <string.h>

// #include "ft_container.hpp"
#include "iterator_traits.hpp"
#include "reverse_iterator.hpp"
// #include "My_iterator.hpp"

template <typename iterator> class MyIterator;
namespace ft
{
template < typename T, typename Alloc = std::allocator<T> >
class vector{

public:
    typedef T													value_type;
    typedef 													Alloc allocator_type;
    typedef typename allocator_type::reference 					reference;
    typedef typename allocator_type::const_reference 			const_reference;
    typedef typename allocator_type::pointer 					pointer;
    typedef typename allocator_type::const_pointer 				const_pointer;
    typedef MyIterator<pointer> 								iterator;
    typedef MyIterator<const_pointer> 							const_iterator;
	typedef Reverse_Iterator<iterator> 							reverse_iterator;
	typedef Reverse_Iterator<const_iterator> 					const_reverse_iterator;
    typedef ptrdiff_t                                           difference_type;
    typedef size_t                                              size_type;
private:
    allocator_type _allocator;
    size_type _capacity;
    size_type _size;
    pointer _arry;

	// void    reallocate(void)
    // {
    //     size_type       newCapacity = (!_capacity) ? 1 : 2 * _capacity;
    //     value_type      *ptr = _arry.allocate(newCapacity);

    //     for (size_type i = 0; i < _size; i++)
    //         ptr[i] = _allocator[i];
    //     _arry.deallocate(_allocator, _capacity);
    //     _allocator = ptr;
    //     _capacity = newCapacity;
    // }
public:
    
    explicit vector (const allocator_type& alloc = allocator_type()):
            _allocator(alloc), _capacity(0), _size(0), _arry(NULL){};
    explicit vector (size_type n, const value_type& val = value_type(),
    const allocator_type& alloc = allocator_type()): _allocator(alloc), _capacity(n), _size(0)
    {
        _arry = _allocator.allocate(_capacity);
    	while (_size < _capacity)
    		_arry[_size++] = val;
    };
    template <class InputIterator>
    vector (InputIterator first, InputIterator last,
    const allocator_type& alloc = allocator_type())
	: _capacity(static_cast<size_type>(last - first)), _size(0), _allocator(alloc)
    {
		_arry = _allocator.allocate(_capacity);
    	while (first != last)
    	_arry[_size++] = *first++;
    };
    vector(const vector& x): _capacity(x.size()), _size(0), _allocator(x._allocator)
	{
		_arry = _allocator.allocate(_capacity);
		while (_size < _capacity) {
			_arry[_size] = x[_size];
			++_size;
		}
	}
    ~vector()
	{
    	clear();
    	_allocator.deallocate(_arry, _capacity);
	}
	vector& operator=(const vector& x)
	{
    	if (_capacity < x.size())
		{
        	_allocator.deallocate(_arry, _capacity);
        	_capacity = x.size();
        	_arry = _allocator.allocate(_capacity);
    	}
    	_size = 0;
    	while (_size < x.size()) {
    	    _arry[_size] = x[_size];
    	    ++_size;
    	}
    	return *this;
	}
    iterator begin(){return (iterator(_arry));}
    const_iterator begin() const{return (iterator(_arry));}
    iterator end(){return (iterator(_arry + _size));}
    const_iterator end() const{return (iterator(_arry + _size));}
    size_type size() const{return (_size);}
    size_type max_size() const
    {
            return allocator_type().max_size(); 
    }
       
    void resize(size_type n, value_type val = value_type())
	{
		size_t i;
		if (n > _capacity)
		{
			_capacity = n;
			T *new_arr = _allocator.allocate(n);
			for (size_t i = 0; i < _size; i++)
				new_arr[i] = _arry[i];
			delete [] _arry;
			_arry = new_arr;
		}
		for (i = _size; i < n; i++)
			_allocator.construct(_arry + i, val);
		_size = n;
	}
            
    size_type capacity() const{return (_capacity);}
    reference front(){return iterator(*_arry);}
    const_reference front() const{return (*_arry);}
    reference back(){return (*(_arry + _size - 1));}
    const_reference back() const{return (*(_arry + _size - 1));}
    reference at (size_type n){return (*(_arry + n));}
    const_reference at (size_type n) const{return (*(_arry + n));}
    bool empty() const
    {
            if (_size == 0)
                    return(true);
            return(false);
    }
	reverse_iterator rbegin() { return (reverse_iterator(this->end())); }
	const_reverse_iterator rbegin() const { return (const_reverse_iterator(this->end())); }
	reverse_iterator rend() { return (reverse_iterator(this->begin())); }
	const_reverse_iterator rend() const { return (const_reverse_iterator(this->begin())); }
	void reserve(size_type n)
	{
		if (_capacity >= n)
			return;
		T *new_arr = _allocator.allocate(n);
		for (size_t i = 0; i < _size; i++)
			new_arr[i] = _arry[i];
		delete [] _arry;
		_arry = new_arr;
		_capacity = n;
	}
    template <class InputIterator>
	void assign(InputIterator first, InputIterator last)
	{
		for (size_t i = 0; i < _size; i++)
			_allocator.destroy(_arry + i);
		_size = 0;
		InputIterator tmp = first;
		while (tmp++ < last)
			_size++;
		if (_size > _capacity)
		{
			_allocator.deallocate(_arry, _capacity);
			_arry = _allocator.allocate(_size);
			_capacity = _size;
		}
		for (_size = 0; first < last; _size++)
		{
			_arry[_size] = *first;
			first++;
		}
	}
	
	void assign(size_t n, T val)
	{
		for (size_t i = 0; i < _size; i++)
			_allocator.destroy(_arry + i);
		if (n && n > _capacity)
		{
			_allocator.deallocate(_arry, _capacity);
			_arry = _allocator.allocate(n);
			_capacity = n;
		}
		for (size_t i = 0; i < n; i++)
			_allocator.construct(_arry + i, val);
		_size = n;
	}
	void push_back(const value_type &val)
	{
		if (_size >= _capacity)
		{
			if (_capacity != 0)
			{
				_capacity = _capacity * 2;
				T *new_arr = _allocator.allocate(_capacity);
				for (size_t i = 0; i < _size; i++)
					new_arr[i] = _arry[i];
				delete [] _arry;
				_arry = new_arr;
			}
			else
			{
				_capacity = 1;
				_arry = _allocator.allocate(_capacity);
			}
		}
		_arry[_size] = val;
		_size++;
	}
    void pop_back()
	{
		if (_size == 0)
			return;
		allocator_type().destroy(_arry + --_size);
	}
	iterator erase(iterator position)
	{
		return vector::erase(position, position + 1);
	}
	iterator erase(iterator first, iterator last)
	{
		size_t count = 0;
		if (last < first)
			return first;
		if (last > vector::end())
			last = vector::end();
		for (; first + count != last; count++)
			;
		for (size_t i = 0; last + i < vector::end(); i++)
		{
			*(first + i) = *(last + i);
		}
		for (size_t i = 0; i < count; i++)
			_allocator.destroy(_arry + _size - 1 - i);
		_size -= count;
		return (first);
	}
	template <class data>
	void SwapData(data &x, data &y)
	{
		data tmp;
		tmp = x;
		x = y;
		y = tmp;
	}
	void swap(vector &x)
	{
		if (this == &x)
			return;
		else
		{
			this->SwapData(this->_arry, x._arry);
			this->SwapData(this->_size, x._size);
			this->SwapData(this->_capacity, x._capacity);
		}
	} 
	void clear()
	{
		if (_size == 0)
			return ;
		for (size_t i = 0; i < _size; i++)
			_allocator.destroy(_arry + i);
		_size = 0;
	}
	allocator_type get_allocator() const { return _allocator; }

	value_type&     operator[](size_type n) {
        return _arry[n];
    }
    const value_type&   operator[](size_type n) const {
        return _arry[n];
    }
}; //class vector

    template <class T, class Alloc>
    bool operator==(const vector<T, Alloc> &lhs,
                    const vector<T, Alloc> &rhs)
    {
        if (lhs.size() != rhs.size())
            return (false);
        return std::equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    template <class T, class Alloc>
    bool operator!=(const vector<T, Alloc> &lhs,
                    const vector<T, Alloc> &rhs)
    {
        return (!(lhs == rhs));
    }

    template <class T, class Alloc>
    bool operator<(const vector<T, Alloc> &lhs,
                   const vector<T, Alloc> &rhs)
    {
        return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    template <class T, class Alloc>
    bool operator<=(const vector<T, Alloc> &lhs,
                    const vector<T, Alloc> &rhs)
    {
        return (!(rhs < lhs));
    }

    template <class T, class Alloc>
    bool operator>(const vector<T, Alloc> &lhs,
                   const vector<T, Alloc> &rhs)
    {
        return (!(lhs < rhs));
    }

    template <class T, class Alloc>
    bool operator>=(const vector<T, Alloc> &lhs,
                    const vector<T, Alloc> &rhs)
    {
        return (!(lhs <= rhs));
    }

} //ft_namespace

//my_iterator
template <typename T> 
class MyIterator
{
        typedef T                                                       		iterator_type;
        typedef typename iterator_traits<iterator_type>::difference_type       difference_type;
        typedef typename iterator_traits<iterator_type>::value_type            value_type;
        typedef typename iterator_traits<iterator_type>::pointer               pointer;
        typedef typename iterator_traits<iterator_type>::reference             reference;
        typedef typename iterator_traits<iterator_type>::iterator_category     iterator_category;

	private:

        iterator_type    p;
   	public:
        MyIterator(): p() {}
        explicit MyIterator(iterator_type x): p(x) {}
        MyIterator(const MyIterator& x): p(x.p) {}
        template <typename iter>
        MyIterator(const MyIterator<iter>& i): p(i.base()) {}
       	int& operator*() {return *p;}
       	T base() const{return (*p);}
       	MyIterator &operator++()
       	{ 
           ++p;
           return (*this);
	   	}
	   	MyIterator operator++(int)
       	{
           MyIterator cop(*this); 
           operator++(); 
           return (cop);
       	}
	   	MyIterator &operator--()
       	{
           --p; 
           return (*this);
       	}
		MyIterator &operator--(int)
       	{
           MyIterator cop(*this); 
           operator--(); 
           return (cop);
       	}
	   	MyIterator &operator+(difference_type nbr)
       	{
           p += nbr; 
           return (*this);
       	}
	   	MyIterator &operator-(difference_type nbr)
       	{ 
           p -= nbr;
           return (*this);
       	}
		MyIterator &operator+=(difference_type nbr)
       	{
           p += nbr;
           return (*this);
       	}
		MyIterator &operator-=(difference_type nbr)
       	{ 
           p -= nbr;
           return (*this);
       	}
		bool operator==(MyIterator const &tmp) const
       	{ 
           return (p == tmp.p);
       	}
		bool operator!=(MyIterator const &tmp) const
       	{ 
           return (p != tmp.p);
       	}
		T &operator[](difference_type nbr) 
       	{
           return (p[nbr]);
		}
};


template <typename iterator_A, typename iterator_B>
bool operator==(const MyIterator<iterator_A>& x, const MyIterator<iterator_B>& y) {
    return (x.base() == y.base());
}
template <typename iterator_A, typename iterator_B>
bool operator>(const MyIterator<iterator_A>& x, const MyIterator<iterator_B>& y) {
    return x.base() > y.base();
}
template <typename iterator_A, typename iterator_B>
bool operator<(const MyIterator<iterator_A>& x, const MyIterator<iterator_B>& y) {
    return x.base() < y.base();
}
template <typename iterator_A, typename iterator_B>
bool operator!=(const MyIterator<iterator_A>& x, const MyIterator<iterator_B>& y) {
    return !(x == y);
}
template <typename iterator_A, typename iterator_B>
bool operator>=(const MyIterator<iterator_A>& x, const MyIterator<iterator_B>& y) {
    return !(x < y);
}
template <typename iterator_A, typename iterator_B>
bool operator<=(const MyIterator<iterator_A>& x, const MyIterator<iterator_B>& y) {
    return !(x > y);
}
template <typename iterator_A, typename iterator_B>
typename MyIterator<iterator_A>::difference_type operator-(const MyIterator<iterator_A>& x, const MyIterator<iterator_B>& y) {
    return x.base() - y.base();
}
template<typename iterator>
MyIterator<iterator>   operator+(typename MyIterator<iterator>::difference_type n, const MyIterator<iterator>& x) {
    return MyIterator<iterator>(x.base() + n);
}

#endif