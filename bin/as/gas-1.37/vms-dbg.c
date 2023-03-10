#include <stdio.h>
#include "as.h"
#include "struc-symbol.h"
#include "symbols.h"
#include "objrecdef.h"
#include <stab.h>

/* This file contains many of the routines needed to output debugging info into
 * the object file that the VMS debugger needs to understand symbols.  These
 * routines are called very late in the assembly process, and thus we can be
 * fairly lax about changing things, since the GSD and the TIR sections have
 * already been output.
 */

/* We need this info to cross correlate between the stabs def for a symbol and
 * the actual symbol def.  The actual symbol def contains the psect number and
 * offset, which is needed to declare a variable to the debugger for global
 * and static variables
 */
struct VMS_Symbol {
	struct VMS_Symbol *Next;
	struct symbol *Symbol;
	int Size;
	int Psect_Index;
	int Psect_Offset;
	};
extern struct VMS_Symbol *VMS_Symbols;

enum advanced_type {BASIC,POINTER,ARRAY,ENUM,STRUCT,UNION,FUNCTION,VOID,UNKNOWN};

/* this structure contains the information from the stabs directives, and the
 * information is filled in by VMS_typedef_parse.  Everything that is needed
 * to generate the debugging record for a given symbol is present here.
 * This could be done more efficiently, using nested struct/unions, but for now
 * I am happy that it works.
 */
struct VMS_DBG_Symbol{
	struct VMS_DBG_Symbol * next;
	enum advanced_type advanced;	/* description of what this is */
	int dbx_type;	/* this record is for this type */
	int type2;	/* For advanced types this is the type referred to.
				i.e. the type a pointer points to, or the type
				of object that makes up an array */
	int VMS_type;	/* Use this type when generating a variable def */
	int index_min;	/* used for arrays - this will be present for all */
	int index_max;	/* entries, but will be meaningless for non-arrays */
	int data_size;	/* size in bytes of the data type.  For an array, this
			   is the size of one element in the array */
	int struc_numb; /* Number of the structure/union/enum - used for ref */
};

struct VMS_DBG_Symbol *VMS_Symbol_type_list={(struct VMS_DBG_Symbol*) NULL};

/* we need this structure to keep track of forward references to
 * struct/union/enum that have not been defined yet.  When they are ultimately
 * defined, then we can go back and generate the TIR commands to make a back
 * reference.
 */

struct forward_ref{
	struct forward_ref * next;
	int dbx_type;
	int struc_numb;
	char resolved;
	};

struct forward_ref * f_ref_root={(struct forward_ref*) NULL};

static char * symbol_name;
static structure_count=0;

/* this routine converts a number string into an integer, and stops when it
 * sees an invalid character the return value is the address of the character 
 * just past the last character read.  No error is generated.
 */
static char * cvt_integer(char* str,int * rtn){
	int ival, neg;
		neg = *str == '-' ? ++str, -1 : 1;
		ival=0;	/* first get the number of the type for dbx */
		while((*str <= '9') && (*str >= '0'))
			ival = 10*ival + *str++ -'0';
	*rtn = neg*ival;
	return str;
}

/* this routine fixes the names that are generated by C++, ".this" is a good
 * example.  The period does not work for the debugger, since it looks like
 * the syntax for a structure element, and thus it gets mightily confused
 */
static fix_name(char* pnt){
	for( ;*pnt != 0; pnt++){
	if(*pnt == '.') *pnt = '$';
	};
}

/* this routine is used to compare the names of certain types to various
 * fixed types that are known by the debugger.
 */
#define type_check(x)  !strcmp( symbol_name , x )

/* When defining a structure, this routine is called to find the name of
 * the actual structure.  It is assumed that str points to the equal sign
 * in the definition, and it moves backward until it finds the start of the
 * name.  If it finds a 0, then it knows that this structure def is in the
 * outermost level, and thus symbol_name points to the symbol name.
 */
static char* get_struct_name(char* str){
	char* pnt;
	pnt=str;
	while((*pnt != ':') && (*pnt != '\0')) pnt--;
	if(*pnt == '\0') return symbol_name;
	*pnt-- = '\0';
	while((*pnt != ';') && (*pnt != '=')) pnt--;
	if(*pnt == ';') return pnt+1;
	while((*pnt < '0') || (*pnt > '9')) pnt++;
	while((*pnt >= '0') && (*pnt <= '9')) pnt++;
	return pnt;
}	
/* search symbol list for type number dbx_type.  Return a pointer to struct */
static struct VMS_DBG_Symbol* find_symbol(int dbx_type){
	struct VMS_DBG_Symbol* spnt;
	spnt=VMS_Symbol_type_list;
	while (spnt!=(struct VMS_DBG_Symbol*) NULL){
		if(spnt->dbx_type==dbx_type) break;
		spnt=spnt->next;};
	if(spnt==(struct VMS_DBG_Symbol*) NULL) return 0;/*Dunno what this is*/
	return spnt;
}


/* Many good programmers cringe when they see a fixed size array - since I am
 * using this to generate the various descriptors for the data types present,
 * you might argue that the descriptor could overflow the array for a
 * complicated variable, and then I am in deep doo-doo.  My answer to this is
 * that the debugger records that we write have all sorts of length bytes
 * stored in them all over the place, and if we exceed 127 bytes (since the top
 * bit indicates data, rather than a command), we are dead anyhow.  So I figure
 * why not do this the easy way.  Besides, to get 128 bytes, you need something
 * like an array with 10 indicies, or something like
 *       char **************************************** var; 
 * Lets get real.  If some idiot writes programs like that he/she gets what
 * they deserve.  (It is possible to overflow the record with a somewhat
 * simpler example, like: int (*(*(*(*(*(* sarr6)[1])[1])[2])[3])[4])[5];
 * but still...).  And if someone in the peanut gallery wants to know "What
 * does VAX-C do with something like this?", I will tell you.  It crashes.
 * At least this code has the good sense to convert it to *void.
 * In practice, I do not think that this presents too much of a problem, since
 * struct/union/enum all use defined types, which sort of terminate the
 * definition.  It occurs to me that we could possibly do the same thing with
 * arrays and pointers, but I don't know quite how it would be coded.
 *
 * And now back to the regularly scheduled program...
 */
