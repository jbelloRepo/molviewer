import os
import time
import sqlite3
import molecule
import MolDisplay



def runsdf():
    mol1 = MolDisplay.Molecule()
    fp = open('CID_31260.sdf')
    mol1.parse(fp)
    mol1.sort()

    print(mol1)

    mx = molecule.mx_wrapper(90, 0, 0)
    mol1.xform(mx.xform_matrix)

    print(mol1)

    fp1 = open("CIDrr" + ".svg", "w")
    fp1.write(mol1.svg())
    fp1.close()
        # runsdf()

class Database:
    connection = None  # class variable

    def __init__(self, reset=False):
        if reset:  # if True delete existing database
            try:
                conn = sqlite3.connect("molecules.db")
                conn.close()
                os.remove("molecules.db")
            except:
                pass
        # create/open connection to database file
        self.connection = sqlite3.connect("molecules.db")  # create connection
        # self.cursor = self.conn.cursor()  # create connection object

    def create_tables(self):
        # Element Table
        # cursor = self.connection.cursor()
        self.connection.execute("""
                        CREATE TABLE IF NOT EXISTS Elements 
                        (   ELEMENT_NO   INTEGER     NOT NULL,
                            ELEMENT_CODE VARCHAR(3)  NOT NULL    PRIMARY KEY,
                            ELEMENT_NAME VARCHAR(32) NOT NULL,
                            COLOUR1      CHAR(6)     NOT NULL,
                            COLOUR2      CHAR(6)     NOT NULL,
                            COLOUR3      CHAR(6)     NOT NULL,
                            RADIUS       DECIMAL(3)  NOT NULL);
                            """)

        # Atom Table
        self.connection.execute("""
                        CREATE TABLE IF NOT EXISTS Atoms 
                        (   ATOM_ID      INTEGER        NOT NULL    PRIMARY KEY     AUTOINCREMENT,
                            ELEMENT_CODE VARCHAR(3)     NOT NULL,
                            x            DECIMAL(7,4)   NOT NULL,
                            y            DECIMAL(7,4)   NOT NULL,
                            z            DECIMAL(7,4)   NOT NULL,
                            FOREIGN KEY (ELEMENT_CODE) REFERENCES Elements);
                            """)

        # Bond Table
        self.connection.execute("""
                        CREATE TABLE IF NOT EXISTS Bonds 
                        (   BOND_ID      INTEGER     NOT NULL    PRIMARY KEY     AUTOINCREMENT,
                            A1           INTEGER     NOT NULL,
                            A2           INTEGER     NOT NULL,
                            EPAIRS       INTEGER     NOT NULL);
                        """)

        # Molecules Table
        self.connection.execute("""
                        CREATE TABLE IF NOT EXISTS Molecules 
                        (   MOLECULE_ID  INTEGER     NOT NULL    PRIMARY KEY     AUTOINCREMENT,
                            NAME         TEXT        NOT NULL    UNIQUE);
                        """)

        # MoleculeAtom Table
        self.connection.execute("""
                        CREATE TABLE IF NOT EXISTS MoleculeAtom 
                        (   MOLECULE_ID  INTEGER     NOT NULL,
                            ATOM_ID      INTEGER     NOT NULL,
                            PRIMARY KEY  (MOLECULE_ID, ATOM_ID),
                            FOREIGN KEY (MOLECULE_ID) REFERENCES Molecules,
                            FOREIGN KEY (ATOM_ID) REFERENCES Atoms);
                        """)

        # MoleculeBond Table
        self.connection.execute("""
                        CREATE TABLE IF NOT EXISTS MoleculeBond 
                        (   MOLECULE_ID  INTEGER     NOT NULL,
                            BOND_ID      INTEGER     NOT NULL,
                            PRIMARY KEY  (MOLECULE_ID, BOND_ID),
                            FOREIGN KEY (MOLECULE_ID) REFERENCES Molecules,
                            FOREIGN KEY (BOND_ID) REFERENCES Bonds);
                        """)

    def __setitem__(self, table, values):
        emptyList = ",".join(["?" for _ in values])
        with self.connection:
            # cursor = self.connection.cursor()
            self.connection.execute(
                f"INSERT INTO {table} VALUES ({emptyList})", values)

    def add_molecule(self, name, fp):
        mol_name = name
        mol = MolDisplay.Molecule()
        mol.parse(fp)  # parse file pointer
        # print(mol)
        atom_count = mol.atom_no
        bond_count = mol.bond_no

        # Add to the molecules db
        with self.connection:
            # cursor = self.connection.cursor()
            self.connection.execute(
                "INSERT INTO Molecules (NAME) VALUES (?)", (mol_name,))

        # Add to the Atoms and Bond db
        for i in range(atom_count):
            self.add_atom(mol_name, mol.get_atom(i))
        for j in range(bond_count):
            self.add_bond(mol_name, mol.get_bond(j))

    def add_atom(self, molname, atom):
        with self.connection:
            cursor = self.connection.cursor()  # Create cursor object
            atom_attributes = (atom.element, float(atom.x), float(atom.y),
                               float(atom.z))  # Create atom attributes
            # Insert atom attributes into SQL
            cursor.execute("""
                        INSERT INTO Atoms (ELEMENT_CODE, x, y, z) 
                        VALUES (?, ?, ?, ?);
                        """, atom_attributes)
            # print(cursor.lastrowid)

            cursor.execute(
                "SELECT MOLECULE_ID FROM Molecules WHERE NAME=?", (molname,))
            row = cursor.fetchone()
            # print(row)
            if row is None:
                raise ValueError(f"No molecule named {molname}")
            molecule_id = row[0]
            atom_id = cursor.lastrowid
            cursor.execute(
                "INSERT INTO MoleculeAtom VALUES (?, ?)", (molecule_id, atom_id))

    def add_bond(self, molname, bond):
        with self.connection:
            cursor = self.connection.cursor()
            bond_attributes = (bond.a1, bond.a2, bond.epairs)

            cursor.execute("""
                            INSERT INTO Bonds (A1, A2, EPAIRS) 
                            VALUES (?, ?, ?);
                            """, bond_attributes)

            cursor.execute(
                "SELECT MOLECULE_ID FROM Molecules WHERE NAME=?", (molname,))
            row = cursor.fetchone()
            # print(row)
            if row is None:
                raise ValueError(f"No molecule named {molname}")
            molecule_id = row[0]
            bond_id = cursor.lastrowid
            cursor.execute(
                "INSERT INTO MoleculeBond VALUES (?, ?)", (molecule_id, bond_id))

    def load_mol(self, name):
        atomSQL = f"""
                    SELECT e.ELEMENT_CODE, a.x, a.y, a.z
                    FROM Atoms AS a
                    JOIN MoleculeAtom AS ma ON a.ATOM_ID = ma.ATOM_ID
                    JOIN Molecules AS m ON ma.MOLECULE_ID = m.MOLECULE_ID
                    JOIN Elements AS e ON a.ELEMENT_CODE = e.ELEMENT_CODE
                    WHERE m.NAME = '{name}'
                    ORDER BY a.ATOM_ID ASC
                    """
        atomValues = self.connection.execute(atomSQL).fetchall()

        bondSQL = f"""
                    SELECT b.A1, b.A2, b.EPAIRS
                    FROM Bonds AS b
                    JOIN MoleculeBond AS mb ON b.BOND_ID = mb.BOND_ID
                    JOIN Molecules AS m ON mb.MOLECULE_ID = m.MOLECULE_ID
                    WHERE m.NAME = '{name}'
                    ORDER BY b.BOND_ID ASC
                    """
        bondValues = self.connection.execute(bondSQL).fetchall()

        # add bonds and atom to the molecule object
        molecule = MolDisplay.Molecule()  # Create a new molecule object
        for atom_data in atomValues:
            element_code, x, y, z = atom_data
            molecule.append_atom(element_code,  x, y, z)
        for bond_data in bondValues:
            a1, a2, epairs = bond_data
            molecule.append_bond(a1, a2, epairs)

        return molecule

    def radius(self):
        cursor = self.connection.cursor()
        cursor.execute("SELECT ELEMENT_CODE, RADIUS FROM Elements")
        rows = cursor.fetchall()
        radiusDIC = dict(rows)
        return radiusDIC

    def element_name(self):
        cursor = self.connection.cursor()
        cursor.execute("SELECT ELEMENT_CODE, ELEMENT_NAME FROM Elements")
        rows = cursor.fetchall()
        element_namesDIC = dict(rows)
        return element_namesDIC

    def radial_gradients(self):
        gradients = []  # create a list for each row
        # Get data from the Elements table
        cursor = self.connection.execute(
            "SELECT ELEMENT_NAME, COLOUR1, COLOUR2, COLOUR3 FROM Elements")
        for row in cursor:
            gradient = ("""
            <radialGradient id="%s" cx="-50%%" cy="-50%%" r="220%%" fx="20%%" fy="20%%">
                <stop offset="0%%" stop-color="#%s"/>
                <stop offset="50%%" stop-color="#%s"/>
                <stop offset="100%%" stop-color="#%s"/>
            </radialGradient>""" % (row[0], row[1], row[2], row[3]))
            gradients.append(gradient)
        return "\n".join(gradients)  # join te strings


