
# line 30 "expread.y"
#include <stdio.h>
#include "defs.h"
#include "param.h"
#include "symtab.h"
#include "frame.h"
#include "expression.h"

#include <a.out.h>

static struct expression *expout;
static int expout_size;
static int expout_ptr;

static int yylex ();
static void yyerror ();
static void write_exp_elt ();
static void write_exp_elt_opcode ();
static void write_exp_elt_sym ();
static void write_exp_elt_longcst ();
static void write_exp_elt_dblcst ();
static void write_exp_elt_type ();
static void write_exp_elt_intern ();
static void write_exp_string ();
static void start_arglist ();
static int end_arglist ();
static void free_funcalls ();
static char *copy_name ();

/* If this is nonzero, this block is used as the lexical context
   for symbol names.  */

static struct block *expression_context_block;

/* The innermost context required by the stack and register variables
   we've encountered so far. */
struct block *innermost_block;

/* The block in which the most recently discovered symbol was found. */
struct block *block_found;

/* Number of arguments seen so far in innermost function call.  */
static int arglist_len;

/* Data structure for saving values of arglist_len
   for function calls whose arguments contain other function calls.  */

struct funcall
  {
    struct funcall *next;
    int arglist_len;
  };

struct funcall *funcall_chain;

/* This kind of datum is used to represent the name
   of a symbol token.  */

struct stoken
  {
    char *ptr;
    int length;
  };

/* For parsing of complicated types.
   An array should be preceded in the list by the size of the array.  */
enum type_pieces
  {tp_end = -1, tp_pointer, tp_reference, tp_array, tp_function};
static enum type_pieces *type_stack;
static int type_stack_depth, type_stack_size;

static void push_type ();
static enum type_pieces pop_type ();

/* Allow debugging of parsing.  */
#define YYDEBUG 1

# line 111 "expread.y"
typedef union 
  {
    LONGEST lval;
    unsigned LONGEST ulval;
    double dval;
    struct symbol *sym;
    struct type *tval;
    struct stoken sval;
    int voidval;
    struct block *bval;
    enum exp_opcode opcode;
    struct internalvar *ivar;

    struct type **tvec;
    int *ivec;
  } YYSTYPE;
# define INT 257
# define CHAR 258
# define UINT 259
# define FLOAT 260
# define NAME 261
# define TYPENAME 262
# define BLOCKNAME 263
# define STRING 264
# define STRUCT 265
# define UNION 266
# define ENUM 267
# define SIZEOF 268
# define UNSIGNED 269
# define COLONCOLON 270
# define SIGNED 271
# define LONG 272
# define SHORT 273
# define INT_KEYWORD 274
# define LAST 275
# define REGNAME 276
# define VARIABLE 277
# define ASSIGN_MODIFY 278
# define THIS 279
# define ABOVE_COMMA 280
# define OR 281
# define AND 282
# define EQUAL 283
# define NOTEQUAL 284
# define LEQ 285
# define GEQ 286
# define LSH 287
# define RSH 288
# define UNARY 289
# define INCREMENT 290
# define DECREMENT 291
# define ARROW 292
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern short yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
YYSTYPE yylval, yyval;
# define YYERRCODE 256

# line 810 "expread.y"


/* Begin counting arguments for a function call,
   saving the data about any containing call.  */

static void
start_arglist ()
{
  register struct funcall *new = (struct funcall *) xmalloc (sizeof (struct funcall));

  new->next = funcall_chain;
  new->arglist_len = arglist_len;
  arglist_len = 0;
  funcall_chain = new;
}

/* Return the number of arguments in a function call just terminated,
   and restore the data for the containing function call.  */

static int
end_arglist ()
{
  register int val = arglist_len;
  register struct funcall *call = funcall_chain;
  funcall_chain = call->next;
  arglist_len = call->arglist_len;
  free (call);
  return val;
}

/* Free everything in the funcall chain.
   Used when there is an error inside parsing.  */

static void
free_funcalls ()
{
  register struct funcall *call, *next;

  for (call = funcall_chain; call; call = next)
    {
      next = call->next;
      free (call);
    }
}

/* This page contains the functions for adding data to the  struct expression
   being constructed.  */

/* Add one element to the end of the expression.  */

/* To avoid a bug in the Sun 4 compiler, we pass things that can fit into
   a register through here */

static void
write_exp_elt (expelt)
     union exp_element expelt;
{
  if (expout_ptr >= expout_size)
    {
      expout_size *= 2;
      expout = (struct expression *) xrealloc (expout,
					       sizeof (struct expression)
					       + expout_size * sizeof (union exp_element));
    }
  expout->elts[expout_ptr++] = expelt;
}

static void
write_exp_elt_opcode (expelt)
     enum exp_opcode expelt;
{
  union exp_element tmp;

  tmp.opcode = expelt;

  write_exp_elt (tmp);
}

static void
write_exp_elt_sym (expelt)
     struct symbol *expelt;
{
  union exp_element tmp;

  tmp.symbol = expelt;

  write_exp_elt (tmp);
}

static void
write_exp_elt_longcst (expelt)
     LONGEST expelt;
{
  union exp_element tmp;

  tmp.longconst = expelt;

  write_exp_elt (tmp);
}

static void
write_exp_elt_dblcst (expelt)
     double expelt;
{
  union exp_element tmp;

  tmp.doubleconst = expelt;

  write_exp_elt (tmp);
}

static void
write_exp_elt_type (expelt)
     struct type *expelt;
{
  union exp_element tmp;

  tmp.type = expelt;

  write_exp_elt (tmp);
}

static void
write_exp_elt_intern (expelt)
     struct internalvar *expelt;
{
  union exp_element tmp;

  tmp.internalvar = expelt;

  write_exp_elt (tmp);
}

/* Add a string constant to the end of the expression.
   Follow it by its length in bytes, as a separate exp_element.  */

static void
write_exp_string (str)
     struct stoken str;
{
  register int len = str.length;
  register int lenelt
    = (len + sizeof (union exp_element)) / sizeof (union exp_element);

  expout_ptr += lenelt;

  if (expout_ptr >= expout_size)
    {
      expout_size = max (expout_size * 2, expout_ptr + 10);
      expout = (struct expression *)
	xrealloc (expout, (sizeof (struct expression)
			   + (expout_size * sizeof (union exp_element))));
    }
  bcopy (str.ptr, (char *) &expout->elts[expout_ptr - lenelt], len);
  ((char *) &expout->elts[expout_ptr - lenelt])[len] = 0;
  write_exp_elt_longcst ((LONGEST) len);
}

/* During parsing of a C expression, the pointer to the next character
   is in this variable.  */

static char *lexptr;

/* Tokens that refer to names do so with explicit pointer and length,
   so they can share the storage that lexptr is parsing.

   When it is necessary to pass a name to a function that expects
   a null-terminated string, the substring is copied out
   into a block of storage that namecopy points to.

   namecopy is allocated once, guaranteed big enough, for each parsing.  */

static char *namecopy;

/* Current depth in parentheses within the expression.  */

static int paren_depth;

/* Nonzero means stop parsing on first comma (if not within parentheses).  */

static int comma_terminates;

/* Take care of parsing a number (anything that starts with a digit).
   Set yylval and return the token type; update lexptr.
   LEN is the number of characters in it.  */

/*** Needs some error checking for the float case ***/

