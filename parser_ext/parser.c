/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdlib.h>

#include "reader.h"
#include "scanner.h"
#include "parser.h"
#include "error.h"

Token *currentToken;
Token *lookAhead;

void scan(void) {
  Token* tmp = currentToken;
  currentToken = lookAhead;
  lookAhead = getValidToken();
  free(tmp);
}

void eat(TokenType tokenType) {
  if (lookAhead->tokenType == tokenType) {
    printToken(lookAhead);
    scan();
  } else missingToken(tokenType, lookAhead->lineNo, lookAhead->colNo);
}

void compileProgram(void) {
  assert("Parsing a Program ....");
  eat(KW_PROGRAM);
  eat(TK_IDENT);
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_PERIOD);
  assert("Program parsed!");
}

void compileBlock(void) {
  assert("Parsing a Block ....");
  if (lookAhead->tokenType == KW_CONST) {
    eat(KW_CONST);
    compileConstDecl();
    compileConstDecls();
    compileBlock2();
  } 
  else compileBlock2();
  assert("Block parsed!");
}

void compileBlock2(void) {
  if (lookAhead->tokenType == KW_TYPE) {
    eat(KW_TYPE);
    compileTypeDecl();
    compileTypeDecls();
    compileBlock3();
  } 
  else compileBlock3();
}

void compileBlock3(void) {
  if (lookAhead->tokenType == KW_VAR) {
    eat(KW_VAR);
    compileVarDecl();
    compileVarDecls();
    compileBlock4();
  } 
  else compileBlock4();
}

void compileBlock4(void) {
  assert("Parsing subtoutines ....");
  compileFuncDels();
  assert("Subtoutines parsed ....");
  compileBlock5();
}


void compileBlock5(void) {
  assert("Parsing subtoutines ....");
  compileProcDecls();
  assert("Subtoutines parsed ....");
  compileBlock6();
}

void compileBlock6(void) {
  eat(KW_BEGIN);
  compileStatements();
  eat(KW_END);
}

void compileConstDecls(void) {
  while (lookAhead->tokenType==TK_IDENT) 
  compileConstDecl(); 
}

void compileConstDecl(void) {
  eat(TK_IDENT);
  eat(SB_EQ);
  compileConstant();
  eat(SB_SEMICOLON);
}

void compileTypeDecls(void) {
  // TODO
  while(lookAhead->tokenType==TK_IDENT){
    compileTypeDecl();
  }
}

void compileTypeDecl(void) {
  // TODO
  eat(TK_IDENT);
  eat(SB_EQ);
  compileType();
  eat(SB_SEMICOLON);
}

void compileVarDecls(void) {
  // TODO
  while(lookAhead->tokenType==TK_IDENT){
    compileVarDecl();
  }
}

void compileVarDecl(void) {
  // TODO
  eat(TK_IDENT);
  eat(SB_COLON);
  compileType();
  eat(SB_SEMICOLON);
}

void compileSubDecls(void) {
  // TODO
  switch(lookAhead->tokenType){
    case KW_FUNCTION:
      compileFuncDecl();
      compileSubDecls();
      break;
    case KW_PROCEDURE:
      compileProcDecl();
      compileSubDecls();
      break;
  }
}


void compileFuncDels(void) {
  assert("Parsing a function ....");
  // TODO
  while(lookAhead->tokenType==KW_FUNCTION){
    compileFuncDecl();
  assert("Function parsed ....");}
}

void compileFuncDecl(void) {
  assert("Parsing a function ....");
  // TODO
  eat(KW_FUNCTION);
  eat(TK_IDENT);
  compileParams();
  eat(SB_COLON);
  compileBasicType();
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_SEMICOLON);
  assert("Function parsed ....");
}

void compileProcDecls(void){
	while(lookAhead->tokenType==KW_PROCEDURE){
    compileProcDecl();}
}

void compileProcDecl(void) {
  assert("Parsing a procedure ....");
  // TODO
  eat(KW_PROCEDURE);
  eat(TK_IDENT);
  compileParams();
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_SEMICOLON);
  assert("Procedure parsed ....");
}

void compileUnsignedConstant(void) {
  // TODO
  switch(lookAhead->tokenType){
    case TK_NUMBER:
    case TK_IDENT:
    case TK_CHAR:
      eat(lookAhead->tokenType);
      break;
    default:
      error(ERR_INVALIDCONSTANT,lookAhead->lineNo,lookAhead->colNo);
  }
}

void compileConstant(void) {
  // TODO
  switch(lookAhead->tokenType){
    case SB_PLUS:
      eat(SB_PLUS);
      compileConstant2();
      break;
    case SB_MINUS:
      eat(SB_MINUS);
      compileConstant2();
      break;
    case TK_CHAR:
      eat(TK_CHAR);
      break;
    default:
      compileConstant2();
  }
}