if __name__ == "__main__":
    # start = time.time()
    db = Database(reset=True)
    db.create_tables()

    db['Elements'] = (1, 'H', 'hydrogen', 'FFFFFF', '050505', '020202', 25)
    db['Elements'] = (6, 'C', 'Carbon', '808080', '010101', '000000', 40)
    db['Elements'] = (7, 'N', 'Nitrogen', '0000FF', '000005', '000002', 40)
    db['Elements'] = (8, 'O', 'Oxygen', 'FF0000', '050000', '020000', 40)

    fp = open('caffeine-3D-structure-CT1001987571.sdf')
    db.add_molecule('Caffeine', fp)

    fp = open('CID_31260.sdf')
    db.add_molecule('Isopentanol', fp)

    fp = open('water-3D-structure-CT1000292221.sdf')
    db.add_molecule('Water', fp)

    # mol = db.load_mol("Water")
    # print(mol)

    # db = Database(reset=False); # or use default
    MolDisplay.radius = db.radius()
    MolDisplay.element_name = db.element_name()
    MolDisplay.header += db.radial_gradients()
    for molecule1 in ['Water', 'Caffeine', 'Isopentanol']:
        mol = db.load_mol(molecule1)
        mol.sort()
        fp = open(molecule1 + ".svg", "w")
        fp.write(mol.svg())
        fp.close()
        runsdf()

    # def runsdf():
    #     mol1 = MolDisplay.Molecule()
    #     fp = open('water-3D-structure-CT1000292221.sdf')
    #     mol1.parse(fp)
    #     mol1.sort()

    #     print(mol1)

    #     mx = molecule.mx_wrapper(90, 0, 0)
    #     mol1.xform(mx.xform_matrix)

    #     print(mol1)

    # end = time.time()
    # total_time = end - start
    # print("\nTotal time: "+ str(total_time))
