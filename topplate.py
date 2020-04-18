# coding: utf-8
from functools import reduce
from sexpdata import load, Symbol
from solid import *
from solid.utils import *

def parse(p):
    try:
        _, x, y, deg = p
        return linear_extrude(1.4)(translate ([x, -y, 0.0])(rotate(deg, [0, 0, 1])(square(14, True))))
    except Exception:
        _, x, y = p
        return linear_extrude(1.4)(translate ([x, -y, 0.0])(square(14, True)))

f = load(open("./right.kicad_pcb"))
y = list(filter(lambda x: x[0] == Symbol("module") if isinstance(x, list) else False, f))
z = list(filter(lambda x: x[1] == Symbol("base:ToprePad"), y))
sws = list(map(lambda y: list(filter(lambda x: x[0] == Symbol("at") if isinstance(x, list) else False, y))[0], z))
scad_render_to_file(union()(*list(map(parse, sws))), "holes.scad")
