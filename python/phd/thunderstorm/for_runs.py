
import os
from string import Template

from dataforge import Meta
from phd.utils.run_tools import create_one_file, dir_name_generator, values_from_dict, InputData, GdmlGenerator


class GGFieldHeigth(GdmlGenerator):
    def __init__(self,  fields, heights):
        self.fields = fields
        self.heights = heights
        # fields = [10e-4, 7e-4, 6.0e-4]
        # heights = [100, 200, 300]

    def generate(self, template_file):
        os.makedirs("./gdml", exist_ok=True)
        paths = []
        values_gdml = []
        with open(template_file) as fin:
            gdml_template = fin.read()
        for indx, pair in enumerate(zip(self.heights, self.fields)):
            height, field = pair
            temp_gdml = {
                'height': 0,
                'cellHeight': height,
                'fieldValueZ': field,
            }
            path = os.path.join("./gdml", "{}.gdml".format(indx))
            paths.append(path)
            values_gdml.append(temp_gdml)
            create_one_file(gdml_template, path, temp_gdml)
        return paths, values_gdml


def input_generator_custom_gdml_dwyer2003(meta: Meta, gdml_template_file: str, macros_template: str, gdml_generator : GdmlGenerator):
    paths, values_gdml = gdml_generator.generate(gdml_template_file)
    paths = list(map(lambda x: os.path.join("..", x), paths))
    meta["macros"]["path"] = paths
    macros_template = Template(macros_template)
    for path, values in zip(
            dir_name_generator(".", "sim"),
            values_from_dict(meta["macros"])
    ):
        path_gdml = values["path"]
        indx = paths.index(path_gdml)
        gdml = values_gdml[indx]
        values["posZ"] = gdml["cellHeight"] / 2 - 0.1
        text = macros_template.substitute(values)
        input_data_meta = {
            "macros": values,
            "gdml": gdml
        }
        data = InputData(
            text=text,
            path=path,
            values=Meta(input_data_meta)
        )
        yield data