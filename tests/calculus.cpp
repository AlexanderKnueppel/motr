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

#include <motr.h>
#include <logic/calculus.h>
#include <gmock/gmock.h>

using ::testing::Eq;
using namespace motr;

#define l(x) std::string(#x)

TEST(Formula, TestGatherAtoms) {
	Formula f = (l(A) >> l(C)) & (l(B) | l(C) | (l(D) & l(E)));
	std::set<std::string> atoms = apply_visitor(GatherAtoms(), f);
	std::set<std::string> control = {l(A), l(B), l(C), l(D), l(E)};
	ASSERT_THAT(atoms, Eq(control));
}

TEST(Formula, TestSameFormula) {
	Formula f = (l(A) >> l(C)) & (l(B) | l(C) | (l(D) & l(E)));
	Formula f2 = (l(A) >> l(C)) & (l(B) | l(C) | (l(D) & l(F)));
	ASSERT_THAT(apply_visitor(SameFormula(), Formula(l(A)&l(B)), Formula(l(A)&l(B))), Eq(true));
	ASSERT_THAT(apply_visitor(SameFormula(), f, f), Eq(true));
	ASSERT_THAT(apply_visitor(SameFormula(), f, f2), Eq(false));
}

TEST(Formula, TestDeMorgan) {
	Formula f = ~(l(A) | l(B) | l(C));
	Formula demorgan = apply_visitor(DeMorgan(), f);
	ASSERT_THAT(apply_visitor(SameFormula(), demorgan, Formula(~l(A) & ~l(B) & ~l(C)) ), Eq(true));
	
	Formula f2 = ~(l(A) & l(B) & l(C));
	Formula demorgan2 = apply_visitor(DeMorgan(), f2);
	ASSERT_THAT(apply_visitor(SameFormula(), demorgan2, Formula(~l(A) | ~l(B) | ~l(C))), Eq(true));

	Formula f3 = ~(l(A) & l(B) & ~l(C));
	Formula demorgan3 = apply_visitor(DeMorgan(), f3);
	ASSERT_THAT(apply_visitor(SameFormula(), demorgan3, Formula(~l(A) | ~l(B) | l(C))), Eq(true));
}

TEST(Formula, TestDistributionOr) {
	Formula f = l(A) | (l(B) & l(C));
	Formula distr = apply_visitor(DistributionOr(), f);
	ASSERT_THAT(apply_visitor(SameFormula(), distr, Formula( (l(A) | l(B)) & (l(A) | l(C)))), Eq(true));
}

