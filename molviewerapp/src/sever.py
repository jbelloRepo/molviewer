from http.server import HTTPServer
from http.server import BaseHTTPRequestHandler
import sqlite3
import MolDisplay
import molsql
import molecule
import sys
import os
import json
from urllib.parse import parse_qs, unquote

class Myserver(BaseHTTPRequestHandler):
    def do_GET(self):
        if self.path == "/":
            self.send_response(200)
            self.send_header("Content-type", "text/html")
            with open("webDesign/html/index.html", "r") as f:
                html = f.read()
            self.send_header("Content-length", len(html))
            self.end_headers()
            self.wfile.write(html.encode())
        
        elif self.path == "/elements":
            print("GET==elements")
            # Connect to the SQLite databasec
            conn = sqlite3.connect("molecules.db")
            c = conn.cursor()

            # Get elements data from the Elements table starting after row 4
            # c.execute("SELECT * FROM Elements WHERE rowid > 4")
            # rows = c.fetchall()
            c.execute("SELECT rowid, * FROM Elements WHERE rowid > 4")
            rows = c.fetchall()
            print(rows)

            
            # Build a list of element objects
            elements = []
            for row in rows:
                print(row)
                element = {
                    "elementNumber": row[1],
                    "elementCode": row[2],
                    "elementName": row[3],
                    "color1": row[4],
                    "color2": row[5],
                    "color3": row[6],
                    "radius": row[7],
                    "id": row[0]
                }
                elements.append(element)
            
            # Close the database connection
            conn.close()
            
            # Return the elements data as a JSON array
            self.send_response(200)
            self.send_header("Content-type", "application/json")
            self.end_headers()
            self.wfile.write(json.dumps(elements).encode())
            

        elif self.path == "/styles.css":
            self.send_response(200)
            self.send_header("Content-type", "text/css")
            with open("webDesign/css/styles.css", "r") as f:
                css = f.read()
            self.end_headers()

            self.wfile.write(css.encode())

        elif self.path == "/index.js":
            self.send_response(200)
            self.send_header("Content-type", "text/javascript")
            with open("webDesign/js/index.js", "r") as f:
                js = f.read()
            self.end_headers()
            self.wfile.write(js.encode())

        elif self.path == "/form.js":
            self.send_response(200)
            self.send_header("Content-type", "text/javascript")
            with open("webDesign/js/form.js", "r") as f:
                form = f.read()
            self.end_headers()
            self.wfile.write(form.encode())

        elif self.path == "/molist.js":
            self.send_response(200)
            self.send_header("Content-type", "text/javascript")
            with open("webDesign/js/molist.js", "r") as f:
                molist = f.read()
            self.end_headers()
            self.wfile.write(molist.encode())

        elif self.path == "/uploadSDF.html":
            self.send_response(200)
            self.send_header("Content-type", "text/html")
            with open("webDesign/html/uploadSDF.html", "r") as f:
                sdfHtml = f.read()
            self.send_header("Content-length", len(sdfHtml))
            self.end_headers()
            self.wfile.write(sdfHtml.encode())

        elif self.path == "/display.html":
            self.send_response(200)
            self.send_header("Content-type", "text/html")
            with open("webDesign/html/display.html", "r") as f:
                display = f.read()
            self.send_header("Content-length", len(display))
            self.end_headers()
            self.wfile.write(display.encode())

        elif self.path == "/modele.html":
            self.send_response(200)
            self.send_header("Content-type", "text/html")
            with open("webDesign/html/modele.html", "r") as f:
                modele = f.read()
            self.send_header("Content-length", len(modele))
            self.end_headers()
            self.wfile.write(modele.encode())

        elif self.path == "/molist.html":
            self.send_response(200)
            self.send_header("Content-type", "text/html")
            with open("webDesign/html/molist.html", "r") as f:
                molist = f.read()
            self.send_header("Content-length", len(molist))
            self.end_headers()
            self.wfile.write(molist.encode())

        elif self.path == "/logo.png":
            self.send_response(200)
            self.send_header("Content-type", "image/png")
            with open("webDesign/images/icon.png", "rb") as f:
                logo = f.read()
            self.end_headers()

            self.wfile.write(logo)

        elif self.path == "/icon.png":
            # Serve the favicon.ico file
            # print ("in here")
            try:
                with open("webDesign/images/icon.png", "rb") as f:
                    favicon = f.read()
                    # print("File opened successfully")
                self.send_response(200)
                self.send_header("Content-type", "image/png")
                self.end_headers()
                self.wfile.write(favicon)
            except Exception as e:
                # print(f"Error serving favicon: {e}")
                self.send_error(404)

        else:
            self.send_response(404)
            self.end_headers()
            self.wfile.write(bytes("404: not found", "utf-8"))

    def do_POST(self):
        if self.path == "/moleculelist":
            print("POST==molist")
            self.send_response(200)
            print("self.send_response(200)")

            # Connect to the SQLite database
            conn = sqlite3.connect("molecules.db")
            c = conn.cursor()
            # Retrieve all the molecules from the database
            c.execute("SELECT NAME FROM Molecules")
            molecules = c.fetchall()
            # Convert the molecules to a list of dictionaries
            molecule_list = [{"id": i, "name": m[0]}
                             for i, m in enumerate(molecules, 1)]
            print(molecule_list)

            # Send the molecules data as a JSON response
            self.send_header("Content-type", "application/json")
            self.end_headers()
            self.wfile.write(json.dumps(molecule_list).encode())
            # Close the database connection
            conn.close()
        
        elif self.path == "/rowDELETE":
            print("POST==rowDELETE")

            content_length = int(self.headers['Content-Length'])
            post_data = self.rfile.read(content_length).decode()
            print("printing..... ")
            print(post_data)
            rowID = post_data.split('=')[1]
            print(f'The molecule id is {rowID}')

            # Connect to the SQLite database
            conn = sqlite3.connect("molecules.db")
            c = conn.cursor()
            try:
                # Delete the row with the specified ID from the Elements table
                c.execute("DELETE FROM Elements WHERE rowid=?", (rowID,))
                conn.commit()
                # Send a success response
                self.send_response(200)
                self.send_header('Content-type', 'application/json')
                self.end_headers()
                response = {'status': 'success', 'message': 'Row deleted successfully.'}
                self.wfile.write(json.dumps(response).encode())
            except:
                # Send an error response
                self.send_response(500)
                self.send_header('Content-type', 'application/json')
                self.end_headers()
                response = {'status': 'error', 'message': 'Error deleting row.'}
                self.wfile.write(json.dumps(response).encode())

            # Close the database connection
            conn.close()
            

        elif self.path == "/rotate":
            print("POST==rotate")
            content_length = int(self.headers['Content-Length'])
            post_data = self.rfile.read(content_length).decode()
            print("printing..... ")
            print(post_data)
            molecule_id = post_data.split('=')[1]
            print(f'The molecule id is {molecule_id}')

            molecule_svg = get_RTR_svg_file(molecule_id)
            self.send_response(200)
            self.send_header('Content-type', 'image/svg+xml')
            self.end_headers()
            self.wfile.write(molecule_svg.encode()) # Send the SVG file as the response
        
        elif self.path == "/form":
            print("POST==form")
            content_length = int(self.headers['Content-Length'])
            post_data = self.rfile.read(content_length).decode()
            print(post_data)

            # parse the form data
            parsed_data = parse_qs(post_data)

            # access the values using the keys
            # access the values using the keys
            element_number = parsed_data['elementNumber'][0]
            element_code = parsed_data['elementCode'][0]
            element_name = parsed_data['elementName'][0]
            color1 = unquote(parsed_data['color1'][0])[1:].upper()  # remove '#' from color value
            color2 = unquote(parsed_data['color2'][0])[1:].upper()  # remove '#' from color value
            color3 = unquote(parsed_data['color3'][0])[1:].upper()  # remove '#' from color value
            radius = parsed_data['radius'][0]

            addformData(element_number,element_code,element_name, color1,color2,color3,radius)


        elif self.path == "/doPOST":
            print("POST==doPOST")
            content_length = int(self.headers['Content-Length'])
            post_data = self.rfile.read(content_length).decode()
            print(post_data)
            molecule_id = post_data.split('=')[1]
            print(f'The molecule id is {molecule_id}')

            # # Retrieve the SVG file for the selected molecule using the molecule ID
            # # Replace this part with your own logic to retrieve the SVG file from the server/database
            molecule_svg = get_svg_file(molecule_id)
            # print(molecule_svg)

            self.send_response(200)
            self.send_header('Content-type', 'image/svg+xml')
            self.end_headers()
            self.wfile.write(molecule_svg.encode()) # Send the SVG file as the response

            # data = json.loads(post_data.decode('utf-8'))
            # molecule_id = data['id']

            # data = json.loads(request.data.decode('utf-8'))
            # molecule_id = data['id']

            # # Get the molecule ID from the request data
            # molecule_id = request.form['id']

        elif self.path == "/molecule":
            print("POST==molecule")
            length = int(self.headers.get("content-length"))
            post_data = self.rfile.read(length).decode().split('\n')[4:]
            print(post_data)

            # Get form mole name
            length_of_list = len(post_data)
            third_from_end = post_data[-3]
            third_from_end = third_from_end.replace('\r', '')
            # print(f'The name of molecule is {third_from_end}')
            moleName = third_from_end

            if not os.path.exists('molecules.db'):
                print(f'POST==The database does not exist yet')
                print(f'POST==Sending the {moleName} molecule')
                success_msg = initDB(moleName, post_data)

            else:
                print(f'POST==The database already exists yet')
                print(f'POST==Sending the {moleName} molecule')
                success_msg = add_to_DB(moleName, post_data)

            self.send_response(200)
            self.send_header('Content-type', 'text/html')
            self.end_headers()

            # if success_msg:
            #     response = {"success": True,
            #                 "message": "File uploaded successfully"}
            # else:
            #     response = {
            #         "success": False, "message": "An error occurred while processing the file"}

            # # send the response to the client
            # self.wfile.write(json.dumps(response).encode())

        else:
            print("---------------------")
            print("POST-ERROR==unknown")
            self.send_error(404)
            print("---------------------")


