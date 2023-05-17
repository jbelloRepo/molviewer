#include "mol.h"
#define PI 3.14159265358979323846 // define PI
#define DEBUG_OFF				  // for debugging

/************************************
******** Function Definition ********
************************************/

int atom_comp(const void *a, const void *b);
int bond_comp(const void *a, const void *b);

/******************************************
		Functions for the ATOM
******************************************/

void atomset(atom *atom, char element[3], double *x, double *y, double *z) // setter for atom
{
	//* Copy elements into struct members
	strcpy(atom->element, element); // strcpy to struct
	atom->x = *x;
	atom->y = *y;
	atom->z = *z;

#ifdef DEBUG_
	printf("The string read from char array is: %s \n", atom->element);
#endif
}

void atomget(atom *atom, char element[3], double *x, double *y, double *z)
{
	//* Copy elements into struct members
	if (atom != NULL)
	{
		strcpy(element, atom->element); // strcpy to struct
		*x = atom->x;
		*y = atom->y;
		*z = atom->z;
	}
}

/******************************************
		Functions for the molecule
******************************************/

molecule *molmalloc(unsigned short atom_max, unsigned short bond_max) // dym. allocate space for the molecule -> return pointer of type malloc
{
	molecule *molecule_ptr = (molecule *)malloc(sizeof(molecule)); //* allocate molecule on the heap

#ifdef DEBUG_
	printf("address of molecule_ptr in header file : %p\n", (void *)molecule_ptr);
#endif

	if (molecule_ptr != NULL) //* allocate space for "atoms" array & atom_ptrs
	{
		molecule_ptr->atom_max = atom_max;
		molecule_ptr->atom_no = 0;									   // set to 0 initially
		molecule_ptr->atoms = (atom *)malloc(atom_max * sizeof(atom)); // unsigned short vs int

#ifdef DEBUG_
		printf("address of molecule_ptr->atoms in header file : %p\n", (void *)molecule_ptr->atoms); //! error checking
#endif
		molecule_ptr->atom_ptrs = (atom **)malloc(atom_max * sizeof(atom *));
		molecule_ptr->bond_max = bond_max;
		molecule_ptr->bonds = (bond *)malloc(bond_max * sizeof(bond));
		molecule_ptr->bond_ptrs = (bond **)malloc(bond_max * sizeof(bond *));
		molecule_ptr->bond_no = 0; // set to 0 initially
	}
	else
	{
		return NULL;
	}
	// molecule *mol_malloced_address = (molecule *)malloc(sizeof(molecule)); // pointer to molecule
	return molecule_ptr;
}

