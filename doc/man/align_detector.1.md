% align_detector(1)

NAME
====

align_detector - refine detector geometry


SYNOPSIS
========

align_detector -g _input.geom_ -o _output.geom_ -l _level_ [--out-of-plane] _millepede-files_


DESCRIPTION
===========

**align_detector** refines the detector geometry based on the calibration data
written by **indexamajig**.  The refinement takes into account all the
inter-dependencies between crystal orientations, cell parameters and the panel
positions, as if a single large minimisation had been performed with all frames
at once.  The algorithm is nevertheless very fast - this is achieved using the
Millepede-II algorithm.  For more information, see
https://www.desy.de/~kleinwrt/MP2/doc/html/index.html

To refine the detector geometry, first make sure that the geometry file
includes hierarchy information.  For this, see **man crystfel_geometry**,
section **Detector hierarchy**.

Next, run **indexamajig** as usual, but with option **--mille**.  This will
produce an additional output file with default name **mille-data.bin** (use
**--mille-file** to set a different location).  This file contains calibration
data that can be read by **align_detector** and **show_residuals**.

Finally, run **align_detector**, giving it the input geometry file, the "Mille"
files, a refinement level and a filename for the updated geometry file.  The
input geometry file must match the file used for the indexamajig run.

Refinement level **0** allows only the overall detector position to vary.
Higher levels allow groups of panels to move according to the hierarchy.  For
example, for a CSPAD detector with hierarchy defined as in the example files,
level **1** would refine the quadrant positions, **2** would refine the 2-by-1
panel "dominoes", and level **3** would refine the individual (roughly square)
ASICs.  Higher refinement levels will generally require more and
higher-resolution diffraction data.

You can use **indexamajig** option **--max-mille-level** to reduce the size of
the calibration data files, at the cost of limiting the maximum refinement
level.

The default behaviour is to refine only the position components in the x-y
plane, perpendicular to the beam.  In favourable circumstances, you can add
option **--out-of-plane** to refine the panel **translations** out of this
plane. However, be aware that this introduces additional cross-dependencies
and is less stable.  Add option **--out-of-plane-tilts** to additionally
refine the panel **tilts** out of the x-y plane, which is even less stable.

The overall detector z-position (camera length) is usually quite strongly
correlated with the crystal cell parameters, making it difficult to refine.
With high-resolution data, it can be possible, however.  To refine the overall
camera length, add **--camera-length**.  Without this option, the overall
camera length will not be altered, even with **--out-of-plane**.  Instead, only
the z-positions of the panels relative to one another will be refined.

**align_detector** relies on the program **pede** from the Millepede-II
package.  Usually, this will be installed as part of the CrystFEL installation
procedure.  If not, it can easily be installed from the Millepede-II repository
at https://gitlab.desy.de/claus.kleinwort/millepede-ii


OPTIONS
=======

**-g** _input.geom_, **--geometry**=_input.geom_
: Specify the input geometry filename.

**-o** _output.geom_, **--output**=_output.geom_
: Specify the output geometry filename.
: Note that the geometry file will be re-written, meaning that any formatting
: and comments will be lost.

**-l** _level_, **--level**=_level_
: Specify the refinement level.  **-l 0** refines the overall detector position
: only.  The maximum refinement level is determined by the hierarchy of the
: detector.

**--out-of-plane**
: Additionally refine out-of-plane panel translations, relative to one another
: (see **--camera-length**).

**--out-of-plane-tilts**
: Additionally refine out-of-plane panel tilts.

**--camera-length**
: Additionally refine the overall camera length.

**--panel-totals**
: Display the total translation and rotation changes for each panel. This does
: not affect the refinement or the output geometry file in any way, only the
: console output.


AUTHOR
======

This page was written by Thomas White.


REPORTING BUGS
==============

Report bugs to <taw@physics.org>, or visit <http://www.desy.de/~twhite/crystfel>.


COPYRIGHT AND DISCLAIMER
========================

Copyright © 2023-2025 Deutsches Elektronen-Synchrotron DESY, a research centre
of the Helmholtz Association.

align_detector, and this manual, are part of CrystFEL.

CrystFEL is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

CrystFEL is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
CrystFEL.  If not, see <http://www.gnu.org/licenses/>.


SEE ALSO
========

**crystfel**(7), **indexamajig**(1), **adjust_detector**(1), **show_residuals**(1)
**crystfel_geometry**(5)
