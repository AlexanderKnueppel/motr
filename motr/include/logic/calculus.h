#ifndef __MOTR_LOGIC_CALCULUS_H__
#define __MOTR_LOGIC_CALCULUS_H__  

/*
* ****************************************
*
* \Brief This file is part of motr (Model transformation test framework),
*	     a library for solving model transformation problems in the domain of feature diagrams.
*
* Copyright (c) 2015 motr. All rights reserved.
*
* For the full copyright and license information, please view the LICENSE.txt
* file that was distributed with this source code.
*
* \Author Alexander Knueppel
*
* ****************************************
*/

#include <string>
#include <iostream>
#include "../configuration.h"
#include "../util/variant.h"
#include <memory>
#include <set>
#include <map>
#include <vector>
#include <sstream>

MOTR_NS_BEGIN

struct Symbols { 
	const std::string true_s, 
					  false_s, 
					  not_s, 
					  and_s, 
					  or_s, 
					  xor_s, 
					  implies_s,
					  iff_s;
	Symbols(std::string true_s_, std::string false_s_, std::string not_s_,
		std::string and_s_, std::string or_s_, std::string xor_s_,
		std::string implies_s_, std::string iff_s_)
		: true_s(true_s_), false_s(false_s_), not_s(not_s_), and_s(and_s_),
		or_s(or_s_), xor_s(xor_s_), implies_s(implies_s_), iff_s(iff_s_) {
	}
};

auto const WrittenForm = Symbols("True", "False", "Not", "And", "Or", "Xor", "Implies", "Iff");
auto const Symbolic = Symbols("True", "False", "~", "&", "|", "<>", "=>", "<=>");

struct True;
struct False;
class Not;
struct And;
struct Or;
struct Xor;
struct Implies;
struct Iff;

using Formula = variant<
	std::string,
	True,
	False,
	recursive_wrapper<Not>,
	recursive_wrapper<And>,
	recursive_wrapper<Or>,
	recursive_wrapper<Xor>,
	recursive_wrapper<Implies>,
	recursive_wrapper<Iff>> ;

struct True {};
struct False {};



class Not {
public:
	Not(const Formula &p)
		: p_(std::make_shared<Formula>(p)) {
	}

	auto getFm() const -> Formula { 
		return Formula(*p_);
	}
private:
	const std::shared_ptr<Formula> p_;
};

struct And {
	And(const Formula &p, const Formula &q)
		: p_(std::make_shared<Formula>(p)), q_(std::make_shared<Formula>(q)) {
	}
	const std::shared_ptr<Formula> p_, q_;
};

/** Binary 'or' (inclusive disjunction) operator. */
struct Or {
	Or(const Formula &p, const Formula &q)
	: p_(std::make_shared<Formula>(p)), q_(std::make_shared<Formula>(q)) {
	}
	const std::shared_ptr<Formula> p_, q_;
};

struct Xor {
	Xor(const Formula &p, const Formula &q)
	: p_(std::make_shared<Formula>(p)), q_(std::make_shared<Formula>(q)) {
	}
	const std::shared_ptr<Formula> p_, q_;
};

struct Implies {
	Implies(const Formula &p, const Formula &q)
	: p_(std::make_shared<Formula>(p)), q_(std::make_shared<Formula>(q)) {
	}
	const std::shared_ptr<Formula> p_, q_;
};

struct Iff {
	Iff(const Formula &p, const Formula &q)
	: p_(std::make_shared<Formula>(p)), q_(std::make_shared<Formula>(q)) {
	}
	const std::shared_ptr<Formula> p_, q_;
};

/**
*
* Operator overload; API
*/
auto operator&(const Formula &f1, const Formula &f2) -> Formula {
	return And(f1,f2);
}
auto operator|(const Formula &f1, const Formula &f2) -> Formula {
	return Or(f1, f2);
}
auto operator~(const Formula &f) -> Formula {
	return Not(f);
}
auto operator>>(const Formula &f1, const Formula &f2) -> Formula {
	return Implies(f1, f2);
}
auto operator&=(Formula &f1, const Formula &f2) -> Formula {
	return (f1 = And(f1, f2));
}
auto operator|=(Formula &f1, const Formula &f2) -> Formula {
	return (f1 = Or(f1, f2));
}