static int
parse_number (olen)
     int olen;
{
  register char *p = lexptr;
  register LONGEST n = 0;
  register int c;
  register int base = 10;
  register int len = olen;
  char *err_copy;
  int unsigned_p = 0;

  extern double atof ();

  for (c = 0; c < len; c++)
    if (p[c] == '.')
      {
	/* It's a float since it contains a point.  */
	yylval.dval = atof (p);
	lexptr += len;
	return FLOAT;
      }

  if (len >= 3 && (!strncmp (p, "0x", 2) || !strncmp (p, "0X", 2)))
    {
      p += 2;
      base = 16;
      len -= 2;
    }
  else if (*p == '0')
    base = 8;

  while (len-- > 0)
    {
      c = *p++;
      if (c >= 'A' && c <= 'Z') c += 'a' - 'A';
      if (c != 'l' && c != 'u')
	n *= base;
      if (c >= '0' && c <= '9')
	n += c - '0';
      else
	{
	  if (base == 16 && c >= 'a' && c <= 'f')
	    n += c - 'a' + 10;
	  else if (len == 0 && c == 'l')
	    ;
	  else if (len == 0 && c == 'u')
	    unsigned_p = 1;
	  else if (base == 10 && len != 0 && (c == 'e' || c == 'E'))
	    {
	      /* Scientific notation, where we are unlucky enough not
		 to have a '.' in the string.  */
	      yylval.dval = atof (lexptr);
	      lexptr += olen;
	      return FLOAT;
	    }
	  else
	    {
	      err_copy = (char *) alloca (olen + 1);
	      bcopy (lexptr, err_copy, olen);
	      err_copy[olen] = 0;
	      error ("Invalid number \"%s\".", err_copy);
	    }
	}
    }

  lexptr = p;
  if (unsigned_p)
    {
      yylval.ulval = n;
      return UINT;
    }
  else
    {
      yylval.lval = n;
      return INT;
    }
}

struct token
{
  char *operator;
  int token;
  enum exp_opcode opcode;
};

static struct token tokentab3[] =
  {
    {">>=", ASSIGN_MODIFY, BINOP_RSH},
    {"<<=", ASSIGN_MODIFY, BINOP_LSH}
  };

static struct token tokentab2[] =
  {
    {"+=", ASSIGN_MODIFY, BINOP_ADD},
    {"-=", ASSIGN_MODIFY, BINOP_SUB},
    {"*=", ASSIGN_MODIFY, BINOP_MUL},
    {"/=", ASSIGN_MODIFY, BINOP_DIV},
    {"%=", ASSIGN_MODIFY, BINOP_REM},
    {"|=", ASSIGN_MODIFY, BINOP_LOGIOR},
    {"&=", ASSIGN_MODIFY, BINOP_LOGAND},
    {"^=", ASSIGN_MODIFY, BINOP_LOGXOR},
    {"++", INCREMENT, BINOP_END},
    {"--", DECREMENT, BINOP_END},
    {"->", ARROW, BINOP_END},
    {"&&", AND, BINOP_END},
    {"||", OR, BINOP_END},
    {"::", COLONCOLON, BINOP_END},
    {"<<", LSH, BINOP_END},
    {">>", RSH, BINOP_END},
    {"==", EQUAL, BINOP_END},
    {"!=", NOTEQUAL, BINOP_END},
    {"<=", LEQ, BINOP_END},
    {">=", GEQ, BINOP_END}
  };

/* assign machine-independent names to certain registers 
 * (unless overridden by the REGISTER_NAMES table)
 */
struct std_regs {
	char *name;
	int regnum;
} std_regs[] = {
#ifdef PC_REGNUM
	{ "pc", PC_REGNUM },
#endif
#ifdef FP_REGNUM
	{ "fp", FP_REGNUM },
#endif
#ifdef SP_REGNUM
	{ "sp", SP_REGNUM },
#endif
#ifdef PS_REGNUM
	{ "ps", PS_REGNUM },
#endif
};

#define NUM_STD_REGS (sizeof std_regs / sizeof std_regs[0])

/* Read one token, getting characters through lexptr.  */

