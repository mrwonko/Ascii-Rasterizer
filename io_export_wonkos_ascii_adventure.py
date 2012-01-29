# Wonko's Ascii Adventure File export (custom file format)

bl_info = {
    "name": "Wonko's Ascii Adventure File (.waa)",
    "author": "Willi Schinmeyer",
    "version": (1, 0, 0),
    "blender": (2, 6, 1),
    "api": 37702,
    "location": "File > Export > Wonko's Ascii Adventure (.waa)",
    "description": "Export Wonko's Ascii Adventure File (.waa)",
    "warning": "",
    "wiki_url": "",
    "tracker_url": "",
    "category": "Import-Export"}

# file structure:
# [header]
# "WAAF" (Wonko's Ascii Adventure File)
# int version
# [displayed part]
# uint numFaces
#  [numFaces times]
#   char material
#   float[3][3] vertices (xyz, y is up)
# [gameplay part]
# float[3] playerpos
# float[3] goalpos
# [physics part]
# uint numPhysVerts
#  [numPhysVerts times]
#   float[3] vertex
# uint numPhysTris
#  [numPhysTris times]
#   uint[3] indices

import os, struct
import bpy

VERSION = 3 # keep in sync with game

# write a coordinate, changing it from z-up to y-up
def WriteCoord(file, co):
    file.write(struct.pack("<3f", co.x, co.z, -co.y))

# writes the [displayed part]
def WriteVisible(file, obj):
    assert(obj.type == 'MESH')
    mesh = obj.data
    materials = mesh.materials
    matrix = obj.matrix_world
    # write numFaces
    file.write(struct.pack("<I", len(mesh.faces)))
    # write faces
    for face in mesh.faces:
        material = materials[face.material_index].name
        #assert(len(material) == 1)
        # write material
        file.write(struct.pack("<c", material[0].encode()))
        assert(len(face.vertices) == 3)
        # write vertices
        for vertex in face.vertices:
            co = mesh.vertices[vertex].co
            #WriteCoord(file, matrix * co)
            WriteCoord(file, co)

def WritePhysics(file, obj):
    assert(obj.type == 'MESH')
    file.write(b"PHYS")
    mesh = obj.data
    matrix = obj.matrix_world
    # write numVertices
    file.write(struct.pack("<i", len(mesh.vertices)))
    # write vertices
    for vert in mesh.vertices:
        WriteCoord(file, matrix * vert.co)
    # write numTris
    file.write(struct.pack("<i", len(mesh.faces)))
    # write Tris
    for tri in mesh.faces:
        assert(len(tri.vertices) == 3)
        file.write(struct.pack("<3i", tri.vertices[0], tri.vertices[1], tri.vertices[2]))

def Export(filename, context):
    with open(filename, "wb") as file:
        file.write(b"WAAF")
        file.write(struct.pack("<I", VERSION))
        WriteVisible(file, context.object)
        # todo: write playerpos, goalpos
        WritePhysics(file, context.object)
	

from bpy.props import StringProperty, EnumProperty, BoolProperty

class WAAFExporter(bpy.types.Operator):
    """Export to the Wonko's Ascii Adventure File format (.waa)"""

    bl_idname = "export.waaf"
    bl_label = "Export Wonko's Ascii Adventure File"

    filepath = StringProperty(subtype='FILE_PATH')

    def execute(self, context):
        #Append .x
        FilePath = bpy.path.ensure_ext(self.filepath, ".waa")
        Export(FilePath, context)
        return {"FINISHED"}

    def invoke(self, context, event):
        if not self.filepath:
            self.filepath = bpy.path.ensure_ext(bpy.data.filepath, ".waa")
        WindowManager = context.window_manager
        WindowManager.fileselect_add(self)
        return {"RUNNING_MODAL"}


def menu_func(self, context):
    self.layout.operator(WAAFExporter.bl_idname, text="Wonko's Ascii Adventure (.waa)")


def register():
    bpy.utils.register_module(__name__)

    bpy.types.INFO_MT_file_export.append(menu_func)


def unregister():
    bpy.utils.unregister_module(__name__)

    bpy.types.INFO_MT_file_export.remove(menu_func)


if __name__ == "__main__":
    register()