def initDB(moleName, post_data):
    if not os.path.exists('molecules.db'):
        print(f'initDB==Initial adding to the database')
        db = molsql.Database(reset=False)
        db.create_tables()

        db['Elements'] = (1, 'H', 'hydrogen', 'FFFFFF', '050505', '020202', 25)
        db['Elements'] = (6, 'C', 'Carbon', '808080', '010101', '000000', 40)
        db['Elements'] = (7, 'N', 'Nitrogen', '0000FF', '000005', '000002', 40)
        db['Elements'] = (8, 'O', 'Oxygen', 'FF0000', '050000', '020000', 40)

        db.add_molecule(moleName, post_data)
        return f"{moleName} added to the database!"

        # MolDisplay.radius = db.radius()
        # MolDisplay.element_name = db.element_name()
        # MolDisplay.header += db.radial_gradients()

def addformData(element_number,element_code,element_name, color1,color2,color3,radius):
    db = molsql.Database(reset=False)
    db['Elements'] = (element_number, element_code, element_name, color1, color2, color3, radius)

def add_to_DB(moleName, post_data):
    print(f'add_to_DB==Database file exists..... adding to DB')
    db = molsql.Database(reset=False)
    db.add_molecule(moleName, post_data)
    return f"{moleName} added to the database!"

