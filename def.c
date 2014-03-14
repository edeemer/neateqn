#include <string.h>
#include "eqn.h"

/* null-terminated list of default macros */
char *def_macros[][2] = {
	{"<-",		"\\(<-"},
	{"<=",		"\\(<="},
	{">=",		"\\(>="},
	{"==",		"\\(=="},
	{"->",		"\\(->"},
	{"!=",		"\\(!="},
	{"+-",		"\\(+-"},
	{"...",		"\"\\v'-.25m'\\ .\\ .\\ .\\ \\v'.25m'\""},
	{",...,",	"\"\\f1,\\fP\\ .\\ .\\ .\\ \\f1,\\fP\\|\""},
	{"ALPHA",	"\\(*A"},
	{"BETA",	"\\(*B"},
	{"CHI",		"\\(*X"},
	{"DELTA",	"\\(*D"},
	{"EPSILON",	"\\(*E"},
	{"ETA",		"\\(*Y"},
	{"GAMMA",	"\\(*G"},
	{"IOTA",	"\\(*I"},
	{"KAPPA",	"\\(*K"},
	{"LAMBDA",	"\\(*L"},
	{"MU",		"\\(*M"},
	{"NU",		"\\(*N"},
	{"OMEGA",	"\\(*W"},
	{"OMICRON",	"\\(*O"},
	{"PHI",		"\\(*F"},
	{"PI",		"\\(*P"},
	{"PSI",		"\\(*Q"},
	{"RHO",		"\\(*R"},
	{"SIGMA",	"\\(*S"},
	{"TAU",		"\\(*T"},
	{"THETA",	"\\(*H"},
	{"UPSILON",	"\\(*U"},
	{"XI",		"\\(*C"},
	{"ZETA",	"\\(*Z"},
	{"alpha",	"\\(*a"},
	{"beta",	"\\(*b"},
	{"chi",		"\\(*x"},
	{"delta",	"\\(*d"},
	{"epsilon",	"\\(*e"},
	{"eta",		"\\(*y"},
	{"gamma",	"\\(*g"},
	{"iota",	"\\(*i"},
	{"kappa",	"\\(*k"},
	{"lambda",	"\\(*l"},
	{"mu",		"\\(*m"},
	{"nu",		"\\(*n"},
	{"omega",	"\\(*w"},
	{"omicron",	"\\(*o"},
	{"phi",		"\\(*f"},
	{"pi",		"\\(*p"},
	{"psi",		"\\(*q"},
	{"rho",		"\\(*r"},
	{"sigma",	"\\(*s"},
	{"tau",		"\\(*t"},
	{"theta",	"\\(*h"},
	{"upsilon",	"\\(*u"},
	{"xi",		"\\(*c"},
	{"zeta",	"\\(*z"},
	{"Im",		"roman \"Im\""},
	{"Re",		"roman \"Re\""},
	{"and",		"roman \"and\""},
	{"approx",	"\"\\v'-.2m'\\z\\(ap\\v'.25m'\\(ap\\v'-.05m'\""},
	{"arc",		"roman \"arc\""},
	{"cdot",	"\"\\v'-.3m'.\\v'.3m'\""},
	{"cos",		"roman \"cos\""},
	{"cosh",	"roman \"cosh\""},
	{"coth",	"roman \"coth\""},
	{"del",		"\\(gr"},
	{"det",		"roman \"det\""},
	{"dollar",	"roman $"},
	{"exp",		"roman \"exp\""},
	{"for",		"roman \"for\""},
	{"grad",	"\\(gr"},
	{"half",	"roman \\(12"},
	{"if",		"roman \"if\""},
	{"inf",		"\\(if"},
	{"infinity",	"\\(if"},
	{"int",		"roman size +2 \\(in"},
	{"inter",	"roman size +2 \\(ca"},
	{"lim",		"roman \"lim\""},
	{"ln",		"roman \"ln\""},
	{"log",		"roman \"log\""},
	{"max",		"roman \"max\""},
	{"min",		"roman \"min\""},
	{"nothing",	""},
	{"partial",	"\\(pd"},
	{"prime",	"\"\\f1\\v'.5m'\\s+3\\(fm\\s-3\\v'-.5m'\\fP\""},
	{"prod",	"roman size +2 \\(pr"},
	{"sin",		"roman \"sin\""},
	{"sinh",	"roman \"sinh\""},
	{"sum",		"roman size +2 \\(su"},
	{"tan",		"roman \"tan\""},
	{"tanh",	"roman \"tanh\""},
	{"times",	"\\(mu"},
	{"union",	"roman size +2 \\(cu"},
	{NULL, NULL}
};