void compileConstant2(void) {
  // TODO
  switch(lookAhead->tokenType){
    case TK_IDENT:
    case TK_NUMBER:
      eat(lookAhead->tokenType);
      break;
    default:
      error(ERR_INVALIDCONSTANT,lookAhead->lineNo,lookAhead->colNo);
  }
}

void compileType(void) {
  // TODO
  switch(lookAhead->tokenType){
    case KW_INTEGER:
    case KW_CHAR:
    case TK_IDENT:
      eat(lookAhead->tokenType);
      break;
    case KW_ARRAY:
      eat(KW_ARRAY); eat(SB_LSEL); eat(TK_NUMBER); eat(SB_RSEL); eat(KW_OF);
      compileType();
      break;
    default:
      error(ERR_INVALIDTYPE,lookAhead->lineNo,lookAhead->colNo);
  }
}

void compileBasicType(void) {
  // TODO
  switch(lookAhead->tokenType){
    case KW_INTEGER:
      eat(KW_INTEGER);
      break;
    case KW_CHAR:
      eat(KW_CHAR);
      break;
    default:
      error(ERR_INVALIDBASICTYPE,lookAhead->lineNo,lookAhead->colNo);
      exit(0);
  }
}

void compileParams(void) {
  // TODO
  if(lookAhead->tokenType==SB_LPAR){
    eat(SB_LPAR);
    compileParam();
    compileParams2();
    eat(SB_RPAR);
  }
}

void compileParams2(void) {
  // TODO
  switch(lookAhead->tokenType){
    case SB_SEMICOLON:
      eat(SB_SEMICOLON);
      compileParam();
      compileParams2();
      break;
    case SB_RPAR:
      break;
    default:
      error(ERR_INVALIDPARAM,lookAhead->lineNo,lookAhead->colNo);
  }
}

void compileParam(void) {
  // TODO
  switch(lookAhead->tokenType){
    case TK_IDENT:
      eat(TK_IDENT);
      eat(SB_COLON);
      compileBasicType();
      break;
    case KW_VAR:
      eat(KW_VAR);
      eat(TK_IDENT);
      eat(SB_COLON);
      compileBasicType();
      break;
    default:
      error(ERR_INVALIDPARAM,lookAhead->lineNo,lookAhead->colNo);
  }
}

void compileStatements(void) {
  // TODO
  compileStatement();
  compileStatements2();
}

void compileStatements2(void) {
  // TODO
  switch(lookAhead->tokenType){
    case SB_SEMICOLON:
      eat(SB_SEMICOLON);
      compileStatement();
      compileStatements2();
      break;
    case KW_END:
    case KW_ELSE:
      break;
    default:
      error(ERR_INVALIDSTATEMENT,lookAhead->lineNo,lookAhead->colNo);
  }
}

