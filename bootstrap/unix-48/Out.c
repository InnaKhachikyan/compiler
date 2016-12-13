/* voc 2.00 [2016/12/13]. Bootstrapping compiler for address size 8, alignment 8. xtspaSF */

#define SHORTINT INT8
#define INTEGER  INT16
#define LONGINT  INT32
#define SET      UINT32

#include "SYSTEM.h"
#include "Heap.h"
#include "Platform.h"


export BOOLEAN Out_IsConsole;
static CHAR Out_buf[128];
static INT16 Out_in;

static ADDRESS *typedesc__11__typ;
static ADDRESS *blockdesc__5__typ;
static ADDRESS *chunkdesc__7__typ;

export void Out_Char (CHAR ch);
export void Out_DumpHeap (void);
static void Out_DumpModule (Heap_Module m);
export void Out_DumpTag (INT32 addr);
export void Out_DumpType (SYSTEM_BYTE *o, ADDRESS o__len);
export void Out_Flush (void);
export void Out_Hex (INT64 x, INT64 n);
export void Out_HexDump (SYSTEM_BYTE *m, ADDRESS m__len);
export void Out_HexDumpAdr (INT32 adr, INT64 offset, INT32 length);
export void Out_Int (INT64 x, INT64 n);
static INT32 Out_Length (CHAR *s, ADDRESS s__len);
export void Out_Ln (void);
export void Out_LongReal (LONGREAL x, INT16 n);
export void Out_Open (void);
export void Out_Real (REAL x, INT16 n);
static void Out_RealP (LONGREAL x, INT16 n, BOOLEAN long_);
export void Out_String (CHAR *str, ADDRESS str__len);
export LONGREAL Out_Ten (INT16 e);
static void Out_digit (INT64 n, CHAR *s, ADDRESS s__len, INT16 *i);
static void Out_prepend (CHAR *t, ADDRESS t__len, CHAR *s, ADDRESS s__len, INT16 *i);

#define Out_Entier64(x)	(INT64)(x)
extern ADDRESS Heap_heap;
#define Out_getheap()	Heap_heap

void Out_Flush (void)
{
	INT16 error;
	if (Out_in > 0) {
		error = Platform_Write(1, (ADDRESS)Out_buf, Out_in);
	}
	Out_in = 0;
}

void Out_Open (void)
{
}

void Out_Char (CHAR ch)
{
	if (Out_in >= 128) {
		Out_Flush();
	}
	Out_buf[__X(Out_in, 128)] = ch;
	Out_in += 1;
	if (ch == 0x0a) {
		Out_Flush();
	}
}

static INT32 Out_Length (CHAR *s, ADDRESS s__len)
{
	INT32 l;
	l = 0;
	while ((l < s__len && s[__X(l, s__len)] != 0x00)) {
		l += 1;
	}
	return l;
}

void Out_String (CHAR *str, ADDRESS str__len)
{
	INT32 l;
	INT16 error;
	__DUP(str, str__len, CHAR);
	l = Out_Length((void*)str, str__len);
	if (Out_in + l > 128) {
		Out_Flush();
	}
	if (l > 128) {
		error = Platform_Write(1, (ADDRESS)str, l);
	} else {
		__MOVE((ADDRESS)str, (ADDRESS)&Out_buf[__X(Out_in, 128)], l);
		Out_in += (INT16)l;
	}
	__DEL(str);
}

void Out_Int (INT64 x, INT64 n)
{
	CHAR s[22];
	INT16 i;
	BOOLEAN negative;
	negative = x < 0;
	if (x == (-9223372036854775807LL-1)) {
		__MOVE("8085774586302733229", s, 20);
		i = 19;
	} else {
		if (x < 0) {
			x = -x;
		}
		s[0] = (CHAR)(48 + __MOD(x, 10));
		x = __DIV(x, 10);
		i = 1;
		while (x != 0) {
			s[__X(i, 22)] = (CHAR)(48 + __MOD(x, 10));
			x = __DIV(x, 10);
			i += 1;
		}
	}
	if (negative) {
		s[__X(i, 22)] = '-';
		i += 1;
	}
	while (n > (INT64)i) {
		Out_Char(' ');
		n -= 1;
	}
	while (i > 0) {
		i -= 1;
		Out_Char(s[__X(i, 22)]);
	}
}