/**
* Visitors
*/

struct VisitNot : public static_visitor<Formula> {
	auto operator()(const True &p)  const -> Formula { return False(); }
	auto operator()(const False &p) const -> Formula { return True(); }
	auto operator()(const Not &n)   const -> Formula { return n.getFm(); }

	template <class P>
	auto operator()(const P &p) const -> Formula { return Not(p); }
};

struct VisitAnd : public static_visitor<Formula> {
	auto operator()(const True &p, const True &q) const -> Formula { return True(); }
	auto operator()(const True &p, const False &q) const -> Formula { return False(); }
	auto operator()(const False &p, const True &q) const  -> Formula { return False(); }
	auto operator()(const False &p, const False &q) const -> Formula { return False(); }

	template <class P>
	auto operator()(const P &p, const True &q) const -> Formula{ return p; }

	template <class Q>
	auto operator()(const True &p, const Q &q) const -> Formula{ return q; }

	template <class P>
	auto operator()(const P &p, const False &q) const -> Formula{ return False(); }

	template <class Q>
	auto operator()(const False &p, const Q &q) const -> Formula{ return False(); }

	template <class P, class Q>
	auto operator()(const P &p, const Q &q) const -> Formula{ return And(p, q); }
};

struct VisitOr : public static_visitor<Formula> {
	auto operator()(const True &p, const True &q) const -> Formula { return True(); }
	auto operator()(const True &p, const False &q) const -> Formula  { return True(); }
	auto operator()(const False &p, const True &q) const -> Formula { return True(); }
	auto operator()(const False &p, const False &q) const -> Formula { return False(); }

	template <class P>
	auto operator()(const P &p, const True &q) const -> Formula{ return True(); }

	template <class Q>
	auto operator()(const True &p, const Q &q) const-> Formula { return True(); }

	template <class P>
	auto operator()(const P &p, const False &q) const -> Formula{ return p; }

	template <class Q>
	auto operator()(const False &p, const Q &q) const -> Formula { return q; }

	template <class P, class Q>
	auto operator()(const P &p, const Q &q) const -> Formula { return Or(p, q); }
};

struct VisitXor : public static_visitor<Formula> {
	auto operator()(const True &p, const True &q) const -> Formula { return False(); }
	auto operator()(const True &p, const False &q) const -> Formula { return True(); }
	auto operator()(const False &p, const True &q) const -> Formula { return True(); }
	auto operator()(const False &p, const False &q) const -> Formula { return False(); }

	template <class P>
	auto operator()(const P &p, const True &q) const -> Formula { return Not(p); }

	template <class Q>
	auto operator()(const True &p, const Q &q) const -> Formula { return Not(q); }

	template <class P>
	auto operator()(const P &p, const False &q) const -> Formula { return p; }

	template <class Q>
	auto operator()(const False &p, const Q &q) const -> Formula { return q; }

	template <class P, class Q>
	auto operator()(const P &p, const Q &q) const -> Formula { return Xor(p, q); }
};

/** Visitor for the messed up implication. */
struct VisitImplies : public static_visitor<Formula> {
	auto operator()(const True &p, const True &q) const -> Formula { return True(); }
	auto operator()(const True &p, const False &q) const -> Formula { return False(); }
	auto operator()(const False &p, const True &q) const -> Formula { return True(); }
	auto operator()(const False &p, const False &q) const -> Formula { return True(); }

	template <class P>
	auto operator()(const P &p, const True &q) const -> Formula { return True(); }

	template <class Q>
	auto operator()(const True &p, const Q &q) const -> Formula { return q; }

	template <class P>
	auto operator()(const P &p, const False &q) const -> Formula { return Not(p); }

	template <class Q>
	auto operator()(const False &p, const Q &q) const -> Formula { return True(); }