#define MAX_DEBUG_RECORD 128
static char Local[MAX_DEBUG_RECORD]; /* buffer for variable descriptor */
static int Lpnt;		/* index into Local */
static char Asuffix[MAX_DEBUG_RECORD]; /* buffer for array descriptor */
static int Apoint;	/* index into Asuffix */
static char overflow;	/* flag to indicate we have written too much*/
static int total_len;	/* used to calculate the total length of variable
			descriptor plus array descriptor - used for len byte*/
static int struct_number;	/* counter used to assign indexes to struct
					unions and enums */

/* this routine puts info into either Local or Asuffix, depending on the sign
 * of size.  The reason is that it is easier to build the variable descriptor
 * backwards, while the array descriptor is best built forwards.  In the end
 * they get put together, if there is not a struct/union/enum along the way
 */
push(int value, int size){
	char * pnt;
	int i;
	int size1;
	long int val;
	val=value;
	pnt=(char*) &val;
	size1 = size;
	if (size < 0) {size1 = -size; pnt += size1-1;};
	if(size < 0)
	for(i=0;i<size1;i++) {
		Local[Lpnt--] = *pnt--;
		if(Lpnt < 0) {overflow = 1; Lpnt = 1;};}
	else for(i=0;i<size1;i++){
		 Asuffix[Apoint++] = *pnt++;
		 if(Apoint >= MAX_DEBUG_RECORD) 
			{overflow = 1; Apoint =MAX_DEBUG_RECORD-1;};}
}

/* this routine generates the array descriptor for a given array */
static array_suffix(struct VMS_DBG_Symbol* spnt2){
	struct VMS_DBG_Symbol * spnt;
	struct VMS_DBG_Symbol * spnt1;
	int rank;
	int total_size;
	int i;
	rank=0;
	spnt=spnt2;
	while(spnt->advanced != ARRAY) {
		spnt=find_symbol(spnt->type2);
		if(spnt == (struct VMS_DBG_Symbol *) NULL) return;};
	spnt1=spnt;
	spnt1=spnt;
	total_size= 1;
	while(spnt1->advanced == ARRAY) {rank++;
		total_size *= (spnt1->index_max - spnt1->index_min +1);
		spnt1=find_symbol(spnt1->type2);};
	total_size = total_size * spnt1->data_size;
	push(spnt1->data_size,2);
	if(spnt1->VMS_type == 0xa3) push(0,1);
			else push(spnt1->VMS_type,1);
	push(4,1);
	for(i=0;i<6;i++) push(0,1);
	push(0xc0,1);
	push(rank,1);
	push(total_size,4);
	push(0,4);
	spnt1=spnt;
	while(spnt1->advanced == ARRAY) {
		push(spnt1->index_max - spnt1->index_min+1,4);
		spnt1=find_symbol(spnt1->type2);};
	spnt1=spnt;
	while(spnt1->advanced == ARRAY) {
		push(spnt1->index_min,4);
		push(spnt1->index_max,4);
		spnt1=find_symbol(spnt1->type2);};
}

/* this routine generates the start of a variable descriptor based upon
 * a struct/union/enum that has yet to be defined.  We define this spot as
 * a new location, and save four bytes for the address.  When the struct is
 * finally defined, then we can go back and plug in the correct address
*/
static new_forward_ref(int dbx_type){
	struct forward_ref* fpnt;
	fpnt = (struct forward_ref*) malloc(sizeof(struct forward_ref));
	fpnt->next = f_ref_root;
	f_ref_root = fpnt;
	fpnt->dbx_type = dbx_type;
	fpnt->struc_numb = ++structure_count;
	fpnt->resolved = 'N';
	push(3,-1);
	total_len = 5;
	push(total_len,-2);
	struct_number = - fpnt->struc_numb;
}

/* this routine generates the variable descriptor used to describe non-basic
 * variables.  It calls itself recursively until it gets to the bottom of it
 * all, and then builds the descriptor backwards.  It is easiest to do it this
 *way since we must periodically write length bytes, and it is easiest if we know
 *the value when it is time to write it.
 */
static int gen1(struct VMS_DBG_Symbol * spnt,int array_suffix_len){
	struct VMS_DBG_Symbol * spnt1;
	int i;
	switch(spnt->advanced){
	case VOID:
		push(DBG$C_VOID,-1);
		total_len += 1;
		push(total_len,-2);
		return 0;
	case BASIC:
	case FUNCTION:
		if(array_suffix_len == 0) {
				push(spnt->VMS_type,-1);
				push(DBG$C_BASIC,-1);		
				total_len = 2;
				push(total_len,-2);
				return 1;};
		push(0,-4);
		push(0xfa02,-2);		
		total_len = -2;
		return 1;
	case STRUCT:
	case UNION:
	case ENUM:
		struct_number=spnt->struc_numb;
		if(struct_number < 0) {
			new_forward_ref(spnt->dbx_type);
			return 1;
		}
		push(DBG$C_STRUCT,-1);
		total_len = 5;
		push(total_len,-2);
		return 1;
	case POINTER:
		spnt1=find_symbol(spnt->type2);
		i=1;
		if(spnt1 == (struct VMS_DBG_Symbol *) NULL)	
			new_forward_ref(spnt->type2);
		else  i=gen1(spnt1,0);
		if(i){ /* (*void) is a special case, do not put pointer suffix*/
		  push(DBG$C_POINTER,-1);
		  total_len += 3;
		  push(total_len,-2);
		};
		return 1;
	case ARRAY:
		spnt1=spnt;
		while(spnt1->advanced == ARRAY)
			{spnt1 = find_symbol(spnt1->type2);
			if(spnt1 == (struct VMS_DBG_Symbol *) NULL) {
			    printf("gcc-as warning(debugger output):");
			    printf("Forward reference error, dbx type %d\n",
					spnt->type2);
			    return;}
			};
/* It is too late to generate forward references, so the user gets a message.
 * This should only happen on a compiler error */
		i=gen1(spnt1,1);
		i=Apoint;
		array_suffix(spnt);
		array_suffix_len = Apoint - i;
		switch(spnt1->advanced){
		case BASIC:
		case FUNCTION:
			break;
		default:
			push(0,-2);
			total_len += 2;
			push(total_len,-2);
			push(0xfa,-1);
			push(0x0101,-2);
			push(DBG$C_COMPLEX_ARRAY,-1);
		};
		total_len += array_suffix_len + 8;
		push(total_len,-2);			
	};
}