void Out_Hex (INT64 x, INT64 n)
{
	if (n < 1) {
		n = 1;
	} else if (n > 16) {
		n = 16;
	}
	while ((n < 16 && __LSH(x, -__ASHL(n, 2), 64) != 0)) {
		n += 1;
	}
	x = __ROT(x, __ASHL(16 - n, 2), 64);
	while (n > 0) {
		x = __ROTL(x, 4, 64);
		n -= 1;
		if (__MASK(x, -16) < 10) {
			Out_Char((CHAR)(__MASK(x, -16) + 48));
		} else {
			Out_Char((CHAR)((__MASK(x, -16) - 10) + 65));
		}
	}
}

void Out_Ln (void)
{
	Out_String(Platform_NL, 3);
	Out_Flush();
}

void Out_HexDumpAdr (INT32 adr, INT64 offset, INT32 length)
{
	INT16 i;
	INT32 n, lim;
	CHAR c;
	lim = (INT32)(adr + length);
	while (adr < lim) {
		if (adr + 16 < lim) {
			n = 16;
		} else {
			n = lim - adr;
		}
		Out_Hex(offset, 8);
		Out_Char(' ');
		i = 0;
		while (i < n) {
			if (__MASK(i, -4) == 0) {
				Out_Char(' ');
			}
			__GET(adr + i, c, CHAR);
			Out_Hex((INT16)c, 2);
			Out_Char(' ');
			i += 1;
		}
		while (i < 16) {
			if (__MASK(i, -4) == 0) {
				Out_Char(' ');
			}
			Out_String((CHAR*)"   ", 4);
			i += 1;
		}
		Out_String((CHAR*)" ", 2);
		i = 0;
		while (i < n) {
			__GET(adr + i, c, CHAR);
			if ((INT16)c < 32 || (INT16)c > 126) {
				Out_Char('.');
			} else {
				Out_Char(c);
			}
			i += 1;
		}
		adr += n;
		offset += (INT64)n;
		Out_Ln();
	}
}

void Out_HexDump (SYSTEM_BYTE *m, ADDRESS m__len)
{
	Out_HexDumpAdr((ADDRESS)m, 0, m__len);
}

static void Out_DumpModule (Heap_Module m)
{
	Out_String((CHAR*)"        next:     ", 19);
	Out_Hex((INT32)(ADDRESS)m->next, 1);
	Out_Ln();
	Out_String((CHAR*)"        name:     ", 19);
	Out_String(m->name, 20);
	Out_Ln();
	Out_String((CHAR*)"        refcnt:   ", 19);
	Out_Hex(m->refcnt, 1);
	Out_Ln();
	Out_String((CHAR*)"        cmds:     ", 19);
	Out_Hex((INT32)(ADDRESS)m->cmds, 1);
	Out_Ln();
	Out_String((CHAR*)"        types:    ", 19);
	Out_Hex(m->types, 1);
	Out_Ln();
	Out_String((CHAR*)"        enumPtrs: ", 19);
	Out_Hex((INT32)(ADDRESS)m->enumPtrs, 1);
	Out_Ln();
}

typedef
	struct typedesc__11 *tag__10;

typedef
	struct typedesc__11 {
		INT32 tag, next, level, module;
		CHAR name[24];
		INT32 bases[16];
		INT32 reserved, blksz, ptr0;
	} typedesc__11;

void Out_DumpTag (INT32 addr)
{
	tag__10 desc = NIL;
	INT16 i;
	Out_String((CHAR*)"      obj tag:  ", 17);
	Out_Hex(addr, 1);
	Out_Ln();
	addr -= __MASK(addr, -2);
	desc = (tag__10)(ADDRESS)(addr - 108);
	Out_String((CHAR*)"      desc at:  ", 17);
	Out_Hex((INT32)(ADDRESS)desc, 1);
	Out_Ln();
	Out_String((CHAR*)"      desc contains:", 21);
	Out_Ln();
	Out_String((CHAR*)"      tag:      ", 17);
	Out_Hex(desc->tag, 1);
	Out_Ln();
	Out_String((CHAR*)"      next:     ", 17);
	Out_Hex(desc->next, 1);
	Out_Ln();
	Out_String((CHAR*)"      level:    ", 17);
	Out_Hex(desc->level, 1);
	Out_Ln();
	Out_String((CHAR*)"      module:   ", 17);
	Out_Hex(desc->module, 1);
	Out_Ln();
	if (desc->module != 0) {
		Out_DumpModule((Heap_Module)(ADDRESS)desc->module);
	}
	Out_String((CHAR*)"      name:     ", 17);
	Out_String(desc->name, 24);
	Out_Ln();
	Out_String((CHAR*)"      bases:    ", 17);
	i = 0;
	while (i < 16) {
		Out_Hex(desc->bases[__X(i, 16)], 8);
		if (__MASK(i, -4) == 3) {
			Out_Ln();
			Out_String((CHAR*)"                ", 17);
		} else {
			Out_Char(' ');
		}
		i += 1;
	}
	Out_Ln();
	Out_String((CHAR*)"      reserved: ", 17);
	Out_Hex(desc->reserved, 1);
	Out_Ln();
	Out_String((CHAR*)"      blksz:    ", 17);
	Out_Hex(desc->blksz, 1);
	Out_Ln();
	Out_String((CHAR*)"      ptr0:     ", 17);
	Out_Hex(desc->ptr0, 1);
	Out_Ln();
}

