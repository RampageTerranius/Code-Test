#pragma once

struct Keyboard
{
	bool a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z;
	bool n1, n2, n3, n4, n5, n6, n7, n8, n9, n0;
	bool plus, minus;
	bool leftBracket, rightBracket;
	bool backSpace;
	bool lShift, rShift, lCtrl, rCtrl, lAlt, rAlt;
	bool space, escape, tab, capsLock, tilde;
};

extern Keyboard keyboard;