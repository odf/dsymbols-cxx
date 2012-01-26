id = "$Id: dsutil.py,v 1.1.1.1 2002/10/08 18:09:18 delgado Exp $"

from dsymbol import *


def set_ops(ds, spec):
    for i in spec.keys():
        assert ds.idx_valid(i), \
               "Invalid index %d" % (i)
        op = spec[i]
        for D in op.keys():
            if not ds.elm_valid(D):
                raise ValueError("Invalid element %d" % (D))
            if ds.op_defined(i,D):
                raise ValueError("op(%d,%d) already defined" % (i, D))
            if not ds.set_op(i, D, op[D]):
                raise ValueError(
                    "setting op(%d,%d) to %d failed" % (i, D, op[D]))

def set_v(ds, spec):
    for i in spec.keys():
        assert ds.idx_valid(i), \
               "Invalid index %d" % (i)
        v = spec[i]
        j = ds.idx_next(i)
        for D in v.keys():
            if not ds.elm_valid(D):
                raise ValueError("Invalid element %d" % (D))
            if ds.v_defined(i,j,D):
                raise ValueError("v(%d,%d,%d) already defined" % (i, j, D))
            if not ds.set_v(i, j, D, v[D]):
               raise ValueError(
                   "setting v(%d,%d,%d) to %d failed" % (i, j, D, v[D]))

def parse_ds(code):
    import string

    s = string.split(code[1:-1], ":")

    x = map(string.atoi, string.split(s[1], " "))
    size = x[0]
    if len(x) > 1:
        dim = x[1]
    else:
        dim = 2
    ds = DSymbol(dim, size)

    op = string.split(s[2], ",")
    for i in range(0,dim+1):
        img = map(string.atoi, string.split(op[i], " "))
        k = 0
        for D in range(1,size+1):
            if not ds.op_defined(i, D):
                ds.set_op(i,D,img[k])
                k = k + 1

    v = string.split(s[3], ",")
    for i in range(0,dim):
        j = i+1
        val = map(string.atoi, string.split(v[i], " "))
        k = 0
        for D in range(1,size+1):
            if not ds.v_defined(i, j, D):
                ds.set_v(i, j, D, val[k])
                k = k + 1

    return ds


def make_ds(**args):
    if args.has_key("code"):
        return parse_ds(args["code"])
    else:
        if args.has_key("source"):
            ds = simple_ds(args["source"])
        else:
            if args.has_key("dim"):
                dim = args["dim"]
            else:
                dim = 2
            if args.has_key("size"):
                size = args["size"]
            else:
                size = 1
            ds = DSymbol(dim, size)

        if args.has_key("op"):
            set_ops(ds, args["op"])
        if args.has_key("v"):
            set_v(ds, args["v"])

    return ds


def ggt(a, b):
    while b:
	a, b = b, a % b
    return a

def curvature(ds):
    i = ds.idx_first()
    j = ds.idx_next(i)
    k = ds.idx_next(j)

    sub = DSubsymbol(ds, [i, j, k], ds.elm_first())

    if not sub.is_finite():
	return [0, 0]

    K = [-sub.size(), 1]

    i = sub.idx_first()
    while sub.idx_valid(i):
	j = sub.idx_next(i)
	while sub.idx_valid(j):
	    seen = {}

	    D = sub.elm_first()
	    while sub.elm_valid(D):
		if not seen.has_key(`D`):
		    if sub.v_defined(i, j, D) and sub.v(i, j, D) > 0:
			v = sub.v(i, j, D)
		    else:
			v = 1

		    D1 = D
		    s = 2

		    while 1:
			if sub.op_defined(i, D1) and sub.op(i, D1) != D1:
			    D1 = sub.op(i, D1)
			    seen[`D1`] = 1
			else:
			    s = 1
			if sub.op_defined(j, D1) and sub.op(j, D1) != D1:
			    D1 = sub.op(j, D1)
			    seen[`D1`] = 1
			else:
			    s = 1
			if D1 == D:
			    break

		    num, den = [K[0] * v + K[1] * s, K[1] * v]
		    f = ggt(num, den)
		    K = [num / f, den / f]

		D = sub.elm_next(D)

	    j = sub.idx_next(j)

	i = sub.idx_next(i)

    return (K[0], K[1])


def apply_word(ds, D, word):
    E = D
    for i in word:
        E = ds.op(i, E)
    return E


def elements(ds):
    res = []
    D = ds.elm_first();
    while ds.elm_valid(D):
        res.append(D)
        D = ds.elm_next(D)
    return res


def elm_max(ds):
    max = D = ds.elm_first();
    while ds.elm_valid(D):
        if D > max:
            max = D
        D = ds.elm_next(D)
    return max


