def insert_keepout_(center_x, center_y, radius, corners=16, layer=None, hatch_type=None,
                    no_tracks=True, no_vias=True, no_pour=True):
    """ Adds keepout area in polygon of approximate circle.
        Length have to be passed in internal unit.
        @param center_x center X position
        @param center_y center Y position
        @param radius circle radius
        @param corners number of vertex
        @param layer layer index
        @param hatch_type one of NO_HATCH, DIAGONAL_FULL, DIAGONAL_EDGE
        @param no_tracks True for no tracks
        @param no_vias True for no vias
        @param no_pour True for no copper pour
    """

    import pcbnew
    Point = pcbnew.wxPoint
    ver5 = hasattr(pcbnew, "SHAPE_POLY_SET")

    if corners < 3 or corners > 256:
        raise Exception("Wrong number of corners: %s" % corners)

    if hatch_type is None:
        # NO_HATCH, DIAGONAL_FULL, DIAGONAL_EDGE
        hatch_type = pcbnew.ZONE_CONTAINER.DIAGONAL_EDGE

    if layer is None:
        layer = pcbnew.F_Cu

    board = pcbnew.GetBoard()
    area = board.InsertArea(-1, 0xffff, layer, 0, 0, hatch_type)
    area.SetIsKeepout(True)
    area.SetDoNotAllowTracks(no_tracks)
    area.SetDoNotAllowVias(no_vias)
    area.SetDoNotAllowCopperPour(no_pour)
    outline = area.Outline()
    if ver5:
        outline.RemoveAllContours()
        #outline.RemoveVertex(0) # since CPolyLine -> SHAPE_POLY_SET
    else:
        outline.DeleteCorner(0) # remove first element

    import math
    cos, sin, floor = math.cos, math.sin, math.floor

    thi = 0
    dthi = 2 * math.pi / corners

    for i in range(corners):
        x = int(floor(center_x + radius * cos(thi)))
        y = int(floor(center_y + radius * sin(thi)))
        area.AppendCorner(Point(x, y), -1)
        thi += dthi

    if ver5:
        if hasattr(area, "BuildFilledSolidAreasPolygons"):
            # todo, do we need to call this?
            # BuildFilledSolidAreasPolygons method has been gone since
            # "pcbnew: made zone filling algorithm thread-safe. "
            area.BuildFilledSolidAreasPolygons(board)
        else:
            pass
            #outline.Outline(0).SetClosed(True) # no SHAPE_LINE_CHAIN implemented
    else:
        outline.CloseLastContour()
    if hasattr(area, "Hatch"):
        area.Hatch()


def insert_keepout(center_x, center_y, radius, corners=50, layer=None, hatch_type=None,
                    no_tracks=True, no_vias=True, no_pour=True):
    """ Add keepout area in mm unit. """
    import pcbnew
    frommm = pcbnew.FromMM
    insert_keepout_(frommm(center_x), frommm(center_y), frommm(radius), corners, layer, hatch_type)

from sexpdata import load, Symbol

f = load(open("./right.kicad_pcb"))
y = list(filter(lambda x: x[0] == Symbol("module") if isinstance(x, list) else False, f))
z = list(filter(lambda x: x[1] == Symbol("base:ToprePad"), y))
sws = list(map(lambda y: list(filter(lambda x: x[0] == Symbol("at") if isinstance(x, list) else False, y))[0], z))

for xy in sws:
    insert_keepout(xy[1], xy[2], 5.8)
    try:
        _, x, y, deg = xy
    except Exception:
        _, x, y = xy
        deg = 0.0