	template <class P, class Q>
	auto operator()(const P &p, const Q &q) const -> Formula { return Implies(p, q); }
};

struct VisitIff : public static_visitor<Formula> {
	auto operator()(const True &p, const True &q) const -> Formula { return True(); }
	auto operator()(const True &p, const False &q) const -> Formula { return False(); }
	auto operator()(const False &p, const True &q) const -> Formula { return False(); }
	auto operator()(const False &p, const False &q) const -> Formula { return True(); }

	template <class P>
	auto operator()(const P &p, const True &q) const -> Formula { return p; }

	template <class Q>
	auto operator()(const True &p, const Q &q) const -> Formula { return q; }

	template <class P>
	auto operator()(const P &p, const False &q) const -> Formula { return Not(p); }

	template <class Q>
	auto operator()(const False &p, const Q &q) const -> Formula { return Not(q); }

	template <class P, class Q>
	auto operator()(const P &p, const Q &q) const -> Formula { return Iff(p, q); }
};

struct SimplifyFormula : public static_visitor<Formula> {
	auto operator()(const Not &n) const -> Formula {
		auto fm(n.getFm());
		auto p = apply_visitor(SimplifyFormula(), fm);
		return apply_visitor(VisitNot(), p);
	}

	auto operator()(const And &a) const -> Formula  {
		auto p = apply_visitor(SimplifyFormula(), *a.p_);
		auto q = apply_visitor(SimplifyFormula(), *a.q_);
		return apply_visitor(VisitAnd(), p, q);
	}

	auto operator()(const Or &o) const -> Formula  {
		auto p = apply_visitor(SimplifyFormula(), *o.p_);
		auto q = apply_visitor(SimplifyFormula(), *o.q_);
		return apply_visitor(VisitOr(), p, q);
	}

	auto operator()(const Xor &x) const -> Formula  {
		auto p = apply_visitor(SimplifyFormula(), *x.p_);
		auto q = apply_visitor(SimplifyFormula(), *x.q_);
		return apply_visitor(VisitXor(), p, q);
	}

	auto operator()(const Implies &i) const -> Formula  {
		auto p = apply_visitor(SimplifyFormula(), *i.p_);
		auto q = apply_visitor(SimplifyFormula(), *i.q_);
		return apply_visitor(VisitImplies(), p, q);
	}

	auto operator()(const Iff &i) const -> Formula {
		auto p = apply_visitor(SimplifyFormula(), *i.p_);
		auto q = apply_visitor(SimplifyFormula(), *i.q_);
		return apply_visitor(VisitIff(), p, q);
	}

	template <class P> auto
		operator()(const P &p) const -> Formula { return Formula(p); }
};

struct Decomposition : public static_visitor<Formula> {
	auto operator()(const Not &n) const -> Formula {
		auto fm(n.getFm());
		auto p = apply_visitor(Decomposition(), fm);
		return apply_visitor(VisitNot(), p);
	}

	auto operator()(const And &a) const -> Formula  {
		auto p = apply_visitor(Decomposition(), *a.p_);
		auto q = apply_visitor(Decomposition(), *a.q_);
		return apply_visitor(VisitAnd(), p, q);
	}

	auto operator()(const Or &o) const -> Formula  {
		auto p = apply_visitor(Decomposition(), *o.p_);
		auto q = apply_visitor(Decomposition(), *o.q_);
		return apply_visitor(VisitOr(), p, q);
	}

	auto operator()(const Xor &x) const -> Formula  {
		auto p = apply_visitor(Decomposition(), *x.p_);
		auto q = apply_visitor(Decomposition(), *x.q_);
		return Or(And(p, Not(q)), And(q, Not(p)));
	}

	auto operator()(const Implies &i) const -> Formula  {
		auto p = apply_visitor(SimplifyFormula(), *i.p_);
		auto q = apply_visitor(SimplifyFormula(), *i.q_);
		return Or(Not(p), q);
	}