//! Add NULL for malloc if there is no space FIXME: Add NUL
void molappend_atom(molecule *molecule, atom *atom) // FIXME: after re-allocating you have to reassign the atom pointer, same for the bonds
{
	struct atom *a1, **a2;

	if (molecule->atom_no == molecule->atom_max) //* check atom_no & atom_max before appending to array
	{
		if (molecule->atom_max == 0) //! TEST THIS when atom_max = 0
		{
			molecule->atom_max += 1; //* add 1 if atom_max = 0
		}
		else
		{
			molecule->atom_max *= 2; //* double size of atom_max
		}

		/* Reallocating Memory (ATOM) */

#ifdef DEBUG_ON
		printf("\n==================================== [MOL.H] This is for molappend_atom() ==================================\n");
		printf("The (ATOM) array is now full. REALLOCATING!!!....\n"); //! error checking
#endif
																	   // *(molecule->atoms) = (atom *)realloc(molecule->atoms, molecule->atom_max);
		a1 = (struct atom *)realloc(molecule->atoms, molecule->atom_max * sizeof(struct atom)); // you cannot use atom* here

#ifdef DEBUG_ON
		printf("Address pointed to by {molecule->atoms} (old memory address): %p\n", (void *)molecule->atoms);	  //! error checking
		printf("Address pointed to by {temp a1} in header file (new memory address #REALLOC): %p\n", (void *)a1); //! error checking
#endif

		if (a1 == NULL) // exit if realloc fails
		{
			exit(EXIT_FAILURE);

#ifdef DEBUG_ON
			printf("No additional heap for atoms *atom \n");
#endif
		}

		else
		{
			molecule->atoms = a1;

#ifdef DEBUG_ON
			printf("Address assigned by {temp a1} to {molecule->atoms} in header file (new memory address #REALLOC): %p\n", (void *)molecule->atoms); //! error checking
#endif
		}

		a2 = (struct atom **)realloc(molecule->atom_ptrs, molecule->atom_max * sizeof(struct atom *));

#ifdef DEBUG_ON
		printf("-----------------------------------------------------------------------------------------------------------\n");
		printf("Address pointed to by {molecule->atom_ptr} (old memory address): %p\n", (void *)molecule->atom_ptrs); //! error checking
		printf("Address pointed to by {temp a2} in header file (new memory address #REALLOC) : %p\n", (void *)a2);	  //! error checking
#endif
		if (a2 == NULL) // exit if realloc fails
		{
			exit(EXIT_FAILURE);

#ifdef DEBUG_ON
			printf("No additional heap for atom **atoms \n");
#endif
		}
		else
		{
			molecule->atom_ptrs = a2;

#ifdef DEBUG_ON
			printf("Address assigned by {temp a2} to {molecule->atom_ptrs} in header file(new memory address #REALLOC) : %p\n", (void *)molecule->atom_ptrs); //! error checking
#endif
		}

		for (int i = 0; i < molecule->atom_no; i++)
		{
			molecule->atom_ptrs[i] = &molecule->atoms[i]; // each pointer points to their respective indicies
		}

#ifdef DEBUG_ON
		printf("-----------------------------------------------------------------------------------------------------------\n");
		printf("Address pointed of index[0] in atoms (after re-alloc): %p\n", (void *)&molecule->atoms[0]);			 //! error checking
		printf("Address pointed to by {molecule->atom_ptr} (after re-alloc): %p\n", (void *)molecule->atom_ptrs[0]); //! error checking
																													 // printf("Address pointed of index[1] in atoms (after re-alloc): %p\n", (void *)&molecule->atoms[1]);			 //! error checking
																													 // printf("Address pointed to by {molecule->atom_ptr} (after re-alloc): %p\n", (void *)molecule->atom_ptrs[1]); //! error checking
#endif

		// free(a1); //? FREE THIS ?
		// free(a2);
	}

	if ((molecule->atom_no > 0) && (molecule->atom_no < molecule->atom_max)) //* check atom_no is greater than 0 and lesser than atom_max
	{
		strcpy(molecule->atoms[molecule->atom_no].element, atom->element);
		molecule->atoms[molecule->atom_no].x = atom->x;
		molecule->atoms[molecule->atom_no].y = atom->y;
		molecule->atoms[molecule->atom_no].z = atom->z;
		molecule->atom_ptrs[molecule->atom_no] = &molecule->atoms[molecule->atom_no]; // pointer to the first atom
		molecule->atom_no += 1;														  // increment after addition
	}

	if ((molecule->atom_no == 0) && (molecule->atom_max != 0)) //* check if the molecule is initially empty ( executes for first element only)
	{
		//! index is static for first element in array[0]
		strcpy(molecule->atoms[0].element, atom->element);
		molecule->atoms[0].x = atom->x;
		molecule->atoms[0].y = atom->y;
		molecule->atoms[0].z = atom->z;
		molecule->atom_ptrs[0] = &molecule->atoms[0]; // pointer to the first atom
		molecule->atom_no += 1;						  // increment after adding to atoms

#ifdef DEBUG_ON
		printf("address pointed to by molecule->atom_ptrs[0] in header file : %p\n", (void *)molecule->atom_ptrs[0]); //! error checking
		printf("The value of atom_max is %d \n", molecule->atom_max);
		printf("The number of bonds currently in the molecule is: %d \n", molecule->bond_no);
		printf("===============================================================================================================\n");
#endif
	}
}

