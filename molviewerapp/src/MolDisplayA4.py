import molecule
import math

# Define constants
radius = {'H': 25, 'C': 40, 'O': 40, 'N': 40}
element_name = {'H': 'grey', 'C': 'black', 'O': 'red', 'N': 'blue'}
header = """<svg version="1.1" width="1000" height="1000" xmlns="http://www.w3.org/2000/svg">"""
footer = """</svg>"""
offsetx = 500
offsety = 500
bndCount = [0]

# Format specifier to the gradients


def getCounter():
    bndCount[0] += 1
    return str(bndCount[0])


class Atom:  # Create an ATOM class
    def __init__(self, atom):
        self.atom = atom
        self.z = atom.z

    def __str__(self):  # Define the string
        return f"element: {self.atom.element} x: {self.atom.x} y: {self.atom.y} z:{self.atom.z}"

    def svg(self):  # svg method: description of circle
        cx = self.atom.x * 100.0 + offsetx
        cy = self.atom.y * 100.0 + offsety
        r = radius[self.atom.element]
        fill = element_name[self.atom.element]
        # ! Updated for A2
        return f"  <circle cx=\"{cx:.2f}\" cy=\"{cy:.2f}\" r=\"{r}\" fill=\"url(#{fill})\"/>\n"
        # return f" <circle cx=\"{cx:.2f}\" cy=\"{cy:.2f}\" r=\"{r}\" fill=\"{fill}\"/>\n"


class Bond:
    def __init__(self, bond):
        self.bond = bond
        self.z = bond.z

    def __str__(self):  # Define the string
        return f"a1: {self.bond.a1} a2: {self.bond.a2} x1: {self.bond.x1} x2: {self.bond.x2} y1: {self.bond.y1} y2: {self.bond.y2} z:{self.bond.z}"

    def svg(self):
        x1 = self.bond.x1 * 100.0 + offsetx
        y1 = self.bond.y1 * 100.0 + offsety
        x2 = self.bond.x2 * 100.0 + offsetx
        y2 = self.bond.y2 * 100.0 + offsety
        dx = x2 - x1
        dy = y2 - y1
        length = math.sqrt(dx*dx + dy*dy)
        ux = dx / length
        uy = dy / length
        x3 = x1 + 10*uy
        y3 = y1 - 10*ux
        x4 = x1 - 10*uy
        y4 = y1 + 10*ux
        x5 = x2 + 10*uy
        y5 = y2 - 10*ux
        x6 = x2 - 10*uy
        y6 = y2 + 10*ux
        z = self.bond.z
        # linear Gradient for polygon

        id = getCounter()
        polygonGradient = getPolygonGradient(x3, y3, x4, y4, ("bond"+id))
        ellipseGradient = getEllipsesGradient(x3, y3, x4, y4, ("cap"+id))
        ellipse = addBondEllipses(x3, y3, x4, y4, z, ("cap"+id))
        # linear Gradient for ellipse
        return polygonGradient + ellipseGradient + f'  <polygon points="{x3:.2f},{y3:.2f} {x4:.2f},{y4:.2f} {x6:.2f},{y6:.2f} {x5:.2f},{y5:.2f}" fill="url(#bond{id})"/>\n' + ellipse