	auto operator()(const Iff &i) const -> Formula {
		auto p = apply_visitor(SimplifyFormula(), *i.p_);
		auto q = apply_visitor(SimplifyFormula(), *i.q_);
		return And(Or(Not(p), q), Or(Not(q), p));
	}

	template <class P> auto
		operator()(const P &p) const -> Formula { return Formula(p); }
};
/**
Surrounds a string with parens if a condition is true, returns unmodified
otherwise.
*/
std::string surroundIf(bool b, const std::string &s) {
	return b ? ("(" + s + ")") : s;
}

/** Helper function to handle infix operators. */
auto showInfix(bool b, int pr, const std::string &sym, const Formula &p,
	const Formula &q)->std::string;

/** Generates a string for formula. */
struct FormulaStr : public static_visitor<std::string> {
	const int pr; // Used to decide whether to add a parenthesis.

	FormulaStr(int pr_ = 0) : pr(pr_) {
	}

	std::string operator()(const std::string &s) const { return s; }

	std::string operator()(const True &t) const { return Symbolic.true_s; }

	std::string operator()(const False &f) const { return Symbolic.false_s; }

	std::string operator()(const Not &n) const {
		auto fm(n.getFm());
		if (fm.is<std::string>()) {
			return Symbolic.not_s + get<std::string>(fm);
		}
		auto s0 = apply_visitor(FormulaStr(pr + 1), fm);
		return surroundIf((pr > 10), "" + Symbolic.not_s + "(" + s0 + ")");
	}

	auto operator()(const And &a) const ->std::string{
		return showInfix((pr > 8), 8, Symbolic.and_s, *a.p_, *a.q_);
	}

	auto operator()(const Or &o) const ->std::string{
		return showInfix((pr > 6), 6, Symbolic.or_s, *o.p_, *o.q_);
	}

	auto operator()(const Xor &x) const ->std::string{
		return showInfix((pr > 6), 6, Symbolic.xor_s, *x.p_, *x.q_);
	}

	auto operator()(const Implies &i) const ->std::string{
		return showInfix((pr > 4), 4, Symbolic.implies_s, *i.p_, *i.q_);
	}

	auto operator()(const Iff &i) const ->std::string{
		return showInfix((pr > 4), 4, Symbolic.iff_s, *i.p_, *i.q_);
	}
};

/**
*
* ===> uuups,... need to add const correctness to visitor pattern
* solution: just use nary visitor for now, even if only one variant needs to be visited
*/
auto showInfix(bool b, int pr, const std::string &sym, const Formula &p,
	const Formula &q)->std::string {
	std::string s0 = apply_visitor(FormulaStr(pr + 1), p);
	std::string s1 = apply_visitor(FormulaStr(pr), q);
	return surroundIf(b, ("("+s0+") "+sym+" ("+s1+")"));
}

auto operator<<(std::ostream &oss, Formula &f) -> std::ostream& {
	oss << apply_visitor(FormulaStr(), f);
	return oss;
}

struct SameFormula : public static_visitor<bool> {
	auto operator()(const std::string &a0, const std::string &a1) const -> bool { return a0 == a1; }
	auto operator()(const True &t0, const True &t1) const -> bool { return true; }
	auto operator()(const False &f0, const False &f1) const -> bool { return true; }

	auto operator()(const recursive_wrapper<Not> &n0, const recursive_wrapper<Not> &n1) const -> bool {
		auto fm0(n0.get().getFm());
		auto fm1(n1.get().getFm());
		return apply_visitor(SameFormula(), fm0, fm1);
	}

	auto operator()(const recursive_wrapper<And> &a0, const recursive_wrapper<And> &a1) const -> bool {
		auto p = apply_visitor(SameFormula(), *a0.get().p_, *a1.get().p_);
		auto q = apply_visitor(SameFormula(), *a0.get().q_, *a1.get().q_);
		return p && q;
	}

	auto operator()(const recursive_wrapper<Or> &o0, const recursive_wrapper<Or> &o1) const -> bool {
		auto p = apply_visitor(SameFormula(), *o0.get().p_, *o1.get().p_);
		auto q = apply_visitor(SameFormula(), *o0.get().q_, *o1.get().q_);
		return p && q;
	}