/******************************************
		Functions for the bond
*******************************************/
//! UPDATED FOR A2
void bondset(bond *bond, unsigned short *a1, unsigned short *a2, atom **atoms, unsigned char *epairs) // setter for the bond variable
{
	bond->a1 = *(a1);
	bond->a2 = *(a2);
	bond->epairs = *(epairs);
	bond->atoms = *(atoms);

#ifdef DEBUG_
	printf("\n==================================== [MOL.C] This is for bondset() =======================================\n");
	printf("The value of a1, a2 and eparis is:  %d, %d, %d\n", bond->a1, bond->a2, bond->epairs);
	printf("The first atom in the atom array is:  %s\n", bond->atoms[0].element);
	printf("The second atom in the atom array is: %s\n", bond->atoms[1].element);
	printf("The third atom in the atom array is:  %s\n", bond->atoms[2].element);
	// printf("Address pointed to by bond->a2 in the header file: %p\n", (void *)bond->a2);
#endif

	compute_coords(bond); // function call to set other struct members

	// printf("The values are %p \n", (void *)(bond->atoms));
}
//! UPDATED FOR A2
void bondget(bond *bond, unsigned short *a1, unsigned short *a2, atom **atoms, unsigned char *epairs) // getter for the bond variable
{
	*a1 = (bond->a1);
	*a2 = (bond->a2);
	*atoms = bond->atoms;
	*epairs = (bond->epairs);
}

// void bondget(bond *bond, atom **a1, atom **a2, unsigned char *epairs)
// {
// 	*a1 = (bond->a1); //*a1 yields pointer to atom; (bond->a1) returns pointer to atom
// 	*a2 = (bond->a2);
// 	*epairs = bond->epairs;
// }

