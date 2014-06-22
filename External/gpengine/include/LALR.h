/***************************************************************************
                          LALR.h  -  LALR automata
                          ------------------------
    begin                : Thu Jun 13 2002
    copyright            : (C) 2002 by Manuel Astudillo

 ***************************************************************************/

 /***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/
 #ifndef _GP_LALR_H
 #define _GP_LALR_H

 #include <vector>
 #include <string>
 #include <queue>

 #include "SymbolStack.h"
 #include "Token.h"
 #include "LALRStateTable.h"
 #include "RuleTable.h"
 #include "SymbolTable.h"
 #include "ErrorTable.h"
 #include "Symbol.h"
 #include "Terminal.h"
 #include "NonTerminal.h" 

 #if defined (WIN32) && defined (_USRDLL)
    class __declspec(dllexport) LALR;
 #endif

 enum Actions {ACTION_SHIFT = 1, ACTION_REDUCE = 2, ACTION_GOTO = 3, ACTION_ACCEPT = 4};
 enum Reductions {REDUCTION_OK, REDUCTION_ERROR, REDUCTION_TEXT_ACCEPTED, 
                  REDUCTION_TOKEN_SHIFT, REDUCTION_COMPLETED, REDUCTION_SIMPLIFIED};

 #define DEBUG false

using namespace std;

 class LALR {
 public:
   LALR (const LALRStateTable *stateTable, const SymbolTable *symbolTable,
   const RuleTable *ruleTable, integer startState);
   ~LALR ();

   /*!
     setup the parsing engine.
   */
   void init (const vector <Token*> &tokens);

   /*
     parses just until ONE reduction is performed
   */
   Symbol *nextReduction (bool trimReductions, bool reportOnlyOneError);

   /*!
     Gets the result constant for the last reduction attempt.
   */
   int getResult();

   /*!
     builds a parse tree with Reductions as nodes.
     This tree is supposed to be the input to some abstract tree creator.
     Every Reduction node has a list of tokens, and every token a pointer to
     its correspondent reduction node.
   */
   Symbol *parse (const vector <Token*> &tokens, bool trimReductions, bool reportOnlyOneError);

   void printRule          (integer rule);
   void printReductionTree (Symbol *reduction, int deep);

   ErrorTable *getErrors ();

 private:
   Action *getNextAction (integer symbolIndex, integer index);
   vector<wstring> getPossibleTokens (integer index);

   Symbol *parseToken (Action *actObj, SymbolStack &theStack, int tokenIndex, 
                       integer &currentState);
   Terminal *createTerminal (Token *tok);
   NonTerminal *createNonTerminal (int index, int target);

   // Member variables
   const LALRStateTable *stateTable;
   const RuleTable *ruleTable;
   const SymbolTable *symbolTable;

   integer startState;
   integer currentState;

   bool m_trimReductions;

   integer currentLine, currentCol;

   integer tokenIndex;
   vector<Token*> tokens;

   SymbolStack symbolStack;

   Symbol       *prevReduction; // last correctly reduced non terminal
   Terminal      lastTerminal;  // last parsed symbol

   int reductionResult;

   ErrorTable *errorTab;

   bool trim;

   // Global Error Recovery Members
   void updateBurkeFisher (Symbol *symbol);
   queue <Symbol*> errorQueue;
   SymbolStack errorStack;
   int nbrBackUpTokens;


 };

 #endif