	auto operator()(const recursive_wrapper<Xor> &x0, const recursive_wrapper<Xor> &x1) const -> bool {
		auto p = apply_visitor(SameFormula(), *x0.get().p_, *x1.get().p_);
		auto q = apply_visitor(SameFormula(), *x0.get().q_, *x1.get().q_);
		return p && q;
	}

	auto operator()(const recursive_wrapper<Implies> &i0, const recursive_wrapper<Implies> &i1) const -> bool {
		auto p = apply_visitor(SameFormula(), *i0.get().p_, *i1.get().p_);
		auto q = apply_visitor(SameFormula(), *i0.get().q_, *i1.get().q_);
		return p && q;
	}

	auto operator()(const recursive_wrapper<Iff> &i0, const recursive_wrapper<Iff> &i1) const -> bool {
		auto p = apply_visitor(SameFormula(), *i0.get().p_, *i1.get().p_);
		auto q = apply_visitor(SameFormula(), *i0.get().q_, *i1.get().q_);
		return p && q;
	}


	template <class P, class Q> auto
		operator()(const P &p, const Q &q) const -> bool { 
			if (std::is_same<P, Q>::value)
				return true;
			return false; 
		}
};

auto same(Formula &fm0, Formula &fm1) -> bool {
	return apply_visitor(SameFormula(), fm0, fm1);
}

struct CountAndClauses : public static_visitor<int> {
	auto operator()(const std::string &s) const -> int { return 0; }

	auto operator()(const True &t) const -> int { return 0; }

	auto operator()(const False &f) const -> int { return 0; }

	auto operator()(const Not &n) const -> int {
		auto fm(n.getFm());
		return apply_visitor(CountAndClauses(), fm);
	}

	auto operator()(const And &x) const -> int {
		int np = apply_visitor(CountAndClauses(), *x.p_);
		int nq = apply_visitor(CountAndClauses(), *x.q_);
		return np + nq + 1;
	}

	auto operator()(const Or &x) const -> int {
		int np = apply_visitor(CountAndClauses(), *x.p_);
		int nq = apply_visitor(CountAndClauses(), *x.q_);
		return np + nq + 0;
	}

	auto operator()(const Xor &x) const -> int {
		int np = apply_visitor(CountAndClauses(), *x.p_);
		int nq = apply_visitor(CountAndClauses(), *x.q_);
		return np + nq + 0;
	}

	auto operator()(const Implies &x) const -> int {
		int np = apply_visitor(CountAndClauses(), *x.p_);
		int nq = apply_visitor(CountAndClauses(), *x.q_);
		return np + nq + 0;
	}

	auto operator()(const Iff &x) const -> int {
		int np = apply_visitor(CountAndClauses(), *x.p_);
		int nq = apply_visitor(CountAndClauses(), *x.q_);
		return np + nq + 0;
	}
};

struct GatherAtoms : public static_visitor<std::set<std::string>> {
	auto operator()(const std::string &s) const -> std::set<std::string> { return std::set<std::string>{s}; }

	auto operator()(const True &t) const -> std::set<std::string> { return std::set<std::string>(); }

	auto operator()(const False &f) const -> std::set<std::string> { return std::set<std::string>(); }

	auto operator()(const Not &n) const -> std::set<std::string> {
		auto fm(n.getFm());
		return apply_visitor(GatherAtoms(), fm);
	}

	auto operator()(const And &x) const -> std::set<std::string> {
		auto s = std::set<std::string>();
		auto sp = apply_visitor(GatherAtoms(), *x.p_);
		auto sq = apply_visitor(GatherAtoms(), *x.q_);
		for (auto const &i : sp) s.insert(i);
		for (auto const &i : sq) s.insert(i);
		return s;
	}