void molappend_bond(molecule *molecule, bond *bond) // appends the bonds
{													//! bond_no initially set to 0

	struct bond *b1, **b2; // TEMP variables

#ifdef DEBUG_
	printf("\n==================================== [MOL.H] This is for molappend_bond() =======================================\n");
	printf("The value of bond_no and bond_max is: %d , %d\n", molecule->bond_no, molecule->bond_max); //! error checking
#endif

	if (molecule->bond_no == molecule->bond_max) //* check if bond_no and bond_max are equal
	{
		if (molecule->bond_max == 0) //! TEST THIS when bond_max = 0
		{
			molecule->bond_max += 1; //* add 1 if bond_max = 0
		}
		else
		{
			molecule->bond_max *= 2; //* double size of bond_max
		}

		/* Reallocating Memory (BOND) */

#ifdef DEBUG_
		printf("\n==================================== [MOL.H] This is for molappend_bond() =======================================\n");
		printf("The (BOND) array is now full. REALLOCATING!!!....\n"); //! error checking
#endif

		b1 = (struct bond *)realloc(molecule->bonds, molecule->bond_max * sizeof(struct bond)); // you cannot use atom* here

#ifdef DEBUG_ON
		printf("Address pointed to by {molecule->bonds} (old memory address): %p\n", (void *)molecule->bonds);	  //! error checking
		printf("Address pointed to by {temp b1} in header file (new memory address #REALLOC): %p\n", (void *)b1); //! error checking
#endif

		if (b1 == NULL) //! add more statements or return 0 --CHECK THIS
		{
			exit(EXIT_FAILURE);

#ifdef DEBUG_ON
			printf("No additional heap for atoms *atom \n");
#endif
		}

		else
		{
			molecule->bonds = b1;

#ifdef DEBUG_ON
			printf("Address assigned by {temp b1} to {molecule->bonds} in header file (new memory address #REALLOC): %p\n", (void *)molecule->bonds); //! error checking
#endif
		}

		b2 = (struct bond **)realloc(molecule->bond_ptrs, molecule->bond_max * sizeof(struct bond *));

#ifdef DEBUG_ON
		printf("-----------------------------------------------------------------------------------------------------------\n");
		printf("Address pointed to by {molecule->bond_ptr} (old memory address): %p\n", (void *)molecule->bond_ptrs); //! error checking
		printf("Address pointed to by {temp b2} in header file (new memory address #REALLOC): %p\n", (void *)b2);	  //! error checking
#endif

		if (b2 == NULL) //! add more statements or return 0 --CHECK THIS
		{
			exit(EXIT_FAILURE);

#ifdef DEBUG_ON
			printf("No additional heap for atom **atoms \n");
#endif
		}
		else
		{
			molecule->bond_ptrs = b2;

#ifdef DEBUG_ON
			printf("Address assigned by {temp b2} to molecule->bond_ptrs in header file(new memory address #REALLOC) : %p \n\n", (void *)molecule->bond_ptrs); //! error checking
#endif
		}

		for (int i = 0; i < molecule->bond_no; i++)
		{
			molecule->bond_ptrs[i] = &molecule->bonds[i];
		}

#ifdef DEBUG_ON
		printf("-----------------------------------------------------------------------------------------------------------\n");
		printf("Address pointed of index[0] in atoms (after re-alloc): %p\n", (void *)&molecule->bonds[0]);			 //! error checking
		printf("Address pointed to by {molecule->atom_ptr} (after re-alloc): %p\n", (void *)molecule->bond_ptrs[0]); //! error checking
#endif
	}

	if ((molecule->bond_no > 0) && (molecule->bond_no < molecule->bond_max)) //* check bond_no is greater than 0 and lesser than bond_max
	{
#ifdef DEBUG_
		printf("\n==================================== [MOL.H] This is for molappend_bond() =======================================\n");
		printf("EXE: When bond_no is greater than 0 and less than BOND MAX \n"); //! error checking
		printf("The value of bond_no and bond_max is: %d , %d\n", molecule->bond_no, molecule->bond_max);
#endif
		molecule->bonds[molecule->bond_no].a1 = bond->a1; //* assign the first member from parameter passed to molecule
		molecule->bonds[molecule->bond_no].a2 = bond->a2;
		molecule->bonds[molecule->bond_no].epairs = bond->epairs;
		molecule->bond_ptrs[molecule->bond_no] = &molecule->bonds[molecule->bond_no]; //! CHECK THIS
		//! Updated for A2
		molecule->bonds[molecule->bond_no].atoms = bond->atoms;
		molecule->bonds[molecule->bond_no].x1 = bond->x1;
		molecule->bonds[molecule->bond_no].x2 = bond->x2;
		molecule->bonds[molecule->bond_no].y1 = bond->y1;
		molecule->bonds[molecule->bond_no].y2 = bond->y2;
		molecule->bonds[molecule->bond_no].dx = bond->dx;
		molecule->bonds[molecule->bond_no].dy = bond->dy;
		molecule->bonds[molecule->bond_no].len = bond->len;
		molecule->bonds[molecule->bond_no].z = bond->z;
		//!
		molecule->bond_no += 1; // increment after addition
	}

	if ((molecule->bond_no == 0) && (molecule->bond_max != 0)) //* check if the molecule is initially empty (executes for first element only)
	{
//! index is static for first element in array[0]
#ifdef DEBUG_
		printf("\n==================================== [MOL.H] This is for molappend_bond() =======================================\n");
		printf("EXE: When there is no bond in the BOND ARRAY --> Append bond to index [0] in the array \n"); //! error checking
#endif
		molecule->bonds[0].a1 = bond->a1; //* assign the first member from parameter passed to molecule
		molecule->bonds[0].a2 = bond->a2;
		molecule->bonds[0].epairs = bond->epairs;
		molecule->bond_ptrs[0] = &molecule->bonds[0]; //! CHECK THIS
		//! Updated for A2
		molecule->bonds[0].atoms = bond->atoms;
		molecule->bonds[0].x1 = bond->x1;
		molecule->bonds[0].x2 = bond->x2;
		molecule->bonds[0].y1 = bond->y1;
		molecule->bonds[0].y2 = bond->y2;
		molecule->bonds[0].dx = bond->dx;
		molecule->bonds[0].dy = bond->dy;
		molecule->bonds[0].len = bond->len;
		molecule->bonds[0].z = bond->z;
		//!
		molecule->bond_no += 1;

#ifdef DEBUG_ON
		printf("Address pointed to by molecule->bond_ptrs[0] in header file : %p\n", (void *)molecule->bond_ptrs[0]); //! error checking
		printf("The value of bond_max is %d \n", molecule->bond_max);
		printf("The number of bonds currently in the molecule is: %d \n", molecule->bond_no);
		// printf("===========================================================================================================\n");
#endif
	}
}

/******************************************
		Additional functions
*******************************************/

int atom_comp(const void *a, const void *b) // compare atom pointers
{
	// pass in an array of type pointer to atom

	double double_a, double_b;
	atom *atom_a, *atom_b;

	atom_a = *(atom **)(a); // de-reference a to get the atom pointed to by the pointer
	double_a = atom_a->z;	// get the value pointed to by a (which is the z value)

	atom_b = *(atom **)(b);
	double_b = atom_b->z;

#ifdef DEBUG_
	printf("The value of a is: %lf \n", double_a);
	printf("The value of b is: %lf \n", double_b);
#endif

	if (double_a == double_b)
	{
		return 0;
	}
	else if (double_a > double_b)
	{
		return 1;
	}
	else
	{
		return -1;
	}

	// return (double_a - double_b);
}

