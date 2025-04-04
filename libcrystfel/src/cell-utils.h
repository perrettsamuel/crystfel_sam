/*
 * cell-utils.h
 *
 * Unit Cell utility functions
 *
 * Copyright © 2012-2024 Deutsches Elektronen-Synchrotron DESY,
 *                       a research centre of the Helmholtz Association.
 * Copyright © 2012 Lorenzo Galli
 *
 * Authors:
 *   2009-2024 Thomas White <taw@physics.org>
 *   2012      Lorenzo Galli
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

#ifndef CELL_UTILS_H
#define CELL_UTILS_H

#include <gsl/gsl_matrix.h>

#include "cell.h"
#include "symmetry.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \file cell-utils.h
 * Unit cell utility functions.
 **/


extern double resolution(UnitCell *cell,
                         signed int h, signed int k, signed int l);

extern UnitCell *cell_rotate(UnitCell *in, struct quaternion quat);
extern UnitCell *rotate_cell(UnitCell *in, double omega, double phi,
                             double rot);

extern void cell_print(UnitCell *cell);
extern void cell_print_oneline(UnitCell *cell);
extern void cell_print_full(UnitCell *cell);

extern UnitCell *load_cell_from_pdb(const char *filename);
extern UnitCell *load_cell_from_file(const char *filename);
extern void write_cell(UnitCell *cell, FILE *fh);

extern int cell_is_sensible(UnitCell *cell);

extern int validate_cell(UnitCell *cell);

extern UnitCell *uncenter_cell(UnitCell *in, IntegerMatrix **pC,
                               RationalMatrix **pCi);

extern int bravais_lattice(UnitCell *cell);
extern int has_unique_axis(LatticeType l);
extern int right_handed(UnitCell *cell);

extern const char *str_lattice(LatticeType l);
extern LatticeType lattice_from_str(const char *s);

extern int forbidden_reflection(UnitCell *cell,
                                signed int h, signed int k, signed int l);

extern double cell_get_volume(UnitCell *cell);

extern double lowest_reflection(UnitCell *cell);

extern int compare_cell_parameters(UnitCell *cell, UnitCell *reference,
                                   const double *tols);

extern int compare_cell_parameters_and_orientation(UnitCell *cell,
                                                   UnitCell *reference,
                                                   const double *tols);

extern int compare_permuted_cell_parameters_and_orientation(UnitCell *cell,
                                                            UnitCell *reference,
                                                            const double *tols,
                                                            IntegerMatrix **pmb);

extern int compare_derivative_cell_parameters(UnitCell *cell, UnitCell *reference,
                                              const double *tols, int csl,
                                              RationalMatrix **pmb);

extern UnitCell *compare_reindexed_cell_parameters(UnitCell *cell_in,
                                                   UnitCell *reference_in,
                                                   const double *tols,
                                                   RationalMatrix **pmb);

extern SymOpList *get_lattice_symmetry(UnitCell *cell);

struct powder_ring {
	signed int h;
	signed int k;
	signed int l;
	double resolution;
	int multi;
};

extern struct powder_ring *powder_rings(UnitCell *cell,
                                        SymOpList *sym,
                                        double mres,
                                        int *n_rings);

extern void free_powder_rings(struct powder_ring *r);

extern UnitCell *impose_bravais(UnitCell *cell, LatticeType latt, char ua);

#ifdef __cplusplus
}
#endif

#endif	/* CELL_UTILS_H */