	auto operator()(const Or &x) const -> std::set<std::string> {
		auto s = std::set<std::string>();
		auto sp = apply_visitor(GatherAtoms(), *x.p_);
		auto sq = apply_visitor(GatherAtoms(), *x.q_);
		for (auto const &i : sp) s.insert(i);
		for (auto const &i : sq) s.insert(i);
		return s;
	}

	auto operator()(const Xor &x) const -> std::set<std::string> {
		auto s = std::set<std::string>();
		auto sp = apply_visitor(GatherAtoms(), *x.p_);
		auto sq = apply_visitor(GatherAtoms(), *x.q_);
		for (auto const &i : sp) s.insert(i);
		for (auto const &i : sq) s.insert(i);
		return s;
	}

	auto operator()(const Implies &x) const -> std::set<std::string> {
		auto s = std::set<std::string>();
		auto sp = apply_visitor(GatherAtoms(), *x.p_);
		auto sq = apply_visitor(GatherAtoms(), *x.q_);
		for (auto const &i : sp) s.insert(i);
		for (auto const &i : sq) s.insert(i);
		return s;
	}

	auto operator()(const Iff &x) const -> std::set<std::string> {
		auto s = std::set<std::string>();
		auto sp = apply_visitor(GatherAtoms(), *x.p_);
		auto sq = apply_visitor(GatherAtoms(), *x.q_);
		for (auto const &i : sp) s.insert(i);
		for (auto const &i : sq) s.insert(i);
		return s;
	}
};

struct DeMorganHelper : public static_visitor<Formula> {
	auto operator()(const std::string &s) const -> Formula {
		return Not(s);
	}

	auto operator()(const True &t) const -> Formula {
		return False();
	}

	auto operator()(const False &t) const -> Formula {
		return True();
	}

	auto operator()(const Not &n) const -> Formula {
		auto fm(n.getFm());
		return fm;
	}

	auto operator()(const And &x) const -> Formula {
		auto fm = Or(Not(*x.p_), Not(*x.q_));
		return fm;
	}

	auto operator()(const Or &x) const -> Formula {
		auto fm = And(Not(*x.p_), Not(*x.q_));
		return fm;
	}

	template <class P> auto
		operator()(const P &p) const -> Formula { return Formula(Not(p)); }
};

struct DeMorgan : public static_visitor<Formula> {
	auto operator()(const Not &n) const -> Formula {
		auto fm(n.getFm());

		if (!fm.is<std::string>()) {
			auto helper = apply_visitor(DeMorganHelper(), fm);
			return apply_visitor(DeMorgan(), helper);
		}

		return Formula(n);
	}
	auto operator()(const And &a) const -> Formula  {
		auto p = apply_visitor(DeMorgan(), *a.p_);
		auto q = apply_visitor(DeMorgan(), *a.q_);
		return apply_visitor(VisitAnd(), p, q);
	}

	auto operator()(const Or &o) const -> Formula  {
		auto p = apply_visitor(DeMorgan(), *o.p_);
		auto q = apply_visitor(DeMorgan(), *o.q_);
		return apply_visitor(VisitOr(), p, q);
	}

	auto operator()(const Xor &x) const -> Formula  {
		auto p = apply_visitor(DeMorgan(), *x.p_);
		auto q = apply_visitor(DeMorgan(), *x.q_);
		return apply_visitor(VisitXor(), p, q);
	}

	auto operator()(const Implies &i) const -> Formula  {
		auto p = apply_visitor(DeMorgan(), *i.p_);
		auto q = apply_visitor(DeMorgan(), *i.q_);
		return apply_visitor(VisitImplies(), p, q);
	}

	auto operator()(const Iff &i) const -> Formula {
		auto p = apply_visitor(DeMorgan(), *i.p_);
		auto q = apply_visitor(DeMorgan(), *i.q_);
		return apply_visitor(VisitIff(), p, q);
	}

	template <class P> auto
		operator()(const P &p) const -> Formula { return Formula(p); }
};