int bond_comp(const void *a, const void *b)
{
	double double_a, double_b;
	bond *bond_a, *bond_b;

	//! Updated for A2
	bond_a = *(bond **)(a); // de-reference a to get the bond pointed to by the pointer
	double_a = bond_a->z;	//(bond_a->a1->z + bond_a->a2->z) / 2; // get the value pointed to by a (which is the z value)

	bond_b = *(bond **)(b);
	double_b = bond_b->z; //(bond_b->a1->z + bond_b->a2->z) / 2;

#ifdef DEBUG_
	printf("The value of a is: %lf \n", double_a);
	printf("The value of b is: %lf \n", double_b);
#endif

	if (double_a == double_b)
	{
		return 0;
	}
	else if (double_a > double_b)
	{
		return 1;
	}
	else
	{
		return -1;
	}
	// return (double_a - double_b);
}

void molsort(molecule *molecule) // This function sorts an array of pointers (pointing to atoms in an array)
{
#ifdef DEBUG_
	printf("\n==================================== [MOL.C] This is for molsort() [atom_ptrs] ==============================\n");
	printf("The number of atoms in the molecule is %d\n", molecule->atom_no);
	printf("BEFORE atom_ptr is sorted:");
	for (int i = 0; i < molecule->atom_no; i++)
	{
		if (i == 0)
		{
			printf(" %p:[%lf]", (void *)molecule->atom_ptrs[i], molecule->atom_ptrs[i]->z);
		}
		else
		{
			printf(" %p:[%lf]", (void *)molecule->atom_ptrs[i], molecule->atom_ptrs[i]->z);
		}
	}
	printf("\n");
#endif

	qsort(molecule->atom_ptrs, molecule->atom_no, sizeof(atom *), atom_comp); // sort the atom_ptrs

#ifdef DEBUG_
	printf("AFTER  atom_ptr is sorted:");
	for (int i = 0; i < molecule->atom_no; i++)
	{
		if (i == 0)
		{
			printf(" %p:[%lf]", (void *)molecule->atom_ptrs[i], molecule->atom_ptrs[i]->z);
		}
		else
		{
			printf(" %p:[%lf]", (void *)molecule->atom_ptrs[i], molecule->atom_ptrs[i]->z);
		}
	}
	printf("\n");
#endif

#ifdef DEBUG_
	printf("\n==================================== [MOL.C] This is for molsort() [bond_ptrs] ==============================\n");
	printf("The number of bonds in the molecule is %d\n", molecule->bond_no);
	printf("BEFORE bond_ptr is sorted:");
	for (int i = 0; i < molecule->bond_no; i++)
	{
		if (i == 0)
		{
			printf(" %p:[%lf]", (void *)molecule->bond_ptrs[i], molecule->bond_ptrs[i]->z);
		}
		else
		{
			printf(" %p:[%lf]", (void *)molecule->bond_ptrs[i], molecule->bond_ptrs[i]->z);
		}
	}
	printf("\n");
#endif

	qsort(molecule->bond_ptrs, molecule->bond_no, sizeof(bond *), bond_comp); // sort the bond_ptrs

#ifdef DEBUG_
	printf("AFTER  bond_ptr is sorted:");
	for (int i = 0; i < molecule->bond_no; i++)
	{
		if (i == 0)
		{
			printf(" %p:[%lf]", (void *)molecule->bond_ptrs[i], molecule->bond_ptrs[i]->z);
		}
		else
		{
			printf(" %p:[%lf]", (void *)molecule->bond_ptrs[i], molecule->bond_ptrs[i]->z);
		}
	}
	printf("\n====END==== \n");
#endif
}

