try:
    import pybind11_stubgen
except:
    print("The python package pybind11_stubgen is required to generate the python docs.")
    exit()
import os
import sys

def write(self):
    filename = self.short_name + ".pyi"
    if len(sys.argv) > 1:
        filename = os.path.join(sys.argv[1], filename)
    with open(filename, "w") as pyi:
        pyi.write("\n".join(self.to_lines()))

pybind11_stubgen.ModuleStubsGenerator.write = write


if __name__ == "__main__":
    if len(sys.argv) > 1:
        sys.path = [sys.argv[1]] + sys.path
    try:
        _module = pybind11_stubgen.ModuleStubsGenerator("pyTORS")
        _module.parse()
        if pybind11_stubgen.FunctionSignature.n_fatal_errors() == 0:
            _module.stub_suffix = ""
            _module.write_setup_py = False
            _module.write()
    except:
        print("Error in generating docs for pyTORS")
