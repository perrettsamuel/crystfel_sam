/*
 * predict-refine.h
 *
 * Prediction refinement
 *
 * Copyright © 2012-2021 Deutsches Elektronen-Synchrotron DESY,
 *                       a research centre of the Helmholtz Association.
 *
 * Authors:
 *   2010-2019 Thomas White <taw@physics.org>
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

#ifndef PREDICT_REFINE_H
#define PREDICT_REFINE_H

#include <gsl/gsl_matrix.h>

struct reflpeak;

/** Enumeration of parameters which may want to be refined */
enum gparam {
	GPARAM_A_STAR,
	GPARAM_B_STAR,
	GPARAM_C_STAR,
	GPARAM_AL_STAR,
	GPARAM_BE_STAR,
	GPARAM_GA_STAR,
	GPARAM_CELL_RX,  /* Cell rotation around +x */
	GPARAM_CELL_RY,  /* Cell rotation around +y */
	GPARAM_CELL_RZ,  /* Cell rotation around +z */
	GPARAM_DET_TX,
	GPARAM_DET_TY,
	GPARAM_DET_TZ,
	GPARAM_DET_RX,  /* Detector panel (group) rotation about +x */
	GPARAM_DET_RY,  /* Detector panel (group) rotation about +y */
	GPARAM_DET_RZ,  /* Detector panel (group) rotation about +z */
};


#include "crystal.h"
#include "crystfel-mille.h"

struct reflpeak {
	Reflection *refl;
	struct imagefeature *peak;
	double Ih;   /* normalised */
};

/* Weighting of excitation error term (m^-1) compared to position term (pixels) */
#define EXC_WEIGHT (1.0e-7)


/**
 * \file predict-refine.h
 * Prediction refinement: refinement of indexing solutions before integration.
 */

extern int refine_prediction(struct image *image, Crystal *cr,
                             Mille *mille, int max_mille_level);

extern int refine_radius(Crystal *cr, struct image *image);

extern double r_dev(struct reflpeak *rp);

extern double fs_dev(struct reflpeak *rp, struct detgeom *det);

extern double ss_dev(struct reflpeak *rp, struct detgeom *det);

extern double r_gradient(int param, Reflection *refl, UnitCell *cell,
                         double wavelength);

extern int fs_ss_gradient(int param, Reflection *refl, UnitCell *cell,
                          struct detgeom_panel *p, gsl_matrix *panel_Minv,
                          double cx, double cy, double cz,
                          float *fsg, float *ssg);

extern void crossp_norm(double c1[3], double c2[3], double u[3]);
extern void rotate3d(double vec[3], double axis[3], double ang);
extern void adjust_vector_length(double vec[3], double adj);

#endif	/* PREDICT_REFINE_H */