class Molecule(molecule.molecule):
    def __str__(self):  # Define the string for debugging
        out = "Atoms:\n"
        for i in range(self.atom_no):
            out += str(self.get_atom(i).element)
        out += "\nBonds:\n"
        for i in range(self.bond_no):
            out += "a1:"+str(self.get_bond(i).a1) + " "
            out += "a2:"+str(self.get_bond(i).a2) + " "
            out += "z:"+str(self.get_bond(i).z) + " "
            out += "eparis:"+str(self.get_bond(i).epairs) + " "
            out += "x1:"+str(self.get_bond(i).x1) + " "
            out += "y1:"+str(self.get_bond(i).y1) + " "
            out += "x2:"+str(self.get_bond(i).x2) + " "
            out += "y2:"+str(self.get_bond(i).y2) + " "
            out += "len:"+str(self.get_bond(i).len) + " "
            out += "dx:"+str(self.get_bond(i).dx) + " "
            out += "dy:"+str(self.get_bond(i).dy) + " " + "\n"
        out += f"atom max: {self.atom_max} atom no: {self.atom_no} bond max: {self.bond_max} bond no: {self.bond_no}"
        return out

    def svg(self):
        atom_no = self.atom_no
        bond_no = self.bond_no
        appended_count = 0
        i = 0
        j = 0
        svg_str = header

        atom = self.get_atom(i)
        anAtom = Atom(atom)
        bond = self.get_bond(i)
        aBond = Bond(bond)
        # for i in range(atom_no + bond_no):
        while True:
            atom = self.get_atom(i)
            anAtom = Atom(atom)

            bond = self.get_bond(j)
            aBond = Bond(bond)

            if (anAtom.z < aBond.z):
                svg_str += anAtom.svg()
                # print(f"==Appended Atom== {anAtom.z}")
                appended_count += 1
                i += 1
                if ((i == atom_no)):
                    while (bond_no > j):
                        bond = self.get_bond(j)
                        aBond = Bond(bond)
                        svg_str += aBond.svg()
                        # print(f"==Appended Bond== {aBond.z}")
                        appended_count += 1
                        i += 1
                        # print(f"The value of i is {i}")

            else:
                svg_str += aBond.svg()
                # print(f"==Appended Bond== {aBond.z}")
                appended_count += 1
                j += 1
                if ((j == bond_no)):
                    while (atom_no > i):
                        atom = self.get_atom(i)
                        anAtom = Atom(atom)
                        svg_str += anAtom.svg()
                        # print(f"==Appended Atom== {anAtom.z}")
                        appended_count += 1

                        i += 1
                        # print(f"The value of i is {j}")

            if ((i == atom_no) and (j == bond_no)):
                # print(f"The value of i,j is: {i,j}")
                # print(f"The appended number of items: {appended_count}")
                False
                break

        svg_str += footer
        return svg_str

    # @classmethod

    #! UPDATED FOR A3
    def parse(self, fileobj):
        atomCount, bondCount = None, None  # declare as object to make Iterable
        lineCount = 0
        for line in fileobj:
            lineCount += 1
            if lineCount == 4:  # read after the header file
                atomCount = int(line[:3])
                bondCount = int(line[3:6])
                # print(f"atom count:{atomCount}  bond count:{bondCount}")
            elif atomCount is not None and self.atom_no < atomCount and (lineCount < (4+atomCount+1)):
                x, y, z, element = line.split()[:4]
                x = float(x)
                y = float(y)
                z = float(z)
                self.append_atom(element, x, y, z)
            elif bondCount is not None and self.bond_no < bondCount and (lineCount > (4+atomCount)) and (lineCount < (4+atomCount+1+bondCount)):
                bondVar = (int(line[:3].strip()), int(
                    line[3:6].strip()), int(line[6:9].strip()))
                self.append_bond(bondVar[0]-1, bondVar[1]-1, bondVar[2])
                # print(bondVar)
        # print("\n=============== Class method: parse() ===============")
        # print(f"atom_no: {self.atom_no}  bond_no: {self.bond_no}")
        # print("=====================================================")

#! Updated for A4/A3
def getPolygonGradient(x1, y1, x2, y2, gradient_id):
    gradient_string = f'<linearGradient id="{gradient_id}" x1="{x1:.2f}" y1="{y1:.2f}" x2="{x2:.2f}" y2="{y2:.2f}" gradientUnits="userSpaceOnUse">\n'
    gradient_string += '\t<stop offset="0%" stop-color="#454545" />\n'
    gradient_string += '\t<stop offset="25%" stop-color="#606060" />\n'
    gradient_string += '\t<stop offset="50%" stop-color="#454545" />\n'
    gradient_string += '\t<stop offset="100%" stop-color="#252525" />\n'
    gradient_string += '</linearGradient>\n'
    return gradient_string

def getEllipsesGradient(x1, y1, x2, y2, gradient_id):
    angle = math.degrees(math.atan2(y2 - y1, x2 - x1))
    rotation_point = f"{(x1+x2)/2:.2f},{(y1+y2)/2:.2f}"
    gradient_string = f'<linearGradient id="{gradient_id}" x1="{x1:.2f}" y1="{y1:.2f}" x2="{x2:.2f}" y2="{y2:.2f}" gradientUnits="userSpaceOnUse" gradientTransform="rotate({angle:.2f},{rotation_point})">\n'
    gradient_string += '\t<stop offset="0%" stop-color="#454545" />\n'
    gradient_string += '\t<stop offset="25%" stop-color="#606060" />\n'
    gradient_string += '\t<stop offset="50%" stop-color="#454545" />\n'
    gradient_string += '\t<stop offset="100%" stop-color="#252525" />\n'
    gradient_string += '</linearGradient>\n'
    return gradient_string