void compileStatement(void) {
  switch (lookAhead->tokenType) {
  case TK_IDENT:
    compileAssignSt();
    break;
  case KW_CALL:
    compileCallSt();
    break;
  case KW_BEGIN:
    compileGroupSt();
    break;
  case KW_IF:
    compileIfSt();
    break;
  case KW_WHILE:
    compileWhileSt();
    break;
  case KW_FOR:
    compileForSt();
    break;
    // EmptySt needs to check FOLLOW tokens
  case SB_SEMICOLON:
  case KW_END:
  case KW_ELSE:
    break;
    // Error occurs
  default:
    error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

void compileAssignSt(void) {
  assert("Parsing an assign statement ....");
  // TODO
  eat(TK_IDENT);
  compileIndexes();
  eat(SB_ASSIGN);
  compileExpression();
  assert("Assign statement parsed ....");
}

void compileCallSt(void) {
  assert("Parsing a call statement ....");
  // TODO
  eat(KW_CALL);
  eat(TK_IDENT);
  compileArguments();
  assert("Call statement parsed ....");
}

void compileGroupSt(void) {
  assert("Parsing a group statement ....");
  // TODO
  eat(KW_BEGIN);
  compileStatements();
  eat(KW_END);
  assert("Group statement parsed ....");
}

void compileIfSt(void) {
  assert("Parsing an if statement ....");
  eat(KW_IF);
  compileCondition();
  eat(KW_THEN);
  compileStatement();
  if (lookAhead->tokenType == KW_ELSE) 
    compileElseSt();
  assert("If statement parsed ....");
}

void compileElseSt(void) {
  eat(KW_ELSE);
  compileStatement();
}

void compileWhileSt(void) {
  assert("Parsing a while statement ....");
  // TODO
  eat(KW_WHILE);
  compileCondition();
  eat(KW_DO);
  compileStatement();
  assert("While statement parsed ....");
}

void compileForSt(void) {
  assert("Parsing a for statement ....");
  // TODO
  eat(KW_FOR);
  eat(TK_IDENT);
  eat(SB_ASSIGN);
  compileExpression();
  eat(KW_TO);
  compileExpression();
  eat(KW_DO);
  compileStatement();
  assert("For statement parsed ....");
}

void compileArguments(void) {
  // TODO
  if(lookAhead->tokenType==SB_LPAR){
    eat(SB_LPAR);
    switch(lookAhead->tokenType){
      case SB_PLUS:
      case SB_MINUS:
      case TK_NUMBER:
      case TK_CHAR:
      case TK_IDENT:
      case SB_LPAR:
        compileExpression();
        compileArguments2();
        break;
      default:
        error(ERR_INVALIDARGUMENTS,lookAhead->lineNo,lookAhead->colNo);
    }
    eat(SB_RPAR);
  }
}

void compileArguments2(void) {
  // TODO
  if(lookAhead->tokenType==SB_COMMA){
    eat(SB_COMMA);
    switch(lookAhead->tokenType){
      case SB_PLUS:
      case SB_MINUS:
      case TK_NUMBER:
      case TK_CHAR:
      case TK_IDENT:
      case SB_LPAR:
        compileExpression();
        compileArguments2();
        break;
      default:
        error(ERR_INVALIDARGUMENTS,lookAhead->lineNo,lookAhead->colNo);
    }
  }
}

void compileCondition(void) {
  // TODO
  compileExpression();
  compileCondition2();
}

void compileCondition2(void) {
  // TODO
  switch(lookAhead->tokenType){
    case SB_EQ:
    case SB_NEQ:
    case SB_LE:
    case SB_GE:
    case SB_LT:
    case SB_GT:
      eat(lookAhead->tokenType);
      compileExpression();
      break;
    default:
      error(ERR_INVALIDCOMPARATOR,lookAhead->lineNo,lookAhead->colNo);
      exit(0);
  }
}

void compileExpression(void) {
  assert("Parsing an expression");
  // TODO
  switch(lookAhead->tokenType){
    case SB_PLUS:
    case SB_MINUS:
      eat(lookAhead->tokenType);
      compileExpression2();
      break;
    case KW_TO:
    case SB_SEMICOLON:
    case KW_DO:
    case KW_THEN:
    case SB_COMMA:
    case SB_RPAR:
    case SB_RSEL:
    case SB_EQ:
    case SB_NEQ:
    case SB_LT:
    case SB_LE:
    case SB_GE:
    case SB_GT:
      error(ERR_INVALIDEXPRESSION,lookAhead->lineNo,lookAhead->colNo);
      break;
    default:
      compileExpression2();
  }
  assert("Expression parsed");
}

void compileExpression2(void) {
  compileTerm();
  compileExpression3();
}


void compileExpression3(void) {
  // TODO
  switch(lookAhead->tokenType){
    case SB_PLUS:
    case SB_MINUS:
      eat(lookAhead->tokenType);
      compileTerm();
      compileExpression3();
      break;
  }
}

void compileTerm(void) {
  compileFactor();
  compileTerm2();
}

void compileTerm2(void) {
  // TODO
  switch(lookAhead->tokenType){
    case SB_TIMES:
    case SB_SLASH:
      eat(lookAhead->tokenType);
      compileFactor();
      compileTerm2();
      break;
    case SB_PLUS:
    case SB_MINUS:
    case KW_TO:
    case SB_SEMICOLON:
    case KW_DO:
    case KW_THEN:
    case SB_COMMA:
    case SB_RPAR:
    case SB_RSEL:
    case SB_EQ:
    case SB_NEQ:
    case SB_LT:
    case SB_LE:
    case SB_GE:
    case SB_GT:
    case KW_ELSE:
    case KW_END:
      break;
    default:
      error(ERR_INVALIDTERM,lookAhead->lineNo,lookAhead->colNo);
  }
}

void compileFactor(void) {
  // TODO
  switch(lookAhead->tokenType){
    case TK_CHAR:
    case TK_NUMBER:
      eat(lookAhead->tokenType);
      break;
    case TK_IDENT:
      eat(TK_IDENT);
      switch(lookAhead->tokenType){
        case SB_LPAR:
          compileArguments();
          break;
        case SB_LSEL:
          compileIndexes();
          break;
      }
      break;
    case SB_LPAR:
      eat(SB_LPAR);
      compileExpression();
      eat(SB_RPAR);
      break;
    default:
      error(ERR_INVALIDFACTOR,lookAhead->lineNo,lookAhead->colNo);
  }
}

void compileIndexes(void) {
  // TODO
  if(lookAhead->tokenType==SB_LSEL){
    eat(SB_LSEL);
    compileExpression();
    eat(SB_RSEL);
    compileIndexes();
  }
}

int compile(char *fileName) {
  if (openInputStream(fileName) == IO_ERROR)
    return IO_ERROR;

  currentToken = NULL;
  lookAhead = getValidToken();

  compileProgram();

  free(currentToken);
  free(lookAhead);
  closeInputStream();
  return IO_SUCCESS;

}
