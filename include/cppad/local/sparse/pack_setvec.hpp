# ifndef CPPAD_LOCAL_SPARSE_PACK_SETVEC_HPP
# define CPPAD_LOCAL_SPARSE_PACK_SETVEC_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
# include <cppad/core/cppad_assert.hpp>
# include <cppad/local/pod_vector.hpp>

// BEGIN_CPPAD_LOCAL_SPARSE_NAMESPACE
namespace CppAD { namespace local { namespace sparse {

// forward declaration of iterator class
class pack_setvec_const_iterator;

// ============================================================================
class pack_setvec {
// ============================================================================
/*
$begin pack_setvec_member_data$$
$spell
    setvec
    resize
$$

$section class pack_setvec: Private Member Data$$

$head Pack$$
Type used to pack multiple elements of a set (multiple bits) onto one
$icode Pack$$ value.

$head n_bit_$$
Number of bits (elements) per $icode Pack$$ value.

$head one_$$
The $icode Pack$$ value with all bits zero, except for the lowest order bit.

$head n_set_$$
Number of sets that we are representing.

$head end_$$
The possible elements in each set are $code 0$$, $code 1$$, ...,
$code end_-1$$.

$head n_pack_$$
Number of Pack values used to represent one set in the vector; i.e.,
to represent $code end_$$ bits.

$head data_$$
Data for all of the sets.

$head Source Code$$
$srccode%hpp% */
private:
    typedef size_t    Pack;
    const size_t      n_bit_;
    const Pack        one_;
    size_t            n_set_;
    size_t            end_;
    size_t            n_pack_;
    pod_vector<Pack>  data_;
/* %$$
$end
-----------------------------------------------------------------------------
$begin pack_setvec_vec_memory$$
$spell
    setvec
$$

$section class pack_setvec: Approximate Memory Used by Vector$$

$head Public$$
This function is declared public, but is not part of
$cref SetVector$$ concept.

$head Implementation$$
$srccode%hpp% */
public:
    size_t memory(void) const
    {   return data_.capacity() * sizeof(Pack); }
/* %$$
$end
-------------------------------------------------------------------------------
$begin pack_setvec_iterators$$
$spell
    setvec
    Iterators
    typedef
    const_iterator
$$

$section class pack_setvec: Iterators$$

$head SetVector Concept$$
$cref/const_iterator/SetVector/const_iterator/$$

$head typedef$$
$srccode%hpp% */
public:
    /// declare a const iterator
    friend class pack_setvec_const_iterator;
    typedef pack_setvec_const_iterator const_iterator;
/* %$$
$end
-------------------------------------------------------------------------------
$begin pack_setvec_default_ctor$$
$spell
    setvec
$$

$section class pack_setvec: Default Constructor$$

$head SetVector Concept$$
$cref/constructor/SetVector/Vector Operations/Constructor/$$

$head n_bit_$$
This member variable is set to the number of bits in a $icode Pack$$ value.

$head one_$$
This member variable has only its lowest order bit non-zero;

$head data_$$
This member is initialized as the empty vector; i.e., size zero..

$head Other$$
All the other member data are $code size_t$$ values
that are initialized as zero.

$head Implementation$$
$srccode%hpp% */
public:
    pack_setvec(void) :
    n_bit_( std::numeric_limits<Pack>::digits ),
    one_(1), n_set_(0), end_(0), n_pack_(0), data_(0)
    { }
/* %$$
$end
-------------------------------------------------------------------------------
$begin pack_setvec_destructor$$
$spell
    setvec
$$

$section class pack_setvec: Destructor$$

$head Implementation$$
$srccode%hpp% */
public:
    ~pack_setvec(void)
    { }
/* %$$
$end
-------------------------------------------------------------------------------
$begin pack_setvec_copy_ctor$$
$spell
    setvec
    CppAD
$$

$section class pack_setvec: Copy Constructor$$

$head v$$
The vector of sets that we are attempting to make a copy of.

$head Implementation$$
Using the copy constructor is probably due to a $code pack_setvec$$
being passed by value instead of by reference.
This is a CppAD programing error (not CppAD user error).
$srccode%hpp% */
public:
    pack_setvec(const pack_setvec& v) :
    n_bit_( std::numeric_limits<Pack>::digits ), one_(1)
    {   CPPAD_ASSERT_UNKNOWN(0); }
/* %$$
$end
-------------------------------------------------------------------------------
$begin pack_setvec_vec_resize$$
$spell
    setvec
    resize
$$

$section class pack_setvec: Vector resize$$

$head SetVector Concept$$
$cref/vector resize/SetVector/Vector Operations/resize/$$

$head Prototype$$
$srccode%hpp% */
public:
    void resize(size_t n_set, size_t end)
/* %$$
$end
*/
    {   n_set_          = n_set;
        end_            = end;
        if( n_set_ == 0 )
        {   CPPAD_ASSERT_UNKNOWN( end == 0 );
            data_.clear();
            return;
        }
        // now start a new vector with empty sets
        Pack zero(0);
        //
        n_pack_         = ( 1 + (end_ - 1) / n_bit_ );
        size_t i        = n_set_ * n_pack_;
        //
        data_.resize(i);
        while(i--)
            data_[i] = zero;
    }
/* %$$
-------------------------------------------------------------------------------
$begin pack_setvec_vec_n_set$$
$spell
    setvec
$$

$section class pack_setvec: Number of Sets$$

$head SetVector Concept$$
$cref/n_set/SetVector/Vector Operations/n_set/$$

$head Implementation$$
$srccode%hpp% */
public:
    size_t n_set(void) const
    {   return n_set_;  }
/* %$$
$end
-------------------------------------------------------------------------------
$begin pack_setvec_vec_end$$
$spell
    setvec
$$

$section class pack_setvec: End Value$$

$head SetVector Concept$$
$cref/end/SetVector/Vector Operations/end/$$

$head Implementation$$
$srccode%hpp% */
public:
    size_t end(void) const
    {   return end_; }
/* %$$
$end
-------------------------------------------------------------------------------
$begin pack_setvec_vec_assignment$$
$spell
    setvec
$$

$section class pack_setvec: Vector Assignment$$

$head SetVector Concept$$
$cref/vector assignment/SetVector/Vector Operations/Assignment/$$

$head Prototype$$
$srccode%hpp% */
public:
    void operator=(const pack_setvec& other)
/* %$$
$end
*/
    {   CPPAD_ASSERT_UNKNOWN( n_bit_  == other.n_bit_);
        CPPAD_ASSERT_UNKNOWN( one_    == other.one_);
        n_set_  = other.n_set_;
        end_    = other.end_;
        n_pack_ = other.n_pack_;
        data_   = other.data_;
    }
/*
-------------------------------------------------------------------------------
$begin pack_setvec_vec_swap$$
$spell
    setvec
$$

$section class pack_setvec: Vector Swap$$

$head SetVector Concept$$
$cref/vector swap/SetVector/Vector Operations/swap/$$

$head Prototype$$
$srccode%hpp% */
public:
    void swap(pack_setvec& other)
/* %$$
$end
*/
    {   // size_t objects
        CPPAD_ASSERT_UNKNOWN( n_bit_  == other.n_bit_);
        CPPAD_ASSERT_UNKNOWN( one_    == other.one_);
        std::swap(n_set_  , other.n_set_);
        std::swap(end_    , other.end_);
        std::swap(n_pack_ , other.n_pack_);
        //
        // pod_vectors
        data_.swap(other.data_);
    }
/*
-------------------------------------------------------------------------------
$begin pack_setvec_number_elements$$
$spell
    setvec
$$

$section class pack_setvec: Number of Elements in a Set$$

$head SetVector Concept$$
$cref/number_elements/SetVector/number_elements/$$

$head Prototype$$
$srccode%hpp% */
public:
    size_t number_elements(size_t i) const
/* %$$
$end
*/
    {   CPPAD_ASSERT_UNKNOWN( i < n_set_ );
        //
        // special case where data_[i] is 0 or 1
        if( end_ == 1 )
        {   CPPAD_ASSERT_UNKNOWN( n_pack_ == 1 );
            return size_t( data_[i] );
        }
        //
        // initialize count of non-zero bits in this set
        size_t count = 0;
        //
        // mask corresonding to first bit in Pack
        Pack mask = one_;
        //
        // number of bits in last Packing unit
        size_t n_last = (end_ - 1) % n_bit_ + 1;
        //
        // count bits in last unit
        Pack unit = data_[(i + 1) * n_pack_ - 1];
        for(size_t bit = 0; bit < n_last; ++bit)
        {   CPPAD_ASSERT_UNKNOWN( mask >= one_ );
            if( mask & unit )
                ++count;
            mask = mask << 1;
        }
        if( n_pack_ == 1 )
            return count;
        //
        // count bits in other units
        for(size_t bit = 0; bit < n_bit_; ++bit)
        {   CPPAD_ASSERT_UNKNOWN( mask >= one_ );
            size_t k = n_pack_;
            while(--k)
            {   if( data_[i * n_pack_ + k] & mask )
                    ++count;
            }
            mask = mask << 1;
        }
        return count;
    }
/*
-------------------------------------------------------------------------------
$begin pack_setvec_add_element$$
$spell
    setvec
$$

$section class pack_setvec: Add an Elements to a Set$$

$head SetVector Concept$$
$cref/add_element/SetVector/add_element/$$

$head Prototype$$
$srccode%hpp% */
public:
    void add_element(size_t i, size_t element)
/* %$$
$end
*/
    {   CPPAD_ASSERT_UNKNOWN( i   < n_set_ );
        CPPAD_ASSERT_UNKNOWN( element < end_ );
        if( end_ == 1 )
            data_[i] |= one_;
        else
        {   size_t j  = element / n_bit_;
            size_t k  = element - j * n_bit_;
            Pack mask = one_ << k;
            data_[ i * n_pack_ + j] |= mask;
        }
    }
/*
-------------------------------------------------------------------------------
$begin pack_setvec_post_element$$
$spell
    setvec
$$

$section class pack_setvec: Add an Elements to a Set$$

$head SetVector Concept$$
$cref/post_element/SetVector/post_element/$$

$head Implementation$$
$srccode%hpp% */
public:
    void post_element(size_t i, size_t element)
    {   add_element(i, element); }
/* %$$
$end
*/
/*
-------------------------------------------------------------------------------
$begin pack_setvec_process_post$$
$spell
    setvec
$$

$section class pack_setvec: Add Posted Elements to a Set$$

$head SetVector Concept$$
$cref/process_post/SetVector/process_post/$$

$head Implementation$$
$srccode%hpp% */
public:
    void process_post(size_t i)
    {   return; }
/* %$$
$end
*/
    // -----------------------------------------------------------------
    /*!
    Is an element of a set.

    \param i
    is the index for this set in the vector of sets.

    \param element
    is the element we are checking to see if it is in the set.
    */
    bool is_element(size_t i, size_t element) const
    {   static Pack one(1);
        static Pack zero(0);
        CPPAD_ASSERT_UNKNOWN( i   < n_set_ );
        CPPAD_ASSERT_UNKNOWN( element < end_ );
        size_t j  = element / n_bit_;
        size_t k  = element - j * n_bit_;
        Pack mask = one << k;
        return (data_[ i * n_pack_ + j] & mask) != zero;
    }
    // -----------------------------------------------------------------
    /*!
    Assign the empty set to one of the sets.

    \param target
    is the index of the set we are setting to the empty set.

    \par Checked Assertions
    \li target < n_set_
    */
    void clear(size_t target)
    {   // value with all its bits set to false
        static Pack zero(0);
        CPPAD_ASSERT_UNKNOWN( target < n_set_ );
        size_t t = target * n_pack_;

        size_t j = n_pack_;
        while(j--)
            data_[t++] = zero;
    }
    // -----------------------------------------------------------------
    /*!
    Assign one set equal to another set.

    \param this_target
    is the index (in this pack_setvec object) of the set being assinged.

    \param other_value
    is the index (in the other pack_setvec object) of the
    that we are using as the value to assign to the target set.

    \param other
    is the other pack_setvec object (which may be the same as this
    pack_setvec object).

    \par Checked Assertions
    \li this_target  < n_set_
    \li other_value  < other.n_set_
    \li n_pack_     == other.n_pack_
    */
    void assignment(
        size_t               this_target  ,
        size_t               other_value  ,
        const pack_setvec&   other        )
    {   CPPAD_ASSERT_UNKNOWN( this_target  <   n_set_        );
        CPPAD_ASSERT_UNKNOWN( other_value  <   other.n_set_  );
        CPPAD_ASSERT_UNKNOWN( n_pack_      ==  other.n_pack_ );
        size_t t = this_target * n_pack_;
        size_t v = other_value * n_pack_;

        size_t j = n_pack_;
        while(j--)
            data_[t++] = other.data_[v++];
    }
    // -----------------------------------------------------------------
    /*!
    Assing a set equal to the union of two other sets.

    \param this_target
    is the index (in this pack_setvec object) of the set being assinged.

    \param this_left
    is the index (in this pack_setvec object) of the
    left operand for the union operation.
    It is OK for this_target and this_left to be the same value.

    \param other_right
    is the index (in the other pack_setvec object) of the
    right operand for the union operation.
    It is OK for this_target and other_right to be the same value.

    \param other
    is the other pack_setvec object (which may be the same as this
    pack_setvec object).

    \par Checked Assertions
    \li this_target <  n_set_
    \li this_left   <  n_set_
    \li other_right <  other.n_set_
    \li n_pack_     == other.n_pack_
    */
    void binary_union(
        size_t                  this_target  ,
        size_t                  this_left    ,
        size_t                  other_right  ,
        const pack_setvec&      other        )
    {   CPPAD_ASSERT_UNKNOWN( this_target < n_set_         );
        CPPAD_ASSERT_UNKNOWN( this_left   < n_set_         );
        CPPAD_ASSERT_UNKNOWN( other_right < other.n_set_   );
        CPPAD_ASSERT_UNKNOWN( n_pack_    ==  other.n_pack_ );

        size_t t = this_target * n_pack_;
        size_t l  = this_left  * n_pack_;
        size_t r  = other_right * n_pack_;

        size_t j = n_pack_;
        while(j--)
            data_[t++] = ( data_[l++] | other.data_[r++] );
    }
    // -----------------------------------------------------------------
    /*!
    Assing a set equal to the intersection of two other sets.

    \param this_target
    is the index (in this pack_setvec object) of the set being assinged.

    \param this_left
    is the index (in this pack_setvec object) of the
    left operand for the intersection operation.
    It is OK for this_target and this_left to be the same value.

    \param other_right
    is the index (in the other pack_setvec object) of the
    right operand for the intersection operation.
    It is OK for this_target and other_right to be the same value.

    \param other
    is the other pack_setvec object (which may be the same as this
    pack_setvec object).

    \par Checked Assertions
    \li this_target <  n_set_
    \li this_left   <  n_set_
    \li other_right <  other.n_set_
    \li n_pack_     == other.n_pack_
    */
    void binary_intersection(
        size_t                  this_target  ,
        size_t                  this_left    ,
        size_t                  other_right  ,
        const pack_setvec&      other        )
    {   CPPAD_ASSERT_UNKNOWN( this_target < n_set_         );
        CPPAD_ASSERT_UNKNOWN( this_left   < n_set_         );
        CPPAD_ASSERT_UNKNOWN( other_right < other.n_set_   );
        CPPAD_ASSERT_UNKNOWN( n_pack_    ==  other.n_pack_ );

        size_t t = this_target * n_pack_;
        size_t l  = this_left  * n_pack_;
        size_t r  = other_right * n_pack_;

        size_t j = n_pack_;
        while(j--)
            data_[t++] = ( data_[l++] & other.data_[r++] );
    }
    // -----------------------------------------------------------------
    /*!
    Print the vector of sets (used for debugging)
    */
    void print(void) const;
};
// ==========================================================================
/*!
cons_iterator for one set of positive integers in a pack_setvec object.

All the public members for this class are also in the
sparse::pack_setvec_const_iterator and sparse::svec_setvec_const_iterator classes.
This defines the CppAD vector_of_sets iterator concept.
*/
class pack_setvec_const_iterator {
private:
    /// Type used to pack elements in pack_setvec
    typedef pack_setvec::Pack Pack;

