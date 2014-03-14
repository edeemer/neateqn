#define FNLEN		32	/* font name length */
#define SZLEN		32	/* point size length */
#define LNLEN		1000	/* line length */
#define NMLEN		32	/* macro name length */
#define RLEN		8	/* register name length */

/* registers used by neateqn */
#define EQNSZ		".eqnsz"	/* register for surrounding point size */
#define EQNFN		".eqnfn"	/* register for surrounding font */
#define EQNS		".eqns"		/* eqn-collected line register */
#define EQNMK		".eqnmk"	/* eqn horizontal mark */

/* helpers */
#define MIN(a, b)	((a) < (b) ? (a) : (b))
#define MAX(a, b)	((a) < (b) ? (b) : (a))
#define LEN(a)		(sizeof(a) / sizeof((a)[0]))

/* token and atom types */
#define T_TOK(t)	((t) & 0x00ff)
#define T_ATOM(t)	((t) & 0x00f0)
#define T_FONT(t)	((t) & T_FNMASK)
#define T_ATOMIDX(t)	((((t) >> 4) & 15) - 1)

#define T_EOF		0x0000		/* parser-specific */
#define T_SPACE		0x0001
#define T_TAB		0x0002
#define T_GAP		0x0003
#define T_KEYWORD	0x0004
#define T_ORD		0x0010		/* ordinary */
#define T_LETTER	0x0011
#define T_NUMBER	0x0012
#define T_STRING	0x0013
#define T_BIGOP		0x0020		/* big operators */
#define T_BINOP		0x0030		/* binary operators */
#define T_RELOP		0x0040		/* relational symbol */
#define T_LEFT		0x0050		/* opening token */
#define T_RIGHT		0x0060		/* closing token */
#define T_PUNC		0x0070		/* punctuation symbol */
#define T_INNER		0x0080		/* like fractions */

#define T_FNMASK	0x0f00		/* font mask */
#define T_FN0		0x0000		/* nothing */
#define T_FN2		0x0100		/* italic font */
#define T_FNX		0x0200		/* other fonts */

/* spaces in hundredths of em */
#define S_S1		17		/* thin space */
#define S_S2		22		/* medium space */
#define S_S3		28		/* thick space */

/* reading input */
int in_next(void);
void in_back(int c);
void in_define(char *name, char *def);
int in_expand(char *name, char **args);
int in_macrocall(char *name);
int in_arg(int i);

/* tokenizer */
int tok_eqn(void);
void tok_eqnout(char *s);
char *tok_get(void);
char *tok_pop(void);
char *tok_poptext(void);
int tok_jmp(char *kwd);
int tok_type(void);
int tok_sep(void);
void tok_delim(void);
void tok_macro(void);

/* default definitions and operators */
int def_binop(char *s);
int def_relop(char *s);
int def_punc(char *s);
int def_left(char *s);
int def_right(char *s);
char *def_pieces(char *sign, char **top, char **mid, char **bot, char **cen);
extern char *def_macros[][2];

/* variable length string buffer */
struct sbuf {
	char *s;		/* allocated buffer */
	int sz;			/* buffer size */
	int n;			/* length of the string stored in s */
};

void sbuf_init(struct sbuf *sbuf);
void sbuf_done(struct sbuf *sbuf);
char *sbuf_buf(struct sbuf *sbuf);
void sbuf_add(struct sbuf *sbuf, int c);
void sbuf_append(struct sbuf *sbuf, char *s);
void sbuf_printf(struct sbuf *sbuf, char *s, ...);
void sbuf_cut(struct sbuf *sbuf, int n);
int sbuf_len(struct sbuf *sbuf);
int sbuf_empty(struct sbuf *sbuf);

/* equations */
struct box {
	struct sbuf raw;	/* the contents */
	int szreg;		/* number register holding box size */
	int reg;		/* register holding the contents */
	int empty;		/* nothing has been inserted yet */
	int tbeg, tcur;		/* type of the first and the last atoms */
	int tgap;		/* the last item added was a T_GAP */
};

struct box *box_alloc(int szreg, int at_pre);
void box_free(struct box *box);
void box_puttext(struct box *box, int type, char *s, ...);
void box_putf(struct box *box, char *s, ...);
void box_preputf(struct box *box, char *s, ...);
void box_merge(struct box *box, struct box *sub);
void box_sub(struct box *box, struct box *sub, struct box *sup);
void box_from(struct box *box, struct box *lim, struct box *llim, struct box *ulim);
void box_over(struct box *box, struct box *num, struct box *denom);
void box_sqrt(struct box *box, struct box *sub);
void box_bar(struct box *box);
void box_under(struct box *box);
void box_accent(struct box *box, char *c);
void box_wrap(struct box *box, struct box *sub, char *left, char *right);
void box_move(struct box *box, int dy, int dx);
void box_gap(struct box *box, int n);
char *box_buf(struct box *box);
char *box_toreg(struct box *box);
void box_vertspace(struct box *box);
int box_empty(struct box *box);
void box_width(struct box *box, int reg);
void box_vcenter(struct box *box, struct box *sub);

/* managing registers */
char *escarg(char *arg);
int sregmk(void);
void sregrm(int id);
int nregmk(void);
void nregrm(int id);
char *nreg(int id);
char *sreg(int id);
char *nregname(int id);
char *sregname(int id);
void reg_reset(void);