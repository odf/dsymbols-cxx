from dsutil import *

def CHECK(ds, prop):
    res = eval("ds.is_%s()"%prop)
    if res == "Yes":
        print prop,
    elif res == "Maybe":
        print prop+"?",

def showsymbol(ds):
    print ds
    print "  properties: ",
    CHECK(ds, "complete")
    CHECK(ds, "proper")
    CHECK(ds, "loopless")
    CHECK(ds, "simple")
    CHECK(ds, "connected")
    CHECK(ds, "weakly_oriented")
    CHECK(ds, "oriented")
    print

    D = ds.elm_first()
    i = ds.idx_first()
    j = ds.idx_next(i)

    print "  v(%d, %d, %s) = %d" % (i, j, `D`, ds.v(i,j,D))
    print "  r(%d, %d, %s) = %d" % (i, j, `D`, ds.r(i,j,D))
    print "  m(%d, %d, %s) = %d" % (i, j, `D`, ds.m(i,j,D))
    c = curvature(ds)
    if c[1] == 1:
	print "  curvature = %d" % c[0]
    else:
	print "  curvature = %d/%d" % c
    print

# ------------------------------------------------------------------------

ds = make_ds(code = "<1.1:8:2 4 6 8,8 3 5 7,5 6 8 7:1,1>")

print "Testing class DSymbol:"
print "  ds:"
showsymbol(ds)

ds.unset_op(2, 1)
ds.unset_op(2, 2)
print "after ds.unset_op(2, 1) an ds.unset_op(2, 2):"
showsymbol(ds);

ds.unset_v(0, 1, 1)
print "after ds.unset_v(0, 1, 1):"
showsymbol(ds)

ds.set_op(2, 1, 2)
ds.set_op(2, 5, 5)
print "after ds.set_op(2, 1, 2) and ds.set_op(2, 5, 5):"
showsymbol(ds)

ds.set_op(2, 6, 6)
print "after ds.set_op(2, 6, 6):"
showsymbol(ds)

ds.set_v(0, 1, 1, 3)
print "after ds.set_v(0, 1, 1, 3):"
showsymbol(ds)

dual = DualDSymbol(ds)
print "dual:"
showsymbol(dual)

ddual = DualDSymbol(dual)
print "dual of dual:"
showsymbol(ddual)

simp = simple_ds(ddual)
print "converted back to simple symbol:"
showsymbol(simp)

cov = BinCoverDSymbol(ds)
print "binary cover of ds:"
showsymbol(cov)

ccov = BinCoverDSymbol(cov)
print "second order cover:"
showsymbol(ccov)

simp = make_ds(source = ccov)
print "converted to simple symbol:"
showsymbol(simp)

indices = [ 2, 0, 3, 0, -1 ]

sec = SectionDSymbol(cov, indices)
print "section of binary cover:"
showsymbol(sec)

simp = simple_ds(sec)
print "converted to simple symbol:"
showsymbol(simp)

copy = ds.clone()
print "copy of ds:"
showsymbol(copy)

sub = DSubsymbol(ds, indices, ds.elm_first())
print "subsymbol of ds:"
showsymbol(sub)

subcov = DSubsymbol(cov, indices, cov.elm_first())
print "subsymbol of cov:"
showsymbol(subcov)

ds.unset_op(2, 2)
ds.unset_op(2, 5)

print "subsymbol after ds.unset_op(2, 2) and ds.unset_op(2, 5):"
showsymbol(sub)

print "subcov after ds.unset_op(2, 2) and ds.unset_op(2, 5):"
showsymbol(subcov)

print "copy should stay the same:"
showsymbol(copy)

# --------------------

ds3 = make_ds(dim = 3, size = 10,
              op = {0:{1:2,3:4,5:6,7:8,9:10},
                    1:{1:7,2:8,3:5,4:6,9:10},
                    2:{1:9,2:10,3:4,7:7,8:8},
                    3:{1:3,2:4,8:6,7:5,9:10}})

print "Simple symbol ds3:"
showsymbol(ds3)

trav = DSymbolBFS(ds3, 1)
print "Breadth first traversal of ds3:"
showsymbol(trav)

travd = DSymbolDFS(ds3, 8)
print "Depth first traversal of ds3 starting at 8:"
showsymbol(travd)

travi = DSymbolEPS(ds3, 1)
print "Priority traversal of ds3:"
showsymbol(travi)

travi = DSymbolIPS(ds3, 1)
print "Index priority traversal of ds3:"
showsymbol(travi)

stravi = simple_ds(travi)
print "Converted to simple:"
showsymbol(stravi)

section = SectionDSymbol(ds3, [0, 1])
print "Section for index set (0,1) of ds3:"
showsymbol(section)

reps = RepresentativesComponents(section)
print "Representatives of (0,1) orbits: ",
for D in reps:
    print D,
print