    /// data for the entire vector of sets
    const pod_vector<Pack>&  data_;

    /// Number of bits per Pack value
    const size_t             n_bit_;

    /// Number of Pack values necessary to represent end_ bits.
    const size_t             n_pack_;

    /// Possible elements in each set are 0, 1, ..., end_ - 1;
    const size_t             end_;

    /// index of this set in the vector of sets;
    const size_t             set_index_;

    /// value of the next element in this set
    /// (use end_ for no such element exists; i.e., past end of the set).
    size_t                   next_element_;
public:
    /// construct a const_iterator for a set in a pack_setvec object
    pack_setvec_const_iterator (const pack_setvec& pack, size_t set_index)
    :
    data_          ( pack.data_ )         ,
    n_bit_         ( pack.n_bit_ )        ,
    n_pack_        ( pack.n_pack_ )       ,
    end_           ( pack.end_ )          ,
    set_index_     ( set_index )
    {   static Pack one(1);
        CPPAD_ASSERT_UNKNOWN( set_index_ < pack.n_set_ );
        //
        next_element_ = 0;
        if( next_element_ < end_ )
        {   Pack check = data_[ set_index_ * n_pack_ + 0 ];
            if( check & one )
                return;
        }
        // element with index zero is not in this set of integers,
        // advance to first element or end
        ++(*this);
    }

