# coding: utf-8
from sexpdata import load, Symbol
f = load(open("./right.kicad_pcb"))
y = list(filter(lambda x: x[0] == Symbol("module") if isinstance(x, list) else False, f))
z = list(filter(lambda x: x[1] == Symbol("base:ToprePad"), y))
sws = list(map(lambda y: list(filter(lambda x: x[0] == Symbol("at") if isinstance(x, list) else False, y))[0], z))
cuts = list(filter(lambda x: [Symbol('layer'), Symbol('Edge.Cuts')] in x if isinstance(x, list) else False, f))
print(sws, cuts)