/* this generates a suffix for a variable.  If it is not a defined type yet,
 * then dbx_type contains the type we are expecting so we can generate a
 * forward reference.  This calls gen1 to build most of the descriptor, and
 * then it puts the icing on at the end.  It then dumps whatever is needed
 * to get a complete descriptor (i.e. struct reference, array suffix ).
 */
static generate_suffix(struct VMS_DBG_Symbol * spnt,int dbx_type){
	int ilen;
	int i;
	char pvoid[6] = {5,0xaf,0,1,0,5};
	struct VMS_DBG_Symbol * spnt1;
	Apoint=0;
	Lpnt =MAX_DEBUG_RECORD-1;
	total_len=0;
	struct_number = 0;
	overflow = 0;
	if(spnt == (struct VMS_DBG_Symbol*) NULL)
		new_forward_ref(dbx_type);
	else{
		if(spnt->VMS_type != 0xa3) return 0; /* no suffix needed */
		gen1(spnt,0);
	};
	push(0x00af,-2);
	total_len += 4;
	push(total_len,-1);
/* if the variable descriptor overflows the record, output a descriptor for
 * a pointer to void.
 */
	if((total_len >= MAX_DEBUG_RECORD) || overflow) {
		 printf(" Variable descriptor %d too complicated. Defined as *void ",spnt->dbx_type);
		 VMS_Store_Immediate_Data(pvoid, 6, OBJ$C_DBG);
		  return;
		};		
	i=0;
	while(Lpnt < MAX_DEBUG_RECORD-1) Local[i++] = Local[++Lpnt];
	Lpnt = i; 
/* we use this for a reference to a structure that has already been defined */
	if(struct_number > 0){
		 VMS_Store_Immediate_Data(Local, Lpnt, OBJ$C_DBG);Lpnt=0;
		 VMS_Store_Struct(struct_number);}; 
/* we use this for a forward reference to a structure that has yet to be
*defined.  We store four bytes of zero to make room for the actual address once
* it is known
*/
	if(struct_number < 0){
		 struct_number = -struct_number;
		  VMS_Store_Immediate_Data(Local, Lpnt,OBJ$C_DBG);Lpnt=0;
		  VMS_Def_Struct(struct_number);
		 for(i=0;i<4;i++) Local[Lpnt++] = 0;
		 VMS_Store_Immediate_Data(Local, Lpnt, OBJ$C_DBG);Lpnt=0;
		  };
	i=0;
	while(i<Apoint) Local[Lpnt++] = Asuffix[i++];
	if(Lpnt != 0)
	VMS_Store_Immediate_Data(Local, Lpnt, OBJ$C_DBG);
	Lpnt=0;
 }

/* This routine generates a symbol definition for a C sybmol for the debugger.
 * It takes a psect and offset for global symbols - if psect < 0, then this is
 * a local variable and the offset is relative to FP.  In this case it can
 * be either a variable (Offset < 0) or a parameter (Offset > 0).
 */
VMS_DBG_record(struct VMS_DBG_Symbol* spnt,int Psect,int  Offset, char* Name)
{
	char* pnt;
	int j;
	int maxlen;
	int i=0;
	if(Psect < 0) {	/* this is a local variable, referenced to SP */
	  maxlen=7+strlen(Name);
	  Local[i++] = maxlen;
	  Local[i++]=spnt->VMS_type;
	  if(Offset > 0) Local[i++] = DBG$C_FUNCTION_PARAMETER;
		else Local[i++] = DBG$C_LOCAL_SYM;
	  pnt=(char*) &Offset;
	  for(j=0;j<4;j++) Local[i++]=*pnt++; /* copy the offset */
	} else {
	  maxlen=7+strlen(Name); /* symbols fixed in memory */
	  Local[i++]=7+strlen(Name);
	  Local[i++]=spnt->VMS_type;
	  Local[i++]=1;
	  VMS_Store_Immediate_Data(Local, i, OBJ$C_DBG); i=0;
	  VMS_Set_Data(Psect,Offset,OBJ$C_DBG,0);
	}
	Local[i++]=strlen(Name);
	pnt=Name;
	fix_name(pnt);	/* if there are bad characters in name, convert them */
	while(*pnt!='\0') Local[i++]=*pnt++;
	VMS_Store_Immediate_Data(Local, i, OBJ$C_DBG);
	if(spnt->VMS_type == DBG$C_ADVANCED_TYPE) generate_suffix(spnt,0);
}


/* This routine parses the stabs entries in order to make the definition
 * for the debugger of local symbols and function parameters
 */