void xrotation(xform_matrix xform_matrix, unsigned short deg)
{
	//* the xform_matrix act as a pointer since an array is passed

	//  ---           ---
	// | 1,    0,    0   |
	// | 0, cos(), -sin()|
	// | 0, sin(), cos() |
	//  ---           ---

	double rad = deg * (PI / 180.0);
	xform_matrix[0][0] = 1;
	xform_matrix[0][1] = 0;
	xform_matrix[0][2] = 0;
	xform_matrix[1][0] = 0;
	xform_matrix[1][1] = cos(rad);
	xform_matrix[1][2] = -sin(rad);
	xform_matrix[2][0] = 0;
	xform_matrix[2][1] = sin(rad);
	xform_matrix[2][2] = cos(rad);
}
void yrotation(xform_matrix xform_matrix, unsigned short deg)
{
	//  ---           ---
	// | cos(), 0, sin() |
	// |    0   1,   0   |
	// |-sin(), 0, cos() |
	//  ---           ---

	double rad = deg * (PI / 180.0);
	xform_matrix[0][0] = cos(rad);
	xform_matrix[0][1] = 0;
	xform_matrix[0][2] = sin(rad);
	xform_matrix[1][0] = 0;
	xform_matrix[1][1] = 1;
	xform_matrix[1][2] = 0;
	xform_matrix[2][0] = -sin(rad);
	xform_matrix[2][1] = 0;
	xform_matrix[2][2] = cos(rad);
}
void zrotation(xform_matrix xform_matrix, unsigned short deg)
{
	//  ---             ---
	// | cos(), -sin(),  0 |
	// | sin()   cos(),  0 |
	// |   0,      0,    1 |
	//  ---           ---

	double rad = deg * (PI / 180.0);
	xform_matrix[0][0] = cos(rad);
	xform_matrix[0][1] = -sin(rad);
	xform_matrix[0][2] = 0;
	xform_matrix[1][0] = sin(rad);
	xform_matrix[1][1] = cos(rad);
	xform_matrix[1][2] = 0;
	xform_matrix[2][0] = 0;
	xform_matrix[2][1] = 0;
	xform_matrix[2][2] = 1;
}

void mol_xform(molecule *molecule, xform_matrix matrix)
{
	// apply matrix vector multiplication
	double x, y, z; // position of atom relative to molecule

#ifdef DEBUG_ON
	printf("\n==================================== [MOL.C] This is for mol_xform() ==============================\n");
	for (int i = 0; i < molecule->atom_no; i++)
	{
		printf("BFR MATRIX TRANSFORM: %lf, %lf, %lf\n", molecule->atoms[i].x, molecule->atoms[i].y, molecule->atoms[i].z);
	}
#endif

	for (int i = 0; i < molecule->atom_no; i++) // apply transformation to x,y,z values in molecule
	{
		x = molecule->atoms[i].x;
		y = molecule->atoms[i].y;
		z = molecule->atoms[i].z;

		molecule->atoms[i].x = (matrix[0][0] * x) + (matrix[0][1] * y) + (matrix[0][2] * z);
		molecule->atoms[i].y = (matrix[1][0] * x) + (matrix[1][1] * y) + (matrix[1][2] * z);
		molecule->atoms[i].z = (matrix[2][0] * x) + (matrix[2][1] * y) + (matrix[2][2] * z);
	}

	for (int i = 0; i < molecule->bond_no; i++)
	{
		compute_coords(&(molecule->bonds[i]));
	}

#ifdef DEBUG_ON
	printf("\n==================================== [MOL.C] This is for mol_xform() ==============================\n");
	for (int i = 0; i < molecule->atom_no; i++)
	{
		printf("ATR MATRIX TRANSFORM: %lf, %lf, %lf\n", molecule->atoms[i].x, molecule->atoms[i].y, molecule->atoms[i].z);
	}
#endif
}

void molfree(molecule *ptr)
{
	free(ptr->atoms);
	free(ptr->atom_ptrs);
	free(ptr->bonds);
	free(ptr->bond_ptrs);
	free(ptr);
}

