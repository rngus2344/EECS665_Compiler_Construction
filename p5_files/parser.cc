// A Bison parser, made by GNU Bison 3.4.1.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2019 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// Undocumented macros, especially those whose name start with YY_,
// are private implementation details.  Do not rely on them.





#include "grammar.hh"


// Unqualified %code blocks.
#line 34 "cminusminus.yy"

   // C std code for utility functions
   #include <iostream>
   #include <cstdlib>
   #include <fstream>

   // Our code for interoperation between scanner/parser
   #include "scanner.hpp"
   #include "ast.hpp"
   #include "tokens.hpp"

  //Request tokens from our scanner member, not 
  // from a global function
  #undef yylex
  #define yylex scanner.yylex

#line 62 "parser.cc"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif



// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 5 "cminusminus.yy"
namespace cminusminus {
#line 138 "parser.cc"


  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  Parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              else
                goto append;

            append:
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
  Parser::Parser (cminusminus::Scanner &scanner_yyarg, cminusminus::ProgramNode** root_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      scanner (scanner_yyarg),
      root (root_yyarg)
  {}

  Parser::~Parser ()
  {}

  Parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | Symbol types.  |
  `---------------*/

  // basic_symbol.
#if 201103L <= YY_CPLUSPLUS
  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (basic_symbol&& that)
    : Base (std::move (that))
    , value (std::move (that.value))
  {}
#endif

  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value (that.value)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t)
    : Base (t)
    , value ()
  {}

  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, YY_RVREF (semantic_type) v)
    : Base (t)
    , value (YY_MOVE (v))
  {}

  template <typename Base>
  bool
  Parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  void
  Parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    value = YY_MOVE (s.value);
  }

  // by_type.
  Parser::by_type::by_type ()
    : type (empty_symbol)
  {}

#if 201103L <= YY_CPLUSPLUS
  Parser::by_type::by_type (by_type&& that)
    : type (that.type)
  {
    that.clear ();
  }
#endif

  Parser::by_type::by_type (const by_type& that)
    : type (that.type)
  {}

  Parser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  void
  Parser::by_type::clear ()
  {
    type = empty_symbol;
  }

  void
  Parser::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  int
  Parser::by_type::type_get () const YY_NOEXCEPT
  {
    return type;
  }


  // by_state.
  Parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  Parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  Parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  Parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  Parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  Parser::symbol_number_type
  Parser::by_state::type_get () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  Parser::stack_symbol_type::stack_symbol_type ()
  {}

  Parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.value))
  {
#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  Parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.value))
  {
    // that is emptied.
    that.type = empty_symbol;
  }

#if YY_CPLUSPLUS < 201103L
  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  Parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    YYUSE (yysym.type_get ());
  }

#if YYDEBUG
  template <typename Base>
  void
  Parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
#if defined __GNUC__ && ! defined __clang__ && ! defined __ICC && __GNUC__ * 100 + __GNUC_MINOR__ <= 408
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
#endif
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " (";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  void
  Parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  Parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  Parser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Parser::debug_level_type
  Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  Parser::state_type
  Parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  bool
  Parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  Parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Parser::operator() ()
  {
    return parse ();
  }

  int
  Parser::parse ()
  {
    // State.
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << '\n';

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.type = yytranslate_ (yylex (&yyla.value));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* If YYLEN is nonzero, implement the default value of the
         action: '$$ = $1'.  Otherwise, use the top of the stack.

         Otherwise, the following line sets YYLHS.VALUE to garbage.
         This behavior is undocumented and Bison users should not rely
         upon it.  */
      if (yylen)
        yylhs.value = yystack_[yylen - 1].value;
      else
        yylhs.value = yystack_[0].value;


      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2:
#line 158 "cminusminus.yy"
    {
		  (yylhs.value.transProgram) = new ProgramNode((yystack_[0].value.transDeclList));
		  *root = (yylhs.value.transProgram);
		  }
#line 611 "parser.cc"
    break;

  case 3:
#line 164 "cminusminus.yy"
    { 
	  	  (yylhs.value.transDeclList) = (yystack_[1].value.transDeclList); 
	  	  DeclNode * declNode = (yystack_[0].value.transDecl);
		  (yylhs.value.transDeclList)->push_back(declNode);
	  	  }
#line 621 "parser.cc"
    break;

  case 4:
#line 170 "cminusminus.yy"
    {
		  (yylhs.value.transDeclList) = new std::list<DeclNode * >();
		  }
#line 629 "parser.cc"
    break;

  case 5:
#line 175 "cminusminus.yy"
    { (yylhs.value.transDecl) = (yystack_[0].value.transVarDecl); }
#line 635 "parser.cc"
    break;

  case 6:
#line 177 "cminusminus.yy"
    { (yylhs.value.transDecl) = (yystack_[0].value.transFn); }
#line 641 "parser.cc"
    break;

  case 7:
#line 180 "cminusminus.yy"
    {
		  Position * p = new Position((yystack_[2].value.transType)->pos(), (yystack_[1].value.transID)->pos());
		  (yylhs.value.transVarDecl) = new VarDeclNode(p, (yystack_[2].value.transType), (yystack_[1].value.transID));
		  }
#line 650 "parser.cc"
    break;

  case 8:
#line 186 "cminusminus.yy"
    {
		  (yylhs.value.transType) = (yystack_[0].value.transType);
		  }
#line 658 "parser.cc"
    break;

  case 9:
#line 190 "cminusminus.yy"
    {
		  Position * p = new Position((yystack_[1].value.transToken)->pos(), (yystack_[0].value.transType)->pos());
		  (yylhs.value.transType) = new PtrTypeNode(p, (yystack_[0].value.transType));
		  }
#line 667 "parser.cc"
    break;

  case 10:
#line 195 "cminusminus.yy"
    { 
		  (yylhs.value.transType) = new IntTypeNode((yystack_[0].value.transToken)->pos());
		  }
#line 675 "parser.cc"
    break;

  case 11:
#line 199 "cminusminus.yy"
    {
		  (yylhs.value.transType) = new BoolTypeNode((yystack_[0].value.transToken)->pos());
		  }
#line 683 "parser.cc"
    break;

  case 12:
#line 203 "cminusminus.yy"
    {
		  (yylhs.value.transType) = new StringTypeNode((yystack_[0].value.transToken)->pos());
		  }
#line 691 "parser.cc"
    break;

  case 13:
#line 207 "cminusminus.yy"
    {
		  (yylhs.value.transType) = new ShortTypeNode((yystack_[0].value.transToken)->pos());
		  }
#line 699 "parser.cc"
    break;

  case 14:
#line 211 "cminusminus.yy"
    {
		  (yylhs.value.transType) = new VoidTypeNode((yystack_[0].value.transToken)->pos());
		  }
#line 707 "parser.cc"
    break;

  case 15:
#line 216 "cminusminus.yy"
    {
		  Position * pos = new Position((yystack_[6].value.transType)->pos(), (yystack_[0].value.transToken)->pos());
		  std::list<FormalDeclNode *> * f = new std::list<FormalDeclNode *>();
		  (yylhs.value.transFn) = new FnDeclNode(pos, (yystack_[6].value.transType), (yystack_[5].value.transID), f, (yystack_[1].value.transStmts));
		  }
#line 717 "parser.cc"
    break;

  case 16:
#line 222 "cminusminus.yy"
    {
		  Position * pos = new Position((yystack_[7].value.transType)->pos(), (yystack_[0].value.transToken)->pos());
		  (yylhs.value.transFn) = new FnDeclNode(pos, (yystack_[7].value.transType), (yystack_[6].value.transID), (yystack_[4].value.transFormalList), (yystack_[1].value.transStmts));
		  }
#line 726 "parser.cc"
    break;

  case 17:
#line 228 "cminusminus.yy"
    {
		  (yylhs.value.transFormalList) = new std::list<FormalDeclNode *>();
		  (yylhs.value.transFormalList)->push_back((yystack_[0].value.transFormal));
		  }
#line 735 "parser.cc"
    break;

  case 18:
#line 233 "cminusminus.yy"
    {
		  (yylhs.value.transFormalList) = (yystack_[2].value.transFormalList);
		  (yylhs.value.transFormalList)->push_back((yystack_[0].value.transFormal));
		  }
#line 744 "parser.cc"
    break;

  case 19:
#line 239 "cminusminus.yy"
    {
		  Position * pos = new Position((yystack_[1].value.transType)->pos(), (yystack_[0].value.transID)->pos());
		  (yylhs.value.transFormal) = new FormalDeclNode(pos, (yystack_[1].value.transType), (yystack_[0].value.transID));
		  }
#line 753 "parser.cc"
    break;

  case 20:
#line 245 "cminusminus.yy"
    {
		  (yylhs.value.transStmts) = new std::list<StmtNode *>();
	   	  }
#line 761 "parser.cc"
    break;

  case 21:
#line 249 "cminusminus.yy"
    {
		  (yylhs.value.transStmts) = (yystack_[1].value.transStmts);
		  (yylhs.value.transStmts)->push_back((yystack_[0].value.transStmt));
	  	  }
#line 770 "parser.cc"
    break;

  case 22:
#line 255 "cminusminus.yy"
    {
		  Position * p = (yystack_[0].value.transVarDecl)->pos();
		  (yylhs.value.transStmt) = new VarDeclNode(p, (yystack_[0].value.transVarDecl)->getTypeNode(), (yystack_[0].value.transVarDecl)->ID());
		  }
#line 779 "parser.cc"
    break;

  case 23:
#line 260 "cminusminus.yy"
    {
		  Position * p = new Position((yystack_[1].value.transAssignExp)->pos(), (yystack_[0].value.transToken)->pos());
		  (yylhs.value.transStmt) = new AssignStmtNode(p, (yystack_[1].value.transAssignExp)); 
		  }
#line 788 "parser.cc"
    break;

  case 24:
#line 265 "cminusminus.yy"
    {
		  Position * p = new Position((yystack_[2].value.transLVal)->pos(), (yystack_[0].value.transToken)->pos());
		  (yylhs.value.transStmt) = new PostDecStmtNode(p, (yystack_[2].value.transLVal));
		  }
#line 797 "parser.cc"
    break;

  case 25:
#line 270 "cminusminus.yy"
    {
		  Position * p = new Position((yystack_[2].value.transLVal)->pos(), (yystack_[0].value.transToken)->pos());
		  (yylhs.value.transStmt) = new PostIncStmtNode(p, (yystack_[2].value.transLVal));
		  }
#line 806 "parser.cc"
    break;

  case 26:
#line 275 "cminusminus.yy"
    {
		  Position * p = new Position((yystack_[2].value.transToken)->pos(), (yystack_[0].value.transToken)->pos());
		  (yylhs.value.transStmt) = new ReadStmtNode(p, (yystack_[1].value.transLVal));
		  }
#line 815 "parser.cc"
    break;

  case 27:
#line 280 "cminusminus.yy"
    {
		  Position * p = new Position((yystack_[2].value.transToken)->pos(), (yystack_[0].value.transToken)->pos());
		  (yylhs.value.transStmt) = new WriteStmtNode(p, (yystack_[1].value.transExp));
		  }
#line 824 "parser.cc"
    break;

  case 28:
#line 285 "cminusminus.yy"
    {
		  Position * p = new Position((yystack_[6].value.transToken)->pos(), (yystack_[0].value.transToken)->pos());
		  (yylhs.value.transStmt) = new WhileStmtNode(p, (yystack_[4].value.transExp), (yystack_[1].value.transStmts));
		  }
#line 833 "parser.cc"
    break;

  case 29:
#line 290 "cminusminus.yy"
    {
		  Position * p = new Position((yystack_[6].value.transToken)->pos(), (yystack_[0].value.transToken)->pos());
		  (yylhs.value.transStmt) = new IfStmtNode(p, (yystack_[4].value.transExp), (yystack_[1].value.transStmts));
		  }
#line 842 "parser.cc"
    break;

  case 30:
#line 295 "cminusminus.yy"
    {
		  Position * p = new Position((yystack_[10].value.transToken)->pos(), (yystack_[0].value.transToken)->pos());
		  (yylhs.value.transStmt) = new IfElseStmtNode(p, (yystack_[8].value.transExp), (yystack_[5].value.transStmts), (yystack_[1].value.transStmts));
		  }
#line 851 "parser.cc"
    break;

  case 31:
#line 300 "cminusminus.yy"
    {
		  Position * p = new Position((yystack_[2].value.transToken)->pos(), (yystack_[0].value.transToken)->pos());
		  (yylhs.value.transStmt) = new ReturnStmtNode(p, (yystack_[1].value.transExp));
		  }
#line 860 "parser.cc"
    break;

  case 32:
#line 305 "cminusminus.yy"
    {
		  Position * p = new Position((yystack_[1].value.transToken)->pos(), (yystack_[0].value.transToken)->pos());
		  (yylhs.value.transStmt) = new ReturnStmtNode(p, nullptr);
		  }
#line 869 "parser.cc"
    break;

  case 33:
#line 310 "cminusminus.yy"
    { 
		  Position * p = new Position((yystack_[1].value.transCallExp)->pos(), (yystack_[0].value.transToken)->pos());
		  (yylhs.value.transStmt) = new CallStmtNode(p, (yystack_[1].value.transCallExp)); 
		  }
#line 878 "parser.cc"
    break;

  case 34:
#line 316 "cminusminus.yy"
    { (yylhs.value.transExp) = (yystack_[0].value.transAssignExp); }
#line 884 "parser.cc"
    break;

  case 35:
#line 318 "cminusminus.yy"
    {
		  Position * p = new Position((yystack_[2].value.transExp)->pos(), (yystack_[0].value.transExp)->pos());
		  (yylhs.value.transExp) = new MinusNode(p, (yystack_[2].value.transExp), (yystack_[0].value.transExp));
		  }
#line 893 "parser.cc"
    break;

  case 36:
#line 323 "cminusminus.yy"
    {
		  Position * p = new Position((yystack_[2].value.transExp)->pos(), (yystack_[0].value.transExp)->pos());
		  (yylhs.value.transExp) = new PlusNode(p, (yystack_[2].value.transExp), (yystack_[0].value.transExp));
		  }
#line 902 "parser.cc"
    break;

  case 37:
#line 328 "cminusminus.yy"
    {
		  Position * p = new Position((yystack_[2].value.transExp)->pos(), (yystack_[0].value.transExp)->pos());
		  (yylhs.value.transExp) = new TimesNode(p, (yystack_[2].value.transExp), (yystack_[0].value.transExp));
		  }
#line 911 "parser.cc"
    break;

  case 38:
#line 333 "cminusminus.yy"
    {
		  Position * p = new Position((yystack_[2].value.transExp)->pos(), (yystack_[0].value.transExp)->pos());
		  (yylhs.value.transExp) = new DivideNode(p, (yystack_[2].value.transExp), (yystack_[0].value.transExp));
		  }
#line 920 "parser.cc"
    break;

  case 39:
#line 338 "cminusminus.yy"
    {
		  Position * p = new Position((yystack_[2].value.transExp)->pos(), (yystack_[0].value.transExp)->pos());
		  (yylhs.value.transExp) = new AndNode(p, (yystack_[2].value.transExp), (yystack_[0].value.transExp));
		  }
#line 929 "parser.cc"
    break;

  case 40:
#line 343 "cminusminus.yy"
    {
		  Position * p = new Position((yystack_[2].value.transExp)->pos(), (yystack_[0].value.transExp)->pos());
		  (yylhs.value.transExp) = new OrNode(p, (yystack_[2].value.transExp), (yystack_[0].value.transExp));
		  }
#line 938 "parser.cc"
    break;

  case 41:
#line 348 "cminusminus.yy"
    {
		  Position * p = new Position((yystack_[2].value.transExp)->pos(), (yystack_[0].value.transExp)->pos());
		  (yylhs.value.transExp) = new EqualsNode(p, (yystack_[2].value.transExp), (yystack_[0].value.transExp));
		  }
#line 947 "parser.cc"
    break;

  case 42:
#line 353 "cminusminus.yy"
    {
		  Position * p = new Position((yystack_[2].value.transExp)->pos(), (yystack_[0].value.transExp)->pos());
		  (yylhs.value.transExp) = new NotEqualsNode(p, (yystack_[2].value.transExp), (yystack_[0].value.transExp));
		  }
#line 956 "parser.cc"
    break;

  case 43:
#line 358 "cminusminus.yy"
    {
		  Position * p = new Position((yystack_[2].value.transExp)->pos(), (yystack_[0].value.transExp)->pos());
		  (yylhs.value.transExp) = new GreaterNode(p, (yystack_[2].value.transExp), (yystack_[0].value.transExp));
		  }
#line 965 "parser.cc"
    break;

  case 44:
#line 363 "cminusminus.yy"
    {
		  Position * p = new Position((yystack_[2].value.transExp)->pos(), (yystack_[0].value.transExp)->pos());
		  (yylhs.value.transExp) = new GreaterEqNode(p, (yystack_[2].value.transExp), (yystack_[0].value.transExp));
		  }
#line 974 "parser.cc"
    break;

  case 45:
#line 368 "cminusminus.yy"
    {
		  Position * p = new Position((yystack_[2].value.transExp)->pos(), (yystack_[0].value.transExp)->pos());
		  (yylhs.value.transExp) = new LessNode(p, (yystack_[2].value.transExp), (yystack_[0].value.transExp));
		  }
#line 983 "parser.cc"
    break;

  case 46:
#line 373 "cminusminus.yy"
    {
		  Position * p = new Position((yystack_[2].value.transExp)->pos(), (yystack_[0].value.transExp)->pos());
		  (yylhs.value.transExp) = new LessEqNode(p, (yystack_[2].value.transExp), (yystack_[0].value.transExp));
		  }
#line 992 "parser.cc"
    break;

  case 47:
#line 378 "cminusminus.yy"
    {
		  Position * p = new Position((yystack_[1].value.transToken)->pos(), (yystack_[0].value.transExp)->pos());
		  (yylhs.value.transExp) = new NotNode(p, (yystack_[0].value.transExp));
		  }
#line 1001 "parser.cc"
    break;

  case 48:
#line 383 "cminusminus.yy"
    {
		  Position * p = new Position((yystack_[1].value.transToken)->pos(), (yystack_[0].value.transExp)->pos());
		  (yylhs.value.transExp) = new NegNode(p, (yystack_[0].value.transExp));
		  }
#line 1010 "parser.cc"
    break;

  case 49:
#line 388 "cminusminus.yy"
    { (yylhs.value.transExp) = (yystack_[0].value.transExp); }
#line 1016 "parser.cc"
    break;

  case 50:
#line 391 "cminusminus.yy"
    {
		  Position * p = new Position((yystack_[2].value.transLVal)->pos(), (yystack_[0].value.transExp)->pos());
		  (yylhs.value.transAssignExp) = new AssignExpNode(p, (yystack_[2].value.transLVal), (yystack_[0].value.transExp));
		  }
#line 1025 "parser.cc"
    break;

  case 51:
#line 397 "cminusminus.yy"
    {
		  Position * p = new Position((yystack_[2].value.transID)->pos(), (yystack_[0].value.transToken)->pos());
		  std::list<ExpNode *> * noargs =
		    new std::list<ExpNode *>();
		  (yylhs.value.transCallExp) = new CallExpNode(p, (yystack_[2].value.transID), noargs);
		  }
#line 1036 "parser.cc"
    break;

  case 52:
#line 404 "cminusminus.yy"
    {
		  Position * p = new Position((yystack_[3].value.transID)->pos(), (yystack_[0].value.transToken)->pos());
		  (yylhs.value.transCallExp) = new CallExpNode(p, (yystack_[3].value.transID), (yystack_[1].value.transActuals));
		  }
#line 1045 "parser.cc"
    break;

  case 53:
#line 410 "cminusminus.yy"
    {
		  std::list<ExpNode *> * list =
		    new std::list<ExpNode *>();
		  list->push_back((yystack_[0].value.transExp));
		  (yylhs.value.transActuals) = list;
		  }
#line 1056 "parser.cc"
    break;

  case 54:
#line 417 "cminusminus.yy"
    {
		  (yylhs.value.transActuals) = (yystack_[2].value.transActuals);
		  (yylhs.value.transActuals)->push_back((yystack_[0].value.transExp));
		  }
#line 1065 "parser.cc"
    break;

  case 55:
#line 423 "cminusminus.yy"
    { (yylhs.value.transExp) = (yystack_[0].value.transLVal); }
#line 1071 "parser.cc"
    break;

  case 56:
#line 425 "cminusminus.yy"
    { (yylhs.value.transExp) = new IntLitNode((yystack_[0].value.transIntToken)->pos(), (yystack_[0].value.transIntToken)->num()); }
#line 1077 "parser.cc"
    break;

  case 57:
#line 427 "cminusminus.yy"
    { (yylhs.value.transExp) = new ShortLitNode((yystack_[0].value.transShortToken)->pos(), (yystack_[0].value.transShortToken)->num()); }
#line 1083 "parser.cc"
    break;

  case 58:
#line 429 "cminusminus.yy"
    { (yylhs.value.transExp) = new StrLitNode((yystack_[0].value.transStrToken)->pos(), (yystack_[0].value.transStrToken)->str()); }
#line 1089 "parser.cc"
    break;

  case 59:
#line 431 "cminusminus.yy"
    { (yylhs.value.transExp) = new RefNode((yystack_[1].value.transToken)->pos(), (yystack_[0].value.transID)); }
#line 1095 "parser.cc"
    break;

  case 60:
#line 433 "cminusminus.yy"
    { (yylhs.value.transExp) = new TrueNode((yystack_[0].value.transToken)->pos()); }
#line 1101 "parser.cc"
    break;

  case 61:
#line 435 "cminusminus.yy"
    { (yylhs.value.transExp) = new FalseNode((yystack_[0].value.transToken)->pos()); }
#line 1107 "parser.cc"
    break;

  case 62:
#line 437 "cminusminus.yy"
    { (yylhs.value.transExp) = (yystack_[1].value.transExp); }
#line 1113 "parser.cc"
    break;

  case 63:
#line 439 "cminusminus.yy"
    {
		  (yylhs.value.transExp) = (yystack_[0].value.transCallExp);
		  }
#line 1121 "parser.cc"
    break;

  case 64:
#line 444 "cminusminus.yy"
    {
		  (yylhs.value.transLVal) = (yystack_[0].value.transID);
		  }
#line 1129 "parser.cc"
    break;

  case 65:
#line 448 "cminusminus.yy"
    {
		  Position * pos = new Position((yystack_[1].value.transToken)->pos(), (yystack_[0].value.transID)->pos());
		  (yylhs.value.transLVal) = new DerefNode(pos, (yystack_[0].value.transID));
		  }
#line 1138 "parser.cc"
    break;

  case 66:
#line 454 "cminusminus.yy"
    {
		  Position * pos = (yystack_[0].value.transIDToken)->pos();
		  (yylhs.value.transID) = new IDNode(pos, (yystack_[0].value.transIDToken)->value()); 
		  }
#line 1147 "parser.cc"
    break;


#line 1151 "parser.cc"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yysyntax_error_ (yystack_[0].state, yyla));
      }


    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }


      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  Parser::error (const syntax_error& yyexc)
  {
    error (yyexc.what ());
  }

  // Generate an error message.
  std::string
  Parser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (!yyla.empty ())
      {
        int yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char Parser::yypact_ninf_ = -29;

  const signed char Parser::yytable_ninf_ = -1;

  const short
  Parser::yypact_[] =
  {
     -29,     8,   430,   -29,   -29,   -29,    35,   -29,   -29,   -29,
     -29,   -29,    -7,   -29,   -29,   -29,   -29,   -17,   237,   -29,
      -8,    -7,    -5,   -29,   -29,   -29,   430,    15,   102,   -29,
     -29,    -7,     1,     4,   225,   -29,     3,   279,   -29,    -7,
     -29,    11,    14,     6,    19,   120,   -29,   279,    18,   -29,
      -7,   -29,   -29,   279,    31,   279,   -29,   -29,   -29,   -29,
     286,   -29,   -29,   -29,    33,   279,   313,    21,   -29,   -29,
     279,    39,    40,   252,   -29,   335,   -29,   -29,   362,   -29,
     -29,   -29,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   -29,   279,   389,   -29,   416,   -29,   -29,
     -29,   416,    -2,    29,   -29,   459,   -29,   475,   475,   475,
     475,   475,    -9,   475,   436,    -9,   -29,    55,   279,   -29,
     -29,   -29,   416,   153,   171,    28,   -29,    58,   -29,   204,
     -29
  };

  const unsigned char
  Parser::yydefact_[] =
  {
       4,     0,     2,     1,    11,    10,     0,    13,    12,    14,
       3,     5,     0,     8,     6,     9,    66,     0,     0,     7,
       0,     0,     0,    17,    20,    19,     0,     0,     0,    18,
      20,     0,     0,     0,     0,    15,     0,     0,    22,     0,
      21,     0,     0,     0,    64,     0,    65,     0,     0,    64,
       0,    61,    56,     0,     0,     0,    32,    57,    58,    60,
       0,    34,    63,    49,    55,     0,     0,     0,    23,    33,
       0,     0,     0,     0,    16,     0,    26,    59,     0,    48,
      55,    47,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    31,     0,     0,    27,    50,    24,    25,
      51,    53,     0,     0,    62,    39,    38,    41,    43,    44,
      45,    46,    35,    42,    40,    36,    37,     0,     0,    52,
      20,    20,    54,     0,     0,    29,    28,     0,    20,     0,
      30
  };

  const signed char
  Parser::yypgoto_[] =
  {
     -29,   -29,   -29,   -29,    78,    76,    75,   -29,   -29,    56,
     -14,   -29,   -25,   -26,   -24,   -29,    30,   -28,     2
  };

  const signed char
  Parser::yydefgoto_[] =
  {
      -1,     1,     2,    10,    38,    39,    13,    14,    22,    23,
      28,    40,    60,    61,    62,   102,    63,    64,    44
  };

  const short
  Parser::yytable_[] =
  {
      43,    83,    41,    26,    42,    48,   118,    18,     3,    16,
      31,    70,    66,    24,    17,    71,    45,    43,    19,    41,
      16,    42,    75,    25,    72,    47,    80,    65,    78,    27,
      81,    94,   119,    46,    50,    49,    30,    31,    70,   127,
      95,    67,     4,    73,    51,    97,    68,    16,   101,    69,
     120,    52,    77,    76,     5,    53,    19,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,    57,   116,
      58,     7,    59,     8,    98,    99,   121,     9,    12,   128,
      11,    15,    29,     0,    79,     0,     0,     0,     0,     0,
       0,     0,     0,   122,    21,    43,    43,    41,    41,    42,
      42,    43,    21,    41,     0,    42,   123,   124,    31,     4,
       0,     0,     0,     0,   129,     0,     0,     0,    16,    32,
       0,     5,     0,     0,     0,     0,    31,     4,     0,     0,
       0,     0,     6,    33,    34,    35,    16,    32,     7,     5,
       8,     0,     0,     0,     9,    36,    37,     0,     0,     0,
       6,    33,    34,    74,     0,     0,     7,     0,     8,    31,
       4,     0,     9,    36,    37,     0,     0,     0,     0,    16,
      32,     0,     5,     0,     0,     0,     0,    31,     4,     0,
       0,     0,     0,     6,    33,    34,   125,    16,    32,     7,
       5,     8,     0,     0,     0,     9,    36,    37,     0,     0,
       0,     6,    33,    34,   126,     0,     0,     7,     0,     8,
      31,     4,     0,     9,    36,    37,     0,     0,     0,     0,
      16,    32,     0,     5,     0,     0,     0,     0,    50,     0,
       0,    31,     0,     0,     6,    33,    34,   130,    51,     0,
       7,    16,     8,     0,     4,    52,     9,    36,    37,    53,
      54,    55,     0,     0,     0,    50,     5,     0,    31,     0,
      56,     0,    57,     0,    58,    51,    59,     6,    16,     0,
       0,    20,    52,     7,     0,     8,    53,    54,    55,     9,
       0,     0,    50,     0,     0,    31,   100,     0,     0,    57,
      82,    58,    51,    59,     0,    16,    83,     0,    84,    52,
      85,    86,     0,    53,    54,    55,     0,     0,    87,    88,
       0,    89,     0,    90,    91,    92,    57,    82,    58,     0,
      59,    93,     0,    83,     0,    84,    94,    85,    86,     0,
       0,     0,     0,     0,     0,    87,    88,     0,    89,    82,
      90,    91,    92,     0,     0,    83,     0,    84,    96,    85,
      86,     0,     0,    94,     0,     0,     0,    87,    88,     0,
      89,     0,    90,    91,    92,     0,    82,     0,     0,   103,
       0,     0,    83,     0,    84,    94,    85,    86,     0,     0,
       0,     0,     0,     0,    87,    88,     0,    89,     0,    90,
      91,    92,     0,    82,     0,     0,   104,     0,     0,    83,
       0,    84,    94,    85,    86,     0,     0,     0,     0,     0,
       0,    87,    88,     0,    89,     0,    90,    91,    92,     0,
      82,     0,     0,   117,     0,     0,    83,     0,    84,    94,
      85,    86,     0,     0,     0,     0,     0,     4,    87,    88,
      82,    89,     0,    90,    91,    92,    83,     0,    84,     5,
      85,    86,     0,     0,     0,     0,    94,     0,    87,    88,
       6,    89,     0,    90,     0,    92,     7,     0,     8,    83,
       0,    84,     9,    85,    86,     0,    94,     0,     0,     0,
       0,    87,    88,     0,    89,    83,    90,    -1,    92,    -1,
      -1,     0,     0,     0,     0,     0,     0,    -1,    -1,    94,
      89,     0,    -1,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94
  };

  const short
  Parser::yycheck_[] =
  {
      28,    10,    28,     8,    28,    33,     8,    24,     0,    16,
       6,     5,    37,    21,    12,     9,    30,    45,    35,    45,
      16,    45,    47,    21,    18,    24,    54,    24,    53,    34,
      55,    40,    34,    31,     3,    33,    21,     6,     5,    11,
      65,    39,     7,    24,    13,    70,    35,    16,    73,    35,
      21,    20,    50,    35,    19,    24,    35,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    37,    94,
      39,    36,    41,    38,    35,    35,    21,    42,     2,    21,
       2,     6,    26,    -1,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,    18,   123,   124,   123,   124,   123,
     124,   129,    26,   129,    -1,   129,   120,   121,     6,     7,
      -1,    -1,    -1,    -1,   128,    -1,    -1,    -1,    16,    17,
      -1,    19,    -1,    -1,    -1,    -1,     6,     7,    -1,    -1,
      -1,    -1,    30,    31,    32,    33,    16,    17,    36,    19,
      38,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,    -1,
      30,    31,    32,    33,    -1,    -1,    36,    -1,    38,     6,
       7,    -1,    42,    43,    44,    -1,    -1,    -1,    -1,    16,
      17,    -1,    19,    -1,    -1,    -1,    -1,     6,     7,    -1,
      -1,    -1,    -1,    30,    31,    32,    33,    16,    17,    36,
      19,    38,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,
      -1,    30,    31,    32,    33,    -1,    -1,    36,    -1,    38,
       6,     7,    -1,    42,    43,    44,    -1,    -1,    -1,    -1,
      16,    17,    -1,    19,    -1,    -1,    -1,    -1,     3,    -1,
      -1,     6,    -1,    -1,    30,    31,    32,    33,    13,    -1,
      36,    16,    38,    -1,     7,    20,    42,    43,    44,    24,
      25,    26,    -1,    -1,    -1,     3,    19,    -1,     6,    -1,
      35,    -1,    37,    -1,    39,    13,    41,    30,    16,    -1,
      -1,    34,    20,    36,    -1,    38,    24,    25,    26,    42,
      -1,    -1,     3,    -1,    -1,     6,    34,    -1,    -1,    37,
       4,    39,    13,    41,    -1,    16,    10,    -1,    12,    20,
      14,    15,    -1,    24,    25,    26,    -1,    -1,    22,    23,
      -1,    25,    -1,    27,    28,    29,    37,     4,    39,    -1,
      41,    35,    -1,    10,    -1,    12,    40,    14,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    23,    -1,    25,     4,
      27,    28,    29,    -1,    -1,    10,    -1,    12,    35,    14,
      15,    -1,    -1,    40,    -1,    -1,    -1,    22,    23,    -1,
      25,    -1,    27,    28,    29,    -1,     4,    -1,    -1,    34,
      -1,    -1,    10,    -1,    12,    40,    14,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    23,    -1,    25,    -1,    27,
      28,    29,    -1,     4,    -1,    -1,    34,    -1,    -1,    10,
      -1,    12,    40,    14,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    23,    -1,    25,    -1,    27,    28,    29,    -1,
       4,    -1,    -1,    34,    -1,    -1,    10,    -1,    12,    40,
      14,    15,    -1,    -1,    -1,    -1,    -1,     7,    22,    23,
       4,    25,    -1,    27,    28,    29,    10,    -1,    12,    19,
      14,    15,    -1,    -1,    -1,    -1,    40,    -1,    22,    23,
      30,    25,    -1,    27,    -1,    29,    36,    -1,    38,    10,
      -1,    12,    42,    14,    15,    -1,    40,    -1,    -1,    -1,
      -1,    22,    23,    -1,    25,    10,    27,    12,    29,    14,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    22,    23,    40,
      25,    -1,    27,    -1,    29,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    40
  };

  const unsigned char
  Parser::yystos_[] =
  {
       0,    46,    47,     0,     7,    19,    30,    36,    38,    42,
      48,    49,    50,    51,    52,    51,    16,    63,    24,    35,
      34,    50,    53,    54,    21,    63,     8,    34,    55,    54,
      21,     6,    17,    31,    32,    33,    43,    44,    49,    50,
      56,    58,    59,    62,    63,    55,    63,    24,    62,    63,
       3,    13,    20,    24,    25,    26,    35,    37,    39,    41,
      57,    58,    59,    61,    62,    24,    57,    63,    35,    35,
       5,     9,    18,    24,    33,    57,    35,    63,    57,    61,
      62,    57,     4,    10,    12,    14,    15,    22,    23,    25,
      27,    28,    29,    35,    40,    57,    35,    57,    35,    35,
      34,    57,    60,    34,    34,    57,    57,    57,    57,    57,
      57,    57,    57,    57,    57,    57,    57,    34,     8,    34,
      21,    21,    57,    55,    55,    33,    33,    11,    21,    55,
      33
  };

  const unsigned char
  Parser::yyr1_[] =
  {
       0,    45,    46,    47,    47,    48,    48,    49,    50,    50,
      51,    51,    51,    51,    51,    52,    52,    53,    53,    54,
      55,    55,    56,    56,    56,    56,    56,    56,    56,    56,
      56,    56,    56,    56,    57,    57,    57,    57,    57,    57,
      57,    57,    57,    57,    57,    57,    57,    57,    57,    57,
      58,    59,    59,    60,    60,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    62,    62,    63
  };

  const unsigned char
  Parser::yyr2_[] =
  {
       0,     2,     1,     2,     0,     1,     1,     3,     1,     2,
       1,     1,     1,     1,     1,     7,     8,     1,     3,     2,
       0,     2,     1,     2,     3,     3,     3,     3,     7,     7,
      11,     3,     2,     2,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     1,
       3,     3,     4,     1,     3,     1,     1,     1,     1,     2,
       1,     1,     3,     1,     1,     2,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const Parser::yytname_[] =
  {
  "\"end file\"", "error", "$undefined", "AMP", "AND", "ASSIGN", "AT",
  "BOOL", "COMMA", "DEC", "DIVIDE", "ELSE", "EQUALS", "FALSE", "GREATER",
  "GREATEREQ", "ID", "IF", "INC", "INT", "INTLITERAL", "LCURLY", "LESS",
  "LESSEQ", "LPAREN", "MINUS", "NOT", "NOTEQUALS", "OR", "PLUS", "PTR",
  "READ", "RETURN", "RCURLY", "RPAREN", "SEMICOL", "SHORT", "SHORTLITERAL",
  "STRING", "STRLITERAL", "TIMES", "TRUE", "VOID", "WHILE", "WRITE",
  "$accept", "program", "globals", "decl", "varDecl", "type", "primType",
  "fnDecl", "formals", "formalDecl", "stmtList", "stmt", "exp",
  "assignExp", "callExp", "actualsList", "term", "lval", "id", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short
  Parser::yyrline_[] =
  {
       0,   157,   157,   163,   170,   174,   176,   179,   185,   189,
     194,   198,   202,   206,   210,   215,   221,   227,   232,   238,
     245,   248,   254,   259,   264,   269,   274,   279,   284,   289,
     294,   299,   304,   309,   315,   317,   322,   327,   332,   337,
     342,   347,   352,   357,   362,   367,   372,   377,   382,   387,
     390,   396,   403,   409,   416,   422,   424,   426,   428,   430,
     432,   434,   436,   438,   443,   447,   453
  };

  // Print the state stack on the debug stream.
  void
  Parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << '\n';
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  Parser::yy_reduce_print_ (int yyrule)
  {
    unsigned yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  Parser::token_number_type
  Parser::yytranslate_ (int t)
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const token_number_type
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44
    };
    const unsigned user_token_number_max_ = 299;
    const token_number_type undef_token_ = 2;

    if (static_cast<int> (t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }

#line 5 "cminusminus.yy"
} // cminusminus
#line 1744 "parser.cc"

#line 459 "cminusminus.yy"


void cminusminus::Parser::error(const std::string& msg){
	std::cout << msg << std::endl;
	std::cerr << "syntax error" << std::endl;
}