TEST(Formula, VioletOutput) {
	//////////////////////////// violet
	int number_correct_constraints = 19;
	int number_constraints = 27;
	std::string File = "File";
	std::string NewFile = "NewFile";
	std::string Edit = "Edit";
	std::string View = "View";
	std::string Window = "Window";
	std::string Help = "Help";
	std::string Additional = "Additional";
	std::string MenuRessources = "MenuRessources";
	std::string GraphUtility = "GraphUtility";
	std::string ExtensionFilter = "ExtensionFilter";
	std::string FileUtility = "FileUtility";
	std::string InternalFrame = "InternalFrame";
	std::string Read = "Read";
	std::string FileMenu = "FileMenu";
	std::string ClassDiagram = "ClassDiagram";
	std::string SequenceDiagram = "SequenceDiagram";
	std::string StateDiagram = "StateDiagram";
	std::string ObjectDiagram = "ObjectDiagram";
	std::string UseCaseDiagram = "UseCaseDiagram";
	std::string DiagramSupport = "DiagramSupport";
	std::string ImageFilter = "ImageFilter";
	std::string ExportImage = "ExportImage";
	std::string ClassDependencyEdge = "ClassDependencyEdge";
	std::string ClassInheritanceEdge = "ClassInheritanceEdge";
	std::string ClassAggregationEdge = "ClassAggregationEdge";
	std::string ClassAssociationEdge = "ClassAssociationEdge";
	std::string ClassCompositionEdge = "ClasCompositionsEdge";
	std::string ClassInterfaceEdge = "ClassInterfaceEdge";
	std::string ClassNode = "ClassNode";
	std::string SequenceCallEdge = "SequenceCallEdge";
	std::string SequenceReturnEdge = "SequenceReturnEdge";
	std::string SequenceCallNode = "SequenceCallNode";
	std::string ObjectReferenceEdge = "ObjectReferenceEdge";
	std::string ObjectAssociationEdge = "ObjectAssociationEdge";
	std::string ObjectNode = "SequenceCallNode";
	std::string UseCaseAssociationEdge = "UseCaseAssociationEdge";
	std::string ExtendRelationshipEdge = "ExtendRelationshipEdge";
	std::string IncludeReleationshipEdge = "IncludeRelationshipEdge";
	std::string UseCaseGeneralizationEdge = "UseCasegeneralizationNode";
	std::string Actor = "Actor";
	std::vector<Formula> violet_constraints;
	violet_constraints.push_back(Or(Not(Or(File, Or(NewFile, Or(Edit, Or(View, Or(Window, Or(Help, Additional))))))), And(MenuRessources, And(GraphUtility, And(ExtensionFilter, And(FileUtility, And(InternalFrame, Read)))))));
	violet_constraints.push_back(Or(Not(Or(File, NewFile)), FileMenu));
	violet_constraints.push_back(Or(Not(Or(ClassDiagram, Or(SequenceDiagram, Or(StateDiagram, Or(ObjectDiagram, UseCaseDiagram))))),And(DiagramSupport, NewFile)));
	violet_constraints.push_back(Or(Not(ImageFilter), And(ExtensionFilter, ExportImage)));
	violet_constraints.push_back(Or(Not(Or(ClassDependencyEdge, Or(ClassInheritanceEdge, Or(ClassAggregationEdge, Or(ClassAssociationEdge, Or(ClassCompositionEdge, ClassInterfaceEdge)))))), ClassNode));
	violet_constraints.push_back(Or(Not(Or(SequenceCallEdge, SequenceReturnEdge)), SequenceCallNode));
	violet_constraints.push_back(Or(Not(Or(ObjectReferenceEdge, ObjectAssociationEdge)), ObjectNode));
	violet_constraints.push_back(Or(Not(Or(UseCaseAssociationEdge, Or(ExtendRelationshipEdge, Or(IncludeReleationshipEdge, UseCaseGeneralizationEdge)))), Actor));

	//combine constraints
	Formula combined;
	int n_constraints = number_correct_constraints;
	for (auto c : violet_constraints) {
		combined = And(combined, c);
	}

	//use demorgan und distribution to create cnf
	Formula violet_cnf = motr::apply_visitor(motr::DistributionOr(), motr::apply_visitor(motr::DeMorgan(), combined));
	std::cout << violet_cnf << std::endl;
	std::cout << "Number of clauses: " << apply_visitor(CountAndClauses(), violet_cnf) << std::endl;
	std::cout << "Number of constraints before: " << number_constraints << std::endl;
	std::cout << "Number of constraints after: " << apply_visitor(CountAndClauses(), violet_cnf) + number_correct_constraints << std::endl;
	
	std::vector<motr::Formula> violet_mapping = cnfmapping(violet_cnf);
	for (auto f : violet_mapping) {
		std::cout << f << std::endl;
	}
	
	std::cout << "Number constraints before: " << number_constraints << std::endl;
	std::cout << "Number constraints after: " << (number_correct_constraints) + violet_mapping.size() << std::endl;

	/**
	* Number of clauses: 70
	* ---> without mapping (not needed because every clause consists of only two literals [requires/excludes])
	* Constraints before: 27
	* Constraints after: 89
	* ---> with mapping
	* Constraints before: 27
	* Constraints after: 160
	*/
}
//