def addBondEllipses(x1, y1, x2, y2, z, gradient_id):
    dx = x2 - x1
    dy = y2 - y1
    dz = z
    length = math.sqrt(dx**2 + dy**2 + dz**2)
    cos_theta = dz / length
    sin_theta = math.sqrt(1 - cos_theta**2)
    cos_phi = dx / math.sqrt(dx**2 + dy**2)
    sin_phi = dy / math.sqrt(dx**2 + dy**2)

    if dz == 0:
        # Bond is in x-y plane, so make the ellipse have zero width
        rx = 0
        ry = length / 2
        rotation = 0
    else:
        # Bond is not in x-y plane, so scale the ellipse proportionally
        rx = length / 2
        ry = rx * sin_theta
        rotation = math.degrees(math.atan2(sin_phi, cos_phi))

    # Compute the coordinates of the center of the ellipse
    cx = x2 - rx * cos_phi
    cy = y2 - rx * sin_phi * cos_theta

    # Construct the SVG ellipse element
    ellipse = f'<ellipse cx="{cx:.2f}" cy="{cy:.2f}" rx="{rx:.2f}" ry="{ry:.2f}" transform="rotate({rotation:.2f},{cx:.2f},{cy:.2f})" fill="url(#{gradient_id})" />\n'
    return ellipse
# def addBondEllipses(x1, y1, x2, y2, z,  gradient_id):
#     dx = x2 - x1
#     dy = y2 - y1
#     dz = z
#     length = math.sqrt(dx**2 + dy**2 + dz**2)
#     cos_theta = dz / length
#     sin_theta = math.sqrt(dx**2 + dy**2) / length
#     cos_phi = dx / math.sqrt(dx**2 + dy**2)
#     sin_phi = dy / math.sqrt(dx**2 + dy**2)

#     # Compute the size and orientation of the ellipse
#     if dz == 0:
#         # Bond is in x-y plane, so make the ellipse have zero width
#         rx = 0
#         ry = length / 2
#         rotation = 0
#     else:
#         # Bond is not in x-y plane, so scale the ellipse proportionally
#         rx = length / 2
#         ry = rx * math.sqrt(dx**2 + dy**2) / abs(dz)
#         rotation = math.degrees(math.atan2(sin_phi, cos_phi))

#      # Compute the coordinates of the center of the ellipse
#         cx = x2 - rx * cos_phi
#         cy = y2 - rx * sin_phi
#     ellipse = f'<ellipse cx="{cx:.2f}" cy="{cy:.2f}" rx="{rx:.2f}" ry="{ry:.2f}" transform="rotate({rotation:.2f},{cx:.2f},{cy:.2f})" fill="url(#{gradient_id})" />\n'
#     return ellipse
    # dwg.add(svgwrite.shapes.Ellipse(center=(cx, cy), r=(rx, ry), transform=f"rotate({rotation},{cx},{cy})"))

    # Compute the coordinates of the center of the ellipse
    #     cx = x2 - rx * cos_phi
    #     cy = y2 - rx * sin_phi
    # for bond in bonds:
    #     x1, y1 = bond['atoms'][0]['coords']
    #     x2, y2 = bond['atoms'][1]['coords']
    #     dx = x2 - x1
    #     dy = y2 - y1
    #     angle = math.degrees(math.atan2(dy, dx))
    #     cx = (x1 + x2) / 2
    #     cy = (y1 + y2) / 2
    #     transform = f"rotate({angle}, {cx}, {cy})"
    #     ellipse = f'<ellipse cx="{cx}" cy="{cy}" rx="{rx}" ry="{ry}" transform="{transform}" fill="{fill}" />'
    #     svg.write(ellipse + '\n')


if __name__ == "__main__":
    mol1 = Molecule()
    # create 3 atoms
    with open('CID_31260.sdf', 'r') as sdfile:
        mol1.parse(sdfile)
    # print(mol1)
    mol1.sort()
    # print(mol1.sort())
    print(mol1.svg())