void Out_DumpType (SYSTEM_BYTE *o, ADDRESS o__len)
{
	INT32 addr;
	__GET((ADDRESS)o - 4, addr, INT32);
	Out_DumpTag(addr);
}

typedef
	INT32 (*adrptr__3)[1];

typedef
	struct blockdesc__5 *block__4;

typedef
	struct blockdesc__5 {
		INT32 tag, size, sentinel, next;
	} blockdesc__5;

typedef
	struct chunkdesc__7 *chunk__6;

typedef
	struct chunkdesc__7 {
		INT32 next, end, reserved;
		blockdesc__5 firstblock;
	} chunkdesc__7;

void Out_DumpHeap (void)
{
	INT32 caddr;
	chunk__6 c = NIL;
	INT32 baddr;
	block__4 b = NIL;
	adrptr__3 tag = NIL;
	caddr = Heap_heap;
	while (caddr != 0) {
		Out_String((CHAR*)"Chunk at: ", 11);
		Out_Hex(caddr, 1);
		Out_Ln();
		c = (chunk__6)(ADDRESS)caddr;
		Out_String((CHAR*)"  next:   ", 11);
		Out_Hex(c->next, 1);
		Out_Ln();
		Out_String((CHAR*)"  end:    ", 11);
		Out_Hex(c->end, 1);
		Out_String((CHAR*)" => size: ", 11);
		Out_Hex(c->end - caddr, 1);
		Out_Ln();
		Out_String((CHAR*)"  rsvd:   ", 11);
		Out_Hex(c->reserved, 1);
		Out_Ln();
		baddr = (ADDRESS)&c->firstblock;
		while (c->end - baddr > 0) {
			Out_String((CHAR*)"  Block at:   ", 15);
			Out_Hex(baddr, 1);
			Out_Ln();
			b = (block__4)(ADDRESS)baddr;
			tag = (adrptr__3)(ADDRESS)b->tag;
			Out_String((CHAR*)"    tag:      ", 15);
			Out_Hex(b->tag, 1);
			if (__MASK(b->tag, -2) != 0) {
				Out_String((CHAR*)" <--- ODD! ---", 15);
			}
			Out_Ln();
			Out_String((CHAR*)"    tag^:     ", 15);
			Out_Hex((*tag)[0], 1);
			Out_Ln();
			Out_String((CHAR*)"    size:     ", 15);
			Out_Hex(b->size, 1);
			Out_Ln();
			Out_String((CHAR*)"    sentinel: ", 15);
			Out_Hex(b->sentinel, 1);
			Out_Ln();
			Out_String((CHAR*)"    next:     ", 15);
			Out_Hex(b->next, 1);
			Out_Ln();
			if (b->tag != (ADDRESS)&b->size) {
				Out_DumpTag(b->tag);
			}
			baddr += (*tag)[0];
			Out_Ln();
		}
		caddr = c->next;
		Out_Ln();
	}
}

static void Out_digit (INT64 n, CHAR *s, ADDRESS s__len, INT16 *i)
{
	*i -= 1;
	s[__X(*i, s__len)] = (CHAR)(__MOD(n, 10) + 48);
}

static void Out_prepend (CHAR *t, ADDRESS t__len, CHAR *s, ADDRESS s__len, INT16 *i)
{
	INT16 j;
	INT32 l;
	__DUP(t, t__len, CHAR);
	l = Out_Length((void*)t, t__len);
	if (l > *i) {
		l = *i;
	}
	*i -= (INT16)l;
	j = 0;
	while (j < l) {
		s[__X(*i + j, s__len)] = t[__X(j, t__len)];
		j += 1;
	}
	__DEL(t);
}

LONGREAL Out_Ten (INT16 e)
{
	LONGREAL r, power;
	r = (LONGREAL)1;
	power = (LONGREAL)10;
	while (e > 0) {
		if (__ODD(e)) {
			r = r * power;
		}
		power = power * power;
		e = __ASHR(e, 1);
	}
	return r;
}