int VMS_local_stab_Parse(symbolS * sp){
	char *pnt;
	char *pnt1;
	char *str;
	struct VMS_DBG_Symbol* spnt;
   	struct VMS_Symbol *	vsp;
	int dbx_type;
	int VMS_type;
	dbx_type=0;
	str=sp->sy_nlist.n_un.n_name;
	pnt=(char*) strchr(str,':');
	if(pnt==(char*) NULL) return;	/* no colon present */
	pnt1=pnt++;	/* save this for later, and skip colon */
	if(*pnt == 'c') return 0;	/* ignore static constants */
/* there is one little catch that we must be aware of.  Sometimes function
 * parameters are optimized into registers, and the compiler, in its infiite
 * wisdom outputs stabs records for *both*.  In general we want to use the
 * register if it is present, so we must search the rest of the symbols for 
 * this function to see if this parameter is assigned to a register.
 */
	{
	char *str1;
	char *pnt2;
	symbolS * sp1;
	if(*pnt == 'p'){
	  for(sp1 = sp->sy_next; sp1; sp1 = sp1->sy_next) {
	    if ((sp1->sy_nlist.n_type & N_STAB) == 0) continue;
	    if((unsigned char)sp1->sy_nlist.n_type == N_FUN) break;
	    if((unsigned char)sp1->sy_nlist.n_type != N_RSYM) continue;
	    str1=sp1->sy_nlist.n_un.n_name;	/* and get the name */
	    pnt2=str;
	    while(*pnt2 != ':') {
		if(*pnt2 != *str1) break;
		pnt2++; str1++;};
	    if((*str1 != ':') || (*pnt2 != ':') ) continue;
	    return;	/* they are the same!  lets skip this one */
	  }; /* for */
/* first find the dbx symbol type from list, and then find VMS type */
	  pnt++;	/* skip p in case no register */
	};/* if */ }; /* p block */
	pnt = cvt_integer( pnt, &dbx_type);
	spnt = find_symbol(dbx_type);
	if(spnt==(struct VMS_DBG_Symbol*) NULL) return 0;/*Dunno what this is*/
	*pnt1='\0';
	VMS_DBG_record(spnt,-1,sp->sy_nlist.n_value,str);
	*pnt1=':';	/* and restore the string */
	return 1;
}

/* this routine parses a stabs entry to find the information required to define
 * a variable.  It is used for global and static variables. 
 * Basically we need to know the address of the symbol.  With older versions
 * of the compiler, const symbols are
 * treated differently, in that if they are global they are written into the
 * text psect.  The global symbol entry for such a const is actually written
 * as a program entry point (Yuk!!), so if we cannot find a symbol in the list
 * of psects, we must search the entry points as well.  static consts are even
 * harder, since they are never assigned a memory address.  The compiler passes
 * a stab to tell us the value, but I am not sure what to do with it.
 */
static gave_compiler_message = 0;

static int VMS_stab_parse(symbolS * sp,char expected_type,
	int type1,int type2,int Text_Psect){
	char *pnt;
	char *pnt1;
	char *str;
	symbolS * sp1;
	struct VMS_DBG_Symbol* spnt;
   	struct VMS_Symbol *	vsp;
	int dbx_type;
	int VMS_type;
	dbx_type=0;
	str=sp->sy_nlist.n_un.n_name;
	pnt=(char*) strchr(str,':');
	if(pnt==(char*) NULL) return;	/* no colon present */
	pnt1=pnt;	/* save this for later*/
	pnt++;
	if(*pnt==expected_type){
		pnt = cvt_integer(pnt+1,&dbx_type);
		spnt = find_symbol(dbx_type);
		if(spnt==(struct VMS_DBG_Symbol*) NULL) return 0;/*Dunno what this is*/
/* now we need to search the symbol table to find the psect and offset for
 * this variable.
 */
	*pnt1='\0';
	vsp=VMS_Symbols;
	while(vsp != (struct VMS_Symbol*) NULL) 
	  {pnt=vsp->Symbol->sy_nlist.n_un.n_name;
	   if(pnt!=(char*) NULL)  if(*pnt++ == '_') 
/* make sure name is the same, and make sure correct symbol type */
	   if((strlen(pnt) == strlen(str)) && (strcmp(pnt,str)==0) 
		&& ((vsp->Symbol->sy_type == type1) ||
		(vsp->Symbol->sy_type == type2))) break;
			vsp=vsp->Next;};
		if(vsp != (struct VMS_Symbol*) NULL){
	    VMS_DBG_record(spnt,vsp->Psect_Index,vsp->Psect_Offset,str);
		*pnt1=':';	/* and restore the string */
		return 1;};
/* the symbol was not in the symbol list, but it may be an "entry point"
   if it was a constant */
	   for(sp1 = symbol_rootP; sp1; sp1 = sp1->sy_next) {
		  /*
		   *	Dispatch on STAB type
		   */
		  if(sp1->sy_type != (N_TEXT | N_EXT) && sp1->sy_type!=N_TEXT)
		      continue;
		  pnt = sp1->sy_nlist.n_un.n_name;
		  if(*pnt == '_') pnt++;
		  if(strcmp(pnt,str) == 0){
			if(!gave_compiler_message && expected_type=='G'){
printf("***Warning - the assembly code generated by the compiler has placed\n");
printf("global constant(s) in the text psect.  These will not be available to\n");
printf("other modules, since this is not the correct way to handle this. You\n");
printf("have two options: 1) get a patched compiler that does not put global\n");
printf("constants in the text psect, or 2) remove the 'const' keyword from\n");
printf("definitions of global variables in your source module(s).  Don't say\n");
printf("I didn't warn you!");
gave_compiler_message = 1;};
				VMS_DBG_record(spnt,
	 			Text_Psect,
	 			sp1->sy_nlist.n_value,
	 			str);
	 		*pnt1=':';
	 		*(sp1->sy_nlist.n_un.n_name) = 'L';
					/* fool assembler to not output this
					 * as a routine in the TBT */
			return 1;};
	    };
	};
	*pnt1=':';	/* and restore the string */
	return 0;
}


VMS_GSYM_Parse(symbolS * sp,int Text_Psect){ /* Global variables */
	VMS_stab_parse(sp,'G',(N_UNDF | N_EXT),(N_DATA | N_EXT),Text_Psect);
}


VMS_LCSYM_Parse(symbolS * sp,int Text_Psect){/* Static symbols - uninitialized */
	VMS_stab_parse(sp,'S',N_BSS,-1,Text_Psect);
}

VMS_STSYM_Parse(symbolS * sp,int Text_Psect){ /*Static symbols - initialized */
	VMS_stab_parse(sp,'S',N_DATA,-1,Text_Psect);
}


