# ifndef CppADFunIncluded
# define CppADFunIncluded

/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
------------------------------------------------------------------------ */
/*
$begin ADFun$$
$spell 
	xk
	Ind
	bool
	Taylor
	sizeof
	const
	std
	ind_taddr
	dep_taddr
$$

$spell
$$

$section ADFun Objects$$

$index ADFun, object$$
$index object, ADFun$$

$head Purpose$$
An $syntax%AD<%Base%>%$$ 
$xref/AD/AD Operation Sequence/operation sequence/$$
is stored in an $code ADFun$$ object by its $xref/FunConstruct/$$.
The $code ADFun$$ object can then be used to calculate function values,
derivative values, and other values related to the corresponding function.

$childtable%
	CppAD/local/Independent.h%
	CppAD/local/FunConstruct.h%
	omh/SeqProperty.omh%
	CppAD/local/FunEval.h%
	omh/Drivers.omh%
	omh/MulTape.omh%
	omh/FunDeprecated.omh
%$$

$end
*/


// BEGIN CppAD namespace
namespace CppAD {

template <class Base>
class ADFun {
	// type used for packing sparsity patters
	typedef size_t Pack;

public:
	// constructor
	template <typename VectorADBase>
	ADFun(const VectorADBase &u, const VectorADBase &z);

	// destructor
	~ADFun(void)
	{	delete Rec;
		CppADTrackDelVec(Taylor);
		if( ForJac != CppADNull )
			CppADTrackDelVec(ForJac);
	}

	// forward mode sweep
	template <typename VectorBase>
	VectorBase Forward(size_t p, const VectorBase &u);

	// reverse mode sweep
	template <typename VectorBase>
	VectorBase Reverse(size_t p, const VectorBase &v) const;

	// forward mode Jacobian sparsity 
	template <typename VectorBase>
	VectorBase ForSparseJac(size_t q, const VectorBase &Px);

	// reverse mode Jacobian sparsity 
	template <typename VectorBase>
	VectorBase RevSparseJac(size_t q, const VectorBase &Py) const;

	// reverse mode Hessian sparsity 
	template <typename VectorBase>
	VectorBase RevSparseHes(size_t q, const VectorBase &Py);

	// does this AD operation sequence use VecAD<Base>::reference operands
	bool use_VecAD(void) const
	{	return Rec->NumVecInd() > 0; }

	// number of variables in opertion sequence
	size_t size_var(void) const
	{	return totalNumVar; }
	size_t Size(void) const       // (Deprecated)
	{	return totalNumVar; }

	// number of Taylor coefficients currently stored (per variable)
	size_t taylor_size(void) const
	{	return taylor_per_var; } 
	size_t Order(void) const      // (Deprecated)
	{	return taylor_per_var - 1; }

	// number of independent variables
	size_t Domain(void) const
	{	return ind_taddr.size(); }

	// number of dependent variables
	size_t Range(void) const
	{	return dep_taddr.size(); }

	// is variable a parameter
	bool Parameter(size_t i)
	{	CppADUsageError(
			i < dep_taddr.size(),
			"Argument to Parameter is >= dimension of range space"
		);
		return dep_parameter[i]; 
	}

	// amount of memory for each variable (Deprecated)
	size_t Memory(void) const
	{	size_t pervar  = TaylorColDim * sizeof(Base)
		+ ForJacColDim * sizeof(Pack);
		size_t total   = totalNumVar * pervar + Rec->Memory();
		return total;
	}

# ifndef NDEBUG
	// in not NDEBUG case, number of comparison operations that change
	size_t CompareChange(void) const
	{	return compareChange; }
# endif

	// calculate entire Jacobian
	template <typename VectorBase>
	VectorBase Jacobian(const VectorBase &x); 

	// calculate Hessian for one component of f
	template <typename VectorBase>
	VectorBase Hessian(const VectorBase &x, size_t i); 

	// forward mode calculation of partial w.r.t one domain component
	template <typename VectorBase>
	VectorBase ForOne(
		const VectorBase   &x ,
		size_t              j );

	// reverse mode calculation of derivative of one range component
	template <typename VectorBase>
	VectorBase RevOne(
		const VectorBase   &x ,
		size_t              i );

	// forward mode calculation of a subset of second order partials
	template <typename VectorBase, typename VectorSize_t>
	VectorBase ForTwo(
		const VectorBase   &x ,
		const VectorSize_t &J ,
		const VectorSize_t &K );

	// reverse mode calculation of a subset of second order partials
	template <typename VectorBase, typename VectorSize_t>
	VectorBase RevTwo(
		const VectorBase   &x ,
		const VectorSize_t &I ,
		const VectorSize_t &J );

private:
	// maximum amount of memory required for this function object
	// mutable size_t memoryMax;

	// debug checking number of comparision operations that changed
	size_t compareChange;

	// number of Taylor coefficieint per variable (currently stored)
	size_t taylor_per_var;

	// number of bits currently calculated per row of the ForJac array
	size_t ForJacBitDim; 

	// number of columns currently allocated for Taylor array
	size_t TaylorColDim;

	// number of columns currently allocated for ForJac array
	size_t ForJacColDim;

	// number of rows (variables) in the recording (Rec)
	size_t totalNumVar;

	// tape address for the independent variables
	CppAD::vector<size_t> ind_taddr;

	// tape address and parameter flag for the dependent variables
	CppAD::vector<size_t> dep_taddr;
	CppAD::vector<bool>   dep_parameter;

	// the operations corresponding to this function
	TapeRec<Base> *Rec;

	// results of the forward mode calculations
	Base *Taylor;

	// results of the forward mode Jacobian sparsity calculations
	Pack *ForJac;
};
// ---------------------------------------------------------------------------

} // END CppAD namespace

// non-user interfaces
# include <CppAD/local/ForwardSweep.h>
# include <CppAD/local/ReverseSweep.h>
# include <CppAD/local/ForJacSweep.h>
# include <CppAD/local/RevJacSweep.h>
# include <CppAD/local/RevHesSweep.h>


// user interfaces
# include <CppAD/local/Independent.h>
# include <CppAD/local/FunConstruct.h>
# include <CppAD/local/FunEval.h>

// driver routines
# include <CppAD/local/Jacobian.h>
# include <CppAD/local/Hessian.h>
# include <CppAD/local/ForOne.h>
# include <CppAD/local/RevOne.h>
# include <CppAD/local/ForTwo.h>
# include <CppAD/local/RevTwo.h>

# endif