static int
yylex ()
{
  register int c;
  register int namelen;
  register int i;
  register char *tokstart;

 retry:

  tokstart = lexptr;
  /* See if it is a special token of length 3.  */
  for (i = 0; i < sizeof tokentab3 / sizeof tokentab3[0]; i++)
    if (!strncmp (tokstart, tokentab3[i].operator, 3))
      {
	lexptr += 3;
	yylval.opcode = tokentab3[i].opcode;
	return tokentab3[i].token;
      }

  /* See if it is a special token of length 2.  */
  for (i = 0; i < sizeof tokentab2 / sizeof tokentab2[0]; i++)
    if (!strncmp (tokstart, tokentab2[i].operator, 2))
      {
	lexptr += 2;
	yylval.opcode = tokentab2[i].opcode;
	return tokentab2[i].token;
      }

  switch (c = *tokstart)
    {
    case 0:
      return 0;

    case ' ':
    case '\t':
    case '\n':
      lexptr++;
      goto retry;

    case '\'':
      lexptr++;
      c = *lexptr++;
      if (c == '\\')
	c = parse_escape (&lexptr);
      yylval.lval = c;
      c = *lexptr++;
      if (c != '\'')
	error ("Invalid character constant.");
      return CHAR;

    case '(':
      paren_depth++;
      lexptr++;
      return c;

    case ')':
      if (paren_depth == 0)
	return 0;
      paren_depth--;
      lexptr++;
      return c;

    case ',':
      if (comma_terminates && paren_depth == 0)
	return 0;
      lexptr++;
      return c;

    case '.':
      /* Might be a floating point number.  */
      if (lexptr[1] >= '0' && lexptr[1] <= '9')
	break;			/* Falls into number code.  */

    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
    case '|':
    case '&':
    case '^':
    case '~':
    case '!':
    case '@':
    case '<':
    case '>':
    case '[':
    case ']':
    case '?':
    case ':':
    case '=':
    case '{':
    case '}':
      lexptr++;
      return c;

    case '"':
      for (namelen = 1; (c = tokstart[namelen]) != '"'; namelen++)
	if (c == '\\')
	  {
	    c = tokstart[++namelen];
	    if (c >= '0' && c <= '9')
	      {
		c = tokstart[++namelen];
		if (c >= '0' && c <= '9')
		  c = tokstart[++namelen];
	      }
	  }
      yylval.sval.ptr = tokstart + 1;
      yylval.sval.length = namelen - 1;
      lexptr += namelen + 1;
      return STRING;
    }

  /* Is it a number?  */
  /* Note:  We have already dealt with the case of the token '.'.
     See case '.' above.  */
  if ((c >= '0' && c <= '9') || c == '.')
    {
      /* It's a number.  */
      int got_dot = 0, got_e = 0;
      register char *p = tokstart;
      int hex = c == '0' && (p[1] == 'x' || p[1] == 'X');
      if (hex)
	p += 2;
      for (;; ++p)
	{
	  if (!hex && !got_e && (*p == 'e' || *p == 'E'))
	    got_dot = got_e = 1;
	  else if (!hex && !got_dot && *p == '.')
	    got_dot = 1;
	  else if (got_e && (p[-1] == 'e' || p[-1] == 'E')
		   && (*p == '-' || *p == '+'))
	    /* This is the sign of the exponent, not the end of the
	       number.  */
	    continue;
	  else if (*p < '0' || *p > '9'
		   && (!hex || ((*p < 'a' || *p > 'f')
				&& (*p < 'A' || *p > 'F'))))
	    break;
	}
      return parse_number (p - tokstart);
    }

  if (!(c == '_' || c == '$'
	|| (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')))
    /* We must have come across a bad character (e.g. ';').  */
    error ("Invalid character '%c' in expression.", c);

  /* It's a name.  See how long it is.  */
  namelen = 0;
  for (c = tokstart[namelen];
       (c == '_' || c == '$' || (c >= '0' && c <= '9')
	|| (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
       c = tokstart[++namelen])
    ;

  /* The token "if" terminates the expression and is NOT 
     removed from the input stream.  */
  if (namelen == 2 && tokstart[0] == 'i' && tokstart[1] == 'f')
    {
      return 0;
    }

  lexptr += namelen;

  /* Handle the tokens $digits; also $ (short for $0) and $$ (short for $$1)
     and $$digits (equivalent to $<-digits> if you could type that).
     Make token type LAST, and put the number (the digits) in yylval.  */

  if (*tokstart == '$')
    {
      register int negate = 0;
      c = 1;
      /* Double dollar means negate the number and add -1 as well.
	 Thus $$ alone means -1.  */
      if (namelen >= 2 && tokstart[1] == '$')
	{
	  negate = 1;
	  c = 2;
	}
      if (c == namelen)
	{
	  /* Just dollars (one or two) */
	  yylval.lval = - negate;
	  return LAST;
	}
      /* Is the rest of the token digits?  */
      for (; c < namelen; c++)
	if (!(tokstart[c] >= '0' && tokstart[c] <= '9'))
	  break;
      if (c == namelen)
	{
	  yylval.lval = atoi (tokstart + 1 + negate);
	  if (negate)
	    yylval.lval = - yylval.lval;
	  return LAST;
	}
    }

  /* Handle tokens that refer to machine registers:
     $ followed by a register name.  */

  if (*tokstart == '$') {
    for (c = 0; c < NUM_REGS; c++)
      if (namelen - 1 == strlen (reg_names[c])
	  && !strncmp (tokstart + 1, reg_names[c], namelen - 1))
	{
	  yylval.lval = c;
	  return REGNAME;
	}
    for (c = 0; c < NUM_STD_REGS; c++)
     if (namelen - 1 == strlen (std_regs[c].name)
	 && !strncmp (tokstart + 1, std_regs[c].name, namelen - 1))
       {
	 yylval.lval = std_regs[c].regnum;
	 return REGNAME;
       }
  }
  /* Catch specific keywords.  Should be done with a data structure.  */
  switch (namelen)
    {
    case 8:
      if (!strncmp (tokstart, "unsigned", 8))
	return UNSIGNED;
      break;
    case 6:
      if (!strncmp (tokstart, "struct", 6))
	return STRUCT;
      if (!strncmp (tokstart, "signed", 6))
	return SIGNED;
      if (!strncmp (tokstart, "sizeof", 6))      
	return SIZEOF;
      break;
    case 5:
      if (!strncmp (tokstart, "union", 5))
	return UNION;
      if (!strncmp (tokstart, "short", 5))
	return SHORT;
      break;
    case 4:
      if (!strncmp (tokstart, "enum", 4))
	return ENUM;
      if (!strncmp (tokstart, "long", 4))
	return LONG;
      if (!strncmp (tokstart, "this", 4)
	  && lookup_symbol ("$this", expression_context_block,
			    VAR_NAMESPACE, 0))
	return THIS;
      break;
    case 3:
      if (!strncmp (tokstart, "int", 3))
	return INT_KEYWORD;
      break;
    default:
      break;
    }

  yylval.sval.ptr = tokstart;
  yylval.sval.length = namelen;

  /* Any other names starting in $ are debugger internal variables.  */

  if (*tokstart == '$')
    {
      yylval.ivar = (struct internalvar *) lookup_internalvar (copy_name (yylval.sval) + 1);
      return VARIABLE;
    }

  /* Use token-type BLOCKNAME for symbols that happen to be defined as
     functions or symtabs.  If this is not so, then ...
     Use token-type TYPENAME for symbols that happen to be defined
     currently as names of types; NAME for other symbols.
     The caller is not constrained to care about the distinction.  */
  {
    char *tmp = copy_name (yylval.sval);
    struct symbol *sym;

    if (lookup_partial_symtab (tmp))
      return BLOCKNAME;
    sym = lookup_symbol (tmp, expression_context_block,
			 VAR_NAMESPACE, 0);
    if (sym && SYMBOL_CLASS (sym) == LOC_BLOCK)
      return BLOCKNAME;
    if (lookup_typename (copy_name (yylval.sval), expression_context_block, 1))
      return TYPENAME;
    return NAME;
  }
}

static void
yyerror ()
{
  error ("Invalid syntax in expression.");
}

/* Return a null-terminated temporary copy of the name
   of a string token.  */

static char *
copy_name (token)
     struct stoken token;
{
  bcopy (token.ptr, namecopy, token.length);
  namecopy[token.length] = 0;
  return namecopy;
}

/* Reverse an expression from suffix form (in which it is constructed)
   to prefix form (in which we can conveniently print or execute it).  */

static void prefixify_subexp ();

static void
prefixify_expression (expr)
     register struct expression *expr;
{
  register int len = sizeof (struct expression) +
				    expr->nelts * sizeof (union exp_element);
  register struct expression *temp;
  register int inpos = expr->nelts, outpos = 0;

  temp = (struct expression *) alloca (len);

  /* Copy the original expression into temp.  */
  bcopy (expr, temp, len);

  prefixify_subexp (temp, expr, inpos, outpos);
}

/* Return the number of exp_elements in the subexpression of EXPR
   whose last exp_element is at index ENDPOS - 1 in EXPR.  */

static int
length_of_subexp (expr, endpos)
     register struct expression *expr;
     register int endpos;
{
  register int oplen = 1;
  register int args = 0;
  register int i;

  if (endpos < 0)
    error ("?error in length_of_subexp");

  i = (int) expr->elts[endpos - 1].opcode;

  switch (i)
    {
      /* C++  */
    case OP_SCOPE:
      oplen = 4 + ((expr->elts[endpos - 2].longconst
		    + sizeof (union exp_element))
		   / sizeof (union exp_element));
      break;

    case OP_LONG:
    case OP_DOUBLE:
      oplen = 4;
      break;

    case OP_VAR_VALUE:
    case OP_LAST:
    case OP_REGISTER:
    case OP_INTERNALVAR:
      oplen = 3;
      break;

    case OP_FUNCALL:
      oplen = 3;
      args = 1 + expr->elts[endpos - 2].longconst;
      break;

    case UNOP_CAST:
    case UNOP_MEMVAL:
      oplen = 3;
      args = 1;
      break;

    case STRUCTOP_STRUCT:
    case STRUCTOP_PTR:
      args = 1;
    case OP_STRING:
      oplen = 3 + ((expr->elts[endpos - 2].longconst
		    + sizeof (union exp_element))
		   / sizeof (union exp_element));
      break;

    case TERNOP_COND:
      args = 3;
      break;

    case BINOP_ASSIGN_MODIFY:
      oplen = 3;
      args = 2;
      break;

      /* C++ */
    case OP_THIS:
      oplen = 2;
      break;

    default:
      args = 1 + (i < (int) BINOP_END);
    }

  while (args > 0)
    {
      oplen += length_of_subexp (expr, endpos - oplen);
      args--;
    }

  return oplen;
}

/* Copy the subexpression ending just before index INEND in INEXPR
   into OUTEXPR, starting at index OUTBEG.
   In the process, convert it from suffix to prefix form.  */

static void
prefixify_subexp (inexpr, outexpr, inend, outbeg)
     register struct expression *inexpr;
     struct expression *outexpr;
     register int inend;
     int outbeg;
{
  register int oplen = 1;
  register int args = 0;
  register int i;
  int *arglens;
  enum exp_opcode opcode;

  /* Compute how long the last operation is (in OPLEN),
     and also how many preceding subexpressions serve as
     arguments for it (in ARGS).  */

  opcode = inexpr->elts[inend - 1].opcode;
  switch (opcode)
    {
      /* C++  */
    case OP_SCOPE:
      oplen = 4 + ((inexpr->elts[inend - 2].longconst
		    + sizeof (union exp_element))
		   / sizeof (union exp_element));
      break;

    case OP_LONG:
    case OP_DOUBLE:
      oplen = 4;
      break;

    case OP_VAR_VALUE:
    case OP_LAST:
    case OP_REGISTER:
    case OP_INTERNALVAR:
      oplen = 3;
      break;

    case OP_FUNCALL:
      oplen = 3;
      args = 1 + inexpr->elts[inend - 2].longconst;
      break;

    case UNOP_CAST:
    case UNOP_MEMVAL:
      oplen = 3;
      args = 1;
      break;

    case STRUCTOP_STRUCT:
    case STRUCTOP_PTR:
      args = 1;
    case OP_STRING:
      oplen = 3 + ((inexpr->elts[inend - 2].longconst
		    + sizeof (union exp_element))
		   / sizeof (union exp_element));
		   
      break;

    case TERNOP_COND:
      args = 3;
      break;

    case BINOP_ASSIGN_MODIFY:
      oplen = 3;
      args = 2;
      break;

      /* C++ */
    case OP_THIS:
      oplen = 2;
      break;

    default:
      args = 1 + ((int) opcode < (int) BINOP_END);
    }

  /* Copy the final operator itself, from the end of the input
     to the beginning of the output.  */
  inend -= oplen;
  bcopy (&inexpr->elts[inend], &outexpr->elts[outbeg],
	 oplen * sizeof (union exp_element));
  outbeg += oplen;

  /* Find the lengths of the arg subexpressions.  */
  arglens = (int *) alloca (args * sizeof (int));
  for (i = args - 1; i >= 0; i--)
    {
      oplen = length_of_subexp (inexpr, inend);
      arglens[i] = oplen;
      inend -= oplen;
    }

  /* Now copy each subexpression, preserving the order of
     the subexpressions, but prefixifying each one.
     In this loop, inend starts at the beginning of
     the expression this level is working on
     and marches forward over the arguments.
     outbeg does similarly in the output.  */
  for (i = 0; i < args; i++)
    {
      oplen = arglens[i];
      inend += oplen;
      prefixify_subexp (inexpr, outexpr, inend, outbeg);
      outbeg += oplen;
    }
}

/* This page contains the two entry points to this file.  */

/* Read a C expression from the string *STRINGPTR points to,
   parse it, and return a pointer to a  struct expression  that we malloc.
   Use block BLOCK as the lexical context for variable names;
   if BLOCK is zero, use the block of the selected stack frame.
   Meanwhile, advance *STRINGPTR to point after the expression,
   at the first nonwhite character that is not part of the expression
   (possibly a null character).

   If COMMA is nonzero, stop if a comma is reached.  */

struct expression *
parse_c_1 (stringptr, block, comma)
     char **stringptr;
     struct block *block;
{
  struct cleanup *old_chain;

  lexptr = *stringptr;

  paren_depth = 0;
  type_stack_depth = 0;

  comma_terminates = comma;

  if (lexptr == 0 || *lexptr == 0)
    error_no_arg ("expression to compute");

  old_chain = make_cleanup (free_funcalls, 0);
  funcall_chain = 0;

  expression_context_block = block ? block : get_selected_block ();

  namecopy = (char *) alloca (strlen (lexptr) + 1);
  expout_size = 10;
  expout_ptr = 0;
  expout = (struct expression *)
    xmalloc (sizeof (struct expression)
	     + expout_size * sizeof (union exp_element));
  make_cleanup (free_current_contents, &expout);
  if (yyparse ())
    yyerror ();
  discard_cleanups (old_chain);
  expout->nelts = expout_ptr;
  expout = (struct expression *)
    xrealloc (expout,
	      sizeof (struct expression)
	      + expout_ptr * sizeof (union exp_element));
  prefixify_expression (expout);
  *stringptr = lexptr;
  return expout;
}

/* Parse STRING as an expression, and complain if this fails
   to use up all of the contents of STRING.  */

struct expression *
parse_c_expression (string)
     char *string;
{
  register struct expression *exp;
  exp = parse_c_1 (&string, 0, 0);
  if (*string)
    error ("Junk after end of expression.");
  return exp;
}

static void 
push_type (tp)
     enum type_pieces tp;
{
  if (type_stack_depth == type_stack_size)
    {
      type_stack_size *= 2;
      type_stack = (enum type_pieces *)
	xrealloc (type_stack, type_stack_size * sizeof (enum type_pieces));
    }
  type_stack[type_stack_depth++] = tp;
}

static enum type_pieces 
pop_type ()
{
  if (type_stack_depth)
    return type_stack[--type_stack_depth];
  return tp_end;
}

void
_initialize_expread ()
{
  type_stack_size = 80;
  type_stack_depth = 0;
  type_stack = (enum type_pieces *)
    xmalloc (type_stack_size * sizeof (enum type_pieces));
}
short yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
-1, 28,
	270, 60,
	-2, 109,
-1, 145,
	270, 61,
	-2, 62,
	};
# define YYNPROD 110
# define YYLAST 899
short yyact[]={

   7, 148, 147,  90,  89,   5, 171,  13,  84,   4,
  29,  83,   6,  34,  35,  36,  94,  33, 165,  37,
  31,  32,  30,  86,  88,  87, 100, 101,  95, 139,
 133, 173,  39,  45, 158,   7, 175, 134, 158,  43,
   5, 140,  76, 133,   4, 152, 181,   6,  39, 182,
  49,  60,  94,  45, 162,  47,  50, 176,  51,  43,
  48, 172,  91,  92,  95, 164, 138, 106, 136,  81,
   2, 154, 177,  58,  66,  59,  65,  46, 138, 108,
 136, 138,  27, 136,  44, 168,  49,  60, 169,  45,
  12,  47,  50,   8,  51,  43,  48, 151, 138, 164,
 136, 133, 155, 143,  44, 141,  39,  61,  78,  58,
  66,  59,  65,  46, 107, 132,  93, 141, 137,   7,
 133, 142,  24, 179,  18,  12,  13,   1,   8, 141,
   0,   0, 141,   0,  77,   0,  49,  62,   0,  45,
  44,  47,  50,  61,  51,  43,  48, 104,  80, 141,
   0,   0,   0,   0,  99,   0,   0,   0,   0,   0,
  49,  60,   0,  45,   0,  47,  50, 160,  51,  43,
  48,   0,   0,  62, 159,   0, 163,   0,   0, 161,
   0,   0, 166,  58,   0,  59,  65,  46,   0,   0,
  44,   0,   0,   0,   0,  49,  60,   0,  45,   0,
  47,  50,   0,  51,  43,  48,   0,   0,   0,  12,
   0, 131,   8,   0,  44,   0,   0,  61,  58,   0,
  59,   0,  46,   0,  14,  16,  15,  17,  38,  29,
  28,  22,  34,  35,  36,  11,  33,  26,  37,  31,
  32,  30,  19,  20,  21,   0,  23,  62,   0,  44,
   0,   0,  61,  86,  88,  87,   0,   9,  10,  14,
  16,  15,  17,  38,  29,  28,  22,  34,  35,  36,
  11,  33,  26,  37,  31,  32,  30,  19,  20,  21,
   0,  23,  62,  40,  41,  42,  86,  88,  87,   0,
   0,  67,   9,  10,  64,  63,  54,  55,  56,  57,
  52,  53,   0,  40,  41,  42,   0,   0, 144, 178,
   0, 135, 180,   0,   0,  49,   0, 183,  45,   0,
  47,  50,   0,  51,  43,  48,   0,  67,   0,   0,
  64,  63,  54,  55,  56,  57,  52,  53,   0,  40,
  41,  42,  46,  14,  16,  15,  17,  38,  29,  28,
  22,  34,  35,  36,  11,  33,  26,  37,  31,  32,
  30,  19,  20,  21,   0,  23,  86,  88,  87,  44,
  49,   0,   0,  45,   0,  47,   9,  10,   0,  43,
  48,   0,   0,   0,   0,   0,   0,   0,   0,  40,
  41,  42,   0,  49,  60,   0,  45,   0,  47,  50,
   0,  51,  43,  48,  64,  63,  54,  55,  56,  57,
  52,  53,   0,  40,  41,  42,  58,   0,  59,   0,
  46,   0,   0,   0,  44,   0,   0,   0,  49,  60,
   0,  45,   0,  47,  50,   0,  51,  43,  48,   0,
  63,  54,  55,  56,  57,  52,  53,  44,  40,  41,
  42,  58,   0,  59,   0,  46,  49,  60,   0,  45,
   0,  47,  50,   0,  51,  43,  48,  49,   0,   0,
  45,   0,  47,  50,  25,  51,  43,  48,   0,  58,
   0,  59,  44,  46,   0,  61,   0,  79,  82,   0,
  58,   0,  59,  49,  46,   0,  45,   0,  47,  50,
   0,  51,  43,  48,   0,   0,   0,   0,   0,   0,
  44,   0,   0,  61,   0,  62,  58, 146,  59,  49,
  46,  44,  45,   0,  47,  50,   0,  51,  43,  48,
  29,   0,   0,  34,  35,  36,   0,  33,   0,  37,
  31,  32,  30,   0,  85,   0,  46,  44,   0,   0,
   0,  82,  96,  97,  98,   0,   0,   0,   0,   0,
 103, 105,   0,   0,   0,  52,  53,   0,  40,  41,
  42,   0,   0,  44,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0, 145,   0,   0,   0,   0,   0,   0, 157,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,  40,  41,  42,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,  54,
  55,  56,  57,  52,  53,   0,  40,  41,  42,   0,
   0,   0,  79,   0,   0,   0,   0,  79,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,  54,  55,  56,  57,  52,  53,
   0,  40,  41,  42,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,  54,  55,  56,  57,  52,  53,   0,  40,
  41,  42,   0,  54,  55,  56,  57,  52,  53,   0,
  40,  41,  42,   0,   0,   0,   0,   0,   3,   0,
   0,   0,   0,  68,  69,  70,  71,  72,  73,  74,
  75,  56,  57,  52,  53,   0,  40,  41,  42,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0, 102,   0,
   0,   0,  40,  41,  42, 109, 110, 111, 112, 113,
 114, 115, 116, 117, 118, 119, 120, 121, 122, 123,
 124, 125, 126, 127, 128, 129, 130,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0, 149,   0, 150,   0, 153,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0, 156,   0,   0,   0,   0,   0,   0,   0,   0,
   0, 167,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0, 170, 167,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0, 174 };
short yypact[]={

 -33,-1000, -12,  49, -33, -33, -33, -33, -33, -33,
 -33,   2,-252, -33,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-259,-262,-238,-1000,-1000,-1000,
-1000,-270,-271,-210,-238,-238,-238,-246,-1000, -33,
-1000,-1000, 105,  25, -33,-1000, -33, -33, -33, -33,
 -33, -33, -33, -33, -33, -33, -33, -33, -33, -33,
 -33, -33, -33, -33, -33, -33, -33, -33,  -7,  -7,
  -7,  -7,  -7,  -7,  -7,  -7, -33, -10,-1000,  41,
  80,  62,  38,-238,-238,-1000,-1000,-1000,-1000,-1000,
-1000,-272,-273,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,  49,-1000, -33,-1000, -33,   4, -33,  99,
  -7,  -7,  -7, 333, 333, 482, 482, 456, 456, 278,
 278, 278, 278, 430, 356, 419, 391, 158,  13,  49,
  49,  61, -33,-252,-1000,  -4,  26,  14,  58,-1000,
-1000, -75, -33,-1000,  -8,-1000,-1000,-1000,-1000,  -7,
  -7,-1000,  44,  49, -33,  86,  -7,-264,-1000,-1000,
-1000,-1000,  24,  20,-1000,-1000, -62,  -7,-1000, -33,
 123,  -6,-1000,-1000,  49,  16,  32, 268,-1000,   5,
   3,-1000,-252,   3 };
short yypgo[]={

   0, 728,  69, 127, 124, 134, 474, 123, 122,  41,
 118,  37, 108,  29, 517,  82, 116,  79,  45 };
short yyr1[]={

   0,   3,   2,   2,   1,   1,   1,   1,   1,   1,
   1,   1,   1,   1,   1,   1,   1,   1,   1,  17,
   1,  18,  18,  18,   1,   1,   1,   1,   1,   1,
   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
   8,   8,   4,   4,   4,   4,  12,  12,  11,  11,
  11,  10,  10,  10,  10,  10,  13,  13,   9,   5,
   5,   5,   5,   5,   6,   6,   6,   6,   6,   6,
   6,   6,   6,   6,   6,   6,   6,   6,   6,  16,
  16,  16,  16,   7,   7,  14,  14,  14,  15,  15 };
short yyr2[]={

   0,   1,   1,   3,   2,   2,   2,   2,   2,   2,
   2,   2,   2,   2,   3,   4,   3,   4,   4,   0,
   5,   0,   1,   3,   4,   4,   3,   3,   3,   3,
   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,
   3,   3,   3,   3,   3,   3,   5,   3,   3,   1,
   1,   1,   1,   1,   1,   1,   1,   4,   1,   1,
   1,   3,   3,   3,   2,   1,   1,   2,   1,   2,
   1,   3,   2,   1,   2,   1,   2,   3,   2,   1,
   3,   6,   8,   9,   1,   1,   1,   1,   2,   3,
   2,   3,   2,   2,   2,   2,   1,   2,   1,   1,
   1,   1,   1,   1,   3,   1,   1,   1,   1,   1 };
short yychk[]={

-1000,  -3,  -2,  -1,  42,  38,  45,  33, 126, 290,
 291, 268, 123,  40, 257, 259, 258, 260,  -4, 275,
 276, 277, 264, 279,  -8,  -6, 270, -15, 263, 262,
 274, 272, 273, 269, 265, 266, 267, 271, 261,  44,
 290, 291, 292,  46,  91,  40,  64,  42,  47,  37,
  43,  45, 287, 288, 283, 284, 285, 286,  60,  62,
  38,  94, 124, 282, 281,  63,  61, 278,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  40,  -5, -12,  -6,
  -5,  -2,  -6, 270, 270, -14, 261, 263, 262, 274,
 274, 272, 273, -16, 262, 274, -14, -14, -14, -16,
 272, 273,  -1, -14,  42, -14,  42,  -2, -17,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -5, 125,  40, -11, 270,  42, -10,  40, -13,
  -9,  91,  41,  41, 270, -14, -14, 274, 274,  -1,
  -1,  93, -18,  -1,  58,  41,  -1,  -6,  42, -11,
 -13,  -9,  40, -11,  41,  93, 257,  -1,  41,  44,
  -1, 270,  41,  93,  -1,  42,  41,  40,  41,  -7,
  -5,  41,  44,  -5 };
short yydef[]={

   0,  -2,   1,   2,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,  49,  50,  51,  52,  53,  54,
  55,  56,  58,  59,   0,   0,   0,  65,  -2,  84,
  85,  86,  87,  96,   0,   0,   0,  98, 108,   0,
  11,  12,   0,   0,   0,  19,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   4,   5,
   6,   7,   8,   9,  10,  13,   0,   0,  79,  66,
   0,   0,  66,   0,   0,  64, 105, 106, 107,  88,
  90, 101, 102,  95,  99, 100,  92,  93,  94,  97,
 101, 102,   3,  14,   0,  16,   0,   0,  21,  27,
  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,
  38,  39,  40,  41,  42,  43,  44,  45,   0,  47,
  48,   0,   0,   0,  67,   0,  68,  70,   0,  73,
  75,   0,   0,  26,   0,  -2,  63,  89,  91,  15,
  17,  18,   0,  22,   0,  57,  24,   0,  80,  69,
  72,  74,   0,   0,  78,  76,   0,  25,  20,   0,
  46,   0,  71,  77,  23,   0,  81,   0,  82,   0,
 103,  83,   0, 104 };
#ifndef lint
static char yaccpar_sccsid[] = "@(#)yaccpar	4.1	(Berkeley)	2/11/83";
#endif not lint

#
# define YYFLAG -1000
# define YYERROR goto yyerrlab
# define YYACCEPT return(0)
# define YYABORT return(1)

/*	parser for yacc output	*/

#ifdef YYDEBUG
int yydebug = 0; /* 1 for debugging */
#endif
YYSTYPE yyv[YYMAXDEPTH]; /* where the values are stored */
int yychar = -1; /* current input token number */
int yynerrs = 0;  /* number of errors */
short yyerrflag = 0;  /* error recovery flag */

yyparse() {

	short yys[YYMAXDEPTH];
	short yyj, yym;
	register YYSTYPE *yypvt;
	register short yystate, *yyps, yyn;
	register YYSTYPE *yypv;
	register short *yyxi;

	yystate = 0;
	yychar = -1;
	yynerrs = 0;
	yyerrflag = 0;
	yyps= &yys[-1];
	yypv= &yyv[-1];

 yystack:    /* put a state and value onto the stack */

#ifdef YYDEBUG
	if( yydebug  ) printf( "state %d, char 0%o\n", yystate, yychar );
#endif
		if( ++yyps> &yys[YYMAXDEPTH] ) { yyerror( "yacc stack overflow" ); return(1); }
		*yyps = yystate;
		++yypv;
		*yypv = yyval;

 yynewstate:

	yyn = yypact[yystate];

	if( yyn<= YYFLAG ) goto yydefault; /* simple state */

	if( yychar<0 ) if( (yychar=yylex())<0 ) yychar=0;
	if( (yyn += yychar)<0 || yyn >= YYLAST ) goto yydefault;

	if( yychk[ yyn=yyact[ yyn ] ] == yychar ){ /* valid shift */
		yychar = -1;
		yyval = yylval;
		yystate = yyn;
		if( yyerrflag > 0 ) --yyerrflag;
		goto yystack;
		}

 yydefault:
	/* default state action */

	if( (yyn=yydef[yystate]) == -2 ) {
		if( yychar<0 ) if( (yychar=yylex())<0 ) yychar = 0;
		/* look through exception table */

		for( yyxi=yyexca; (*yyxi!= (-1)) || (yyxi[1]!=yystate) ; yyxi += 2 ) ; /* VOID */

		while( *(yyxi+=2) >= 0 ){
			if( *yyxi == yychar ) break;
			}
		if( (yyn = yyxi[1]) < 0 ) return(0);   /* accept */
		}

	if( yyn == 0 ){ /* error */
		/* error ... attempt to resume parsing */

		switch( yyerrflag ){

		case 0:   /* brand new error */

			yyerror( "syntax error" );
		yyerrlab:
			++yynerrs;

		case 1:
		case 2: /* incompletely recovered error ... try again */

			yyerrflag = 3;

			/* find a state where "error" is a legal shift action */

			while ( yyps >= yys ) {
			   yyn = yypact[*yyps] + YYERRCODE;
			   if( yyn>= 0 && yyn < YYLAST && yychk[yyact[yyn]] == YYERRCODE ){
			      yystate = yyact[yyn];  /* simulate a shift of "error" */
			      goto yystack;
			      }
			   yyn = yypact[*yyps];

			   /* the current yyps has no shift onn "error", pop stack */

#ifdef YYDEBUG
			   if( yydebug ) printf( "error recovery pops state %d, uncovers %d\n", *yyps, yyps[-1] );
#endif
			   --yyps;
			   --yypv;
			   }

			/* there is no state on the stack with an error shift ... abort */

	yyabort:
			return(1);


		case 3:  /* no shift yet; clobber input char */

#ifdef YYDEBUG
			if( yydebug ) printf( "error recovery discards char %d\n", yychar );
#endif

			if( yychar == 0 ) goto yyabort; /* don't discard EOF, quit */
			yychar = -1;
			goto yynewstate;   /* try again in the same state */

			}

		}

	/* reduction by production yyn */

#ifdef YYDEBUG
		if( yydebug ) printf("reduce %d\n",yyn);
#endif
		yyps -= yyr2[yyn];
		yypvt = yypv;
		yypv -= yyr2[yyn];
		yyval = yypv[1];
		yym=yyn;
			/* consult goto table to find next state */
		yyn = yyr1[yyn];
		yyj = yypgo[yyn] + *yyps + 1;
		if( yyj>=YYLAST || yychk[ yystate = yyact[yyj] ] != -yyn ) yystate = yyact[yypgo[yyn]];
		switch(yym){
			
case 3:
# line 196 "expread.y"
{ write_exp_elt_opcode (BINOP_COMMA); } break;
case 4:
# line 201 "expread.y"
{ write_exp_elt_opcode (UNOP_IND); } break;
case 5:
# line 204 "expread.y"
{ write_exp_elt_opcode (UNOP_ADDR); } break;
case 6:
# line 207 "expread.y"
{ write_exp_elt_opcode (UNOP_NEG); } break;
case 7:
# line 211 "expread.y"
{ write_exp_elt_opcode (UNOP_ZEROP); } break;
case 8:
# line 215 "expread.y"
{ write_exp_elt_opcode (UNOP_LOGNOT); } break;
case 9:
# line 219 "expread.y"
{ write_exp_elt_opcode (UNOP_PREINCREMENT); } break;
case 10:
# line 223 "expread.y"
{ write_exp_elt_opcode (UNOP_PREDECREMENT); } break;
case 11:
# line 227 "expread.y"
{ write_exp_elt_opcode (UNOP_POSTINCREMENT); } break;
case 12:
# line 231 "expread.y"
{ write_exp_elt_opcode (UNOP_POSTDECREMENT); } break;
case 13:
# line 235 "expread.y"
{ write_exp_elt_opcode (UNOP_SIZEOF); } break;
case 14:
# line 239 "expread.y"
{ write_exp_elt_opcode (STRUCTOP_PTR);
			  write_exp_string (yypvt[-0].sval);
			  write_exp_elt_opcode (STRUCTOP_PTR); } break;
case 15:
# line 245 "expread.y"
{ write_exp_elt_opcode (STRUCTOP_MPTR); } break;
case 16:
# line 249 "expread.y"
{ write_exp_elt_opcode (STRUCTOP_STRUCT);
			  write_exp_string (yypvt[-0].sval);
			  write_exp_elt_opcode (STRUCTOP_STRUCT); } break;
case 17:
# line 255 "expread.y"
{ write_exp_elt_opcode (STRUCTOP_MEMBER); } break;
case 18:
# line 259 "expread.y"
{ write_exp_elt_opcode (BINOP_SUBSCRIPT); } break;
case 19:
# line 265 "expread.y"
{ start_arglist (); } break;
case 20:
# line 267 "expread.y"
{ write_exp_elt_opcode (OP_FUNCALL);
			  write_exp_elt_longcst ((LONGEST) end_arglist ());
			  write_exp_elt_opcode (OP_FUNCALL); } break;
case 22:
# line 276 "expread.y"
{ arglist_len = 1; } break;
case 23:
# line 280 "expread.y"
{ arglist_len++; } break;
case 24:
# line 284 "expread.y"
{ write_exp_elt_opcode (UNOP_MEMVAL);
			  write_exp_elt_type (yypvt[-2].tval);
			  write_exp_elt_opcode (UNOP_MEMVAL); } break;
case 25:
# line 290 "expread.y"
{ write_exp_elt_opcode (UNOP_CAST);
			  write_exp_elt_type (yypvt[-2].tval);
			  write_exp_elt_opcode (UNOP_CAST); } break;
case 26:
# line 296 "expread.y"
{ } break;
case 27:
# line 302 "expread.y"
{ write_exp_elt_opcode (BINOP_REPEAT); } break;
case 28:
# line 306 "expread.y"
{ write_exp_elt_opcode (BINOP_MUL); } break;
case 29:
# line 310 "expread.y"
{ write_exp_elt_opcode (BINOP_DIV); } break;
case 30:
# line 314 "expread.y"
{ write_exp_elt_opcode (BINOP_REM); } break;
case 31:
# line 318 "expread.y"
{ write_exp_elt_opcode (BINOP_ADD); } break;
case 32:
# line 322 "expread.y"
{ write_exp_elt_opcode (BINOP_SUB); } break;
case 33:
# line 326 "expread.y"
{ write_exp_elt_opcode (BINOP_LSH); } break;
case 34:
# line 330 "expread.y"
{ write_exp_elt_opcode (BINOP_RSH); } break;
case 35:
# line 334 "expread.y"
{ write_exp_elt_opcode (BINOP_EQUAL); } break;
case 36:
# line 338 "expread.y"
{ write_exp_elt_opcode (BINOP_NOTEQUAL); } break;
case 37:
# line 342 "expread.y"
{ write_exp_elt_opcode (BINOP_LEQ); } break;
case 38:
# line 346 "expread.y"
{ write_exp_elt_opcode (BINOP_GEQ); } break;
case 39:
# line 350 "expread.y"
{ write_exp_elt_opcode (BINOP_LESS); } break;
case 40:
# line 354 "expread.y"
{ write_exp_elt_opcode (BINOP_GTR); } break;
case 41:
# line 358 "expread.y"
{ write_exp_elt_opcode (BINOP_LOGAND); } break;
case 42:
# line 362 "expread.y"
{ write_exp_elt_opcode (BINOP_LOGXOR); } break;
case 43:
# line 366 "expread.y"
{ write_exp_elt_opcode (BINOP_LOGIOR); } break;
case 44:
# line 370 "expread.y"
{ write_exp_elt_opcode (BINOP_AND); } break;
case 45:
# line 374 "expread.y"
{ write_exp_elt_opcode (BINOP_OR); } break;
case 46:
# line 378 "expread.y"
{ write_exp_elt_opcode (TERNOP_COND); } break;
case 47:
# line 382 "expread.y"
{ write_exp_elt_opcode (BINOP_ASSIGN); } break;
case 48:
# line 386 "expread.y"
{ write_exp_elt_opcode (BINOP_ASSIGN_MODIFY);
			  write_exp_elt_opcode (yypvt[-1].opcode);
			  write_exp_elt_opcode (BINOP_ASSIGN_MODIFY); } break;
case 49:
# line 392 "expread.y"
{ write_exp_elt_opcode (OP_LONG);
			  if (yypvt[-0].lval == (int) yypvt[-0].lval || yypvt[-0].lval == (unsigned int) yypvt[-0].lval)
			    write_exp_elt_type (builtin_type_int);
			  else
			    write_exp_elt_type (BUILTIN_TYPE_LONGEST);
			  write_exp_elt_longcst ((LONGEST) yypvt[-0].lval);
			  write_exp_elt_opcode (OP_LONG); } break;
case 50:
# line 402 "expread.y"
{
			  write_exp_elt_opcode (OP_LONG);
			  if (yypvt[-0].ulval == (unsigned int) yypvt[-0].ulval)
			    write_exp_elt_type (builtin_type_unsigned_int);
			  else
			    write_exp_elt_type (BUILTIN_TYPE_UNSIGNED_LONGEST);
			  write_exp_elt_longcst ((LONGEST) yypvt[-0].ulval);
			  write_exp_elt_opcode (OP_LONG);
			} break;
case 51:
# line 414 "expread.y"
{ write_exp_elt_opcode (OP_LONG);
			  write_exp_elt_type (builtin_type_char);
			  write_exp_elt_longcst ((LONGEST) yypvt[-0].lval);
			  write_exp_elt_opcode (OP_LONG); } break;
case 52:
# line 421 "expread.y"
{ write_exp_elt_opcode (OP_DOUBLE);
			  write_exp_elt_type (builtin_type_double);
			  write_exp_elt_dblcst (yypvt[-0].dval);
			  write_exp_elt_opcode (OP_DOUBLE); } break;
case 54:
# line 431 "expread.y"
{ write_exp_elt_opcode (OP_LAST);
			  write_exp_elt_longcst ((LONGEST) yypvt[-0].lval);
			  write_exp_elt_opcode (OP_LAST); } break;
case 55:
# line 437 "expread.y"
{ write_exp_elt_opcode (OP_REGISTER);
			  write_exp_elt_longcst ((LONGEST) yypvt[-0].lval);
			  write_exp_elt_opcode (OP_REGISTER); } break;
case 56:
# line 443 "expread.y"
{ write_exp_elt_opcode (OP_INTERNALVAR);
			  write_exp_elt_intern (yypvt[-0].ivar);
			  write_exp_elt_opcode (OP_INTERNALVAR); } break;
case 57:
# line 449 "expread.y"
{ write_exp_elt_opcode (OP_LONG);
			  write_exp_elt_type (builtin_type_int);
			  write_exp_elt_longcst ((LONGEST) TYPE_LENGTH (yypvt[-1].tval));
			  write_exp_elt_opcode (OP_LONG); } break;
case 58:
# line 456 "expread.y"
{ write_exp_elt_opcode (OP_STRING);
			  write_exp_string (yypvt[-0].sval);
			  write_exp_elt_opcode (OP_STRING); } break;
case 59:
# line 463 "expread.y"
{ write_exp_elt_opcode (OP_THIS);
			  write_exp_elt_opcode (OP_THIS); } break;
case 60:
# line 470 "expread.y"
{
			  struct symtab *tem = lookup_symtab (copy_name (yypvt[-0].sval));
			  struct symbol *sym;
			  
			  if (tem)
			    yyval.bval = BLOCKVECTOR_BLOCK (BLOCKVECTOR (tem), 1);
			  else
			    {
			      sym = lookup_symbol (copy_name (yypvt[-0].sval),
						   expression_context_block,
						   VAR_NAMESPACE, 0);
			      if (sym && SYMBOL_CLASS (sym) == LOC_BLOCK)
				yyval.bval = SYMBOL_BLOCK_VALUE (sym);
			      else
				error ("No file or function \"%s\".",
				       copy_name (yypvt[-0].sval));
			    }
			} break;
case 61:
# line 491 "expread.y"
{ struct symbol *tem
			    = lookup_symbol (copy_name (yypvt[-0].sval), yypvt[-2].bval, VAR_NAMESPACE, 0);
			  if (!tem || SYMBOL_CLASS (tem) != LOC_BLOCK)
			    error ("No function \"%s\" in specified context.",
				   copy_name (yypvt[-0].sval));
			  yyval.bval = SYMBOL_BLOCK_VALUE (tem); } break;
case 62:
# line 500 "expread.y"
{ struct symbol *sym;
			  sym = lookup_symbol (copy_name (yypvt[-0].sval), yypvt[-2].bval, VAR_NAMESPACE, 0);
			  if (sym == 0)
			    error ("No symbol \"%s\" in specified context.",
				   copy_name (yypvt[-0].sval));
			  write_exp_elt_opcode (OP_VAR_VALUE);
			  write_exp_elt_sym (sym);
			  write_exp_elt_opcode (OP_VAR_VALUE); } break;
case 63:
# line 511 "expread.y"
{
			  struct type *type = yypvt[-2].tval;
			  if (TYPE_CODE (type) != TYPE_CODE_STRUCT
			      && TYPE_CODE (type) != TYPE_CODE_UNION)
			    error ("`%s' is not defined as an aggregate type.",
				   TYPE_NAME (type));

			  write_exp_elt_opcode (OP_SCOPE);
			  write_exp_elt_type (type);
			  write_exp_string (yypvt[-0].sval);
			  write_exp_elt_opcode (OP_SCOPE);
			} break;
case 64:
# line 524 "expread.y"
{
			  char *name = copy_name (yypvt[-0].sval);
			  struct symbol *sym;
			  int i;

			  sym = lookup_symbol (name, 0, VAR_NAMESPACE, 0);
			  if (sym)
			    {
			      write_exp_elt_opcode (OP_VAR_VALUE);
			      write_exp_elt_sym (sym);
			      write_exp_elt_opcode (OP_VAR_VALUE);
			      break;
			    }
			  for (i = 0; i < misc_function_count; i++)
			    if (!strcmp (misc_function_vector[i].name, name))
			      break;

			  if (i < misc_function_count)
			    {
			      enum misc_function_type mft =
				(enum misc_function_type)
				  misc_function_vector[i].type;
			      
			      write_exp_elt_opcode (OP_LONG);
			      write_exp_elt_type (builtin_type_int);
			      write_exp_elt_longcst ((LONGEST) misc_function_vector[i].address);
			      write_exp_elt_opcode (OP_LONG);
			      write_exp_elt_opcode (UNOP_MEMVAL);
			      if (mft == mf_data || mft == mf_bss)
				write_exp_elt_type (builtin_type_int);
			      else if (mft == mf_text)
				write_exp_elt_type (lookup_function_type (builtin_type_int));
			      else
				write_exp_elt_type (builtin_type_char);
			      write_exp_elt_opcode (UNOP_MEMVAL);
			    }
			  else
			    if (symtab_list == 0
				&& partial_symtab_list == 0)
			      error ("No symbol table is loaded.  Use the \"symbol-file\" command.");
			    else
			      error ("No symbol \"%s\" in current context.", name);
			} break;
case 65:
# line 570 "expread.y"
{ struct symbol *sym;
			  int is_a_field_of_this;

			  sym = lookup_symbol (copy_name (yypvt[-0].sval),
					       expression_context_block,
					       VAR_NAMESPACE,
					       &is_a_field_of_this);
			  if (sym)
			    {
			      switch (sym->class)
				{
				case LOC_REGISTER:
				case LOC_ARG:
				case LOC_LOCAL:
				  if (innermost_block == 0 ||
				      contained_in (block_found, 
						    innermost_block))
				    innermost_block = block_found;
				}
			      write_exp_elt_opcode (OP_VAR_VALUE);
			      write_exp_elt_sym (sym);
			      write_exp_elt_opcode (OP_VAR_VALUE);
			    }
			  else if (is_a_field_of_this)
			    {
			      /* C++: it hangs off of `this'.  Must
			         not inadvertently convert from a method call
				 to data ref.  */
			      if (innermost_block == 0 || 
				  contained_in (block_found, innermost_block))
				innermost_block = block_found;
			      write_exp_elt_opcode (OP_THIS);
			      write_exp_elt_opcode (OP_THIS);
			      write_exp_elt_opcode (STRUCTOP_PTR);
			      write_exp_string (yypvt[-0].sval);
			      write_exp_elt_opcode (STRUCTOP_PTR);
			    }
			  else
			    {
			      register int i;
			      register char *arg = copy_name (yypvt[-0].sval);

			      for (i = 0; i < misc_function_count; i++)
				if (!strcmp (misc_function_vector[i].name, arg))
				  break;

			      if (i < misc_function_count)
				{
				  enum misc_function_type mft =
				    (enum misc_function_type)
				      misc_function_vector[i].type;
				  
				  write_exp_elt_opcode (OP_LONG);
				  write_exp_elt_type (builtin_type_int);
				  write_exp_elt_longcst ((LONGEST) misc_function_vector[i].address);
				  write_exp_elt_opcode (OP_LONG);
				  write_exp_elt_opcode (UNOP_MEMVAL);
				  if (mft == mf_data || mft == mf_bss)
				    write_exp_elt_type (builtin_type_int);
				  else if (mft == mf_text)
				    write_exp_elt_type (lookup_function_type (builtin_type_int));
				  else
				    write_exp_elt_type (builtin_type_char);
				  write_exp_elt_opcode (UNOP_MEMVAL);
				}
			      else if (symtab_list == 0
				       && partial_symtab_list == 0)
				error ("No symbol table is loaded.  Use the \"symbol-file\" command.");
			      else
				error ("No symbol \"%s\" in current context.",
				       copy_name (yypvt[-0].sval));
			    }
			} break;
case 67:
# line 648 "expread.y"
{
		  /* This is where the interesting stuff happens.  */
		  int done = 0;
		  int array_size;
		  struct type *follow_type = yypvt[-1].tval;
		  
		  while (!done)
		    switch (pop_type ())
		      {
		      case tp_end:
			done = 1;
			break;
		      case tp_pointer:
			follow_type = lookup_pointer_type (follow_type);
			break;
		      case tp_reference:
			follow_type = lookup_reference_type (follow_type);
			break;
		      case tp_array:
			array_size = (int) pop_type ();
			if (array_size != -1)
			  follow_type = create_array_type (follow_type,
							   array_size);
			else
			  follow_type = lookup_pointer_type (follow_type);
			break;
		      case tp_function:
			follow_type = lookup_function_type (follow_type);
			break;
		      }
		  yyval.tval = follow_type;
		} break;
case 68:
# line 683 "expread.y"
{ push_type (tp_pointer); yyval.voidval = 0; } break;
case 69:
# line 685 "expread.y"
{ push_type (tp_pointer); yyval.voidval = yypvt[-0].voidval; } break;
case 71:
# line 690 "expread.y"
{ yyval.voidval = yypvt[-1].voidval; } break;
case 72:
# line 692 "expread.y"
{
			  push_type ((enum type_pieces) yypvt[-0].lval);
			  push_type (tp_array);
			} break;
case 73:
# line 697 "expread.y"
{
			  push_type ((enum type_pieces) yypvt[-0].lval);
			  push_type (tp_array);
			  yyval.voidval = 0;
			} break;
case 74:
# line 703 "expread.y"
{ push_type (tp_function); } break;
case 75:
# line 705 "expread.y"
{ push_type (tp_function); } break;
case 76:
# line 709 "expread.y"
{ yyval.lval = -1; } break;
case 77:
# line 711 "expread.y"
{ yyval.lval = yypvt[-1].lval; } break;
case 78:
# line 715 "expread.y"
{ yyval.voidval = 0; } break;
case 80:
# line 720 "expread.y"
{ yyval.tval = lookup_member_type (builtin_type_int, yypvt[-2].tval); } break;
case 81:
# line 722 "expread.y"
{ yyval.tval = lookup_member_type (yypvt[-5].tval, yypvt[-3].tval); } break;
case 82:
# line 724 "expread.y"
{ yyval.tval = lookup_member_type
			    (lookup_function_type (yypvt[-7].tval), yypvt[-5].tval); } break;
case 83:
# line 727 "expread.y"
{ yyval.tval = lookup_member_type
			    (lookup_function_type (yypvt[-8].tval), yypvt[-6].tval);
			  free (yypvt[-1].tvec); } break;
case 84:
# line 734 "expread.y"
{ yyval.tval = lookup_typename (copy_name (yypvt[-0].sval),
						expression_context_block, 0); } break;
case 85:
# line 737 "expread.y"
{ yyval.tval = builtin_type_int; } break;
case 86:
# line 739 "expread.y"
{ yyval.tval = builtin_type_long; } break;
case 87:
# line 741 "expread.y"
{ yyval.tval = builtin_type_short; } break;
case 88:
# line 743 "expread.y"
{ yyval.tval = builtin_type_long; } break;
case 89:
# line 745 "expread.y"
{ yyval.tval = builtin_type_unsigned_long; } break;
case 90:
# line 747 "expread.y"
{ yyval.tval = builtin_type_short; } break;
case 91:
# line 749 "expread.y"
{ yyval.tval = builtin_type_unsigned_short; } break;
case 92:
# line 751 "expread.y"
{ yyval.tval = lookup_struct (copy_name (yypvt[-0].sval),
					      expression_context_block); } break;
case 93:
# line 754 "expread.y"
{ yyval.tval = lookup_union (copy_name (yypvt[-0].sval),
					     expression_context_block); } break;
case 94:
# line 757 "expread.y"
{ yyval.tval = lookup_enum (copy_name (yypvt[-0].sval),
					    expression_context_block); } break;
case 95:
# line 760 "expread.y"
{ yyval.tval = lookup_unsigned_typename (copy_name (yypvt[-0].sval)); } break;
case 96:
# line 762 "expread.y"
{ yyval.tval = builtin_type_unsigned_int; } break;
case 97:
# line 764 "expread.y"
{ yyval.tval = lookup_typename (copy_name (yypvt[-0].sval),
						expression_context_block, 0); } break;
case 98:
# line 767 "expread.y"
{ yyval.tval = builtin_type_int; } break;
case 100:
# line 772 "expread.y"
{
		  yyval.sval.ptr = "int";
		  yyval.sval.length = 3;
		} break;
case 101:
# line 777 "expread.y"
{
		  yyval.sval.ptr = "long";
		  yyval.sval.length = 4;
		} break;
case 102:
# line 782 "expread.y"
{
		  yyval.sval.ptr = "short";
		  yyval.sval.length = 5;
		} break;
case 103:
# line 790 "expread.y"
{ yyval.tvec = (struct type **)xmalloc (sizeof (struct type *) * 2);
		  yyval.tvec[0] = (struct type *)0;
		  yyval.tvec[1] = yypvt[-0].tval;
		} break;
case 104:
# line 795 "expread.y"
{ int len = sizeof (struct type *) * ++(yypvt[-2].ivec[0]);
		  yyval.tvec = (struct type **)xrealloc (yypvt[-2].tvec, len);
		  yyval.tvec[yyval.ivec[0]] = yypvt[-0].tval;
		} break;
		}
		goto yystack;  /* stack new state and value */

	}