def get_RTR_svg_file(molecule_id):
    db = molsql.Database(reset=False)

    # Connect to the SQLite database
    conn = sqlite3.connect("molecules.db")
    c = conn.cursor()

    # Retrieve the molecule name for the given molecule_id from the database
    c.execute("SELECT NAME FROM Molecules WHERE MOLECULE_ID=?", (molecule_id,))
    molecule_name = c.fetchone()[0]

    # Close the database connection
    conn.close()
    print(f'The molecule rotate name is: {molecule_name}')

    # db = Database(reset=False); # or use default
    MolDisplay.radius = db.radius()
    MolDisplay.element_name = db.element_name()
    MolDisplay.header += db.radial_gradients()

    mol = db.load_mol(molecule_name)
    mol.sort()
    
    mx = molecule.mx_wrapper(0,0,90)
    mol.xform( mx.xform_matrix )
    mol_svg = mol.svg()
    # print(mol.svg()) 
    # # save the file
    # fp = open(molecule_name + "ss" + ".svg", "w")
    # fp.write(mol.svg())
    # fp.close()
    return mol_svg
    

def get_svg_file(molecule_id):
    db = molsql.Database(reset=False)

    # Connect to the SQLite database
    conn = sqlite3.connect("molecules.db")
    c = conn.cursor()

    # Retrieve the molecule name for the given molecule_id from the database
    c.execute("SELECT NAME FROM Molecules WHERE MOLECULE_ID=?", (molecule_id,))
    molecule_name = c.fetchone()[0]

    # Close the database connection
    conn.close()

    # Return the molecule name
    print(molecule_name)

    # db = Database(reset=False); # or use default
    MolDisplay.radius = db.radius()
    MolDisplay.element_name = db.element_name()
    MolDisplay.header += db.radial_gradients()

    mol = db.load_mol(molecule_name)
    mol.sort()
    mol_svg = mol.svg()
    # print(mol.svg()) 
    # # save the file
    # fp = open(molecule_name + "ss" + ".svg", "w")
    # fp.write(mol.svg())
    # fp.close()
    return mol_svg


# httpd = HTTPServer(('localhost', int(sys.argv[1])), Myserver)
httpd = HTTPServer(('0.0.0.0', int(sys.argv[1])), Myserver)
print(f'Starting server on http://localhost:{sys.argv[1]}...')
httpd.serve_forever()
# REF: https://stackoverflow.com/questions/4688528fullstack/molviewerapp 1/start-python-server-in-docker-access-from-localhost

# This was edited in docker