/*
 * datatemplate_priv.h
 *
 * Data template structure (private parts)
 *
 * Copyright © 2019-2021 Deutsches Elektronen-Synchrotron DESY,
 *                       a research centre of the Helmholtz Association.
 *
 * Authors:
 *   2019-2021 Thomas White <taw@physics.org>
 *
 * This file is part of CrystFEL.
 *
 * CrystFEL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CrystFEL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with CrystFEL.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/* NB This file is NOT part of the public API, and should NOT
 * be installed, but rather stays in the libcrystfel source folder. */

#ifndef DATATEMPLATE_PRIV_H
#define DATATEMPLATE_PRIV_H

#include "detgeom.h"

/* Maximum number of dimensions expected in data files */
#define MAX_DIMS (16)

/* Maximum number of placeholders expected in path structure */
#define MAX_PATH_PARTS (16)

/* Maximum number of panel groups */
#define MAX_PANEL_GROUPS (512)

/* Maximum number of panel groups that can derive from one panel.
 * This isn't just a memory allocation thing.  If there are more than 99
 * children, the numbering scheme (struct detgeom_panel_group.serial) will
 * overflow and break geometry refinement in a horrible way. */
#define MAX_PANEL_GROUP_CHILDREN (64)

enum adu_per_unit
{
	ADU_PER_PHOTON,
	ADU_PER_EV
};


enum wavelength_unit
{
	WAVELENGTH_M,
	WAVELENGTH_A,
	WAVELENGTH_ELECTRON_KV,
	WAVELENGTH_ELECTRON_V,
	WAVELENGTH_PHOTON_KEV,
	WAVELENGTH_PHOTON_EV
};

#define MAX_FLAG_VALUES (16)

enum flag_value_type
{
	FLAG_NOTHING,
	FLAG_EQUAL,
	FLAG_MORETHAN,
	FLAG_LESSTHAN
};

enum peak_layout
{
	PEAK_LIST_AUTO,
	PEAK_LIST_CXI,
	PEAK_LIST_LIST3
};

/* Special values for dimension IDs */
#define DIM_FS (-1)
#define DIM_SS (-2)
#define DIM_UNDEFINED (-3)
#define DIM_PLACEHOLDER (-4)

/* Maximum number of headers to cache, in addition to anything already
 * referenced by the DataTemplate */
#define MAX_COPY_HEADERS (32)

/* Maximum number of masks per panel */
#define MAX_MASKS (8)

struct mask_template
{
	/** Location of mask data */
	char *data_location;

	/** Filename for mask data */
	char *filename;

	/** Bit mask for bad pixels
	 * (pixel is bad if any of these are set) */
	unsigned int bad_bits;

	/** Bit mask for good pixels
	 * (pixel cannot be good unless all of these are set) */
	unsigned int good_bits;

	/** If non-zero, this mask came from the top level */
	int mask_default;
};


/**
 * Represents one panel of a detector
 */
struct panel_template
{
	/** Text name for panel */
	char *name;

	/** \name Location of corner in units of the pixel size of this panel */
	/**@{*/
	double cnx;
	double cny;
	/**@}*/

	/** The offset to be applied from clen (in m) */
	double cnz_offset;

	/** Mask definitions */
	struct mask_template masks[MAX_MASKS];

	/** Location of per-pixel saturation map */
	char *satmap;
	int satmap_default;

	/** Filename for saturation map */
	char *satmap_file;
	int satmap_file_default;

	/** Mark entire panel as bad if set */
	int bad;

	/** Mark this number of edge rows as bad */
	int mask_edge_pixels;
	int mask_edge_pixels_default;

	/** Pixel size in metres */
	double pixel_pitch;
	int pixel_pitch_default;

	/** Number of detector intensity units per photon, or eV */
	double adu_scale;
	enum adu_per_unit adu_scale_unit;
	int adu_scale_default;

	/** Treat pixel as unreliable if higher than this */
	double max_adu;
	int max_adu_default;

	/** Pixels with exactly this value will be marked as bad */
	enum flag_value_type flag_types[MAX_FLAG_VALUES];
	signed int flag_values[MAX_FLAG_VALUES];
	int flag_values_default;

	/** Location of data in file (possibly with placeholders) */
	char *data;
	int data_default;

	/** Dimensions (see definitions for DIM_FS etc above) */
	signed int dims[MAX_DIMS];
	int dims_default[MAX_DIMS];

	/** \name Transformation matrix from pixel coordinates to lab frame */
	/*@{*/
	double fsx;
	double fsy;
	double fsz;
	double ssx;
	double ssy;
	double ssz;
	/*@}*/

	/** \name Position of the panel in the data block in the file. */
	/*@{*/
	int orig_min_fs;
	int orig_max_fs;
	int orig_min_ss;
	int orig_max_ss;
	/*@}*/
};


#define PANEL_WIDTH(p) ((p)->orig_max_fs - (p)->orig_min_fs + 1)
#define PANEL_HEIGHT(p) ((p)->orig_max_ss - (p)->orig_min_ss + 1)


struct dt_badregion
{
	char name[1024];
	int is_fsss;

	double min_x;
	double max_x;
	double min_y;
	double max_y;

	/* Coordinates are specified INCLUSIVELY */
	int      panel_number;
	char    *panel_name;
	int      min_fs;
	int      max_fs;
	int      min_ss;
	int      max_ss;

};


struct panel_group_template
{
	char *name;
	int n_children;
	struct panel_group_template *children[MAX_PANEL_GROUP_CHILDREN];
};


struct _datatemplate
{
	struct panel_template     *panels;
	int                        n_panels;

	struct dt_badregion       *bad;
	int                        n_bad;

	char                      *wavelength_from;
	enum wavelength_unit       wavelength_unit;

	double                     bandwidth;

	struct panel_group_template *groups[MAX_PANEL_GROUPS];
	int                          n_groups;

	char                      *peak_list;
	enum peak_layout           peak_list_type;

	/* Shift of whole detector, in m */
	char                      *shift_x_from;
	char                      *shift_y_from;

	/** Location to get detector z from, e.g. from HDF5 file */
	char                      *cnz_from;

	char                      *headers_to_copy[MAX_COPY_HEADERS];
	int                        n_headers_to_copy;
};

extern double convert_to_m(double val, int units);
extern struct detgeom *create_detgeom(struct image *image,
                                      const DataTemplate *dtempl,
                                      int no_clen_ok);

#endif	/* DATATEMPLATE_PRIV_H */