    /// advance to next element in this set
    pack_setvec_const_iterator& operator++(void)
    {   static Pack one(1);
        CPPAD_ASSERT_UNKNOWN( next_element_ <= end_ );
        if( next_element_ == end_ )
            return *this;
        //
        ++next_element_;
        if( next_element_ == end_ )
            return *this;
        //
        // initialize packed data index
        size_t j  = next_element_ / n_bit_;

        // initialize bit index
        size_t k  = next_element_ - j * n_bit_;

        // initialize mask
        size_t mask = one << k;

        // start search at this packed value
        Pack check = data_[ set_index_ * n_pack_ + j ];
        //
        while( true )
        {   // check if this element is in the set
            if( check & mask )
                return *this;

            // increment next element before checking this one
            next_element_++;
            if( next_element_ == end_ )
                return *this;

            // shift mask to left one bit so corresponds to next_element_
            // (use mask <<= 1. not one << k, so compiler knows value)
            k++;
            mask <<= 1;
            CPPAD_ASSERT_UNKNOWN( k <= n_bit_ );

            // check if we must go to next packed data index
            if( k == n_bit_ )
            {   // get next packed value
                k     = 0;
                mask  = one;
                j++;
                CPPAD_ASSERT_UNKNOWN( j < n_pack_ );
                check = data_[ set_index_ * n_pack_ + j ];
            }
        }
        // should never get here
        CPPAD_ASSERT_UNKNOWN(false);
        return *this;
    }