molecule *molcopy(molecule *src)
{
	atom anAtom;		// an atom variable
	bond aBond;			// a bond variable
	molecule *moleCopy; // molecule variable

	unsigned short atom_max = src->atom_max, bond_max = src->bond_max;

	// printf("\nThe atom max and bond max: %d %d\n", atom_max, bond_max);
	moleCopy = molmalloc(atom_max, bond_max); // dym allocate space for mole copy

	// set strcut members in molecule
	moleCopy->atom_max = src->atom_max;
	// moleCopy->atom_no = src->atom_no; //! FIXME -- do not set this
	moleCopy->bond_max = src->bond_max;
	// moleCopy->bond_no = src->bond_no; //! FIXME -- do not set this

	// append atoms in atom arrays
	for (int i = 0; i < src->atom_no; i++)
	{
		//? Create the atom at array index
		strcpy(anAtom.element, src->atoms[i].element);
		anAtom.x = src->atoms[i].x;
		anAtom.y = src->atoms[i].y;
		anAtom.z = src->atoms[i].z;

#ifdef DEBUG_ON
		printf("\n==MOLCOPY== Adding atom to the molecule copy\n");
#endif

		molappend_atom(moleCopy, &anAtom); // append all atoms to molcopy

#ifdef DEBUG_ON
		printf("==MOLCOPY== A new atom has been added. TOTAL COUNT: %d \n", i + 1);
#endif
	}

	for (int i = 0; i < src->bond_no; i++)
	{
		//? Create the bond at bond index
		aBond.epairs = src->bonds[i].epairs;
		aBond.a1 = src->bonds[i].a1;
		aBond.a2 = src->bonds[i].a2;

		//! Updated for A2
		aBond.atoms = src->bonds[i].atoms;
		aBond.x1 = src->bonds[i].x1;
		aBond.x2 = src->bonds[i].x2;
		aBond.y1 = src->bonds[i].y1;
		aBond.y2 = src->bonds[i].y2;
		aBond.dx = src->bonds[i].dx;
		aBond.dy = src->bonds[i].dy;
		aBond.len = src->bonds[i].len;
		aBond.z = src->bonds[i].z;

		molappend_bond(moleCopy, &aBond); // append all bonds
	}
	return moleCopy;
}

//! Updated for A2
void compute_coords(bond *bond)
{
	bond->x1 = bond->atoms[bond->a1].x;
	bond->y1 = bond->atoms[bond->a1].y;
	bond->x2 = bond->atoms[bond->a2].x;
	bond->y2 = bond->atoms[bond->a2].y;
	bond->z = ((bond->atoms[bond->a1].z + bond->atoms[bond->a2].z)) / 2;
	bond->len = sqrt((pow(bond->x2 - bond->x1, 2)) + (pow(bond->y2 - bond->y1, 2)));
	bond->dx = (bond->x2 - bond->x1) / bond->len;
	bond->dy = (bond->y2 - bond->y1) / bond->len;

#ifdef DEBUG_
	printf("\n==================================== [MOL.H] This is for compute_coords()-- AFTER =======================================\n");
	printf("The value of a1, a2 is: %d, %d\n", bond->a1, bond->a2);
	printf("The value of x1, y1 is: %lf, %lf \n", bond->atoms[bond->a1].x, bond->atoms[bond->a1].y);
	printf("The value of x2, y2 is: %lf, %lf \n", bond->atoms[bond->a2].x, bond->atoms[bond->a2].y);
#endif
}

/******************************************
	Functions for the NIGHTMARE MODE
******************************************/

rotations *spin(molecule *mol)
{
	rotations *rots = (rotations *)malloc(sizeof(rotations)); // allocate space for struct

	// create 72 new molecules and rotate them by 5 degrees each around x, y, and z axis
	for (int i = 0; i < 72; i++)
	{
		xform_matrix xrot;					// declare x-rotation matrix
		molecule *xmolecule = molcopy(mol); // create an x mol var
		xrotation(xrot, 5 * i);				// apply 5 degree inc. to mat.
		mol_xform(xmolecule, xrot);			// apply matrix to molecule
		molsort(xmolecule);
		rots->x[i] = xmolecule; // store molecule

		xform_matrix yrot;
		molecule *ymolecule = molcopy(mol);
		yrotation(yrot, 5 * i);
		mol_xform(ymolecule, yrot);
		molsort(ymolecule);
		rots->y[i] = ymolecule;

		xform_matrix zrot;
		molecule *zmolecule = molcopy(mol);
		zrotation(zrot, 5 * i);
		mol_xform(zmolecule, zrot);
		molsort(zmolecule);
		rots->z[i] = zmolecule;
	}

	return rots;

	// sort the molecules
	// 1. when do you sort?
}

void rotationsfree(rotations *rot)
{
	if (rot == NULL)
	{
		return;
	}

	for (int i = 0; i < 72; i++)
	{
		free(rot->x[i]);
		free(rot->y[i]);
		free(rot->z[i]);
	}

	free(rot);
}