/* list of binary operations */
static char *binops[] = {
	"+", "\\(pl",
	"−", "-", "\\(mi",
	"÷", "\\(-:", "\\(di",
	"×", "xx", "\\(mu",
	"±", "\\(+-",
	"⊗", "\\(Ox",
	"⊕", "\\(O+",
	"∧", "\\(l&",
	"∨", "\\(l|",
	"∩", "\\(ca",
	"∪", "\\(cu",
};

/* list of relations */
static char *relops[] = {
	"<", ">", ":=",
	"=", "\\(eq",
	"≅", "\\(cg",
	"≤", "\\(<=",
	"≥", "\\(>=",
	"≠", "\\(!=",
	"≡", "\\(==",
	"≈", "\\(~~",
	"⊃", "\\(sp",
	"⊇", "\\(ip",
	"⊄", "\\(!b",
	"⊂", "\\(sb",
	"⊆", "\\(ib",
	"∈", "\\(mo",
	"∉", "\\(!m",
	"↔", "\\(ab",
	"←", "\\(<-",
	"↑", "\\(ua",
	"→", "\\(->",
	"↓", "\\(da",
};

int def_binop(char *s)
{
	int i;
	for (i = 0; i < LEN(binops); i++)
		if (!strcmp(binops[i], s))
			return 1;
	return 0;
}

int def_relop(char *s)
{
	int i;
	for (i = 0; i < LEN(relops); i++)
		if (!strcmp(relops[i], s))
			return 1;
	return 0;
}

int def_punc(char *s)
{
	return !s[1] && strchr(".,;:!", s[0]);
}

int def_left(char *s)
{
	return !s[1] && strchr("[(", s[0]);
}

int def_right(char *s)
{
	return !s[1] && strchr(")]", s[0]);
}

static struct brac {
	char *one;
	char *top;
	char *mid;
	char *bot;
	char *cen;
} bracs[] = {
	{"(", "\\(LT", "\\(LX", "\\(LB"},
	{")", "\\(RT", "\\(RX", "\\(RB"},
	{"[", "\\(lc", "\\(lx", "\\(lf"},
	{"]", "\\(rc", "\\(rx", "\\(rf"},
	{"{", "\\(lt", "\\(bv", "\\(lb", "\\(lk"},
	{"}", "\\(rt", "\\(bv", "\\(rb", "\\(rk"},
	{"|", "\\(br", "\\(br", "\\(br"},
	{"\\(lc", "\\(lc", "\\(lx", "\\(lx"},
	{"\\(rc", "\\(rc", "\\(rx", "\\(rx"},
	{"\\(lf", "\\(lx", "\\(lx", "\\(lf"},
	{"\\(rf", "\\(rx", "\\(rx", "\\(rf"},
};

/* return the pieces of a bracket; return the single glyph form */
char *def_pieces(char *sign, char **top, char **mid, char **bot, char **cen)
{
	int i;
	for (i = 0; i < LEN(bracs); i++) {
		if (!strcmp(bracs[i].one, sign)) {
			*top = bracs[i].top;
			*mid = bracs[i].mid;
			*bot = bracs[i].bot;
			*cen = bracs[i].cen;
			return bracs[i].one;
		}
	}
	*top = sign;
	*mid = sign;
	*bot = sign;
	*cen = NULL;
	return sign;
}