/* for register symbols, we must figure out what range of addresses within the
 * psect are valid. We will use the brackets in the stab directives to give us
 * guidance as to the PC range that this variable is in scope.  I am still not
 * completely comfortable with this but as I learn more, I seem to get a better
 * handle on what is going on.
 * Caveat Emptor.
 */
VMS_RSYM_Parse(symbolS * sp,symbolS * Current_Routine,int Text_Psect){
	char* pnt;
	char* pnt1;
	char* str;
	int dbx_type;
	struct VMS_DBG_Symbol* spnt;
	int j;
	int maxlen;
	int i=0;
	int bcnt=0;
	int Min_Offset=-1;	/* min PC of validity */
	int Max_Offset=0; /* max PC of validity */
	symbolS * symbolP;
	   for(symbolP = sp; symbolP; symbolP = symbolP->sy_next) {
		  /*
		   *	Dispatch on STAB type
		   */
		  switch((unsigned char)symbolP->sy_type) {
		     case N_LBRAC:
				if(bcnt++==0) Min_Offset = symbolP->sy_nlist.n_value;
				break;
		     case N_RBRAC:
				if(--bcnt==0) Max_Offset = 
					symbolP->sy_nlist.n_value-1;
				break;
 		  }
	   if((Min_Offset != -1) && (bcnt == 0)) break;
	   if((unsigned char)symbolP->sy_type == N_FUN) break;
 	   }
/* check to see that the addresses were defined.  If not, then there were no
 * brackets in the function, and we must try to search for the next function
 * Since functions can be in any order, we should search all of the symbol list
 * to find the correct ending address. */
	if(Min_Offset == -1){
	  int Max_Source_Offset;
	  int This_Offset;
	  Min_Offset = sp->sy_nlist.n_value;
	   for(symbolP = symbol_rootP; symbolP; symbolP = symbolP->sy_next) {
		  /*
		   *	Dispatch on STAB type
		   */
		  This_Offset = symbolP->sy_nlist.n_value;
		  switch(symbolP->sy_type) {
 		     case N_TEXT | N_EXT:
			if((This_Offset > Min_Offset) && (This_Offset < Max_Offset))
					Max_Offset = This_Offset;
				break;
		     case N_SLINE:
			if(This_Offset > Max_Source_Offset)
				Max_Source_Offset=This_Offset;
 		  }
 	   }
/* if this is the last routine, then we use the PC of the last source line
 * as a marker of the max PC for which this reg is valid */
	  if(Max_Offset == 0x7fffffff) Max_Offset = Max_Source_Offset;
	};
	dbx_type=0;
	str=sp->sy_nlist.n_un.n_name;
	pnt=(char*) strchr(str,':');
	if(pnt==(char*) NULL) return;	/* no colon present */
	pnt1=pnt;	/* save this for later*/
	pnt++;
	if(*pnt!='r') return 0;
	pnt = cvt_integer( pnt+1, &dbx_type);
	spnt = find_symbol(dbx_type);
	if(spnt==(struct VMS_DBG_Symbol*) NULL) return 0;/*Dunno what this is yet*/
	*pnt1='\0';
	maxlen=25+strlen(sp->sy_nlist.n_un.n_name);
	Local[i++]=maxlen;
	Local[i++]=spnt->VMS_type;
	Local[i++]=0xfb;
	Local[i++]=strlen(sp->sy_nlist.n_un.n_name)+1;
	Local[i++]=0x00;
	Local[i++]=0x00;
	Local[i++]=0x00;
	Local[i++]=strlen(sp->sy_nlist.n_un.n_name);
	pnt=sp->sy_nlist.n_un.n_name;
	fix_name(pnt);	/* if there are bad characters in name, convert them */
	while(*pnt!='\0') Local[i++]=*pnt++;
	Local[i++]=0xfd;
	Local[i++]=0x0f;
	Local[i++]=0x00;
	Local[i++]=0x03;
	Local[i++]=0x01;
	VMS_Store_Immediate_Data(Local, i, OBJ$C_DBG); i=0;
	VMS_Set_Data(Text_Psect,Min_Offset,OBJ$C_DBG,1);
	VMS_Set_Data(Text_Psect,Max_Offset,OBJ$C_DBG,1);
	Local[i++]=0x03;
	Local[i++]=sp->sy_nlist.n_value;
	Local[i++]=0x00;
	Local[i++]=0x00;
	Local[i++]=0x00;
	VMS_Store_Immediate_Data(Local, i, OBJ$C_DBG);
	*pnt1=':';
	if(spnt->VMS_type == DBG$C_ADVANCED_TYPE) generate_suffix(spnt,0);
}

/* this function examines a structure definition, checking all of the elements
 * to make sure that all of them are fully defined.  The only thing that we
 * kick out are arrays of undefined structs, since we do not know how big
 * they are.  All others we can handle with a normal forward reference.
 */
static int forward_reference(char* pnt){
	int i;
	struct VMS_DBG_Symbol * spnt;
	struct VMS_DBG_Symbol * spnt1;
	pnt = cvt_integer(pnt+1,&i);
	if(*pnt == ';') return 0; /* no forward references */
	do{
	  pnt=(char*) strchr(pnt,':');
	  pnt = cvt_integer(pnt+1,&i);
	  spnt = find_symbol(i);
	  if(spnt == (struct VMS_DBG_Symbol*) NULL) return 0;
	  while((spnt->advanced == POINTER) || (spnt->advanced == ARRAY)){
		  i=spnt->type2;
		  spnt1 = find_symbol(spnt->type2);
		  if((spnt->advanced == ARRAY) &&
			(spnt1 == (struct VMS_DBG_Symbol*) NULL))return 1;
		  if(spnt1 == (struct VMS_DBG_Symbol*) NULL) break;
		  spnt=spnt1;
	  };
	  pnt = cvt_integer(pnt+1,&i);
	  pnt = cvt_integer(pnt+1,&i);
	}while(*++pnt != ';');
	return 0;	/* no forward refences found */
}

