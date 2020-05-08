import pcbnew
board = pcbnew.GetBoard()
mods = board.GetModules()
tracks = board.GetTracks()
for m in mods:
    x, _ = m.GetCenter()
    m.Move(pcbnew.wxPoint(- 2.0 * x, 0))
    # m.Flip(m.GetCenter())
    # m.SetOrientationDegrees(m.GetOrientationDegrees() - 180)
    m.SetOrientationDegrees(180 - m.GetOrientationDegrees())

for t in tracks:
    sx, sy = t.GetStart()
    ex, ey = t.GetEnd()
    # t.Flip(t.GetCenter())
    t.SetStart(pcbnew.wxPoint(-sx, sy))
    t.SetEnd(pcbnew.wxPoint(-ex, ey))