static void Out_RealP (LONGREAL x, INT16 n, BOOLEAN long_)
{
	INT16 e;
	INT64 f;
	CHAR s[30];
	INT16 i, el;
	LONGREAL x0;
	BOOLEAN nn, en;
	INT64 m;
	INT16 d, dr;
	e = (INT16)__MASK(__ASHR((__VAL(INT64, x)), 52), -2048);
	f = __MASK((__VAL(INT64, x)), -4503599627370496LL);
	nn = (__VAL(INT64, x) < 0 && !((e == 2047 && f != 0)));
	if (nn) {
		n -= 1;
	}
	i = 30;
	if (e == 2047) {
		if (f == 0) {
			Out_prepend((CHAR*)"Infinity", 9, (void*)s, 30, &i);
		} else {
			Out_prepend((CHAR*)"NaN", 4, (void*)s, 30, &i);
		}
	} else {
		if (long_) {
			el = 3;
			dr = n - 6;
			if (dr > 17) {
				dr = 17;
			}
			d = dr;
			if (d < 15) {
				d = 15;
			}
		} else {
			el = 2;
			dr = n - 5;
			if (dr > 9) {
				dr = 9;
			}
			d = dr;
			if (d < 6) {
				d = 6;
			}
		}
		if (e == 0) {
			while (el > 0) {
				i -= 1;
				s[__X(i, 30)] = '0';
				el -= 1;
			}
			i -= 1;
			s[__X(i, 30)] = '+';
			m = 0;
		} else {
			if (nn) {
				x = -x;
			}
			e = (INT16)__ASHR((e - 1023) * 77, 8);
			if (e >= 0) {
				x = x / (LONGREAL)Out_Ten(e);
			} else {
				x = Out_Ten(-e) * x;
			}
			if (x >= (LONGREAL)10) {
				x =   1.00000000000000e-001 * x;
				e += 1;
			}
			en = e < 0;
			if (en) {
				e = -e;
			}
			while (el > 0) {
				Out_digit(e, (void*)s, 30, &i);
				e = __DIV(e, 10);
				el -= 1;
			}
			i -= 1;
			if (en) {
				s[__X(i, 30)] = '-';
			} else {
				s[__X(i, 30)] = '+';
			}
			x0 = Out_Ten(d - 1);
			x = x0 * x;
			x = x +   5.00000000000000e-001;
			if (x >= (LONGREAL)10 * x0) {
				x =   1.00000000000000e-001 * x;
				e += 1;
			}
			m = Out_Entier64(x);
		}
		i -= 1;
		if (long_) {
			s[__X(i, 30)] = 'D';
		} else {
			s[__X(i, 30)] = 'E';
		}
		if (dr < 2) {
			dr = 2;
		}
		while ((d > dr && __MOD(m, 10) == 0)) {
			m = __DIV(m, 10);
			d -= 1;
		}
		while (d > 1) {
			Out_digit(m, (void*)s, 30, &i);
			m = __DIV(m, 10);
			d -= 1;
		}
		i -= 1;
		s[__X(i, 30)] = '.';
		Out_digit(m, (void*)s, 30, &i);
	}
	n -= 30 - i;
	while (n > 0) {
		Out_Char(' ');
		n -= 1;
	}
	if (nn) {
		Out_Char('-');
	}
	while (i < 30) {
		Out_Char(s[__X(i, 30)]);
		i += 1;
	}
}

void Out_Real (REAL x, INT16 n)
{
	Out_RealP(x, n, 0);
}

void Out_LongReal (LONGREAL x, INT16 n)
{
	Out_RealP(x, n, 1);
}

__TDESC(typedesc__11, 1, 0) = {__TDFLDS("typedesc__11", 116), {-4}};
__TDESC(blockdesc__5, 1, 0) = {__TDFLDS("blockdesc__5", 16), {-4}};
__TDESC(chunkdesc__7, 1, 0) = {__TDFLDS("chunkdesc__7", 28), {-4}};

export void *Out__init(void)
{
	__DEFMOD;
	__MODULE_IMPORT(Heap);
	__MODULE_IMPORT(Platform);
	__REGMOD("Out", 0);
	__REGCMD("DumpHeap", Out_DumpHeap);
	__REGCMD("Flush", Out_Flush);
	__REGCMD("Ln", Out_Ln);
	__REGCMD("Open", Out_Open);
	__INITYP(typedesc__11, typedesc__11, 0);
	__INITYP(blockdesc__5, blockdesc__5, 0);
	__INITYP(chunkdesc__7, chunkdesc__7, 0);
/* BEGIN */
	Out_IsConsole = Platform_IsConsole(1);
	Out_in = 0;
	__ENDMOD;
}