/* This routine parses the stabs directives to find any definitions of dbx type
 * numbers.  It makes a note of all of them, creating a structure element
 * of VMS_DBG_Symbol that describes it.  This also generates the info for the
 * debugger that describes the struct/union/enum, so that further references
 * to these data types will be by number
 * 	We have to process pointers right away, since there can be references
 * to them later in the same stabs directive.  We cannot have forward
 * references to pointers, (but we can have a forward reference to a pointer to
 * a structure/enum/union) and this is why we process them immediately.
 * After we process the pointer, then we search for defs that are nested even
 * deeper.
 */
static int VMS_typedef_parse(char* str){
	char* pnt;
	char* pnt1;
	char* pnt2;
	int i;
	int dtype;
	struct forward_ref * fpnt;
	int i1,i2,i3;
	int convert_integer;
	struct VMS_DBG_Symbol* spnt;
	struct VMS_DBG_Symbol* spnt1;
/* check for any nested def's */
	pnt=(char*)strchr(str+1,'=');
	if((pnt != (char*) NULL) && (*(str+1) != '*')) 
		if(VMS_typedef_parse(pnt) == 1 ) return 1;
/* now find dbx_type of entry */
	pnt=str-1;
	if(*pnt == 'c'){	/* check for static constants */
		*str = '\0';	/* for now we ignore them */
		return 0;};
	while((*pnt <= '9')&& (*pnt >= '0')) pnt--;
	pnt++;	/* and get back to the number */
	cvt_integer(pnt,&i1);
	spnt = find_symbol(i1);
/* first we see if this has been defined already, due to a forward reference*/
	if(spnt == (struct VMS_DBG_Symbol*) NULL) {
	  if(VMS_Symbol_type_list==(struct VMS_DBG_Symbol*) NULL)
	    {spnt=(struct VMS_DBG_Symbol*) malloc(sizeof(struct VMS_DBG_Symbol));
	     spnt->next = (struct VMS_DBG_Symbol*) NULL;
	    VMS_Symbol_type_list=spnt;}
	  else
	    {spnt=(struct VMS_DBG_Symbol*) malloc(sizeof(struct VMS_DBG_Symbol));
	    spnt->next=VMS_Symbol_type_list;
	    VMS_Symbol_type_list = spnt;};
	  spnt->dbx_type = i1;	/* and save the type */
	};
/* for structs and unions, do a partial parse, otherwise we sometimes get
 * circular definitions that are impossible to resolve. We read enough info
 * so that any reference to this type has enough info to be resolved 
 */
	pnt=str + 1;	/* point to character past equal sign */
	if((*pnt == 'u') || (*pnt == 's')){
	};
	if((*pnt <= '9') && (*pnt >= '0')){
	   if(type_check("void")){ /* this is the void symbol */
		*str='\0';
		spnt->advanced = VOID;
		return 0;};
	   printf("gcc-as warning(debugger output):");
	   printf(" %d is an unknown untyped variable.\n",spnt->dbx_type);
	   return 1;	/* do not know what this is */
	};
/* now define this module*/
	pnt=str + 1;	/* point to character past equal sign */
	switch (*pnt){
	    case 'r':
		spnt->advanced= BASIC;
		if(type_check("int")) {
			spnt->VMS_type=DBG$C_SLINT; spnt->data_size=4;}
		else if(type_check("long int")) {
			spnt->VMS_type=DBG$C_SLINT; spnt->data_size=4;}
		else if(type_check("unsigned int")) {
			spnt->VMS_type=DBG$C_ULINT; spnt->data_size = 4;}
		else if(type_check("long unsigned int")) {
			spnt->VMS_type=DBG$C_ULINT; spnt->data_size = 2;}
		else if(type_check("short int")) {
			spnt->VMS_type=DBG$C_SSINT; spnt->data_size = 2;}
		else if(type_check("short unsigned int")) {
			spnt->VMS_type=DBG$C_USINT; spnt->data_size = 2;}
		else if(type_check("char")) {
			spnt->VMS_type=DBG$C_SCHAR; spnt->data_size = 1;}
		else if(type_check("signed char")) {
			spnt->VMS_type=DBG$C_SCHAR; spnt->data_size = 1;}
		else if(type_check("unsigned char")) {
			spnt->VMS_type=DBG$C_UCHAR; spnt->data_size = 1;}
		else if(type_check("float")) {
			spnt->VMS_type=DBG$C_REAL4; spnt->data_size = 4;}
		else if(type_check("double")) {
			spnt->VMS_type=DBG$C_REAL8; spnt->data_size = 8;}
		pnt1=(char*) strchr(str,';')+1;
		break;
	    case 's':
	    case 'u':
		if(*pnt == 's') spnt->advanced= STRUCT;
				else spnt->advanced= UNION;
		spnt->VMS_type = DBG$C_ADVANCED_TYPE;
		pnt1 = cvt_integer(pnt+1,&spnt->data_size);
		if(forward_reference(pnt)) {
			spnt->struc_numb = -1;
			return 1;
		}
		spnt->struc_numb = ++structure_count;
		pnt1--;
		pnt=get_struct_name(str);
		VMS_Def_Struct(spnt->struc_numb);
		fpnt = f_ref_root;
		while(fpnt != (struct forward_ref*) NULL){
			if(fpnt->dbx_type == spnt->dbx_type) {
				fpnt->resolved = 'Y';
				VMS_Set_Struct(fpnt->struc_numb);
				VMS_Store_Struct(spnt->struc_numb);};
			fpnt = fpnt->next;};
		VMS_Set_Struct(spnt->struc_numb);
		i=0;
		Local[i++] = 11+strlen(pnt);
		Local[i++] = DBG$C_STRUCT_START;
		Local[i++] = 0x80;
		for(i1=0;i1<4;i1++) Local[i++] = 0x00;
		Local[i++] = strlen(pnt);
		pnt2=pnt;
		while(*pnt2 != '\0') Local[i++] = *pnt2++;
		i2=spnt->data_size * 8;		/* number of bits */
		pnt2=(char*) &i2;
		for(i1=0;i1<4;i1++) Local[i++] = *pnt2++;
		VMS_Store_Immediate_Data(Local, i, OBJ$C_DBG); i=0;
		if(pnt != symbol_name) {
			pnt += strlen(pnt);
			*pnt=':';};	/* replace colon for later */
		while(*++pnt1 != ';'){
		  pnt=(char*) strchr(pnt1,':');
		  *pnt='\0';
		  pnt2=pnt1;
		  pnt1 = cvt_integer(pnt+1,&dtype);
		  pnt1 = cvt_integer(pnt1+1,&i2);
		  pnt1 = cvt_integer(pnt1+1,&i3);
		  if((dtype == 1) && (i3 != 32)) { /* bitfield */
		    Apoint = 0;
		    push(19+strlen(pnt2),1);
		    push(0xfa22,2);
		    push(1+strlen(pnt2),4);
		    push(strlen(pnt2),1);
		    while(*pnt2 != '\0') push(*pnt2++,1);
		    push(i3,2);	/* size of bitfield */
		    push(0x0d22,2);
		    push(0x00,4);
		    push(i2,4);	/* start position */
		    VMS_Store_Immediate_Data(Asuffix,Apoint,OBJ$C_DBG);
		    Apoint=0;
		  }else{
		    Local[i++] = 7+strlen(pnt2);
		    spnt1 = find_symbol(dtype);
		/* check if this is a forward reference */
		    if(spnt1 != (struct VMS_DBG_Symbol*) NULL)
				 Local[i++] = spnt1->VMS_type;
			else
				 Local[i++] = DBG$C_ADVANCED_TYPE;
		    Local[i++] = DBG$C_STRUCT_ITEM;
		    pnt=(char*) &i2;		
		    for(i1=0;i1<4;i1++) Local[i++] = *pnt++;
		    Local[i++] = strlen(pnt2);
		    while(*pnt2 != '\0') Local[i++] = *pnt2++;
		    VMS_Store_Immediate_Data(Local, i, OBJ$C_DBG); i=0;
		    if(spnt1 == (struct VMS_DBG_Symbol*) NULL)
				generate_suffix(spnt1,dtype);
		    else if(spnt1->VMS_type == DBG$C_ADVANCED_TYPE)
				generate_suffix(spnt1,0);
		};
		};
		pnt1++;
		Local[i++] = 0x01;	/* length byte */
		Local[i++] = DBG$C_STRUCT_END;
		VMS_Store_Immediate_Data(Local, i, OBJ$C_DBG); i=0;
		break;
	    case 'e':
		spnt->advanced= ENUM;
		spnt->VMS_type = DBG$C_ADVANCED_TYPE;
		spnt->struc_numb = ++structure_count;
		spnt->data_size=4;
		VMS_Def_Struct(spnt->struc_numb);
		fpnt = f_ref_root;
		while(fpnt != (struct forward_ref*) NULL){
			if(fpnt->dbx_type == spnt->dbx_type) {
				fpnt->resolved = 'Y';
				VMS_Set_Struct(fpnt->struc_numb);
				VMS_Store_Struct(spnt->struc_numb);};
			fpnt = fpnt->next;};
		VMS_Set_Struct(spnt->struc_numb);
		i=0;
		Local[i++] = 3+strlen(symbol_name);
		Local[i++] = DBG$C_ENUM_START;
		Local[i++] = 0x20;
		Local[i++] = strlen(symbol_name);
		pnt2=symbol_name;
		while(*pnt2 != '\0') Local[i++] = *pnt2++;
		VMS_Store_Immediate_Data(Local, i, OBJ$C_DBG); i=0;
		while(*++pnt != ';') {
		  pnt1=(char*) strchr(pnt,':');
		  *pnt1++='\0';
		  pnt1 = cvt_integer(pnt1,&i1);
		  Local[i++] = 7+strlen(pnt);
		  Local[i++] = DBG$C_ENUM_ITEM;
		  Local[i++] = 0x00;
		  pnt2=(char*) &i1;
		  for(i2=0;i2<4;i2++) Local[i++] = *pnt2++;
		  Local[i++] = strlen(pnt);
		  pnt2=pnt;
		  while(*pnt != '\0') Local[i++] = *pnt++;
		  VMS_Store_Immediate_Data(Local, i, OBJ$C_DBG); i=0;
		  pnt= pnt1;	/* Skip final semicolon */
		};
		Local[i++] = 0x01; /* len byte */
		Local[i++] = DBG$C_ENUM_END;
		VMS_Store_Immediate_Data(Local, i, OBJ$C_DBG); i=0;
		pnt1=pnt + 1;
		break;
	    case 'a':
		spnt->advanced= ARRAY;
		spnt->VMS_type = DBG$C_ADVANCED_TYPE;
		pnt=(char*)strchr(pnt,';');  if (pnt == (char*) NULL) return 1;
		pnt1 = cvt_integer(pnt+1,&spnt->index_min);
		pnt1 = cvt_integer(pnt1+1,&spnt->index_max);
		pnt1 = cvt_integer(pnt1+1,&spnt->type2);
		break;
	    case 'f':
		spnt->advanced= FUNCTION;
		spnt->VMS_type = DBG$C_FUNCTION_ADDR;
			/* this masquerades as a basic type*/
		spnt->data_size=4;
		pnt1 = cvt_integer(pnt+1,&spnt->type2);
		break;
	    case '*':
		spnt->advanced= POINTER;
		spnt->VMS_type = DBG$C_ADVANCED_TYPE;
		spnt->data_size=4;
		pnt1 = cvt_integer(pnt+1,&spnt->type2);
		pnt=(char*)strchr(str+1,'=');
		if((pnt != (char*) NULL)) 
			if(VMS_typedef_parse(pnt) == 1 ) return 1;
		break;
	    default:
		spnt->advanced= UNKNOWN;
		spnt->VMS_type = 0;
		printf("gcc-as warning(debugger output):");
	   	printf(" %d is an unknown type of variable.\n",spnt->dbx_type);
		return 1; /* unable to decipher */
	};
/* this removes the evidence of the definition so that the outer levels of 
parsing do not have to worry about it */
	pnt=str;
	while (*pnt1 != '\0') *pnt++ = *pnt1++;
	*pnt = '\0';
	return 0;
}


