//#ifndef __FURRY_UTIL_VISITOR_H__
//#define __FURRY_UTIL_VISITOR_H__  
//
///*
//* ****************************************
//*
//* \Brief This file is part of motr (Model transformation test framework),
//*	     a library for solving model transformation problems in the domain of feature diagrams.
//*
//* Copyright (c) 2015 motr. All rights reserved.
//*
//* For the full copyright and license information, please view the LICENSE.txt
//* file that was distributed with this source code.
//*
//* \Author Alexander Knueppel
//*
//* ****************************************
//*/
//
//
//#include "variant.h"
//
//MOTR_NS_BEGIN
//
///**
//* \brief Implements the visitor pattern for variant types (similiar to boost)
//*
//* \ingroup util
//*/
//
//template <typename R = void>
//struct static_visitor { using result_type = R; };
//
////
//// Visitation (unary)
////
//namespace internal {
//	template <typename Visitor, typename Variant>
//	struct UnaryApplier {
//		using result_type = typename Visitor::result_type;
//
//		UnaryApplier(Visitor& visitor, Variant& variant) : visitor(visitor), variant(variant) {}
//		Visitor& visitor;
//		Variant& variant;
//
//		template <typename T>
//		auto apply() -> result_type { return visitor(*variant.template get<T>()); }
//	}; // struct UnaryApplier
//} // namespace internal
//
//template <typename V, typename T0, typename... Types>
//auto apply_unary_visitor(V&& visitor, variant<T0, Types...>& v0) -> typename V::result_type {
//	using result_type = typename V::result_type;
//
//	using Variant = variant<T0, Types...>;
//	using Applier = internal::UnaryApplier<V, Variant>;
//	using Member = result_type(Applier::*)();
//
//	static Member const members[] = {
//		(&Applier::template apply<T0>),
//		(&Applier::template apply<Types>)...
//	};
//
//	Member const m = members[v0.which()];
//	Applier a{ visitor, v0 };
//	return (a.*m)();
//} // apply_unary_visitor
//
////
//// Visitation (n-ary)
////
//namespace internal {
//	template <typename Visitor, typename T>
//	struct NaryVisitor {
//		using result_type = typename Visitor::result_type;
//
//		NaryVisitor(Visitor& visitor, T& t) : visitor(visitor), ref(t) {}
//		Visitor& visitor;
//		T& ref;
//
//		template <typename... Args>
//		auto operator()(Args&&... args) -> result_type {
//			return visitor(ref, std::forward<Args>(args)...);
//		} // apply
//	}; // struct NaryVisitor
//
//	template <typename Visitor, typename T0, typename... Types, typename... Vs>
//	auto apply_nary_visitor_impl(Visitor& visitor, variant<T0, Types...>&& v0, Vs&&... vs) -> typename Visitor::result_type;
//
//	template <typename Visitor, typename Variant>
//	struct NaryApplier {
//		using result_type = typename Visitor::result_type;
//
//		NaryApplier(Visitor& visitor, Variant& variant) : visitor(visitor), variant(variant) {}
//		Visitor& visitor;
//		Variant& variant;
//
//		template <typename T>
//		auto apply() -> result_type { return visitor(*variant.template get<T>()); }
//
//		template <typename T, typename V0, typename... Vs>
//		auto apply(V0&& v0, Vs&&... vs) -> result_type {
//			NaryVisitor<Visitor, T> nary{ visitor, *variant.template get<T>() };
//			return apply_nary_visitor_impl(nary, std::forward<V0>(v0), std::forward<Vs>(vs)...);
//		}
//	}; // struct NaryApplier
//
//	template <typename Visitor, typename T0, typename... Types, typename... Vs>
//	auto apply_nary_visitor_impl(Visitor& visitor, variant<T0, Types...>& v0, Vs&&... vs) -> typename Visitor::result_type {
//		using result_type = typename Visitor::result_type;
//
//		using Variant = variant<T0, Types...>;
//		using Applier = internal::NaryApplier<Visitor, Variant>;
//		using Member = result_type(Applier::*)(Vs&&...);
//
//		static Member const members[] = {
//			(&Applier::template apply<T0, Vs...>),
//			(&Applier::template apply<Types, Vs...>)...
//		};
//
//		Member const m = members[v0.which()];
//		Applier a{ visitor, v0 };
//		return (a.*m)(std::forward<Vs>(vs)...);
//	} // apply_nary_visitor_impl
//
//} // namespace internal
//
//template <typename Visitor, typename... Variants>
//auto apply_nary_visitor(Visitor&& visitor, Variants&&... vs) -> typename Visitor::result_type {
//	return internal::apply_nary_visitor_impl(visitor, std::forward<Variants>(vs)...);
//} // apply_nary_visitor
//
//
//MOTR_NS_END
//
//#endif