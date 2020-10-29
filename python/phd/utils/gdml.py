import xml.etree.ElementTree as ET
import math


def position(x=0.0, y=0.0, z=0.0, unit="mm"):
    return ET.Element("position", attrib={"x": str(x), "y": str(y), "z": str(z), "unit": unit})


class GDMLPhys:
    def __init__(self, volume, position):
        self.phys = ET.Element("physvol")
        ET.SubElement(self.phys, "volumeref", attrib={"ref": volume})
        if isinstance(position, str):
            ET.SubElement(self.phys, "positionref", attrib={"ref": position})
        elif isinstance(position, ET.Element):
            self.phys.append(position)


class GDMLVolume:

    def __init__(self, name, material, solid):
        self.name = name
        self.volume = ET.Element("volume", attrib={"name": str(name)})
        ET.SubElement(self.volume, "solidref", attrib={"ref": solid})
        ET.SubElement(self.volume, "materialref", attrib={"ref": material})

    def add_phys(self, phys: GDMLPhys):
        self.volume.append(phys.phys)


class GDML:

    def __init__(self, name="test", world="World"):
        self.gdml = ET.Element("gdml")
        self.define = ET.SubElement(self.gdml, "define")
        self.materials = ET.SubElement(self.gdml, "materials")
        self.solids = ET.SubElement(self.gdml, "solids")
        self.structure = ET.SubElement(self.gdml, "structure")
        self.setup = ET.SubElement(self.gdml, "setup", attrib={"name": name, "version": "1.0"})
        ET.SubElement(self.setup, "world", attrib={"ref": world})

    def add_solid(self, solid):
        self.solids.append(solid)
        return self

    def add_custom_solid(self, solid, **kwargs):
        ET.SubElement(self.solids, solid, attrib=kwargs)
        return self

    def add_material(self, name, density,  Z=None, atom=None, formula=None, composite=None, fraction=None):
        # unit = "g/cm3"
        material = ET.Element("material", attrib={"name": name})
        ET.SubElement(material, "D", attrib={"value": str(density) }) # "unit": unit
        if formula is not None:
            material.attrib["formula"] = formula
        if composite is not None:
            for ref, n in composite:
                ET.SubElement(material, "composition", attrib={"ref": str(ref), "n": str(n)})
        elif fraction is not None:
            for ref, n in fraction:
                ET.SubElement(material, "fraction", attrib={"ref": str(ref), "n": str(n)})
        elif Z is not None and atom is not None:
            material.attrib["Z"] = str(Z)
            ET.SubElement(material, "atom", attrib={"value": atom})
        else:
            print("Material can't added")
        self.materials.append(material)
        return self

    def add_volume(self, volume: GDMLVolume):
        self.structure.append(volume.volume)
        return self



def box(name, x, y, z):
    x = str(x)
    y = str(y)
    z = str(z)
    return ET.Element("box", attrib={"name": name, "x": x, "y": y, "z": z})


def sphere(name, rmax, rmin=0, start_phi=0, delta_phi=2 * math.pi, start_theta=0, delta_theta=math.pi):
    return ET.Element("sphere",
                      attrib={
                          "name": name,
                          "rmax": str(rmax),
                          "rmin": str(rmin),
                          "deltaphi": str(delta_phi),
                          "deltatheta": str(delta_theta),
                          "starttheta": str(start_theta),
                          "startphi": str(start_phi)
                      })


def tube(name, rmax, z, rmin=0, start_phi=0, delta_phi=2 * math.pi, luint= "mm", aunit="rad"):
    return ET.Element("tube",
                      attrib={
                          "name": name,
                          "rmax": str(rmax),
                          "rmin": str(rmin),
                          "z": str(z),
                          "deltaphi": str(delta_phi),
                          "startphi": str(start_phi),
                          "lunit" : luint,
                          "aunit" : aunit
                      })