/* 
 * This is the root routine that parses the stabs entries for definitions.
 * it calls VMS_typedef_parse, which can in turn call itself.
 * We need to be careful, since sometimes there are forward references to
 * other symbol types, and these cannot be resolved until we have completed
 * the parse.
 */
int VMS_LSYM_Parse(){
	char *pnt;
	char *pnt1;
	char *pnt2;
	char *str;
	char fixit[10];
	int incomplete,i,pass,incom1;
	struct VMS_DBG_Symbol* spnt;
   	struct VMS_Symbol *	vsp;
	struct forward_ref * fpnt;
	symbolS * sp;
	pass=0;
	incomplete = 0;
	   do{
	   incom1=incomplete;
	   incomplete = 0;
	   for(sp = symbol_rootP; sp; sp = sp->sy_next) {
		/*
		 *	Deal with STAB symbols
		 */
		if ((sp->sy_nlist.n_type & N_STAB) != 0) {
		  /*
		   *	Dispatch on STAB type
		   */
		  switch((unsigned char)sp->sy_nlist.n_type) {
			case N_GSYM:
			case N_LCSYM:
			case N_STSYM:
 			case N_PSYM:
 			case N_RSYM:
			case N_LSYM:
			case N_FUN:	/*sometimes these contain typedefs*/
				str=sp->sy_nlist.n_un.n_name;
				symbol_name = str;
				pnt=(char*)strchr(str,':');
				if(pnt== (char*) NULL) break;
				*pnt='\0';
				pnt1=pnt+1;
				pnt2=(char*)strchr(pnt1,'=');
				if(pnt2 == (char*) NULL){
						*pnt=':';	/* replace colon */
						break;};	/* no symbol here */
				incomplete += VMS_typedef_parse(pnt2);
				*pnt=':';    /* put back colon so variable def code finds dbx_type*/
				break;
		  }	/*switch*/
		}	/* if */
	   }		/*for*/
	pass++;
	} while((incomplete != 0) && (incomplete != incom1 ));
			/* repeat until all refs resolved if possible */
/*	if(pass > 1) printf(" Required %d passes\n",pass);*/
	if(incomplete != 0){	
		printf("gcc-as warning(debugger output):");
		printf("Unable to resolve %d circular references.\n",incomplete);
		};
	fpnt = f_ref_root;
	symbol_name="\0";
	while(fpnt != (struct forward_ref*) NULL){
		if(fpnt->resolved != 'Y') {
		  if( find_symbol(fpnt->dbx_type) != 
			(struct VMS_DBG_Symbol*) NULL){
			    printf("gcc-as warning(debugger output):");
			    printf("Forward reference error, dbx type %d\n",
					fpnt->dbx_type);
			    break;};
		  fixit[0]=0;
		  sprintf(&fixit[1],"%d=s4;",fpnt->dbx_type);
		  pnt2=(char*)strchr(&fixit[1],'=');
		  VMS_typedef_parse(pnt2);
		  };
		fpnt = fpnt->next;};
}