def elm_min(ds):
    min = D = ds.elm_first();
    while ds.elm_valid(D):
        if D < min:
            min = D
        D = ds.elm_next(D)
    return min


def indices(ds):
    res = []
    i = ds.idx_first();
    while ds.idx_valid(i):
        res.append(i)
        i = ds.idx_next(i)
    return res


def idx_max(ds):
    max = i = ds.idx_first();
    while ds.idx_valid(i):
        if i > max:
            max = i
        i = ds.idx_next(i)
    return max


def idx_min(ds):
    min = i = ds.idx_first();
    while ds.idx_valid(i):
        if i < min:
            min = i
        i = ds.idx_next(i)
    return min


def oriented_size(ds):
    if ds.is_finite():
        if ds.is_oriented() == 'Yes':
            return ds.size()
        elif ds.is_oriented() == 'No':
            return 2 * ds.size()
        else:
            return None
    else:
        return "infinity"


def oriented_cover(ds):
    if ds.is_oriented() == 'No':
        return BinCoverDSymbol(ds)
    else:
        return ds


def weakly_oriented_size(ds):
    if ds.is_finite():
        if ds.is_weakly_oriented() == 'Yes':
            return ds.size()
        elif ds.is_weakly_oriented() == 'No':
            return 2 * ds.size()
        else:
            return None
    else:
        return "infinity"


def weakly_oriented_cover(ds):
    if ds.is_weakly_oriented() == 'No':
        return BinCoverDSymbol(ds)
    else:
        return ds


def tree_edges(ds):
    if not ds.is_finite():
        raise ValueError("symbol must be finite")

    res = []
    D = ds.elm_first()
    while ds.elm_valid(D):
        i = ds.idx_back_edge(D)
        if ds.idx_valid(i):
            res.append((ds.op(i,D), i, D))
        D = ds.elm_next(D)
    return res


def path_to(ds, D):
    path = []
    E = D
    i = ds.idx_back_edge(E)
    while ds.idx_valid(i):
        path.append(i)
        E = ds.op(i, E)
        i = ds.idx_back_edge(E)
    path.reverse()
    return path


def path(ds, A, B):
    p = path_to(ds, A)
    p.reverse()
    return p + path_to(ds, B)


def partial_orientation(ds):
    D = ds.elm_first()
    sign = { `D`: 1 }
    for edge in tree_edges(DSymbolBFS(ds, D)):
        
        sign[`edge[2]`] = -sign[`edge[0]`]
    return sign


def encode_ds(ds):
    import string
    
    ds = simple_ds(ds)

    code = "1.1:" + `ds.size()`
    if ds.dim() != 2:
        code = code + ' ' + `ds.dim()`
    code = code + ':'

    i = ds.idx_first()
    while ds.idx_valid(i):
        if i != ds.idx_first():
            code = code + ','
        D = ds.elm_first()
        while ds.elm_valid(D):
            Di = ds.op(i, D)
            if Di >= D:
                if D != ds.elm_first():
                    code = code + ' '
                code = code + `Di`
            D = ds.elm_next(D)
        i = ds.idx_next(i)

    code = code + ':'

    i = ds.idx_first()
    while ds.idx_valid(i+1):
        if i != ds.idx_first():
            code = code + ','
        reps = RepresentativesComponents(SectionDSymbol(ds, [i, i+1]))
        for n in range(len(reps)):
            if n > 0:
                code = code + ' '
            code = code + `ds.v(i, i+1, reps[n])`
        i = ds.idx_next(i)
        
    return code


def canonical_ds(ds):
    if not ds.is_finite():
        raise ValueError("symbol must be finite")

    ds = simple_ds(ds)
    seed = ds.elm_first()
    best = simple_ds(DSymbolIPS(ds, seed))
    seed = ds.elm_next(seed)

    while ds.elm_valid(seed):
        nw = simple_ds(DSymbolIPS(ds, seed))
        
        i = ds.idx_first()
        while ds.idx_valid(i):
            D = ds.elm_first()
            while ds.elm_valid(D):
                Di1 = ds.op(i, D)
                Di2 = nw.op(i, D)
                if Di2 != Di1:
                    if Di2 < Di1:
                        best = nw
                    D, i = ds.elm_none(), ds.idx_none()
                D = ds.elm_next(D)
            i = ds.idx_next(i)

        i = ds.idx_first()
        while ds.idx_valid(i+1):
            reps = RepresentativesComponents(SectionDSymbol(ds, [i, i+1]))
            for D in reps:
                v1 = ds.v(i, i+1, D)
                v2 = nw.v(i, i+1, D)
                if v2 != v1:
                    if v2 < v1:
                        best = nw
                    i = ds.idx_none()
                    break
            i = ds.idx_next(i)

        seed = ds.elm_next(seed)
        
    return best
