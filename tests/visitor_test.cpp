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
//#include <motr.h>
//#include <util/variant.h>
//#include <gmock/gmock.h>
//
//using ::testing::Eq;
//
//template<typename T>
//struct _Mult {
//	T a_;
//	T b_;
//	_Mult(T a, T b) : a_(a), b_(b) {}
//};
//typedef _Mult<int> Mult;
//typedef _Mult<double> Mult_d;
//
//template<typename T>
//struct _Add {
//	T a_;
//	T b_;
//	_Add(T a, T b) : a_(a), b_(b) {}
//};
//typedef _Add<int> Add;
//typedef _Add<double> Add_d;
//
//template<typename T>
//struct _Div  {
//	T a_;
//	T b_;
//	_Div(T a, T b) : a_(a), b_(b) {}
//};
//typedef _Div<int> Div;
//typedef _Div<double> Div_d;
//
//template<typename T>
//struct _Minus {
//	T a_;
//	T b_;
//	_Minus(T a, T b) : a_(a), b_(b) {}
//};
//typedef _Minus<int> Minus;
//typedef _Minus<double> Minus_d;
//
//typedef ::motr::Variant<Mult, Add> variant;
//
//struct CalculateVisitor {
//	typedef void result_type;
//
//	int operator()(Mult& visitable) {
//		return visitable.a_ * visitable.b_;
//	}
//	int operator()(Add& visitable) {
//		return visitable.a_ + visitable.b_;
//	}
//};
//
//struct CalculateProductVisitor {
//	typedef void result_type;
//
//	int operator()(Mult& m, Add& a) {
//		return (m.a_ * m.b_) * (a.a_ + a.b_);
//	}
//	int operator()(Mult& m, Mult& a) {
//		return (m.a_ * m.b_) * (a.a_ * a.b_);
//	}
//	int operator()(Add& m, Add& a) {
//		return (m.a_ + m.b_) * (a.a_ + a.b_);
//	}
//	int operator()(Add& a, Mult& m) {
//		return (m.a_ * m.b_) * (a.a_ + a.b_);
//	}
//};
//
//TEST(CalculateVisitor, CanEitherAddOrMultiply) {
//	variant v1(Mult(4, 3));
//	EXPECT_EQ(::motr::apply_visitor(CalculateVisitor, v1), 12);
//	v1 = Add(7, 6);
//	EXPECT_EQ(::motr::apply_visitor(CalculateVisitor, v1), 13);
//}
//
//TEST(CalculateVisitor, CanSolveMultiplicationWithRespectToOrderOfOperation) {
//	variant v1(Mult(4, 3));
//	variant v2(Add(7, 6));
//
//	EXPECT_EQ(::motr::apply_visitor(CalculateProductVisitor{}, v1, v1), 144); //4*3*4*3 = 144
//	EXPECT_EQ(::motr::apply_visitor(CalculateProductVisitor{}, v1, v2), 156); //4*3*(7+6) = 156
//	EXPECT_EQ(::motr::apply_visitor(CalculateProductVisitor{}, v2, v1), 156); //(7+6)*4*3 = 156 
//	EXPECT_EQ(::motr::apply_visitor(CalculateProductVisitor{}, v2, v2), 169); //(7+6)*(7+6) = 169
//}