static symbolS* Current_Routine;
static int Text_Psect;

static Define_Local_Symbols(symbolS* s1,symbolS* s2){
	symbolS * symbolP1;
	for(symbolP1 = s1->sy_next; symbolP1 != s2; symbolP1 = symbolP1->sy_next) {
		if (symbolP1 == (symbolS *)NULL) return;
		if (symbolP1->sy_nlist.n_type == N_FUN) return;
		/*
		 *	Deal with STAB symbols
		 */
		if ((symbolP1->sy_nlist.n_type & N_STAB) != 0) {
		  /*
		   *	Dispatch on STAB type
		   */
		  switch((unsigned char)symbolP1->sy_nlist.n_type) {
 			case N_LSYM:
 			case N_PSYM:
 			   VMS_local_stab_Parse(symbolP1);
 			   break;
 			case N_RSYM:
 			   VMS_RSYM_Parse(symbolP1,Current_Routine,Text_Psect);
 				break;
		  }	/*switch*/
		}	/* if */
	   }		/* for */
}

static symbolS* Define_Routine(symbolS* symbolP,int Level){
	symbolS * sstart;
	symbolS * symbolP1;
	char	str[10];
	char * pnt;
	int rcount = 0;
	int Offset;
	sstart = symbolP;
	for(symbolP1 = symbolP->sy_next; symbolP1; symbolP1 = symbolP1->sy_next) {
		if (symbolP1->sy_nlist.n_type == N_FUN) break;
		/*
		 *	Deal with STAB symbols
		 */
		if ((symbolP1->sy_nlist.n_type & N_STAB) != 0) {
		  /*
		   *	Dispatch on STAB type
		   */
		  if((unsigned char)symbolP1->sy_nlist.n_type == N_FUN) break;
		  switch((unsigned char)symbolP1->sy_nlist.n_type) {
			case N_LBRAC:
				if(Level != 0) {
				   pnt = str +sprintf(str,"$%d",rcount++);
				   *pnt = '\0';
				   VMS_TBT_Block_Begin(symbolP1,Text_Psect,str);
				};
				Offset = symbolP1->sy_nlist.n_value;
				Define_Local_Symbols(sstart,symbolP1);
				symbolP1 = 
				    Define_Routine(symbolP1,Level+1);
				if(Level != 0)
				  VMS_TBT_Block_End(symbolP1->sy_nlist.n_value -
					   Offset);
				sstart=symbolP1;
				break;
			case N_RBRAC:
				return symbolP1;
		  }	/*switch*/
		}	/* if */
	   }		/* for */
	/* we end up here if there were no brackets in this function. Define
everything */
	Define_Local_Symbols(sstart,(symbolS *) 0);
}

VMS_DBG_Define_Routine(symbolS* symbolP,symbolS* Curr_Routine,int Txt_Psect){
	Current_Routine = Curr_Routine;
	Text_Psect = Txt_Psect;
	Define_Routine(symbolP,0);
}
