#include "mol.h"

int main(int argc, char **argv)
{
    double x, y, z;          // position of atom relative to molecule
    atom a1, a2, a3;         // variable for atoms of type (atom)
    bond bond1, bond2;       // variable for bond of type (bond)
    molecule *molecule, *mx; // (pointer) variable for molecule of type (molecule)

    /* set the atoms */
    x = 2.5369;
    y = -0.1550;
    z = -0.074;
    atomset(&a1, "O", &x, &y, &z); // set struct variables for a1

    x = 3.0739;
    y = 0.1550;
    z = -2.4456;
    atomset(&a2, "H", &x, &y, &z); // set struct variables for a2

    x = 2.0000;
    y = 0.1550;
    z = -0.443;
    atomset(&a3, "H", &x, &y, &z); // set struct variables for a3

    molecule = molmalloc(0, 0);
    // printf("address of molecule_ptr in main file: %p\n", (void *)molecule);
    // printf("The atom max is: %d \n", molecule->atom_max);
    // printf("The bond max is: %d \n", molecule->bond_max);

    int i = 0; // index for the loop
    int j = 0; // index for the elements[0];
    do
    {
        molappend_atom(molecule, &a1);
        printf("\n");
        printf("==================================== [MAIN.C] This is for molappend_atom() ==============================\n");
        printf("The atom max is: %d \n", molecule->atom_max);
        printf("After the molappend_atom, the atom in the 0 index is: %s \n", molecule->atoms[j].element);
        j += 1;
        printf("After the molappend_atom, the number of atoms in atoms is: %hu \n", molecule->atom_no);

        molappend_atom(molecule, &a2);

        printf("The atom max is: %d \n", molecule->atom_max);
        printf("After the molappend_atom, the atom in the 1 index is: %s \n", molecule->atoms[j].element);
        j += 1;
        printf("After the molappend_atom, the number of atoms in atoms is: %hu \n", molecule->atom_no);

        molappend_atom(molecule, &a3);
        printf("The atom max is: %d \n", molecule->atom_max);
        printf("After the molappend_atom, the atom in the 2 index is: %s \n", molecule->atoms[j].element);
        j += 1;
        printf("After the molappend_atom, the number of atoms in atoms is: %hu \n", molecule->atom_no);

        i += 1;
    } while (i < 4); //? Tested with i<16000; at which point atom max = 49152

    //! Updated for A2
    unsigned short bondIndex00 = 0;
    unsigned short bondIndex01 = 1;
    unsigned short bondIndex02 = 2;
    unsigned char bondEpair = 1;

    bondset(&bond1, &(bondIndex00), &(bondIndex01), &(molecule->atoms), &(bondEpair));
    bondset(&bond2, &(bondIndex00), &(bondIndex02), &(molecule->atoms), &(bondEpair));

    printf("\nThe mem address of molecule->atoms is: %p \n", (void *)(molecule->atoms));
    printf("The mem address of atom array in bond is: %p \n", (void *)(bond1.atoms));
    
    molappend_bond(molecule, &bond1);
    molappend_bond(molecule, &bond2);
    printf("\nThe mem address of molecule->bonds [0] is: %p \n", (void *)&(molecule->bonds[0]));
    printf("The mem address of molecule->bonds [1] is: %p \n", (void *)&(molecule->bonds[1]));

    // printf("The mem address atoms in molecule bonds is: %s \n", (molecule->bonds[0].atoms->element));

    bond *bond_ptr1, *bond_ptr2;
    unsigned short b_a1, b_a2;
    unsigned char b_epairs;
    atom *bond_atoms;

    bond_ptr1 = molecule->bonds;
    bond_ptr2 = molecule->bonds+1;
    printf("Address pointed to by bond_ptr1: %p\n", (void *)bond_ptr1);
    printf("Address pointed to by bond_ptr2: %p\n", (void *)bond_ptr2);

    bondget(bond_ptr1, &(b_a1), &(b_a2), &bond_atoms, &b_epairs);
    // printf("The values are %d, %d, %d, %p \n", b_a1, b_a2, b_epairs, (void *)(bond_ptr1->atoms));

    bondget(bond_ptr2, &(b_a1), &(b_a2), &bond_atoms, &b_epairs);
    printf("The values are %d, %d, %d \n", b_a1, b_a2, b_epairs);


    
    // printf("bond[0] %p: %d %d %c \n", (void *)bond_ptr1, b_a1, b_a2, b_epairs);

    // bondset(&bond1, &(molecule->atoms[0]), &(molecule->atoms[1]), 1);
    // bondset(&bond2, &(molecule->atoms[0]), &(molecule->atoms[2]), 1);

    // // printf("\n==================================== [MAIN.C] This is for bondset() =======================================\n");
    // // printf("Address pointed to by molecule->atoms[0] & (molecule->atoms[1]: %p & %p \n", (void *)&(molecule->atoms[0]), (void *)&(molecule->atoms[1]));   //! error checking
    // // printf("Address pointed to by molecule->atoms[0] & (molecule->atoms[2]: %p & %p \n\n", (void *)&(molecule->atoms[0]), (void *)&(molecule->atoms[2])); //! error checking

    /* add the bonds */
    int k = 0;
    int l = 0;
    do
    {

        molappend_bond(molecule, &bond1);
        printf("==================================== [MAIN.C] This is for molappend_bond() ==============================\n");
        printf("The bond max is: %d \n", molecule->bond_max);
        // printf("After the molappend_bond, the bond at the 0 index is: %p \n", (void *)(molecule->bonds->a1));
        printf("After the molappend_bond, the number of bond in the molecule is: %hu \n", molecule->bond_no);
        l += 1;
        molappend_bond(molecule, &bond2);
        printf("The bond max is: %d \n", molecule->bond_max);
        // printf("After the molappend_bond, the bond at the 0 index is: %s \n", molecule->bonds->a1->element);
        printf("After the molappend_bond, the number of bond in the molecule is: %hu \n", molecule->bond_no);

        k += 1;
    } while (k < 4);

    printf("Address of the atom and bond\n");
    printf("ATOMS: ");
    for (int i = 0; i < molecule->atom_no; i++)
    {
        if (i == 0)
        {
            printf(" %p", (void *)&molecule->atoms[i]);
            // printf(" %p", molecule->atoms[i]);
        }
        else
        {
            printf(" %p", (void *)&molecule->atoms[i]);
        }
    }
    printf("\n");

    printf("BONDS: ");
    for (int i = 0; i < molecule->bond_no; i++)
    {
        if (i == 0)
        {
            printf(" %p", (void *)&molecule->bonds[i]);
        }
        else
        {
            printf(" %p", (void *)&molecule->bonds[i]);
        }
    }
    printf("\n↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓");
    printf("\n-----------------------------------------------------------------------------------------------------------\n");
    printf("↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑\n");

    printf("Address of the atom and bond pointers\n");
    printf("ATOM_PTRS: ");
    for (int i = 0; i < molecule->atom_no; i++)
    {
        if (i == 0)
        {
            printf(" %p", (void *)molecule->atom_ptrs[i]);
        }
        else
        {
            printf(" %p", (void *)molecule->atom_ptrs[i]);
        }
    }
    printf("\n");

    printf("BOND_PTRS: ");
    for (int i = 0; i < molecule->bond_no; i++)
    {
        if (i == 0)
        {
            printf(" %p", (void *)molecule->bond_ptrs[i]);
        }
        else
        {
            printf(" %p", (void *)molecule->bond_ptrs[i]);
        }
    }
    printf("\n");
    printf("===========================================================================================================\n");
    printf("\n");

    // // molappend_bond(molecule, &bond1);
    // // molappend_bond(molecule, &bond2);
    // // molappend_bond(molecule, &bond2);
    molsort(molecule);

    xform_matrix xrot, yrot, zrot;
    xrotation(xrot, 30);
    yrotation(yrot, 45);
    zrotation(zrot, 60);

    printf("\n==================================== [MAIN.C] This is for xrot() ==============================\n");
    printf("The value of xrot in main.c is: %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf \n", xrot[0][0], xrot[0][1], xrot[0][2], xrot[1][0], xrot[1][1], xrot[1][2], xrot[2][0], xrot[2][1], xrot[2][2]);
    printf("The value of yrot in main.c is: %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf \n", yrot[0][0], yrot[0][1], yrot[0][2], yrot[1][0], yrot[1][1], yrot[1][2], yrot[2][0], yrot[2][1], yrot[2][2]);
    printf("The value of zrot in main.c is: %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf \n", zrot[0][0], zrot[0][1], zrot[0][2], zrot[1][0], zrot[1][1], zrot[1][2], zrot[2][0], zrot[2][1], zrot[2][2]);

    mol_xform(molecule, xrot);
    mol_xform(molecule, yrot);
    mol_xform(molecule, zrot);

    // // molcopy creates a copy of the molecule passed
    // //  allocate space in mem; size should be based on the atom max and bond max

    mx = molcopy(molecule);
    printf("Address of the atom and bond\n");
    printf("ATOMS: ");
    for (int i = 0; i < mx->atom_no; i++)
    {
        if (i == 0)
        {
            printf(" %p", (void *)&mx->atoms[i]);
            // printf(" %p", molecule->atoms[i]);
        }
        else
        {
            printf(" %p", (void *)&mx->atoms[i]);
        }
    }
    printf("\n");

    printf("BONDS: ");
    for (int i = 0; i < mx->bond_no; i++)
    {
        if (i == 0)
        {
            printf(" %p", (void *)&mx->bonds[i]);
        }
        else
        {
            printf(" %p", (void *)&mx->bonds[i]);
        }
    }
    printf("\n↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓");
    printf("\n-----------------------------------------------------------------------------------------------------------\n");
    printf("↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑\n");

    printf("Address of the atom and bond pointers\n");
    printf("ATOM_PTRS: ");
    for (int i = 0; i < mx->atom_no; i++)
    {
        if (i == 0)
        {
            printf(" %p", (void *)mx->atom_ptrs[i]);
        }
        else
        {
            printf(" %p", (void *)mx->atom_ptrs[i]);
        }
    }
    printf("\n");

    printf("BOND_PTRS: ");
    for (int i = 0; i < mx->bond_no; i++)
    {
        if (i == 0)
        {
            printf(" %p", (void *)mx->bond_ptrs[i]);
        }
        else
        {
            printf(" %p", (void *)mx->bond_ptrs[i]);
        }
    }
    printf("\n");
    printf("===========================================================================================================\n");
    printf("\n");

    //!Updated to include all the nightmare modes implementation
    



    molfree(molecule);
    molfree(mx);

    return 0;
}
