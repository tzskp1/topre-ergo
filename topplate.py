# coding: utf-8
from functools import reduce
from sexpdata import load, Symbol
from solid import *
from solid.utils import *

hole = polygon([[-7.25, 6.3], [-6.3, 7.25], [6.3, 7.25], [7.25, 6.3], [7.25, -6.3], [6.3, -7.25], [-6.3, -7.25], [-7.25, -6.3]])

def parse(p):
    try:
        _, x, y, deg = p
        return (translate ([x, -y, 0.0])(rotate(deg, [0, 0, 1])(hole)))
    except Exception:
        _, x, y = p
        return (translate ([x, -y, 0.0])(hole))

def parse_hole(p):
    _, x, y = p
    return (translate ([x, -y, 0.0])(circle(1.1, segments = 30)))

f = load(open("./right.kicad_pcb"))
y = list(filter(lambda x: x[0] == Symbol("module") if isinstance(x, list) else False, f))
z = list(filter(lambda x: x[1] == Symbol("base:ToprePad"), y))
sws = list(map(lambda y: list(filter(lambda x: x[0] == Symbol("at") if isinstance(x, list) else False, y))[0], z))
z = list(filter(lambda x: x[1] == Symbol("MountingHole:MountingHole_2.2mm_M2"), y))
holes = list(map(lambda y: list(filter(lambda x: x[0] == Symbol("at") if isinstance(x, list) else False, y))[0], z))
u = union()(*(list(map(parse, sws)) + list(map(parse_hole, holes))))
scad_render_to_file(u, "holes.scad")
