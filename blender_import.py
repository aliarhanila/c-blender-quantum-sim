import bpy
import csv


csv_file_path = "" """Choose your dataset path"""

vertices = []
with open(csv_file_path, newline='') as csvfile:
    reader = csv.reader(csvfile)
    next(reader) 
    for row in reader:
        if len(row) == 3:
            vertices.append((float(row[0]), float(row[1]), float(row[2])))

mesh = bpy.data.meshes.new("Quantum_Mesh")
obj = bpy.data.objects.new("Quantum_Orbital", mesh)


bpy.context.collection.objects.link(obj)


mesh.from_pydata(vertices, [], [])
mesh.update()