struct DistributionOr : public static_visitor<Formula> {
	auto operator()(const Not &n) const -> Formula {
		auto fm(n.getFm());
		auto f = apply_visitor(DistributionOr(), fm);
		return Not(f);
	}
	auto operator()(const And &a) const -> Formula  {
		auto p = apply_visitor(DistributionOr(), *a.p_);
		auto q = apply_visitor(DistributionOr(), *a.q_);
		return apply_visitor(VisitAnd(), p, q);
	}
	auto operator()(const Or &o) const -> Formula  {
		auto p = apply_visitor(DistributionOr(), *o.p_);
		auto q = apply_visitor(DistributionOr(), *o.q_);
		if (q.is<recursive_wrapper<And>>()) {
			//do and logic
			And tmp = get<And>(q);
			Formula f = And(Or(p,*tmp.p_), Or(p, *tmp.q_));
			return apply_visitor(DistributionOr(), f);
		} 
		if (p.is<recursive_wrapper<And>>()) {
			//do and logic
			And tmp = get<And>(p);
			Formula f = And(Or(q, *tmp.p_), Or(q, *tmp.q_));
			return apply_visitor(DistributionOr(), f);
		}
		return apply_visitor(VisitOr(), p, q);
	}

	template <class P> auto
		operator()(const P &p) const -> Formula { return Formula(p); }
};

struct CNFMapping : public static_visitor<std::vector<Formula>> {
	auto operator()(const std::string &s) const -> std::vector<Formula> {
		std::string name = s + "*";
		auto v = std::vector<Formula>();
		Formula f = Implies(name, s);
		v.push_back(f);
		return v;
	}

	auto operator()(const True &t) const -> std::vector<Formula> { return std::vector<Formula>(); }

	auto operator()(const False &f) const -> std::vector<Formula> { return std::vector<Formula>(); }

	auto operator()(const Not &n) const -> std::vector<Formula> {
		auto fm(n.getFm());
		auto v = std::vector<Formula>();
		std::string s = get<std::string>(fm);
		std::string name = s + "*";
		Formula f = Implies(name, n);
		v.push_back(f);
		return v;

	}

	auto operator()(const And &x) const -> std::vector<Formula> {
		auto s = std::vector<Formula>();
		auto sp = apply_visitor(CNFMapping(), *x.p_);
		auto sq = apply_visitor(CNFMapping(), *x.q_);
		for (auto const &i : sp) s.push_back(i);
		for (auto const &i : sq) s.push_back(i);
		return s;
	}

	auto operator()(const Or &x) const -> std::vector<Formula> {
		auto s = std::vector<Formula>();
		auto sp = apply_visitor(CNFMapping(), *x.p_);
		auto sq = apply_visitor(CNFMapping(), *x.q_);
		for (auto const &i : sp) s.push_back(i);
		for (auto const &i : sq) s.push_back(i);
		return s;
	}

	auto operator()(const Xor &x) const -> std::vector<Formula> {
		auto v = std::vector<Formula>();
		v.push_back(x);
		return v;
	}

	auto operator()(const Implies &x) const -> std::vector<Formula> {
		auto v = std::vector<Formula>();
		v.push_back(x);
		return v;
	}

	auto operator()(const Iff &x) const -> std::vector<Formula> {
		auto v = std::vector<Formula>();
		v.push_back(x);
		return v;
	}

};
//pretty bad actually... but should be fine for now; will be refactored later
std::vector<Formula> cnfmapping(Formula cnf) {
	auto mapping = motr::apply_visitor(motr::CNFMapping(), cnf);
	auto new_mapping = std::vector<Formula>();
	std::set<std::string> names;
	for (auto f : mapping) {
		Implies i = get<Implies>(f);
		std::string name = get<std::string>(*i.p_);
		Formula p(*i.q_);

		int count = 0;
		std::string tmp = name;
		while (names.find(tmp) != names.end()) {
			std::stringstream sstr;
			sstr << name << "-" << (++count);
			tmp = sstr.str();
		}
		names.insert(tmp);
		name = tmp;
		new_mapping.push_back(Implies(name, p));
	}

	return new_mapping;
}

MOTR_NS_END

#endif