    /// obtain value of this element of the set of positive integers
    /// (end_ for no such element)
    size_t operator*(void) const
    {   return next_element_; }
};
// =========================================================================
/*!
Print the vector of sets (used for debugging)
*/
inline void pack_setvec::print(void) const
{   std::cout << "pack_setvec:\n";
    for(size_t i = 0; i < n_set(); i++)
    {   std::cout << "set[" << i << "] = {";
        const_iterator itr(*this, i);
        while( *itr != end() )
        {   std::cout << *itr;
            if( *(++itr) != end() )
                std::cout << ",";
        }
        std::cout << "}\n";
    }
    return;
}

// ==========================================================================

/*!
Copy a user vector of sets sparsity pattern to an internal pack_setvec object.

\tparam SetVector
is a simple vector with elements of type std::set<size_t>.

\param internal
The input value of sparisty does not matter.
Upon return it contains the same sparsity pattern as user
(or the transposed sparsity pattern).

\param user
sparsity pattern that we are placing internal.

\param n_set
number of sets (rows) in the internal sparsity pattern.

\param end
end of set value (number of columns) in the interanl sparsity pattern.

\param transpose
if true, the user sparsity patter is the transposed.

\param error_msg
is the error message to display if some values in the user sparstiy
pattern are not valid.
*/
template<class SetVector>
void sparsity_user2internal(
    pack_setvec&            internal  ,
    const SetVector&        user      ,
    size_t                  n_set     ,
    size_t                  end       ,
    bool                    transpose ,
    const char*             error_msg )
{   CPPAD_ASSERT_KNOWN(size_t( user.size() ) == n_set * end, error_msg );

    // size of internal sparsity pattern
    internal.resize(n_set, end);

    if( transpose )
    {   // transposed pattern case
        for(size_t j = 0; j < end; j++)
        {   for(size_t i = 0; i < n_set; i++)
            {   if( user[ j * n_set + i ] )
                    internal.add_element(i, j);
            }
        }
        return;
    }
    else
    {   for(size_t i = 0; i < n_set; i++)
        {   for(size_t j = 0; j < end; j++)
            {   if( user[ i * end + j ] )
                internal.add_element(i, j);
            }
        }
    }
    return;
}

} } } // END_CPPAD_LOCAL_SPARSE_NAMESPACE
# endif